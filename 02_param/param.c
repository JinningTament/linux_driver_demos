#include<linux/module.h>
#include<linux/init.h>
//Driver parameter passing
#include<linux/moduleparam.h>

static int a=0;
module_param(a,int,S_IRUGO);
MODULE_PARM_DESC(a,"e.g:a=0");
static int moduleparam_init(void){
    printk("a=%d\n",a);
    return 0;
}
static void moduleparam_exit(void){
    printk("seeyouworld!\n");
    
}
module_init(moduleparam_init);//driver loading function
module_exit(moduleparam_exit);//Driver Uninstall Function

MODULE_LICENSE("GPL");

MODULE_AUTHOR("LUOJIN");

MODULE_VERSION("V1.0");