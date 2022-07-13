all:
	make -C XCSP3-Parser
	make -C Graph
	make -C CP
	make -C miniCP

clean:
	make -C Graph $@
	make -C XCSP3-Parser $@
	make -C CP $@
	make -C miniCP $@
