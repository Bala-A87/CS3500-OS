#include<iostream>
#include<queue>
#include<vector>
#include<climits>
#include<algorithm>

#define INF INT_MAX
#define SYS 0
#define IP 1
#define IEP 2
#define BP 3
#define SP 4

using namespace std;

class Process {
public:
    string process_id;
    int type_of_process, arrival_time, burst_time, priority;
    int remaining_time;
    int completion_time, turnaround_time, waiting_time;

    Process(string process_id, int type_of_process, int arrival_time, int burst_time, int priority) {
        this->process_id = process_id;
        this->type_of_process = type_of_process;
        this->arrival_time = arrival_time;
        this->burst_time = burst_time;
        remaining_time = burst_time;
        this->priority = priority;
    }

    void setData(string process_id, int type_of_process, int arrival_time, int burst_time, int priority) {
        this->process_id = process_id;
        this->type_of_process = type_of_process;
        this->arrival_time = arrival_time;
        this->burst_time = burst_time;
        remaining_time = burst_time;
        this->priority = priority;
    }

    void setTimes(int completion_time) {
        this->completion_time = completion_time;
        turnaround_time =  completion_time - arrival_time;
        waiting_time = turnaround_time - burst_time;
    }

    void printData() {
        cout<<"Process "<<process_id
        <<" Completion time: "<<completion_time
        <<", Turnaround time: "<<turnaround_time
        <<", Waiting time: "<<waiting_time<<endl;
    }
};

// Priority queue comparators
struct maxPriority {
    bool operator() (const Process *p1, const Process *p2) const {
        if(p1->priority == p2->priority) return p1->arrival_time > p2->arrival_time;
        return p1->priority > p2->priority;
    }
};
struct firstArrival {
    bool operator() (const Process *p1, const Process *p2) const {
        return p1->arrival_time > p2->arrival_time;
    }
};
struct shortestJob {
    bool operator() (const Process *p1, const Process *p2) const {
        if(p1->remaining_time == p2->remaining_time) return p1->arrival_time > p2->arrival_time;
        return p1->remaining_time > p2->remaining_time;
    }
};

vector<Process*> processes;
vector<string> execution_sequence;
priority_queue<Process*, vector<Process*>, maxPriority> sys;
priority_queue<Process*, vector<Process*>, firstArrival> ip, iep;
priority_queue<Process*, vector<Process*>, shortestJob> bp, sp;
priority_queue<Process*, vector<Process*>, firstArrival> arriving_processes;
int next_arrival, current_time, active_queue;
string prev_pid;

void printOutput() {
    // Prints the necessary data
    cout<<"Execution sequence:"<<endl;
    int i = 0;
    while(i < execution_sequence.size()) {
        for(int count = 0; count < 9 && i < execution_sequence.size(); count++, i++) {
            cout<<execution_sequence[i]<<" ";
        }
        cout<<endl;
    }
    cout<<endl<<"Completion, turnaround and waiting times"<<endl;
    for(Process *process: processes)
        process->printData();
}

int getProcessType(string type_of_process) {
    // Converts process type from string to int
    if(type_of_process == "sys") 
        return SYS;
    if(type_of_process == "ip")  
        return IP;
    if(type_of_process == "iep") 
        return IEP;
    if(type_of_process == "bp")  
        return BP;
    if(type_of_process == "sp")  
        return SP;
    return -1;
}

void pushIntoQueue(Process *next_process) {
    //push into corresponding queue 
    int process_type = next_process->type_of_process;
    switch(process_type) {
        case SYS:
            sys.push(next_process);
            break;
        case IP:
            ip.push(next_process);
            break;
        case IEP:
            iep.push(next_process);
            break;
        case BP:
            bp.push(next_process);
            break;
        case SP:
            sp.push(next_process);
            break;
    }
}

void addArrivals() {
    // Find processes arriving at current time and push into corresponding queues
    while(!arriving_processes.empty() && arriving_processes.top()->arrival_time == next_arrival) {
        pushIntoQueue(arriving_processes.top());
        arriving_processes.pop();
    }
}

