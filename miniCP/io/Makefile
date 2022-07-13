include ../Makefile.config

SRC=$(wildcard *.cpp)

FLAGS_OBJ+=-DEXTERN -I../$(CSP_CORE_DIR) -I../$(GRAPH_CORE_DIR) -I../$(XCSP3_PARSER_IDIR) -I../$(CSP_TOOLS_DIR) `xml2-config --cflags` -I../$(XML_IO_DIR)

DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

all: $(SRC:.cpp=.o)

%.o : %.cpp
%.o : %.cpp $(DEPDIR)/%.d
	$(CC) $(FLAGS_OBJ) $(DEPFLAGS) -c $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

clean:
	rm -f *.o
	rm -rf .d

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRC))))
