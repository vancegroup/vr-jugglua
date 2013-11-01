#!/bin/sh -x

outfile=../../vrjugglua/vrjlua.ico
(
cd $(dirname $0)
fnames=""
for res in 16 32 48 128 256; do
    inkscape --without-gui --file=128px.svg --export-png=${res}px.png --export-area-page --export-width=${res}
    fnames="${fnames} ${res}px.png"
done
rm -f ${outfile}
convert -background transparent ${fnames} ${outfile}
)
