#!/bin/bash

# 获取脚本所在目录。
base_path=$(cd `dirname $0`; pwd)

ilcfe=$base_path/../bin/ilcfe
ilcbe=$base_path/../bin/ilcbe

build_32_llc=0
build_64_llc=1
build_32_clang=0
build_64_clang=1
build_32_ilcbe=1
build_64_ilcbe=1

totalCount=0
successCount=0

bin_path=$base_path/bin
if [ -d $bin_path ]; then
	rm -rf $bin_path
fi
mkdir $bin_path

if [ $build_32_llc != 0 ]; then
	$ilcfe $base_path/testlib.il -arch 32 -incpath "$base_path/" -action compile -target llvmir > $bin_path/testlib_32.ll
	llc -march=x86 $bin_path/testlib_32.ll -filetype=obj -o $bin_path/testlib_ll_32.o

	clang -c -m32 $base_path/testlib_c.c -o $bin_path/testlib_c_32.o
fi

if [ $build_64_llc != 0 ]; then
	$ilcfe $base_path/testlib.il -arch 64 -incpath "$base_path/" -action compile -target llvmir > $bin_path/testlib_64.ll
	llc -march=x86-64 $bin_path/testlib_64.ll -filetype=obj -o $bin_path/testlib_ll_64.o

	clang -c -m64 $base_path/testlib_c.c -o $bin_path/testlib_c_64.o
fi

if [ $build_32_clang != 0 ]; then
	$ilcfe $base_path/testlib.il -arch 32 -incpath "$base_path/" -action compile -target c > $bin_path/testlib_32.c

	cp $base_path/testlib_c.c $bin_path/testlib_c_32.c
fi

if [ $build_64_clang != 0 ]; then
	$ilcfe $base_path/testlib.il -arch 64 -incpath "$base_path/" -action compile -target c > $bin_path/testlib_64.c

	cp $base_path/testlib_c.c $bin_path/testlib_c_64.c
fi

if [ $build_32_ilcbe != 0 ]; then
	echo -e 'wow'
fi

if [ $build_64_ilcbe != 0 ]; then
	$ilcfe $base_path/testlib.il -arch 64 -incpath "$base_path/" -action compile -target ilir -output $bin_path/testlib_64.ir
	$ilcbe $bin_path/testlib_64.ir -arch 64 -incpath "$base_path/" -action compile -target gas -output $bin_path/testlib_64_ir.s
	as $bin_path/testlib_64_ir.s -o $bin_path/testlib_64_ir.o

	$ilcfe $base_path/testlib_il64.il -arch 64 -incpath "$base_path/" -action compile -target ilir -output $bin_path/testlib_il64.ir
	$ilcbe $bin_path/testlib_il64.ir -arch 64 -incpath "$base_path/" -action compile -target gas -output $bin_path/testlib_il64_ir.s
	as $bin_path/testlib_il64_ir.s -o $bin_path/testlib_il64_ir.o
fi

