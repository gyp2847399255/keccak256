target := $(shell ls *.c | awk '{gsub(/\.c/, ".o"); print $0}')
objects := ./*.o
output := keccak256

.PHONY: build clean test

%.o: %.c
	gcc -c $<

build: $(target)
	gcc -o $(output) $(objects)

test: build
	./$(output) "A perfect hash function"

clean:
	rm -rf *.o $(output)
