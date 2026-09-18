#include<linux/module.h>
#include<linux/init.h>
static int helloworld_init(void){
    printk("helloworld!\n");
    return 0;
}
static void helloworld_exit(void){
    printk("seeyouworld!\n");
    
}
module_init(helloworld_init);//driver loading function
module_exit(helloworld_exit);//Driver Uninstall Function

MODULE_LICENSE("GPL");

MODULE_AUTHOR("LUOJIN");

MODULE_VERSION("V1.0");