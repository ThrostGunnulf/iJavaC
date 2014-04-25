flex ijparser.l
bison --defines=y.tab.h ijparser.y
gcc -o ijparser *.c -ll -ly

rm ijparser.zip
zip -r ijparser.zip ijparser.l ijparser.y

