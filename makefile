CC=gcc
CFlags=-o0 -g -std=c99

CommonSource=$(wildcard *.c)
CommonHeader=$(wildcard *.h)

CompilerFrontendSource=$(wildcard compiler/frontend/*.c)
CompilerFrontendHeader=$(wildcard compiler/frontend/*.h)

CompilerBackendSource=$(wildcard compiler/backend/*.c)
CompilerBackendHeader=$(wildcard compiler/backend/*.h)

CompilerPreprocessorSource=$(wildcard compiler/preprocessor/*.c)
CompilerPreprocessorHeader=$(wildcard compiler/preprocessor/*.h)

CompilerSource=$(wildcard compiler/*.c)
CompilerHeader=$(wildcard compiler/*.h)

all: bin/ilcfe bin/ilcbe

bin/ilcfe: bin/ $(CommonSource) $(CommonHeader)	\
				$(CompilerSource) $(CompilerHeader)	\
				$(CompilerFrontendSource) $(CompilerFrontendHeader)	\
				$(CompilerPreprocessorSource) $(CompilerPreprocessorHeader)
	$(CC) $(CFlags) $(CommonSource) $(CompilerSource) $(CompilerFrontendSource) $(CompilerPreprocessorSource) -o bin/ilcfe

bin/ilcbe: bin/ $(CommonSource) $(CommonHeader)	\
				$(CompilerSource) $(CompilerHeader)	\
				$(CompilerBackendSource) $(CompilerBackendHeader)	\
				$(CompilerPreprocessorSource) $(CompilerPreprocessorHeader)
	$(CC) $(CFlags) $(CommonSource) $(CompilerSource) $(CompilerBackendSource) $(CompilerPreprocessorSource) -o bin/ilcbe

bin/:
	-mkdir bin/

clean:
	rm -rf bin

.PHONY: clean
