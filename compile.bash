flex ijparser.l &&
bison --defines=y.tab.h ijparser.y &&
gcc -o ijparser *.c -ll -ly -g

rm ijparser.zip
zip -r ijparser.zip ijparser.l ijparser.y astNodes.c astNodes.h show.c show.h symbols.c symbols.h

