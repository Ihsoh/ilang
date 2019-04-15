sh build.sh
cd test1

successCount=0
failedCount=0
for i in {1..1}; do
	../bin/ilcbe test.ir -arch 64 -incpath "./" -action printast -output ../bin/test.s
	if [ $? -eq 0 ]; then
		successCount=`expr $successCount + 1`
	else
		failedCount=`expr $failedCount + 1`
	fi
done
echo "success: $successCount, failed: $failedCount"

cd ..
