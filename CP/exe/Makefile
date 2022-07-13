include ../Makefile.config

SRC=$(wildcard *.cpp)

FLAGS_OBJ+=-I../$(CSP_CORE_DIR) -I../$(CSP_COUNTER_DIR) -I../$(CSP_COUNTERS_DIR) -I../$(CSP_DECOMPOSITION_DIR) -I../$(CSP_FILTERING_DIR) -I../$(CSP_HEURISTICS_DIR) -I../$(CSP_IO_DIR) -I../$(CSP_MICROSTRUCTURE_DIR) -I../$(CSP_OPTIMIZER_DIR) -I../$(CSP_RANDOM_GENERATION_DIR) -I../$(CSP_RESTART_DIR) -I../$(CSP_REWRITING_RULES_DIR) -I../$(CSP_SOLVER_DIR) -I../$(CSP_TOOLS_DIR) -I../$(CSP_TRACTABLE_CLASSES_DIR) -I../$(CSP_VALUE_MERGING_DIR) -I../$(CSP_VARIABLE_ELIMINATION_DIR) -I../$(GRAPH_CORE_DIR) -I../$(GRAPH_RANDOM_GENERATION_DIR) -I../$(GRAPH_TOOLS_DIR)  -I../$(GRAPH_TREE_DECOMPOSITION_DIR) -I../$(GRAPH_TRIANGULATION_DIR) -I../$(GRAPH_VISUALIZATION_DIR) -I../$(OPTION_MANAGER_DIR)

DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

FLEXE= $(FLAGS) -L../$(GRAPH_DIR) -L../$(OPTION_MANAGER_DIR) -L../$(XML_IO_DIR) -L../$(XCSP3_PARSER_DIR)/lib -L.. -o $@ $^ 
FLEXE2 = $(FLAGS) -static  -L../$(GRAPH_DIR) -L../$(OPTION_MANAGER_DIR) -L../$(XML_IO_DIR) -L../$(XCSP3_PARSER_DIR)/lib -L.. -o $@ $^ 


all: $(SRC:.cpp=.o) $(SRC:.cpp=) $(SRC:.cpp=_static)

%: %.o ../libcsp.a
	$(CC) $(FLEXE) -lcsp $(COMMON_LIBS) `xml2-config --libs`
	
%_static: %.o ../libcsp.a
	$(CC) $(FLEXE2) -lcsp $(COMMON_STATIC_LIBS)
	
RBO: RBO.o ../libcop.a
	$(CC) $(FLEXE) -lcop $(COMMON_LIBS) `xml2-config --libs`
	
RBO_static: RBO.o ../libcop.a
	$(CC) $(FLEXE2) -lcop $(COMMON_STATIC_LIBS)

%.o : %.cpp
%.o : %.cpp $(DEPDIR)/%.d
	$(CC) $(FLAGS_OBJ) $(DEPFLAGS) -c $(OUTPUT_OPTION) -DVERSION=\"`cat ../../VERSION`-`date +%y.%m.%d`\" $<
	$(POSTCOMPILE)

clean:
	rm -f *.o
	rm -rf .d
	rm -f `ls *.cpp | cut -d. -f1`
	rm -f *_static

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRC))))
