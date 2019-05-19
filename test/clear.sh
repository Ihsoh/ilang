#!/bin/bash

# 获取脚本所在目录。
base_path=$(cd `dirname $0`; pwd)

bin_path=$base_path/bin
if [ -d $bin_path ]; then
	rm -rf $bin_path
fi

# 获取所有测试例子的目录。
for example_path in $base_path/$1*; do
	if [ -d $example_path ]; then
		if [ -f $main_il_path ]; then
			bin_path=$example_path/bin
			if [ -d $bin_path ]; then
				rm -rf $bin_path
			fi
		fi
	fi
done
