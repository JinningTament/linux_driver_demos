#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>

static dev_t dev_num;
static struct cdev cdev_test;
static struct file_operations cdev_test_ops = {
    .owner = THIS_MODULE
};

static int moduledev_init(void)
{
    int ret;
    int major, minor;

    ret = alloc_chrdev_region(&dev_num, 0, 1, "alloc_name");
    if (ret < 0) {
        printk(KERN_ERR "alloc_chrdev_region failed, ret=%d\n", ret);
        return ret;
    }

    major = MAJOR(dev_num);
    minor = MINOR(dev_num);
    printk(KERN_INFO "alloc success, major=%d, minor=%d\n", major, minor);

    cdev_init(&cdev_test, &cdev_test_ops);
    cdev_test.owner = THIS_MODULE;

    ret = cdev_add(&cdev_test, dev_num, 1);
    if (ret < 0) {
        printk(KERN_ERR "cdev_add failed, ret=%d\n", ret);
        unregister_chrdev_region(dev_num, 1);
        return ret;
    }

    printk(KERN_INFO "cdev_add success\n");
    return 0;
}

static void moduledev_exit(void)
{
    cdev_del(&cdev_test);
    unregister_chrdev_region(dev_num, 1);
    printk(KERN_INFO "seeyouworld!\n");
}

module_init(moduledev_init);
module_exit(moduledev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LUOJIN");
MODULE_VERSION("V1.0");