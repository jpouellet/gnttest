#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <xengnttab.h>

int main(int argc, char *argv[]) {
	xengntshr_handle *xgs;
	char *page;
	uint32_t dom;
	uint32_t ref;

	if (argc != 2)
		errx(1, "usage: %s domid", argv[0]);

	dom = atoi(argv[1]);

	if ((xgs = xengntshr_open(NULL, 0)) == NULL)
		err(1, "xengntshr_open");

	if ((page = xengntshr_share_pages(xgs, dom, 1, &ref, 1)) == NULL)
		err(1, "xengntshr_share_pages to dom %u\n", dom);

	// dump initial page contents (should be all '\0')
	write(1, page, 4096);

	fprintf(stderr, "offered gntref %u to dom %u (mapped at %p)\n", ref, dom, page);

	// loop reading stdin into page, so we have something interesting for the other side
	while (fgets(page, 4096, stdin)) {}

	if (xengntshr_unshare(xgs, page, 1) == -1)
		err(1, "xengntshr_unshare");

	if (xengntshr_close(xgs) == -1)
		err(1, "xengntshr_close");

	printf("done\n");

	return 0;
}
