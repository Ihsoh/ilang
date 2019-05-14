sh build.sh
# valgrind bin/ilc compile test.il > bin/test.c
cd test1

../bin/ilcfe test.il -arch 64 -incpath "./" -action compile -target ilir -output ../bin/test.ir
../bin/ilcbe ../bin/test.ir -arch 64 -incpath "./" -action compile -target gas -output ../bin/test.s

cd ..
