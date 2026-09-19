#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/string.h> // 添加 strlen 头文件

// Device structure
struct device_test {
    dev_t dev_num;
    int major;
    int minor;
    struct cdev cdev_test;
    struct class *class;
    struct device *device;
    char kbuf[32];
}; // 加上分号

struct device_test dev1;

static int cdev_test_open(struct inode *inode, struct file *file)
{
    file->private_data = &dev1;
    printk(KERN_INFO "this is open\n");
    return 0;
}

static ssize_t cdev_test_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
    struct device_test *test_dev = (struct device_test *)file->private_data;
    
    // 安全性检查：确保不超过用户请求的大小，也不超过内核缓冲区实际长度
    size_t len = strlen(test_dev->kbuf);
    if (size < len) {
        len = size;
    }

    if (copy_to_user(buf, test_dev->kbuf, len) != 0) {
        printk(KERN_ERR "copy to user is error\n");
        return -EFAULT; // 返回标准错误码
    }
    
    return len; // 返回实际读取的字节数
}

static ssize_t cdev_test_write(struct file *file, const char __user *buf, size_t size, loff_t *ppos)
{
    struct device_test *test_dev = (struct device_test *)file->private_data; // 补上变量定义

    // 安全防护：防止缓冲区溢出
    if (size > sizeof(test_dev->kbuf) - 1) {
        size = sizeof(test_dev->kbuf) - 1;
    }

    if (copy_from_user(test_dev->kbuf, buf, size) != 0) {
        printk(KERN_ERR "copy from user is error\n");
        return -EFAULT;
    }
    
    test_dev->kbuf[size] = '\0'; // 确保字符串结束符
    
    printk(KERN_INFO "kbuf=%s\n", test_dev->kbuf);
    printk(KERN_INFO "this is write\n");
    return size; // 返回实际写入的字节数
}

static int cdev_test_release(struct inode *inode, struct file *file)
{
    file->private_data = NULL; // 清理私有数据
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

    ret = alloc_chrdev_region(&dev1.dev_num, 0, 1, "alloc_name");
    if (ret < 0) {
        printk(KERN_ERR "alloc_chrdev_region failed, ret=%d\n", ret);
        return ret;
    }

    dev1.major = MAJOR(dev1.dev_num);
    dev1.minor = MINOR(dev1.dev_num);
    printk(KERN_INFO "alloc success, major=%d, minor=%d\n", dev1.major, dev1.minor);

    // 使用正确的全局操作集变量名
    cdev_init(&dev1.cdev_test, &cdev_test_ops);
    dev1.cdev_test.owner = THIS_MODULE;

    // 使用结构体成员
    ret = cdev_add(&dev1.cdev_test, dev1.dev_num, 1);
    if (ret < 0) {
        printk(KERN_ERR "cdev_add failed, ret=%d\n", ret);
        unregister_chrdev_region(dev1.dev_num, 1);
        return ret;
    }

    // 使用结构体成员，并适配新的 class_create API
    dev1.class = class_create(THIS_MODULE,"test");
    if (IS_ERR(dev1.class)) {
        printk(KERN_ERR "class_create failed\n");
        cdev_del(&dev1.cdev_test);
        unregister_chrdev_region(dev1.dev_num, 1);
        return PTR_ERR(dev1.class);
    }

    dev1.device = device_create(dev1.class, NULL, dev1.dev_num, NULL, "test");
    if (IS_ERR(dev1.device)) {
        printk(KERN_ERR "device_create failed\n");
        class_destroy(dev1.class);
        cdev_del(&dev1.cdev_test);
        unregister_chrdev_region(dev1.dev_num, 1);
        return PTR_ERR(dev1.device);
    }

    printk(KERN_INFO "cdev_add success\n");
    return 0;
}

static void moduledev_exit(void)
{
    device_destroy(dev1.class, dev1.dev_num);
    class_destroy(dev1.class);
    cdev_del(&dev1.cdev_test);
    unregister_chrdev_region(dev1.dev_num, 1);
    printk(KERN_INFO "seeyouworld!\n");
}

module_init(moduledev_init);
module_exit(moduledev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LUOJIN");
MODULE_VERSION("V1.0");