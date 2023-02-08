# CS3500 Lab 7 - Disk Scheduling

## Contents

Contents of this folder:
- this README,
- the problem statement,
- the source code, <em>disk-scheduler.c</em>,
- a Makefile, 
- a folder, <em>Screenshots</em>, containing the screenshots of the output, and
- the report, <em>Report.pdf</em>, containing assumptions used and observations


## Running the code

The code can be compiled by running either `make` or `make all`. <br>
The generated executable will be named <em>disk-scheduler</em>. <br>
The executable can be run according to the following specifications:
- Usage: `./disk-scheduler -h <head position> -a <scheduling algorithm>`
- Head position is an optional argument, with default value 2000
- The two arguments may be interchanged, i.e., head position may be specified first, last or not at all, as long as the flags -a and -h are specified before each argument to inform which argument is which
- Valid values of head position: Integer between 0 and 4999, both inclusive
- Valid scheduling algorithms:
    - FCFS
    - SSTF
    - SCAN
    - CSCAN
    - LOOK
    - CLOOK
- The generated executable can be removed by running `make clean`.
