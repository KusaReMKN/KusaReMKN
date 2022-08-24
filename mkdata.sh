#! /bin/sh

# Generate Sample Data

git clone https://github.com/kusaremkn/atcoder.git temp
mkdir -p data
SUFF=`find temp/* -type f | sed -e's/.*\.//' | sort | uniq`
for e in $SUFF; do
	DEST="./data/$e.kusaremkn"
	NUMB=`find temp/* -type f | grep "\.$e" | wc -l`
	echo $NUMB @@@ >$DEST
	for s in `find temp/* -type f | grep "\.$e"`; do
		cat $s >>$DEST
		echo @@@ >>$DEST
	done
done
