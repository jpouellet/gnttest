Simple pair of programs for testing libxengnttab (Xen library for manipulating grant tables).

To build: `make`

In a VM to offer pages, invoke: `./gntshr <other_domid>`

In a VM to receive pages, invoke: `./gnttab <first_domid> <gnt_ref_no>`

Then pass stuff to gntshr's stdin and see it on gnttab's stdout (in a different domain).

Get domids from `xl list`.
