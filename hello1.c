#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/bug.h>
#include "hello1.h"
MODULE_AUTHOR("Original code: Serhii Popovych <serhii.popovych@globallogic.com>\nHomework (modifications): Natalia Duplii (IV-82)");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training\nThis module is used in hello2");
MODULE_LICENSE("Dual BSD/GPL");
struct list_of_times {
    struct list_head elem;
    ktime_t time_before;
    ktime_t time_after;
};

struct list_of_times head_elem = {
    .elem = LIST_HEAD_INIT(head_elem.elem),
};

static print_hello(void){
    pr_info(KERN_EMERG "Hello, world!\n");
    return 0;
}

struct list_of_times *new_elem, *tmp;

static print_hello_and_add_elem(void){
    new_elem = kmalloc(sizeof(struct list_of_times), GFP_KERNEL);
    if (!new_elem){
        goto clean;
    }
    new_elem->time_before = ktime_get();
    print_hello();
    new_elem->time_after = ktime_get();
    list_add_tail(&new_elem->elem, &head_elem.elem);
    return 0;
clean:
    list_for_each_entry_safe(new_elem, tmp, &head_elem.elem, elem){
        list_del(&new_elem->elem);
        kfree(new_elem);
    }
    BUG_ON(1);
}

static clean_list(void){
    pr_debug("start cleaning\n");
    ktime_t time;
    list_for_each_entry_safe(new_elem, tmp, &head_elem.elem, elem){
        time = new_elem->time_after - new_elem->time_before;
        pr_debug("time to print the greeting: %lld\n", time);
        list_del(&new_elem->elem);
        kfree(new_elem);
    }
    pr_debug("stop cleaning\n");
    BUG_ON(!list_empty(&head_elem.elem));
}

EXPORT_SYMBOL(print_hello);
EXPORT_SYMBOL(print_hello_and_add_elem);
EXPORT_SYMBOL(clean_list);
