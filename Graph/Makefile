include Makefile.config

SUBDIRS=core graph_tools tree-decomposition

all:
	for i in $(SUBDIRS); do (cd $$i; $(MAKE); if [ $$? -ne 0 ]; then exit 1; fi); done
	make libgraph.a

libgraph.a:	$(shell find -name '*.o' 2> /dev/null)
	ar r libgraph.a $^
	ranlib libgraph.a


documentation:
	make -C doc

clean:
	for i in $(SUBDIRS); do (cd $$i; $(MAKE) clean); done
	rm -f libgraph.a
