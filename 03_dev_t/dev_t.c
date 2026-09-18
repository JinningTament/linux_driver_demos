#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>

static int major = 0;
static int minor = 0;
static dev_t dev_num;

module_param(major, int, S_IRUGO);
module_param(minor, int, S_IRUGO);

static int moduleparam_init(void)
{
    int ret;

    if (major) {
        printk(KERN_INFO "major is %d, minor is %d\n", major, minor);
        dev_num = MKDEV(major, minor);
        ret = register_chrdev_region(dev_num, 1, "chrdev_name");
        if (ret < 0) {
            printk(KERN_ERR "register_chrdev_region failed, ret=%d\n", ret);
            return ret;
        }
        printk(KERN_INFO "register_chrdev_region success\n");
    } else {
        ret = alloc_chrdev_region(&dev_num, 0, 1, "alloc_name");
        if (ret < 0) {
            printk(KERN_ERR "alloc_chrdev_region failed, ret=%d\n", ret);
            return ret;
        }
        major = MAJOR(dev_num);
        minor = MINOR(dev_num);
        printk(KERN_INFO "alloc success, major=%d, minor=%d\n", major, minor);
    }

    return 0;
}

static void moduleparam_exit(void)
{
    unregister_chrdev_region(dev_num, 1);
    printk(KERN_INFO "seeyouworld!\n");
}

module_init(moduleparam_init);
module_exit(moduleparam_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LUOJIN");
MODULE_VERSION("V1.0");