bool emptyActive() {
    // checks whether the active queue is empty
    switch(active_queue) {
        case SYS:
            return sys.empty();
        case IP:
            return ip.empty();
        case IEP:
            return iep.empty();
        case BP:
            return bp.empty();
        case SP:
            return sp.empty();
    }
    return true;
}

int nextActive() {
    // Finds the next queue after the current active queue that is non-empty and can be made active
    for(int i = (active_queue + 1)%5; i != active_queue; i = (i+1)%5) {
        switch(i) {
            case SYS:
                if(!sys.empty()) return SYS;
                break;
            case IP:
                if(!ip.empty()) return IP;
                break;
            case IEP:
                if(!iep.empty()) return IEP;
                break;
            case BP:
                if(!bp.empty()) return BP;
                break;
            case SP:
                if(!sp.empty()) return SP;
                break;
        }
    }
    return -1;
}

Process *getNextProcess() {
    // Finds the next process to execute
    Process *temp = NULL;
    switch(active_queue) {
        case SYS:
            temp = sys.top();
            sys.pop();
            break;
        case IP:
            temp = ip.top();
            ip.pop();
            break;
        case IEP:
            temp = iep.top();
            iep.pop();
            break;
        case BP:
            temp = bp.top();
            bp.pop();
            break;
        case SP:
            temp = sp.top();
            sp.pop();
            break;
    }
    return temp;
}

int main() {
    int quantum_time, N;
    cin>>quantum_time>>N;
    current_time = 0;
    for(int i = 0; i < N; i++) {
        string process_id, type_of_process;
        int arrival_time, burst_time, priority;
        cin>>process_id>>arrival_time>>burst_time>>type_of_process>>priority;
        processes.push_back(new Process(process_id, getProcessType(type_of_process), arrival_time, burst_time, priority));
        arriving_processes.push(processes[i]);
    }
    prev_pid = "";

    next_arrival = arriving_processes.top()->arrival_time;
    active_queue = 0; // The currently active queue number/process type
    int quantum_end_time = quantum_time; // The time at which the current time quantum ends

    while(current_time != INF) { 
        if(current_time == next_arrival) addArrivals(); // There are arrivals occurring at the current time, so add them
        if(!arriving_processes.empty()) next_arrival = arriving_processes.top()->arrival_time; // Find the next arrival time
        else next_arrival = INF; // No more processes => set next_arrival to "infinity"

        // Check if active queue is empty, if empty determine the next active queue
        if(emptyActive()) active_queue = nextActive();
        if(active_queue == -1) { // If there are no active queues currently, move to the instant when there are more arrivals
            current_time = next_arrival; // This ensures the loop breaking, since next_arrival is eventually set to infinity
            // And when there are no more processes, current_time gets set to next_arrival, which is infinity, and the loop breaks
            active_queue = 0;
            continue;
        }

        Process *current_process = getNextProcess();
        int start_time = current_time; // The time when the current process begins execution
        if(prev_pid != current_process->process_id) execution_sequence.push_back(current_process->process_id);
        // Checks whether the process has been preempted or not, if not preempted, it doesn't add to execution sequence again
        int process_end_time = current_time + current_process->remaining_time;
        int stop_time = min(process_end_time, min(quantum_end_time, next_arrival));
        // The time at which the process either ends or is stopped due to end of quantum or (possible) preemption

        // First check burst time to see if process ends, then check end of quantum time to switch active queue, then check for next arrival time
        if(stop_time == process_end_time) current_process->setTimes(stop_time); // We are ready to calculate the required times
        else {
            current_process->remaining_time -= (stop_time - start_time);
            pushIntoQueue(current_process); //The process was popped by getNextProcess, so since it hasn't ended yet, we push it back in
        }

        if(stop_time == quantum_end_time) { // Move to the next queue 
            active_queue = (active_queue + 1)%5;
            quantum_end_time += quantum_time;
        }

        current_time = stop_time;
        prev_pid = current_process->process_id;
    }

    printOutput();
    return 0;
}