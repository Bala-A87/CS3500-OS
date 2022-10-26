CS3500 Lab 7 - Disk Scheduling

CONTENTS OF SUBMISSION:

The submitted zip file will expand to a folder, 'CS20B012', containing the following:
    - this README,
    - the source code, disk-scheduler.c,
    - a Makefile, 
    - a folder, 'Screenshots', containing the screenshots of the output, and
    - the report, Report.pdf, containing assumptions used and observations


RUNNING THE CODE:

The code can be compiled by running either 'make' or 'make all'.
The generated executable will be named 'disk-scheduler'.
The executable can be run according to the following specifications:
    Usage: ./disk-scheduler -h <head position> -a <scheduling algorithm>
    Head position is an optional argument, with default value 2000
    The two arguments may be interchanged, i.e., head position may be specified first, last or not at all
    As long as the flags -a and -h are specified before each argument to inform which argument is which
    Valid values of head position: Integer between 0 and 4999, both inclusive
    Valid scheduling algorithms:
        FCFS
        SSTF
        SCAN
        CSCAN
        LOOK
        CLOOK
The generated executable can be removed by running 'make clean'.
