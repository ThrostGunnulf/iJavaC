lex ijscanner.l
bison -v -d ijparser.y
gcc -o ijparser lex.yy.c y.tab.c -ll -Wall

#rm ijparser.zip
#zip -r ijparser.zip ijscanner.l ijparser.y

