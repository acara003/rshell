#!/bin/sh
./bin/rshell << EOF
#Check precedence general

(echo A)
(ls -j)

#Check precedence with rshell

(ls -j) || (echo A)
(echo 1) && (echo 1)
(echo 1) #&& (echo 2)
((echo 1 && echo 2) || echo 3)
((echo 1 && echo 2) && echo 3)
((echo 1 && echo 2) || (echo 3 && echo 4))
((echo 1 || echo 2); (echo 3 || echo 4))
exit
EOF
