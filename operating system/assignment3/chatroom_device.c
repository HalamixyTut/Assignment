#include <linux/fs.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/cdev.h>
#include <linux/kfifo.h>
#include <linux/list.h>
#include <linux/slab.h>

#define FIFO_SIZE 128
#define DEV_NAME "chatroom"
MODULE_LICENSE("Dual BSD/GPL");

struct user_node {
    int id;
    struct kfifo_rec_ptr_1 kfifo;
    spinlock_t lock;
    struct list_head list;
};

static int major;
static dev_t devno;
struct cdev *chatdev;
static struct class *chatdev_class;
struct list_head user_list = {&user_list, &user_list};


static int chat_open(struct inode *inode, struct file *file)
{
    struct user_node *p;
    struct list_head *pos;
    char info[FIFO_SIZE];
    
    snprintf(info, FIFO_SIZE, "user %d entered the chatroom.", current->tgid);
    p = kmalloc(sizeof(struct user_node), GFP_KERNEL);
    p->id = current->tgid;
    BUG_ON(kfifo_alloc(&p->kfifo, FIFO_SIZE, GFP_KERNEL));
    spin_lock_init(&p->lock);
    list_add(&p->list, &user_list);

    for (pos = user_list.next; pos != &user_list; pos = pos->next)
    {
        p = container_of(pos, struct user_node, list);
        if (p->id == current->tgid)
            continue;
        kfifo_in_spinlocked(&p->kfifo, info, strlen(info), &p->lock);
    }
    printk(KERN_ALERT "User %d entered.\n", p->id);

    return 0;
}

static int chat_release(struct inode *inode, struct file *file)
{
    struct user_node *p;
    struct list_head *pos;
    char info[FIFO_SIZE];

    snprintf(info, FIFO_SIZE, "user %d lefted the chatroom.", current->tgid);
    for (pos = user_list.next; pos != &user_list; pos = pos->next)
    {
        p = container_of(pos, struct user_node, list);
        if (p->id == current->tgid)
            continue;
        kfifo_in_spinlocked(&p->kfifo, info, strlen(info), &p->lock);
    }

    for (pos = user_list.next; pos != &user_list; pos = pos->next)
    {
        p = container_of(pos, struct user_node, list);
        if (p->id == current->tgid)
            break;
    }
    list_del(pos);
    kfree(p);
    return 0;
}

static ssize_t chat_read(struct file *file, char *buf, size_t count, loff_t *f_pos)
{
    int ret;
    size_t len = 0;
    char buffer[FIFO_SIZE];
    struct user_node *p;
    struct list_head *pos;

    for (pos = user_list.next; pos != &user_list; pos = pos->next)
    {
        p = container_of(pos, struct user_node, list);
        if (p->id == current->tgid)
            break;
    }

    while (!kfifo_is_empty(&p->kfifo)) {
        printk(KERN_WARNING "read user id: %d\n", p->id);
        ret = kfifo_out_spinlocked(&p->kfifo, buffer, sizeof(buffer), &p->lock);
        buffer[ret] = '\0';
        len = strlen(buffer);
        BUG_ON(copy_to_user(buf, buffer, len));
    }

    return len;
}

static ssize_t chat_write(struct file *file, const char *buf, size_t count, loff_t *f_pos)
{
    char buffer[FIFO_SIZE];
    char info[FIFO_SIZE];
    struct user_node *p;
    struct list_head *pos;

    BUG_ON(copy_from_user(buffer, buf, count));
    snprintf(info, FIFO_SIZE, "Message from user %d: %s", current->tgid, buffer);
    for (pos = user_list.next; pos != &user_list; pos = pos->next)
    {
        p = container_of(pos, struct user_node, list);
        if (p->id == current->tgid)
            continue;
        kfifo_in_spinlocked(&p->kfifo, info, strlen(info), &p->lock);
    }

    return count;
}

static int chat_uevent(struct device *dev, struct kobj_uevent_env *env)
{
    add_uevent_var(env, "DEVMODE=%#o", 0666);
    return 0;
}

static long int chat_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    return 0;
}

struct file_operations fops = {
    .owner      = THIS_MODULE,
    .read       = chat_read,
    .write      = chat_write,
    .open       = chat_open,
    .release    = chat_release,

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 36)
    .unlocked_ioctl = chat_ioctl
#else
.ioctl = chat_ioctl
#endif
};



// called when module is installed
int __init init_module()
{
    BUG_ON(alloc_chrdev_region(&devno, 0, 1, DEV_NAME) < 0);
    major = MAJOR(devno);
    chatdev = cdev_alloc();
    BUG_ON(chatdev == NULL);
    cdev_init(chatdev, &fops);
    BUG_ON(cdev_add(chatdev, devno, 1) != 0);

    chatdev_class = class_create(THIS_MODULE, DEV_NAME);
    BUG_ON(IS_ERR(chatdev_class));
    chatdev_class->dev_uevent = chat_uevent;

    device_create(chatdev_class, NULL, devno, NULL, DEV_NAME);

    printk(KERN_ALERT "Chatroom created.\n");

    return 0;
}



// called when module is removed
void __exit cleanup_module()
{
    device_destroy(chatdev_class, devno);
    class_destroy(chatdev_class);
    cdev_del(chatdev);
    unregister_chrdev_region(devno, 1);

    printk(KERN_ALERT "Chatroom killed.\n");
}

