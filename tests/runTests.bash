#!/bin/bash

NUMBER_TESTS=17

cd ..
bash compile.bash > /dev/null
cd tests

rm -r -f diffs/
mkdir diffs/

cp ../ijcompiler ijcompiler

i=1;
while true
do
	programName='test'$i
	programFileName=$programName'.java'
	diffFileName='diffs/diff'$i'.txt'
	
	if ! [[ -s $programFileName ]] ; then
		break
	fi ;
	
	echo -n $programName': '
	
	javac $programFileName && java $programName `cat 'args/test'$i'args.txt'` &> 'diffs/out'$i'_java.txt'
	
	./ijcompiler < $programFileName > $programName'.ll' && lli $programName'.ll' `cat 'args/test'$i'args.txt'` &> 'diffs/out'$i'_ijava.txt'
	
	diff 'diffs/out'$i'_java.txt' 'diffs/out'$i'_ijava.txt' > 'diffs/diff'$i'.txt'

	if [[ -s $diffFileName ]] ; then
		echo "ERRORS"
	else
		echo "OK"
	fi ;
	
	let i=$i+1
done

