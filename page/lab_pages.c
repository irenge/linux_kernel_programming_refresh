#include <linux/version.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>

static int __init my_init(void)
{

	struct page *p;

	unsigned int pfn, valid = 0, free = 0, locked = 0, reserved = 0, dirty = 0, active = 0, writeback = 0, mappedtodisk = 0;

	unsigned long num_physpages;

	num_physpages = get_num_physpages();

	pr_info("Number of pages is %d", num_physpages);

	for (pfn = 0; pfn <= num_physpages; pfn++)  {

		if (!pfn_valid(pfn))
			continue;
		valid++;

		p = pfn_to_page(pfn);
		
		// check if page is free
		if (!page_count(p)) {
			free++;
			continue;
		}
		if (PageLocked(p))
			locked++;
		if (PageReserved(p))
			reserved++;
		if (PageDirty(p))
			dirty++;
		if (PageActive(p))
			active++;
		if (PageWriteback(p))
			writeback++;
		if (PageMappedToDisk(p))
			mappedtodisk++;
	}
	pr_info ("\nExamining %ld pages (num_phys_pages) = %ld MB = %ld GB\n", num_physpages, num_physpages* PAGE_SIZE / 1024 / 1024, num_physpages* PAGE_SIZE / 1024 / 1024 / 1024);
	pr_info("Pages with valid PFN's= %ld, = %ld MB\n", valid, valid * PAGE_SIZE / 1024 / 1024);
	pr_info("Free pages : %d", free);
	pr_info("Locked pages : %d", locked);
	pr_info("Reserved pages : %d", reserved);

	return 0;


}


static void __exit my_exit(void)
{
	pr_info("Module Unloading\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Jules Octave");
MODULE_DESCRIPTION("lab on pages ");
MODULE_LICENSE("GPL v2");





