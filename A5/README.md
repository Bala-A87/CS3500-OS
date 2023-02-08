# CS3500 Lab 5 - Thread Synchronization

## Contents

Contents of this folder:
- this README, 
- the problem statement,
- a folder for Q1, and 
- a folder for Q2

Each of the two folders contain:
- a Makefile,
- the source code - a C program, and
- a screenshot of the output

Q2 also contains a report for Q2.2


## Running the code

- For either of the 2 questions, the provided Makefile can be used to compile the code via `make all` or `make`.
- The executable generated will be named <em>phil</em> or <em>multi_pc</em> according to the question.
- The generated executable can be run as specified in the question, i.e.,
    - For Q1, it can be run as `./phil <N>`, where N is the number of philosophers
    - For Q2, it can be run as `./multi_pc -p <p> -pc <pc> -c <c> -cc <cc>`, where p, pc, c, cc are the number of producers, producer capacity, number of consumers, consumer capacity respectively [The order of the arguments is flexible, but each argument flag must be immediately followed by that argument's value]
- The generated executable can be removed for either case by running `make clean`.
