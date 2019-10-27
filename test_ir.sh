cd test1

../bin/ilcbe test.ir -arch 32 -incpath "./" -action compile -target gas -output ../bin/test.s
as -m32 ../bin/test.s -o ../bin/test.o
# objdump -D ../bin/test.o
# gcc -fpic ../bin/test.o -o ../bin/mytest
# ../bin/mytest

cd ..
