CC=gcc
CFlags=-o0 -g
CommonSource=$(wildcard *.c)
CommonHeader=$(wildcard *.h)
CompilerFrontendSource=$(wildcard compiler/frontend/*.c)
CompilerFrontendHeader=$(wildcard compiler/frontend/*.h)
CompilerPreprocessorSource=$(wildcard compiler/preprocessor/*.c)
CompilerPreprocessorHeader=$(wildcard compiler/preprocessor/*.h)

bin/ilcfe: bin/ $(CommonSource) $(CommonHeader) $(CompilerFrontendSource) $(CompilerFrontendHeader) $(CompilerPreprocessorSource) $(CompilerPreprocessorHeader)
	$(CC) $(CFlags) $(CommonSource) $(CompilerFrontendSource) $(CompilerPreprocessorSource) -o bin/ilcfe

bin/:
	-mkdir bin/

clean:
	rm -rf bin

.PHONY: clean
