flex ijcompiler.l &&
bison --defines=y.tab.h ijcompiler.y &&
gcc -o ijcompiler *.c -ll -ly -g

rm ijcompiler.zip
zip -r ijcompiler.zip ijcompiler.l ijcompiler.y astNodes.c astNodes.h show.c show.h symbols.c symbols.h semantic.c semantic.h exitClean.h codeGen.c codeGen.h

