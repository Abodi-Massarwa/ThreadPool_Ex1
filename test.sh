export LD_LIBRARY_PATH=.
make clean && make all
cat hello.txt | ./coder 12 -e > hello_encrypted.txt
./coder 12 -d   < hello_encrypted.txt > hello2.txt
wc -m hello.txt
wc -m hello2.txt
diff hello.txt hello2.txt
# ./coder -e 12 <hello.txt | ./coder -d 12 dev ///TODO important command
