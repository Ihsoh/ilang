sh build.sh
# valgrind bin/ilc compile test.il > bin/test.c
cd test1

successCount=0
failedCount=0
for i in {1..1}; do
	../bin/ilcfe test3.il -arch 64 -incpath "./" -action compile -target llvmir -output ../bin/test.ll
	if [ $? -eq 0 ]; then
		successCount=`expr $successCount + 1`
	else
		failedCount=`expr $failedCount + 1`
	fi
done
echo "success: $successCount, failed: $failedCount"

cd ..

lli bin/test.ll

# llc -filetype=asm bin/test.ll -o bin/test.s

# gcc -w bin/test.c -o bin/test
# bin/test
