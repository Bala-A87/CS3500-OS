# Lab 4: CPU scheduling

## Multilevel Queue Scheduling

### Submission contents:
- This README
- The source code: multiLevelQueue.cpp
- A sample input (from the pdf): sample_input.txt
- Screenshot of terminal output for sample_input.txt: sample_output.png

### Running instructions:
- Compile the code as "g++ multiLevelQueue.cpp [-o \<executableName\>]"
- Run the executable as "./\<executableName\>" (or "./a.out")
- Redirection may be used to read input from a source file or write output to a file

### Input format:
- First enter two space-separated (or in two lines) integers corresponding to quantum time and N (number of processes) in order
- Then enter N lines, the ith line containing process id, arrival time, burst time, process type, and priority of ith process in the mentioned order
- Arrival time, burst time and priority: 32-bit integers (though negative values would be accepted, I wouldn't recommend entering them!)
- Process id, process type: strings
- Valid process types:
    1. sys - for system processes
    2. ip  - for interactive processes
    3. iep - for interactive editing processes
    4. bp  - for batch processes
    5. sp  - for student processes

### Output format:
- The execution sequence: A few lines with each line having at most 9 processes, with overall order being the order of their execution (the order in which the processes start/resume execution)
- The required times: Completion, turnaround and waiting times for all the processes, in the same order as they are entered in the input
