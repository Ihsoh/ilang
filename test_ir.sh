sh build.sh
cd test1

../bin/ilcbe test.ir -arch 64 -incpath "./" -action preprocess -output ../bin/test.il
../bin/ilcbe test.ir -arch 64 -incpath "./" -action compile -target gas -output ../bin/test.s
as ../bin/test.s -o ../bin/test.o
objdump -D ../bin/test.o
gcc -fpic ../bin/test.o -o ../bin/mytest
../bin/mytest

cd ..
