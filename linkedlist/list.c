#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/list.h>

MODULE_LICENSE("Dual BSD/GPL");

static LIST_HEAD(todos_list);

struct todos_entry {
	struct list_head tlist; /* link in list */
	int intvar;
	char action[20]; /* name of a thing to do */
};

void choice(int x, struct todos_entry *xl) {

	switch(x) {
		case 1:
			sprintf(xl->action, "No %d Read", x);
			return;
		case 2: 
			sprintf(xl->action, "No %d Code", x);
			return;
		case 3:
			sprintf(xl->action, "No %d Live", x);
			return;
		case 4:
			sprintf(xl->action, "No %d Sleep", x);
			return;
		case 5:
			sprintf(xl->action, "No %d Exercise", x);
			return;
		case 6:
			sprintf(xl->action, "No %d Connect", x);
			return;
		default:
			sprintf(xl->action, "No %d Multiply", x);
			return;
	}
}

static int __init todos_init(void)
{
	struct todos_entry *kwanza;
	struct todos_entry *mwisho;
	int j;
	  struct list_head *p;
        //struct todos_entry *f;


	for (j = 0; j < 10; j++) {
		kwanza = kmalloc(sizeof(struct todos_entry), GFP_KERNEL);

		if(!kwanza) {
			pr_info("Failed to allocate memory for ToDos list\n");
			return -ENOMEM;
		}

		kwanza->intvar = j;
		sprintf(kwanza->action, "ToDo %d", j + 1);
		choice(j+1, kwanza);
		pr_info("Adding %s at position %d to Todo list\n", kwanza->action, kwanza->intvar);
		list_add(&kwanza->tlist, &todos_list);
	}
	//struct list_head *p;
	//struct todos_entry *f;


	pr_info("Traversing a list ");
	pr_info(" ====================");

	list_for_each(p, &todos_list) {
		
	struct todos_entry *f = list_entry(p, struct todos_entry, tlist);
	pr_info("%s and % d", f->action, f->intvar);


	}
	// Add to todo list
	
	

	   mwisho = kmalloc(sizeof(struct todos_entry), GFP_KERNEL);

	   if(!mwisho) {
	   pr_info("Failed to allocate memory for ToDos list\n");
	   return -ENOMEM;
	   }
	   mwisho->intvar = 666;
	   sprintf(mwisho->action, "Last toDo %d", 11);
	   choice(11, mwisho);
	   pr_info("Last adding %s at position %d to Todo list\n", mwisho->action, mwisho->intvar);
	   list_add_tail(&mwisho->tlist, &todos_list);
	   
	   list_for_each(p, &todos_list) {
		   struct todos_entry *f = list_entry(p, struct todos_entry, tlist);
		   pr_info("%s and % d", f->action, f->intvar);
		   }



	return 0;
}

static void __exit todos_exit(void) 
{
	struct list_head *list; /* point to list head object */

	struct list_head *tmp; /* temporary list head used for deletion */

	if (list_empty(&todos_list)) {
		pr_info(" Todos list is empty, exiting\n");
		return;
	}
	pr_info("Todos list is not empty, emptying\n");

	list_for_each_safe(list, tmp, &todos_list) {

		struct todos_entry *pili = list_entry(list, struct todos_entry, tlist);
		list_del(&pili->tlist);

		pr_info("Todos (exit) : %s at position %d removed from list\n", pili->action, pili->intvar);
		kfree(pili);
	}

	/* Checking if our list is empty */

	if (list_empty(&todos_list))
		pr_info("Done: Todos list is empty!\n");
	else
		pr_info("Todos list is still NOT empty though!\n");


}
module_init(todos_init);
module_exit(todos_exit);

MODULE_AUTHOR("Jules Irenge");
MODULE_DESCRIPTION("linked list exercise ");
MODULE_LICENSE("Dual BSD/GPL");

