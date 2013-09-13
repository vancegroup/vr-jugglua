#!/bin/sh -e
format=$(which clang-format || which clang-format-3.4)

styleDirectory() {
	find $1 \( -name "*.cpp" -o -name "*.h" -o -name "*.c" \) -print0 | xargs -0 $format -style=file -i 
}

(
cd $(dirname $0) && cd ..
styleDirectory src
styleDirectory vrjugglua
)
