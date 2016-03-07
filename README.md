# RSHELL 
Shell Terminal for UCR CS100

## Installation
To install and run rshell on your computer make sure you open the terminal and type the following commands:

```
1. git clone https://www.github.com/acara003/rshell.git

2. cd rshell

3. git checkout hw3 

4. make

5. make run OR ./bin/rshell
```

##Features

-Supports all the bash commands located in /bin.

-Supports connectors such as ";", "&&", and "||".

-";" connector works: the next command is always executed.

-"&&" connector works: the next command is executed if the first one succeeds.

-"||" connector works: the next command will execute if the first one fails.

-"#" symbol works: Anything after this symbol will be a comment.

-To exit the program, simply type "exit".

-You can test any file with "test" or brackets with the following flags: -e, -f, or -d (-e will be defaulted to if no flag is provided)

-You can execute files with precedence by using parenthesis 

##Limitations

-Using more than 2 && or || or more than 1 ; will still treat it as a normal operand and not break

-Having one symbol still runs

-uses boost library so you need to install boost when using this program.

-if you use (ls &&), it will execute ls.

-if you use (&& ls ), it will execute ls too.

-Hostname supports only 150 characters at maximum.
 
 -if you echo with "" around the word, it will echo the "" aswell
 
 -requires perfect syntax to work (otherwise it will throw errors)
 
 -sometimes when you pull random lines from different files will be put into the source code. If it does not compile please let us know
 
### Installation
To install and run rshell on your computer, open terminal and type the following commands:

```
1. git clone http://www.github.com/acara003/rshell.git

2. cd rshell

3. git checkout hw3

4. make

5. make run OR ./bin/rhsell
```
