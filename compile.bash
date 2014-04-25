flex ijparser.l
bison -d ijparser.y
gcc -o ijparser *.c -ll -ly

rm ijparser.zip
zip -r ijparser.zip ijparser.l ijparser.y

