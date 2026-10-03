#include <linux/module.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

static struct timer_list test_timer;

static void test_function(struct timer_list *t)
{
    printk(KERN_INFO "this is timer_function\n");

    /* 如果需要周期性执行，重新设置 */
    mod_timer(&test_timer, jiffies + msecs_to_jiffies(5000));
}

static int __init helloworld_init(void)
{
    printk(KERN_INFO "helloworld!\n");

    /* 初始化定时器 */
    timer_setup(&test_timer, test_function, 0);

    /* 5 秒后到期 */
    mod_timer(&test_timer, jiffies + msecs_to_jiffies(5000));

    return 0;
}

static void __exit helloworld_exit(void)
{
    del_timer_sync(&test_timer);
    printk(KERN_INFO "seeyouworld!\n");
}

module_init(helloworld_init);
module_exit(helloworld_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LUOJIN");
MODULE_VERSION("V1.0");