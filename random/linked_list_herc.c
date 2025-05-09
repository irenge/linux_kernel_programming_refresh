/* 12 labour of Hercules 
   === The order of the labours given by the mythographer Apollodorus is:[10]
   Slaying the Nemean lion
   Slaying the nine-headed Lernaean Hydra
   Capturing the Ceryneian Hind
   Capturing the Erymanthian Boar
   Cleaning the Augean stables in a single day
   Slaying the Stymphalian birds
   Capturing the Cretan Bull
   Stealing the Mares of Diomedes
   Obtaining the belt of Hippolyta, queen of the Amazons
   Obtaining the cattle of the three-bodied giant Geryon
   Stealing three of the golden apples of the Hesperides
   Capturing and bringing back Cerberus
 *
 */
#include <linux/module.h>
#include <linux/list.h>
#include <linux/random.h>
#include <linux/types.h>

// Initialiase a list 
static LIST_HEAD(hercules_tasks_list);

struct lst_node_str {
	short unsigned value;
	char action[50];
	struct list_head tlist;

};
void hercules_task(struct lst_node_str *xl);

static  int __init ll_init(void) {

	struct lst_node_str *first_lst;
	short unsigned  j;
	short unsigned r;
	struct list_head *p;


	for(j=1; j < 12; j++) {
		first_lst = kmalloc(sizeof(struct lst_node_str), GFP_KERNEL);
		if(!first_lst) {
			pr_info("Failed to allocate memory for lst_node_str");
			return -ENOMEM;
		}
		//get the random number 
		get_random_bytes(&r, sizeof(short unsigned));
		//r = r % 12;
		first_lst->value = (r % 12) + 1;
		// set action 
		hercules_task(first_lst);
		//pr_info("Adding %s at position %d to Todo list\n", first_lst->action, first_lst->value);
		list_add(&first_lst->tlist, &hercules_tasks_list);
	}


	pr_info("Traversing a list ");
	pr_info(" ====================");

	list_for_each(p, &hercules_tasks_list) {

		struct lst_node_str  *f = list_entry(p, struct lst_node_str, tlist);
		pr_info("%d: %s\n", f->value, f->action);


	}

	return 0;
}

void hercules_task(struct lst_node_str *xl) {
	
	short unsigned x;
	x = xl->value;

	// avoid zero value
	//if (x == 0)
	//	x = 

	switch(x) {
		case 1:
			sprintf(xl->action, "Slaying the Nemean lion");
			return;
		case 2:
			sprintf(xl->action, "Slaying the nine-headed Lernaean Hydra");
			return;
		case 3:
			sprintf(xl->action, "Capturing the Ceryneian Hind");
			return;
		case 4:
			sprintf(xl->action, "Capturing the Erymanthian Boar");
			return;
		case 5:
			sprintf(xl->action, "Cleaning the Augean stables in a single day");
			return;
		case 6:
			sprintf(xl->action, "Slaying the Stymphalian birds");
			return;
		case 7:
			sprintf(xl->action, "Capturing the Cretan Bull");
			return;
		case 8:
			sprintf(xl->action, "Stealing the Mares of Diomedes");
			return;
		case 9:
			sprintf(xl->action, "Obtaining the belt of Hippolyta, queen of the Amazons");
			return;
		case 10:
			sprintf(xl->action, "Obtaining the cattle of the three-bodied giant Geryon");
			return;
		case 11:
			sprintf(xl->action, "Stealing three of the golden apples of the Hesperides");
			return;
		case 12:
			sprintf(xl->action, "Capturing and bringing back Cerberus");
			return;
		default:
			sprintf(xl->action, "Invalid !! from 1-12 not %d", xl->value);
			pr_alert("\nInvalid choice for %d",xl->value);
			return;
	}
}

static void __exit ll_exit(void) {
	struct list_head *ttlist; /* point to list head object */

	struct list_head *tmp; /* temporary list head used for deletion */

	if (list_empty(&hercules_tasks_list)) {
		pr_info(" Hercules task list is empty, exiting\n");
		return;
	}
	pr_info("Hercules task list is not empty, emptying\n");

	list_for_each_safe(ttlist, tmp, &hercules_tasks_list) {

		struct lst_node_str *pili = list_entry(ttlist, struct lst_node_str, tlist);
		list_del(&pili->tlist);

		pr_info("Hercules task (exit) : %s at position %d removed from list\n", pili->action, pili->value);
		kfree(pili);
	}

	/* Checking if our list is empty */

	if (list_empty(&hercules_tasks_list))
		pr_info("Done: Hercules task list is empty!\n");
	else
		pr_info("Hercules task list is still NOT empty though!\n");




}

module_init(ll_init);
module_exit(ll_exit);

MODULE_AUTHOR("Jules Irenge");
MODULE_DESCRIPTION("linked list exercise ");
MODULE_LICENSE("Dual BSD/GPL");

