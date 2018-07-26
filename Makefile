LDFLAGS=-lxengnttab
PROGS=gntshr gnttab

all: $(PROGS)

clean:
	rm -f $(PROGS)
