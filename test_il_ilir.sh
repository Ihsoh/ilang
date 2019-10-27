cd test1

as32_flags=""
if [ "`as --version | grep "GNU"`" != "" ]; then
	as32_flags="--32"
fi
if [ "`as --version | grep "LLVM"`" != "" ]; then
	as32_flags="-m32"
fi

as64_flags=""
if [ "`as --version | grep "GNU"`" != "" ]; then
	as64_flags="--64"
fi
if [ "`as --version | grep "LLVM"`" != "" ]; then
	as64_flags="-m64"
fi

../bin/ilcfe ../rt/rt32.il -arch 32 -incpath "./" -action compile -target ilir -output ../bin/rt32.ir

../bin/ilcbe ../bin/rt32.ir -arch 32 -incpath "./" -action compile -target gas -output ../bin/rt32.s
as $as32_flags ../bin/rt32.s -o ../bin/rt32.o

../bin/ilcfe test.il -arch 32 -incpath "./" -action compile -target ilir -output ../bin/test.ir

../bin/ilcbe ../bin/test.ir -arch 32 -incpath "./" -action compile -target gas -output ../bin/test.s
as $as32_flags ../bin/test.s -o ../bin/test.o

../bin/ilcbe test_inc_32.ir -arch 32 -incpath "./" -action compile -target gas -output ../bin/test_inc_32.s
as $as32_flags ../bin/test_inc_32.s -o ../bin/test_inc_32.o

gcc -m32 ../bin/rt32.o ../bin/test.o ../bin/test_inc_32.o -o ../bin/mytest

../bin/mytest

# gcc -m32 ../bin/rt32.o ../bin/test.o ../bin/test_inc_32.o -shared -o ../bin/mytest.so

# ld -lSystem ../bin/test.o ../bin/test_inc_32.o -o ../bin/mytest

# ../bin/mytest

cd ..
