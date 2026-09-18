#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>

static dev_t dev_num;
struct cdev cdev_test;
struct file_operations cdev_test_ops={
    .owner=THIS_MODULE
};

static int moduledev_init(void)
{
    int ret;

    
        ret = alloc_chrdev_region(&dev_num, 0, 1, "alloc_name");
        if (ret < 0) {
            printk(KERN_ERR "alloc_chrdev_region failed, ret=%d\n", ret);
            return ret;
        }
        major = MAJOR(dev_num);
        minor = MINOR(dev_num);
        printk(KERN_INFO "alloc success, major=%d, minor=%d\n", major, minor);

        //init cdev
        cdev_test.owner=THIS_MODULE;
        cdev_init(&cdev_test,&cdev_test_ops);
        cdev_add(&cdev_test,dev_num,1);

    return 0;
}

static void moduledev_exit(void)
{
    //Delete the device number first, then uninstall the character device
    unregister_chrdev_region(dev_num, 1);
    cdev_del(dev_num);
    printk(KERN_INFO "seeyouworld!\n");
}

module_init(moduledev_init);
module_exit(moduledev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LUOJIN");
MODULE_VERSION("V1.0");