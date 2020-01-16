CC=gcc
CFlags32=-o0 -g -std=c99 -m32
CFlags64=-o0 -g -std=c99 -m64

CommonSource=$(wildcard *.c)
CommonHeader=$(wildcard *.h)

CompilerFrontendSource=$(wildcard compiler/frontend/*.c)
CompilerFrontendHeader=$(wildcard compiler/frontend/*.h)

CompilerBackendSource=$(wildcard compiler/backend/*.c)
CompilerBackendHeader=$(wildcard compiler/backend/*.h)

CompilerPreprocessorSource=$(wildcard compiler/preprocessor/*.c)
CompilerPreprocessorHeader=$(wildcard compiler/preprocessor/*.h)

AssemblerSource=$(wildcard compiler/assembler/*.c)
AssemblerHeader=$(wildcard compiler/assembler/*.h)

CompilerSource=$(wildcard compiler/*.c)
CompilerHeader=$(wildcard compiler/*.h)

CompilerFrontendRequest=$(CommonSource) $(CommonHeader)	\
				$(CompilerSource) $(CompilerHeader)	\
				$(CompilerFrontendSource) $(CompilerFrontendHeader)	\
				$(CompilerPreprocessorSource) $(CompilerPreprocessorHeader)

CompilerBackendRequest=$(CommonSource) $(CommonHeader)	\
				$(CompilerSource) $(CompilerHeader)	\
				$(CompilerBackendSource) $(CompilerBackendHeader)	\
				$(CompilerPreprocessorSource) $(CompilerPreprocessorHeader)

AssemblerRequest=$(CommonSource) $(CommonHeader)	\
				$(CompilerSource) $(CompilerHeader)	\
				$(AssemblerSource) $(AssemblerHeader)	\
				$(CompilerPreprocessorSource) $(CompilerPreprocessorHeader)

all64: bin/ilcfe64 bin/ilcbe64 bin/iasm64

all32: bin/ilcfe32 bin/ilcbe32 bin/iasm32

bin/ilcfe64: bin/ $(CompilerFrontendRequest)
	$(CC) $(CFlags64) $(CommonSource) $(CompilerSource) $(CompilerFrontendSource) $(CompilerPreprocessorSource) -o bin/ilcfe

bin/ilcbe64: bin/ $(CompilerBackendRequest)
	$(CC) $(CFlags64) $(CommonSource) $(CompilerSource) $(CompilerBackendSource) $(CompilerPreprocessorSource) -o bin/ilcbe

bin/iasm64: bin/ $(AssemblerRequest)
	$(CC) $(CFlags64) $(CommonSource) $(CompilerSource) $(AssemblerSource) $(CompilerPreprocessorSource) -o bin/iasm

bin/ilcfe32: bin/ $(CompilerFrontendRequest)
	$(CC) $(CFlags32) $(CommonSource) $(CompilerSource) $(CompilerFrontendSource) $(CompilerPreprocessorSource) -o bin/ilcfe

bin/ilcbe32: bin/ $(CompilerBackendRequest)
	$(CC) $(CFlags32) $(CommonSource) $(CompilerSource) $(CompilerBackendSource) $(CompilerPreprocessorSource) -o bin/ilcbe

bin/iasm32: bin/ $(AssemblerRequest)
	$(CC) $(CFlags32) $(CommonSource) $(CompilerSource) $(AssemblerSource) $(CompilerPreprocessorSource) -o bin/iasm

bin/:
	-mkdir bin/

clean:
	rm -rf bin

.PHONY: clean
