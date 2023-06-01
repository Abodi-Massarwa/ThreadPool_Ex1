.PHONY: all
all: coder

#task:	codec.h basic_main.c
#	gcc basic_main.c -L. -lCodec -o encoder

coder:	stdin_main.c
	gcc stdin_main.c -L. -lCodec -o coder

.PHONY: clean
clean:
	-rm -f coder text_encrypted.txt text2.txt  2>/dev/null
