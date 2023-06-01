export LD_LIBRARY_PATH=.
make clean && make all
cat text.txt | ./coder 12 -e > text_encrypted.txt
./coder 12 -d   < text_encrypted.txt > text2.txt
#wc -m hello.txt
#wc -m hello2.txt
#diff hello.txt hello2.txt
# ./coder -e 12 <hello.txt | ./coder -d 12 dev ///TODO important command
