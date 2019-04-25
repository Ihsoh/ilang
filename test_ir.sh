sh build.sh
cd test1

../bin/ilcbe test.ir -arch 64 -incpath "./" -action compile -target gas -output ../bin/test.s
as ../bin/test.s -o ../bin/test.o
objdump -D ../bin/test.o

cd ..
