#!/bin/sh
./bin/rshell << EOF
#-e flag check

test -e main.cpp
[ -e main.cpp ]
test -e fakefile.cpp
[ -e fakefile.cpp ]
test main.cpp
[ main.cpp ]
test -e main.cpp && echo Yay
test -e fakefile.cpp || echo Yay

#-f flag check

test -f main.cpp
test -f src
[ -f main.cpp ]
[ -f src ]
test -f main.cpp && echo 1
test -f src || echo 1

#-d flag check

test -d main.cpp
test -d src 
[ -d main.cpp ]
[ -d src ]
src -d src && echo 1
src -d main.cpp || echo 1

#works with rshell 

test -e main.cpp && echo 1 || echo 2
test -e fakefile.cpp || echo 1; echo 2
echo 1 && src -e main.cpp
test -f main.cpp && echo 1 || echo 2
test -f fakefile.cpp || echo 1; echo 2
echo 1 && src -f main.cpp
test -d test && echo A || echo 
test -d fakefile || echo 1; echo 2
echo 1 && src -d src
test -e main.cpp #&& echo 1
test -e fakefile && exit
test -e main.cpp; exit
exit
EOF

