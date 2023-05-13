export LD_LIBRARY_PATH=.
make clean && make all
cat hello.txt | ./coder 12 -e > hello_encypted.txt
./coder 12 -d < hello.txt > hello_decrypted.txt
