#include <linux/init.h>
#include <linux/kfifo.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/version.h>

#define FIFO_SIZE 128

struct kfifo_rec_ptr_1 test;

spinlock_t mylock;

static const char *expected_result[] = {
    "a",        "bb",       "ccc",      "dddd",     "eeeee",
    "ffffff",   "ggggggg",  "hhhhhhhh", "iiiiiiiii", "jjjjjjjjjj",  
};

static int __init testfunc(void)
{
    char buf[100];
    unsigned int i;
    unsigned int ret;
    struct {
        unsigned char buf[6];
    } hello = {"hello"};

    printk(KERN_INFO "record fifo test start\n");

    kfifo_in_spinlocked(&test, &hello, sizeof(hello), &mylock);

    printk(KERN_INFO "fifo peek len: %u\n", kfifo_peek_len(&test));

    for (i = 0; i < 10; i++) {
        memset(buf, 'a' + i, i + 1);
        kfifo_in_spinlocked(&test, buf, i + 1, &mylock);
    }

    printk(KERN_INFO "skip 1st element\n");
    kfifo_skip(&test);

    printk(KERN_INFO "fifo len: %u\n", kfifo_len(&test));

    ret = kfifo_out_peek(&test, buf, sizeof(buf));
    if (ret)
        printk(KERN_INFO "%.*s\n", ret, buf);

    i = 0;
    while (!kfifo_is_empty(&test)) {
        ret = kfifo_out_spinlocked(&test, buf, sizeof(buf), &mylock);
        buf[ret] = '\0';
        printk(KERN_INFO "item = %.*s\n", ret, buf);
        if (strcmp(buf, expected_result[i++])) {
            //printk(KERN_INFO "expected_result = %s", expected_result[i]);
            printk(KERN_WARNING "value mismatch: test failed\n");
            return -EIO;
        }
    }
    if (i != ARRAY_SIZE(expected_result)) {
        printk(KERN_WARNING "size mismatch: test failed\n");
        return -EIO;
    }
    printk(KERN_INFO "test passed\n");

    return 0;
}

static int __init example_init(void)
{
    int ret;

    ret = kfifo_alloc(&test, FIFO_SIZE, GFP_KERNEL);
    if (ret) {
        printk(KERN_ERR "error kfifo_alloc\n");
        return ret;
    }

    spin_lock_init(&mylock);

    if (testfunc() < 0) {
        kfifo_free(&test);
        return -EIO;
    }

    return 0;
}

static void __exit example_exit(void)
{
    kfifo_free(&test);
}

module_init(example_init);
module_exit(example_exit);
MODULE_LICENSE("GPL");