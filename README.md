# ilang
```C
#define int int32

function printf(fmt:*char, ...) dummy

function main(argc:int, argv:**char):int {
	printf("Hello, World!\n");
	return 0;
}
```

A C like language compiler. The compiler frontend has been basically developed, backend/assembler/linker is still under development. The frontend can output C language or LLVM IR.

一个类C语言的编译器。编译器前端已经基本完成了，可以输出C语言或者LLVM IR。后端、汇编器、链接器仍然在开发中。

## build
```shell
$ ./build.sh
$ bin/ilcfe
ilang compiler frontend version 0.1

The compiler frontend application used to compile ilang source code, and output LLVM IR or C target code.
The target code is output to standard output.

usage: ilcfe <source file> [-arch (32|64)] [-incpath <include path>] [-action (compile|printast|preprocess)]
                           [-target (llvmir|c)] {-def <name> <value>} [-output <target file>]
                           [-help]

example: ilcfe source.il -arch 64 -incpath "DIR_PATH1;DIR_PATH2;..." -action compile -target llvmir -output target.ll

options:
  -arch (32|64)                            Specify size of machine word.
  -incpath <include path>                  Specify include path, separated by ';'.
  -action (compile|printast|preprocess)    Output ir code or abstract syntax tree that represented by text.
  -target (llvmir|c|ilir)                  Output LLVM IR or C or ILIR when option '-action' is specified to 'compile'.
  -def <name> <value>                      Define a object-like macro.
  -output <target file>                    Specify a target file. write target code to standard output if target file not be specified.
  -help                                    Display available options.
```

The build result is in the directory "bin" and "ilcfe" is the frontend.

构建结果在目录“bin”下，“ilcfe”是前端。

## compile
```shell
# Target code is LLVM IR
$ ilcfe <source file> -arch 64 -action compile -target llvmir -output <target file>

# Target code is C
$ ilcfe <source file> -arch 64 -action compile -target c -output <target file>
```

## examples
More examples in the directory "test".

更多例子在目录“test”下。
