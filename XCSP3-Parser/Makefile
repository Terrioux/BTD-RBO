C++ = g++
CFLAGS = -g -Wall -std=c++11 -O3
EXEC = test

LIBS = `xml2-config --libs`
INC = `xml2-config --cflags`  -Iinclude
CPP_FILES := $(wildcard src/*.cc)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cc=.o)))

lib: $(OBJ_FILES) 
	ar rcsv lib/libparserxcsp3core.a $(OBJ_FILES)

obj/%.o: src/%.cc
	-@(test -d "obj") || mkdir obj
	$(C++) $(CFLAGS) $(INC) -c -o $@ $<

clean: 
	rm -rf obj/*.o lib/*


