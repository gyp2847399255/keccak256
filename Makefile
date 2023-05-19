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
	echo "should be d2694ffc7370e33901d1309e7d66a76798ee84023e94724f6f3c313f0a4ffa56"
	./$(output) "In computer science, a perfect hash function h for a set S is a hash function that maps distinct elements in S to a set of m integers, with no collisions. In mathematical terms, it is an injective function. Perfect hash functions may be used to implement a lookup table with constant worst-case access time. A perfect hash function can, as any hash function, be used to implement hash tables, with the advantage that no collision resolution has to be implemented. In addition, if the keys are not the data and if it is known that queried keys will be valid, then the keys do not need to be stored in the lookup table, saving space. Disadvantages of perfect hash functions are that S needs to be known for the construction of the perfect hash function. Non-dynamic perfect hash functions need to be re-constructed if S changes. For frequently changing S dynamic perfect hash functions may be used at the cost of additional space.[1] The space requirement to store the perfect hash function is in O(n). The important performance parameters for perfect hash functions are the evaluation time, which should be constant, the construction time, and the representation size."
	echo "should be 7c9232f4303dcbf7a0121c622154441549050e85b29997ef38fe8e42e745f5db"
	./$(output) ""
	echo "should be c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470"
	./$(output) "A perfect hash function for a specific set S that can be evaluated in constant time, and with values in a small range, can be found by "
	echo "should be c12b8aebc58f423d9e475528d0ab0822a7d0897b3426255b029f4d822a650df6"

clean:
	rm -rf *.o $(output)
