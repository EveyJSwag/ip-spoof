EXEC := SAND_BOX
DIRS := lib/ src/ 
FILES := $(foreach file,$(DIRS),$(wildcard $(file)*.c))
OBJS := $(foreach c_file,$(FILES),$(patsubst %.c,%.o,$(c_file)))
CLANG := clang
GCC := gcc

$(EXEC): $(OBJS)
	$(CLANG) -o $(EXEC) $(OBJS)

%.o: %.c
	$(CLANG) -c $< -o $@
		
.Phony: clean
clean:
	rm lib/*.o
	rm src/*.o
	rm $(EXEC)
