#!/bin/sh
cd $(dirname $0)
rsync -r -t -v --delete docs/ keymaker:/home/users/rpavlik/public_html/doxygen/vr-jugglua/
