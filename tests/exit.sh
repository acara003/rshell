#!/bin/sh

./bin/rshell << EOF
echo exit; echo "echo"
echo #exit && echo "exit"
exit || exit;
EOF
./bin/rshell << EOF
ls || exit
sl || exit
EOF
./bin/rshell << EOF
exit
EOF
