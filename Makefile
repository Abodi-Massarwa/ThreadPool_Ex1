.PHONY: all
all: stdinExample

#task:	codec.h basic_main.c
#	gcc basic_main.c -L. -lCodec -o encoder

stdinExample:	stdin_main.c
	gcc stdin_main.c -L. -lCodec -o coder

.PHONY: clean
clean:
	-rm -f coder  2>/dev/null
