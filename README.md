# Linux Shell - myShell8804

Aristotle University of Thessaloniki - Electrical and Computer Engineering  
Course : Operating Systems  
Author : [Emmanouil Christos](https://github.com/eachristgr)

------

This repository contains the assignment for the course of Operating Systems. The purpose of it was to implement a command line interpreter, capable of running in interactive or batch mode.

## Run Shell

- **Create Executable File**

In order to create the executable file, first you can install make utility as shown in the following command :

```bash
$ sudo apt install make
```

Then move in the directory of the **Makefile** and **myShell8804.c** files and run the bellow command :

```bash
$ make
```

- **Run Shell**

If you are in the same directory as the executable fil, you can run the shell in interactive mode by the following command :

```bash
$ ./myShell8804
```

Or in batch mode by the following command :

```bash
$ ./myShell8804 bash_file.sh
```

## Shell's Features

The commands that the shell can accept are :

- Linux terminal commands that do not change the directory.
- Build-In command **quit** that prints GoodBye!!! message and exit the shell.

The shell allows more commands per line, by using the following special characters :

- **( ; )** - The commands separated by the character **( ; )** should be executed sequentially, regardless of the successful or unsuccessful completion of the left command.
- **( && )** - The commands separated by the character **( && )** should be executed sequentially, only if the left command return successfully.

The shell allows redirections, by using the following special characters :

- **( < )** - When this symbol is followed by an existing file, redirects the command entry.
- **( > )** - When this symbol is followed by a file, redirects the output of the command (if the file does not exist it creates it with full permissions).

Below are some examples of commands that can be run on the shell :

```bash
ls
ls -l ; pwd
touch test && quit
cat < filename1 && echo hello > filename2 ; ls
```

In case of an error, appropriate messages are displayed, however some errors may lead to the exit from the shell.

------

- More information and examples can be found in [report.pdf](./report.pdf).
