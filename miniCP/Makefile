include Makefile.config

SUBDIRS=core statistical_information decomposition filtering heuristics io optimizer restart rewriting_rules solver tools


all:
	for i in $(SUBDIRS); do (cd $$i; $(MAKE); if [ $$? -ne 0 ]; then exit 1; fi); done
	make libcsp.a
	make libcop.a
	make -C exe

libcsp.a:	$(shell ls */*.o 2> /dev/null | grep -v 'optimizer/' | grep -v '^exe/')
	ar r libcsp.a $^
	ranlib libcsp.a
	
libcop.a:	$(shell ls */*.o 2> /dev/null | grep -v '^exe/')
	ar r libcop.a $^
	ranlib libcop.a

documentation:
	make -C doc

clean:
	for i in $(SUBDIRS); do (cd $$i; $(MAKE) clean); done
	make -C exe clean
	rm -f libcsp.a libcop.a
