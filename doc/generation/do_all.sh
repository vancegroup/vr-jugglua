#!/bin/sh
cd $(dirname $0)
(
	cd bootstrap && ./refresh_all.sh
)
./main.lua
./osgLua_docs.lua
./output_html_docs.sh
./upload_html_docs.sh

