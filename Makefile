.PHONY: all
all: coder

coder:	coder.c
	gcc coder.c -L. -lCodec -o coder

.PHONY: clean
clean:
	-rm -f coder text_encrypted.txt text2.txt  2>/dev/null
