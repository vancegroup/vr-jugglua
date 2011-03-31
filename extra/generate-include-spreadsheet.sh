#!/bin/bash
if [ "x$1" == "x" ];then
	echo "Must pass build directory as first parameter!" >&2
	exit 1
fi

SRC_IN_RELATION_TO_EXTRA=".."

#####
# Variable creation
# Don't need to edit
EXTRA="$(dirname $(readlink -f ${0}))"
BUILD="$(readlink -f ${1})"
SRC="$(readlink -f ${EXTRA}/${SRC_IN_RELATION_TO_EXTRA})"
##########


SPREADSHEET="${EXTRA}/include-spreadsheet.csv"
SORTED="${EXTRA}/include-spreadsheet-sorted.csv"
FLAGS="-I${BUILD} -I${SRC} -I${SRC}/third-party/luabind -I${SRC}/third-party/lua-5.1.4/src -I${SRC}/third-party/osgLua/include -DEMBEDDED_LUA -DOSGLUA_LIBRARY_STATIC -DLUABIND_MAX_BASES=3 -DLUABIND_MAX_ARITY=3 -x c++"


#echo "EXTRA: ${EXTRA}"
#echo "BUILD: ${BUILD}"
#echo "SRC: ${SRC}"
#echo "SPREADSHEET: ${SPREADSHEET}"


####
# Actually create the spreadsheet.
if [ "x$2" == "x" ]; then
	# Replace existing contents with header row
	cat > "${SPREADSHEET}" <<-EOF
	"Filename","Total Includes","Unique Includes","Duplicate Includes"
	EOF

	# VR JuggLua headers
	echo "Processing VR JuggLua"
	cd "${SRC}"
	"${EXTRA}/includecomparison.sh" ${FLAGS} -- vrjugglua/*.h >> "${SPREADSHEET}"

	# osgLua headers
	echo "Processing osgLua"
	cd "${SRC}/third-party/osgLua/include"
	"${EXTRA}/includecomparison.sh" ${FLAGS} -- osgLua/* >> "${SPREADSHEET}"

	# luabind headers
	echo "Processing luabind"
	cd "${SRC}/third-party/luabind"
	"${EXTRA}/includecomparison.sh" ${FLAGS} -- luabind/*.hpp >> "${SPREADSHEET}"

	echo "Sorting"
	sort --field-separator=, --key=2 -g "${SPREADSHEET}" > "${SORTED}"
fi

echo "Graphing"
cd ${EXTRA}
gnuplot <<EOF

set term postscript \
	enhanced \
	eps \
	color \
	size 7.5,10
set output "| ps2pdf - includes.pdf"
set boxwidth 0.9 absolute
set style fill solid 1.00 border -1
set style data histogram
set style histogram rowstacked

set auto x
set xtic rotate by -90 scale 0
set xtics nomirror rotate by -90
set xlabel "Header File" rotate by -90

set ytics 100  rotate by -90
set ylabel "Includes" rotate by -90



# load csv
set datafile separator ","

# every ::2 skips the first line
plot "${SORTED}" every ::2 using 3 title "Unique includes", \
	'' every ::2 using 4:xtic(1) title "Repeat includes"

EOF

