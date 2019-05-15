sh build.sh

cd test1

../bin/ilcfe test.il -arch 64 -incpath "./" -action compile -target ilir -output ../bin/test.ir

../bin/ilcbe ../bin/test.ir -arch 64 -incpath "./" -action compile -target gas -output ../bin/test.s
as ../bin/test.s -o ../bin/test.o

../bin/ilcbe test_inc.ir -arch 64 -incpath "./" -action compile -target gas -output ../bin/test_inc.s
as ../bin/test_inc.s -o ../bin/test_inc.o

ld -lSystem ../bin/test.o ../bin/test_inc.o -o ../bin/mytest

../bin/mytest

cd ..
