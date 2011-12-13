Discount 2.1.2
http://www.pell.portland.or.us/~orc/Code/discount/

Built for Win32 using MinGW32 by Ryan Pavlik, 13 Dec 2011.

VERSION renamed to VERSIONINFO and changed in makefile.in accordingly to support
not confusing "make" on a case-insensitive system.

Configured and built with:
CFLAGS="-O2" CC=/mingw/bin/gcc ./configure.sh --with-fenced-code --with-github-tags --with-id-anchor && make markdown

