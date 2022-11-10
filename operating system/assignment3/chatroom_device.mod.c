#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xd9726f80, "module_layout" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x1bcee483, "cdev_del" },
	{ 0x64b60eb0, "class_destroy" },
	{ 0xe340d421, "device_destroy" },
	{ 0x9f4f34bc, "device_create" },
	{ 0xa946dcde, "__class_create" },
	{ 0xd5f10699, "cdev_add" },
	{ 0x4240b5cb, "cdev_init" },
	{ 0x54dc6ab9, "cdev_alloc" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x56470118, "__warn_printk" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x9c6febfc, "add_uevent_var" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x33fcf44a, "__kfifo_out_r" },
	{ 0xc5850110, "printk" },
	{ 0x139f2189, "__kfifo_alloc" },
	{ 0x69ecc112, "kmem_cache_alloc_trace" },
	{ 0x36c11c94, "kmalloc_caches" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0xc959d152, "__stack_chk_fail" },
	{ 0x37a0cba, "kfree" },
	{ 0xd35cce70, "_raw_spin_unlock_irqrestore" },
	{ 0x3955fcf6, "__kfifo_in_r" },
	{ 0xa916b694, "strnlen" },
	{ 0x34db050b, "_raw_spin_lock_irqsave" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x69af1880, "current_task" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "902B10D6E442CFDBDD48DD6");
