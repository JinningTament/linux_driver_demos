#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

static dev_t dev_num;
static struct cdev cdev_test;
static struct class *class;
static struct device *device;

static int cdev_test_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "this is open\n");
    return 0;
}

static ssize_t cdev_test_read(struct file *file, char __user *buf,size_t size, loff_t *ppos)
{
    //Simulate the data obtained from the register
    char kbuf[32]="this is read";
    if( copy_to_user(buf,kbuf,strlen(kbuf))!=0){
        printk("copy to user is error\n");
        return -1;
    }
    printk(KERN_INFO "this is read\n");
    return 0;
}

static ssize_t cdev_test_write(struct file *file, const char __user *buf,
                               size_t size, loff_t *ppos)
{
    char kbuf[32]={0};
    if( copy_from_user(kbuf,buf,size)!=0){
        printk("copy from user is error\n");
        return -1;
    }
    printk("kbuf=%s",kbuf);
    printk(KERN_INFO "this is write\n");
    return 0;
}

static int cdev_test_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "this is release\n");
    return 0;
}

static struct file_operations cdev_test_ops = {
    .owner   = THIS_MODULE,
    .open    = cdev_test_open,
    .read    = cdev_test_read,
    .write   = cdev_test_write,
    .release = cdev_test_release,
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

    class = class_create(THIS_MODULE, "test");
if (IS_ERR(class)) {
    printk(KERN_ERR "class_create failed\n");
    cdev_del(&cdev_test);
    unregister_chrdev_region(dev_num, 1);
    return PTR_ERR(class);
}

device = device_create(class, NULL, dev_num, NULL, "test");
if (IS_ERR(device)) {
    printk(KERN_ERR "device_create failed\n");
    class_destroy(class);
    cdev_del(&cdev_test);
    unregister_chrdev_region(dev_num, 1);
    return PTR_ERR(device);
}

    printk(KERN_INFO "cdev_add success\n");
    return 0;
}

static void moduledev_exit(void)
{
    device_destroy(class, dev_num);
    class_destroy(class);
    cdev_del(&cdev_test);
    unregister_chrdev_region(dev_num, 1);
    printk(KERN_INFO "seeyouworld!\n");
}

module_init(moduledev_init);
module_exit(moduledev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LUOJIN");
MODULE_VERSION("V1.0");