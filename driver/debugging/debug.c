#include<linux/init.h>
#include<linux/module.h>


int rate_limit_init(void) {
int i;
unsigned long j0,j1;

j0 = jiffies;
j1 = jiffies + 30*HZ;

for(i=0;i<30;i++) {
if(printk_ratelimit()) {
printk(KERN_INFO"Printing %d", i);
    }
}

return 0;
}
void rate_limit_exit(void){
printk(KERN_INFO "Done");

}
module_init(rate_limit_init);
module_exit(rate_limit_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Jules Irenge");
MODULE_INFO(intree, "Y");
MODULE_DESCRIPTION("debug show");
MODULE_VERSION("1.0.0");


