#include<iostream>
#include<queue>
#include<vector>
#include<algorithm>

using namespace std;

class Block {
public:
    int number, size;
    void makeBlock(const int &number, const int &size) {
        this->number = number;
        this->size = size;
    }
};

struct BestFit {
    bool operator() (const Block &b1, const Block &b2) const {
        if(b1.size == b2.size) return b1.number > b2.number;
        return b1.size > b2.size;
    }
};

struct WorstFit {
    bool operator() (const Block &b1, const Block &b2) const {
        if(b1.size == b2.size) return b1.number > b2.number;
        return b1.size < b2.size;
    }
};

void printAllocated(const int &process_number, const int &process_size, const Block &allocated_block) {
    cout<<process_number<<"\t"<<process_size<<"\t"<<allocated_block.size<<"\t"<<allocated_block.number<<endl;
}

void printNotAllocated(const int &process_number, const int &process_size) {
    cout<<process_number<<"\t"<<process_size<<"\t"<<"Not Allocated"<<endl;
}

int main() {
    int N, M;
    cout<<"Enter Number of Blocks: ";
    cin>>N;
    vector<Block> blocks(N);
    cout<<"Enter "<<N<<" block sizes: ";
    for(int i = 0; i < N; i++) {
        int block_size;
        cin>>block_size;
        blocks[i].makeBlock(i+1, block_size);
    }
    cout<<"Enter Number of Processes: ";
    cin>>M;
    vector<int> processes(M);
    cout<<"Enter "<<M<<" process sizes: ";
    for(int i = 0; i < M; i++) cin>>processes[i];
    priority_queue<Block, vector<Block>, BestFit> best_fit(blocks.begin(), blocks.end());
    priority_queue<Block, vector<Block>, WorstFit> worst_fit(blocks.begin(), blocks.end());

    cout<<"First-Fit"<<endl;
    cout<<"Process number | Process size | Block size | Block number"<<endl;
    for(int i = 0; i < processes.size(); i++) {
        bool allocated = false;
        for(auto itr = blocks.begin(); itr != blocks.end(); itr++) {
            if(itr->size >= processes[i]) {
                printAllocated(i+1, processes[i], *itr);
                blocks.erase(itr);
                allocated = true;
                break;
            }
        }
        if(!allocated) 
            printNotAllocated(i+1, processes[i]);
    }

    cout<<"Best-Fit"<<endl;
    cout<<"Process number | Process size | Block size | Block number"<<endl;
    for(int i = 0; i < processes.size(); i++) {
        vector<Block> displaced;
        bool allocated = false;
        while(!best_fit.empty()) {
            Block smallest_block = best_fit.top();
            best_fit.pop();
            if(smallest_block.size >= processes[i]) {
                printAllocated(i+1, processes[i], smallest_block);
                allocated = true;
                break;
            }
            else
                displaced.push_back(smallest_block);
        }
        if(!allocated)
            printNotAllocated(i+1, processes[i]);
        for(Block displaced_block : displaced) best_fit.push(displaced_block);
    }

    cout<<"Worst-Fit"<<endl;
    cout<<"Process number | Process size | Block size | Block number"<<endl;
    for(int i = 0; i < processes.size(); i++) {
        if(worst_fit.empty())
            printNotAllocated(i+1, processes[i]);
        else {
            Block largest_block = worst_fit.top();
            if(largest_block.size >= processes[i]) {
                printAllocated(i+1, processes[i], largest_block);
                worst_fit.pop();
            }
            else
                printNotAllocated(i+1, processes[i]);
        }
    }

    return 0;
}