sh build.sh
cd test1

../bin/ilcfe test.il -arch 64 -incpath "./" -action compile -target llvmir -output ../bin/test.ll

cd ..

lli bin/test.ll

# llc -filetype=asm bin/test.ll -o bin/test.s

# gcc -w bin/test.c -o bin/test
# bin/test