# 获取所有测试例子的目录。
for example_path in $base_path/$1*; do
	if [ -d $example_path ]; then
		main_il_path=$example_path/main.il
		config_txt_path=$example_path/config.txt

		# 如果目录下包含“main.il”，则认为是测试例子目录。
		if [ -f $main_il_path ]; then
			ignore_ilcfe_pp_error=0
			if [ -f $config_txt_path ]; then
				config=`cat $config_txt_path`;
				if [ `echo $config | grep "IGNORE_ILCFE_PP_ERROR"` == "IGNORE_ILCFE_PP_ERROR" ]; then
					ignore_ilcfe_pp_error=1
				fi
			fi

			totalCount=`expr $totalCount + 1`

			echo "=======> $example_path"
			
			# 把bin目录删除后重建。
			bin_path=$example_path/bin
			if [ -d $bin_path ]; then
				rm -rf $bin_path
			fi
			mkdir $bin_path

			# 使用ILCFE输出预处理后的代码。
			$ilcfe $example_path/main.il -incpath "$example_path/../;$example_path/" -action preprocess -output $bin_path/main_pp.il

			# 使用ILCFE输出IL IR（32位）。
			if [ $build_32_ilcbe != 0 ]; then
				echo "===> IL IR(arch32)"
				startTime=$(date +%s)
				$ilcfe $example_path/main.il -arch 32 -incpath "$example_path/../;$example_path/" -action compile -target ilir -output $bin_path/main_32.ir
				ilcfe_rval=$?
				if [ $ilcfe_rval != 0 ]; then
					if [ $ignore_ilcfe_pp_error != 0 ] && [ $ilcfe_rval == 234 ]; then
						echo -e "\033[33m ILCFE COMPILE ERROR, BUT IGNORED \033[0m"
					else
						echo -e "\033[31m ILCFE COMPILE ERROR \033[0m"
						continue
					fi
				else
					$ilcbe $bin_path/main_32.ir -arch 32 -incpath "$example_path/../;$example_path/" -action compile -target gas -output $bin_path/main_32_ir.s
					as -m32 $bin_path/main_32_ir.s -o $bin_path/main_32_ir.o
					if [ $? != 0 ]; then
						echo -e "\033[31m AS ERROR \033[0m"
						continue
					fi


					echo -e 'wow'
				fi
				endTime=$(date +%s)
				cost=$((endTime - startTime))
				echo "cost(ilir): $cost"
			fi

			# 使用ILCFE输出IL IR（64位）。
			if [ $build_64_ilcbe != 0 ]; then
				echo "===> IL IR(arch64)"
				startTime=$(date +%s)
				$ilcfe $example_path/main.il -arch 64 -incpath "$example_path/../;$example_path/" -action compile -target ilir -output $bin_path/main_64.ir
				ilcfe_rval=$?
				if [ $ilcfe_rval != 0 ]; then
					if [ $ignore_ilcfe_pp_error != 0 ] && [ $ilcfe_rval == 234 ]; then
						echo -e "\033[33m ILCFE COMPILE ERROR, BUT IGNORED \033[0m"
					else
						echo -e "\033[31m ILCFE COMPILE ERROR \033[0m"
						continue
					fi
				else
					$ilcbe $bin_path/main_64.ir -arch 64 -incpath "$example_path/../;$example_path/" -action compile -target gas -output $bin_path/main_64_ir.s
					as $bin_path/main_64_ir.s -o $bin_path/main_64_ir.o
					if [ $? != 0 ]; then
						echo -e "\033[31m AS ERROR \033[0m"
						continue
					fi

					clang -m64 -w $base_path/bin/testlib_64_ir.o $base_path/bin/testlib_il64_ir.o $bin_path/main_64_ir.o -o $bin_path/main_ir_64
					if [ $? != 0 ]; then
						echo -e "\033[31m CLANG COMPILER ERROR \033[0m"
						continue
					fi

					chmod +x $bin_path/main_ir_64
					$bin_path/main_ir_64
					if [ $? != 0 ]; then
						echo -e "\033[31m ERROR \033[0m"
						continue
					fi
				fi
				endTime=$(date +%s)
				cost=$((endTime - startTime))
				echo "cost(ilir): $cost"
			fi

			# 使用ILCFE输出LLVM IR（32位）。
			if [ $build_32_llc != 0 ]; then
				echo "===> LLVM IR(arch32)"
				startTime=$(date +%s)
				$ilcfe $example_path/main.il -arch 32 -incpath "$example_path/../;$example_path/" -action compile -target llvmir -output $bin_path/main_32.ll
				ilcfe_rval=$?
				if [ $ilcfe_rval != 0 ]; then
					if [ $ignore_ilcfe_pp_error != 0 ] && [ $ilcfe_rval == 234 ]; then
						echo -e "\033[33m ILCFE COMPILE ERROR, BUT IGNORED \033[0m"
					else
						echo -e "\033[31m ILCFE COMPILE ERROR \033[0m"
						continue
					fi
				else
					llc -march=x86 $bin_path/main_32.ll -filetype=obj -o $bin_path/main_ll_32.o
					if [ $? != 0 ]; then
						echo -e "\033[31m LLVM llc ERROR(OBJ) \033[0m"
						continue
					fi

					llc -march=x86 $bin_path/main_32.ll -filetype=asm -o $bin_path/main_ll_32.s
					if [ $? != 0 ]; then
						echo -e "\033[31m LLVM llc ERROR(ASM) \033[0m"
						continue
					fi

					clang -m32 -w $base_path/bin/testlib_ll_32.o $base_path/bin/testlib_c_32.o $bin_path/main_ll_32.o -o $bin_path/main_ll_32
					if [ $? != 0 ]; then
						echo -e "\033[31m CLANG COMPILER ERROR \033[0m"
						continue
					fi

					chmod +x $bin_path/main_ll_32
					$bin_path/main_ll_32
					if [ $? != 0 ]; then
						echo -e "\033[31m ERROR \033[0m"
						continue
					fi
				fi
				endTime=$(date +%s)
				cost=$((endTime - startTime))
				echo "cost(llvmir): $cost"
			fi

			# 使用ILCFE输出LLVM IR（64位）。
			if [ $build_64_llc != 0 ]; then
				echo "===> LLVM IR(arch64)"
				startTime=$(date +%s)
				$ilcfe $example_path/main.il -arch 64 -incpath "$example_path/../;$example_path/" -action compile -target llvmir -output $bin_path/main_64.ll
				ilcfe_rval=$?
				if [ $ilcfe_rval != 0 ]; then
					if [ $ignore_ilcfe_pp_error != 0 ] && [ $ilcfe_rval == 234 ]; then
						echo -e "\033[33m ILCFE COMPILE ERROR, BUT IGNORED \033[0m"
					else
						echo -e "\033[31m ILCFE COMPILE ERROR \033[0m"
						continue
					fi
				else
					llc -march=x86-64 $bin_path/main_64.ll -filetype=obj -o $bin_path/main_ll_64.o
					if [ $? != 0 ]; then
						echo -e "\033[31m LLVM llc ERROR(OBJ) \033[0m"
						continue
					fi

					llc -march=x86-64 $bin_path/main_64.ll -filetype=asm -o $bin_path/main_ll_64.s
					if [ $? != 0 ]; then
						echo -e "\033[31m LLVM llc ERROR(ASM) \033[0m"
						continue
					fi

					clang -m64 -w $base_path/bin/testlib_ll_64.o $base_path/bin/testlib_c_64.o $bin_path/main_ll_64.o -o $bin_path/main_ll_64
					if [ $? != 0 ]; then
						echo -e "\033[31m CLANG COMPILER ERROR \033[0m"
						continue
					fi

					chmod +x $bin_path/main_ll_64
					$bin_path/main_ll_64
					if [ $? != 0 ]; then
						echo -e "\033[31m ERROR \033[0m"
						continue
					fi
				fi
				endTime=$(date +%s)
				cost=$((endTime - startTime))
				echo "cost(llvmir): $cost"
			fi

			# 使用ILCFE输出C（32位）。
			if [ $build_32_clang != 0 ]; then
				echo "===> C(arch32)"
				startTime=$(date +%s)
				$ilcfe $example_path/main.il -arch 32 -incpath "$example_path/../;$example_path/" -action compile -target c -output $bin_path/main_32.c
				ilcfe_rval=$?
				if [ $ilcfe_rval != 0 ]; then
					if [ $ignore_ilcfe_pp_error != 0 ] && [ $ilcfe_rval == 234 ]; then
						echo -e "\033[33m ILCFE COMPILE ERROR, BUT IGNORED \033[0m"
					else
						echo -e "\033[31m ILCFE COMPILE ERROR \033[0m"
						continue
					fi
				else
					clang -m32 -w $bin_path/main_32.c $base_path/bin/testlib_32.c $base_path/bin/testlib_c_32.c  -o $bin_path/main_c_32
					if [ $? != 0 ]; then
						echo -e "\033[31m CLANG COMPILER ERROR(EXECUTABLE) \033[0m"
						continue
					fi

					clang -m32 -w -S $bin_path/main_32.c -o $bin_path/main_c_32.s
					if [ $? != 0 ]; then
						echo -e "\033[31m CLANG COMPILER ERROR(ASSEMBLY) \033[0m"
						continue
					fi

					chmod +x $bin_path/main_c_32
					$bin_path/main_c_32
					if [ $? != 0 ]; then
						echo -e "\033[31m ERROR \033[0m"
						continue
					fi
				fi
				endTime=$(date +%s)
				cost=$((endTime - startTime))
				echo "cost(c): $cost"
			fi

			# 使用ILCFE输出C（64位）。
			if [ $build_64_clang != 0 ]; then
				echo "===> C(arch64)"
				startTime=$(date +%s)
				$ilcfe $example_path/main.il -arch 64 -incpath "$example_path/../;$example_path/" -action compile -target c -output $bin_path/main_64.c
				ilcfe_rval=$?
				if [ $ilcfe_rval != 0 ]; then
					if [ $ignore_ilcfe_pp_error != 0 ] && [ $ilcfe_rval == 234 ]; then
						echo -e "\033[33m ILCFE COMPILE ERROR, BUT IGNORED \033[0m"
					else
						echo -e "\033[31m ILCFE COMPILE ERROR \033[0m"
						continue
					fi
				else
					clang -w $bin_path/main_64.c $base_path/bin/testlib_64.c $base_path/bin/testlib_c_64.c  -o $bin_path/main_c_64
					if [ $? != 0 ]; then
						echo -e "\033[31m CLANG COMPILER ERROR(EXECUTABLE) \033[0m"
						continue
					fi
					
					clang -w -S $bin_path/main_64.c -o $bin_path/main_c_64.s
					if [ $? != 0 ]; then
						echo -e "\033[31m CLANG COMPILER ERROR(ASSEMBLY) \033[0m"
						continue
					fi

					chmod +x $bin_path/main_c_64
					$bin_path/main_c_64
					if [ $? != 0 ]; then
						echo -e "\033[31m ERROR \033[0m"
						continue
					fi
				fi
				endTime=$(date +%s)
				cost=$((endTime - startTime))
				echo "cost(c): $cost"
			fi

			successCount=`expr $successCount + 1`

			echo -e "\033[32m OK \033[0m"
		fi
	fi
done

echo
echo
echo    "========================================"
echo	" FINISHED"
echo
echo -e " total: $totalCount, \033[32msuccess: $successCount\033[0m, \033[31mfailed: `expr $totalCount - $successCount`\033[0m"
echo    "========================================"
echo
