#!/bin/bash

if [ "$1" == "-h" ] || [ "$1" == "--help" ]; then
	cat <<EOF
Include Comparison
$0 [ [extra arguments to gcc like -I] -- ] [files to analyze]

Outputs data in a CSV-style format, first column is filename as passed,
second column is total number of includes performed, third column is
number of unique includes performed.

EOF
	exit 1
fi

extraargs=

if echo "$*" | egrep -e "--" > /dev/null; then
	while [ "$1" != "--" ]; do
		extraargs="${extraargs} ${1}"
		shift
	done
	shift
	#echo "Will pass these arguments to all instantiations: $extraargs" >&2
fi

for fn; do
	cleanfn=$(echo ${fn} | sed -e 's/_/ /g')
	includes=$(g++ -E -M -H ${extraargs} ${fn} 2>&1 | grep "^\..*" | sed 's/^\.* \(.*\)/\1/')
	count=$(echo "${includes}" | wc -l)
	uniques=$(echo "${includes}" | sort | uniq | wc -l)
	dupes=$(expr $count - $uniques)
	echo -e "\"${cleanfn}\",${count},${uniques},${dupes}"
done

