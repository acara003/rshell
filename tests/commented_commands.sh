#!/bin/sh
./bin/rshell << EOF
#ls
ls #ls
echo #nothing
echo "nothing"
echo "#nothing"
echo "nothing" #something
echo "nothing" #something "nothing"
echo "nothing" && echo #nothing
echo #nothing && echo "something"
echo """"
echo "exit"; echo "exit";
echo "#exit"; #exit
echo exit
exit
EOF
