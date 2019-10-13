sh build.sh

cd test1

../bin/ilcfe ../rt/rt32.il -arch 32 -incpath "./" -action compile -target ilir -output ../bin/rt32.ir

../bin/ilcbe ../bin/rt32.ir -arch 32 -incpath "./" -action compile -target gas -output ../bin/rt32.s
as -m32 ../bin/rt32.s -o ../bin/rt32.o

../bin/ilcfe test.il -arch 32 -incpath "./" -action compile -target ilir -output ../bin/test.ir

../bin/ilcbe ../bin/test.ir -arch 32 -incpath "./" -action compile -target gas -output ../bin/test.s
as -m32 ../bin/test.s -o ../bin/test.o

gcc -m32 ../bin/rt32.o ../bin/test.o -shared -o ../bin/mytest.so

#../bin/ilcbe test_inc.ir -arch 32 -incpath "./" -action compile -target gas -output ../bin/test_inc.s
#as -m32 ../bin/test_inc.s -o ../bin/test_inc.o

# ld -lSystem ../bin/test.o ../bin/test_inc.o -o ../bin/mytest

# ../bin/mytest

cd ..
