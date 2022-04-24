Description:

This project is about creating a custom shell, that will support some of the original shell commands.
This project will help us to understand how the shell actualy works, and also about the logic of proccess inside a real shell.
___________________________________________________________________

Environment:

OS: Ubonto os (Linux)

requierments to install:
                gcc compiler
                sudo apt-get install libreadline-dev
                sudo apt install tree

How to compile and run:
    compile:        gcc -o main main.c -lreadline
    run:            ./main
    compile+run:    gcc -o main main.c -lreadline;./main
___________________________________________________________________

How to Run Your Program:

This program works exactly as shell but with limited commands.

the commands that our custom shell support:
pwd, cd, nano, cat, cat > <file>, wc (-l, -c, -w), cp, pipe (coomand1 | command 2), clear, 
grep (-c), wget, ls (-l), tree, exit.

Exampels:

Example (1) - command: pwd
    gil : /home/gil/Desktop/hw2: >> pwd
    /home/gil/Desktop/hw2

Example (2) - command: ls -l 

    gil : /home/gil/Desktop/hw2: >> ls -l
    total 40
    -rw-rw-r-- 1 gil gil  6123 Apr  8 07:02 funcs.c
    -rw-rw-r-- 1 gil gil   760 Apr  8 07:02 headers.h
    -rwxrwxr-x 1 gil gil 18040 Apr  8 07:22 main
    -rw-rw-r-- 1 gil gil  1034 Apr  8 07:02 main.c
    -rw-rw-r-- 1 gil gil  1193 Apr  8 07:22 readme.txt

Example (3) - command: ls | wc -w

    gil : /home/gil/Desktop/hw2: >> ls | wc -w
    5
