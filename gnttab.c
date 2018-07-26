#include <sys/mman.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <xengnttab.h>

volatile sig_atomic_t done = 0;

void handle(int sig) {
	done = 1;
}

int main(int argc, char *argv[]) {
	xengnttab_handle *xgs;
	char *page;
	uint32_t dom;
	uint32_t ref;

	if (argc != 3)
		errx(1, "usage: %s domid gntref", argv[0]);

	dom = atoi(argv[1]);
	ref = atoi(argv[2]);

	if ((xgs = xengnttab_open(NULL, 0)) == NULL)
		err(1, "xengnttab_open");

	if ((page = xengnttab_map_grant_ref(xgs, dom, ref, PROT_READ | PROT_WRITE)) == NULL)
		err(1, "xengnttab_map_grant_ref dom=%u ref=%u prot=rw\n", dom, ref);

	fprintf(stderr, "mapped gntref %u from dom %u at %p\n", ref, dom, page);

	signal(SIGINT, handle);

	do {
		write(1, page, 4096);
		sleep(1);
	} while (!done);

	if (xengnttab_unmap(xgs, page, 1) == -1)
		err(1, "xengnttab_unmap");

	if (xengnttab_close(xgs) == -1)
		err(1, "xengnttab_close");

	printf("done\n");

	return 0;
}
