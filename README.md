

## Pipe Up

low-level code to simulate the shell pipe (|) operator by creating processes for commands and connecting the output of one process to the input of the next one. The first process takes input from stdin and the last one outputs to stdout.
## Building

run "make" command to build the executable.
## Running
> ./ pipe ls
Makefile pipe pipe.c pipe.o README .md
> ./ pipe ls cat wc
5 5 38

The output of 
./pipe ls cat wc
should be the same as ls|cat|wc
## Cleaning up

run the command 
> make clean
to clean up  *.o and final exectuable
