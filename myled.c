// SPDX-License-Identifier: GPL-3.0
// Copyright (C) 2021 Ryuichi Ueda. All rights reserved.



#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/delay.h>

MODULE_AUTHOR("Ryuichi Ueda & Yuwa Aoki");
MODULE_DESCRIPTION("driver for LED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.1");

static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;

static volatile u32 *gpio_base = NULL;

static ssize_t ROM_read(struct file* filp, char* buf, size_t count, loff_t* pos)
{
	int size = 0;
	char ROM[] = {'R', 'O', 'M', 0x0A};
	if(copy_to_user(buf+size, (const char *)ROM, sizeof(ROM))){
		printk(KERN_INFO "ROM : copy_to_user failed\n");
		return -EFAULT;
	}
	size += sizeof(ROM);
	return size;
}

static ssize_t led_write(struct file* filp, const char* buf, size_t count, loff_t* pos)
{
	char c;
	long t1 = 100;
	long t2 = 1000;
	if(copy_from_user(&c,buf,sizeof(char)))
		return -EFAULT;

	if(c == '0'){
		gpio_base[10] = 1<< 25;
	}else if(c == '1'){
		gpio_base[7] = 1<< 25;
		mdelay(t1);
		gpio_base[10] = 1<< 25;
		mdelay(t2);

		gpio_base[7] = 1<< 25;
		mdelay(t1);
		gpio_base[10] = 1<< 25;
		mdelay(t2);

		gpio_base[7] = 1<< 25;
	       	mdelay(t1);
		gpio_base[10] = 1<< 25;
		mdelay(t2);

		gpio_base[7] = 1<< 25;
	       	mdelay(t1);
		gpio_base[10] = 1<< 25;
		mdelay(t2);

		gpio_base[7] = 1<< 25;
	   	mdelay(t1);
		gpio_base[10] = 1<< 25;
		mdelay(t2);

		gpio_base[7] = 1<< 25;
	       	mdelay(t1);
		gpio_base[10] = 1<< 25;
	}

	return 1;
}

static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.write = led_write,
	.read = ROM_read
};

static int __init init_mod(void)
{
	int retval;

	gpio_base = ioremap_nocache(0xfe200000, 0xA0);

	const u32 led = 25;
	const u32 index = led/10;
	const u32 shift = (led%10)*3;
	const u32 mask = ~(0x7 << shift);
	gpio_base[index] = (gpio_base[index] & mask) | (0x1 << shift);
	retval = alloc_chrdev_region(&dev, 0, 1, "myled");
	if(retval < 0){
		printk(KERN_INFO "alloc_chrdev_region failed.\n");
		return retval;
	}
	printk(KERN_INFO "%s is loaded. major:%d\n", __FILE__,MAJOR(dev));

	cdev_init(&cdv, &led_fops);
	retval = cdev_add(&cdv, dev, 1);
	if(retval < 0){
		printk(KERN_ERR "cdev_add failed. major:%d. minor:%d\n", MAJOR(dev), MINOR(dev));
	}

	cls = class_create(THIS_MODULE, "myled");
	if(IS_ERR(cls)){
		printk(KERN_ERR "class_create failed.\n");
		return PTR_ERR(cls);
	}
	device_create(cls, NULL, dev, NULL, "myled%d", MINOR(dev));

	return 0;
}

static void __exit cleanup_mod(void)
{
	cdev_del(&cdv);
	class_destroy(cls);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "%s is unloaded. major:%d\n", __FILE__,MAJOR(dev));
}

module_init(init_mod);
module_exit(cleanup_mod);
