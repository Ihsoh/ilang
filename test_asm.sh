./build_64.sh
bin/iasm test1/test.asm -action printast -output bin/test_ast.txt
echo 'AST builded'
bin/iasm test1/test.asm -action compile -output bin/test.bin
echo 'bin builded'
