#include<iostream>
#include<vector>
#include<deque>
#include<set>
#include<utility>
#include<climits>

using namespace std;

void ReplacementFIFO(const vector<int> &accesses, int page_frame_size) {
    int page_faults = 0;
    deque<int> page_frame;
    set<int> pages;
    for(int accessed_page : accesses) {
        if(pages.find(accessed_page) != pages.end()) 
            cout<<"No page fault"<<endl;
        else {
            if(page_frame.size() == page_frame_size) {
                int evicted_page = page_frame.front();
                page_frame.pop_front();
                pages.erase(evicted_page);
            }
            page_frame.push_back(accessed_page);
            pages.insert(accessed_page);
            page_faults++;
            for(int page : page_frame)
                cout<<page<<" ";
            cout<<endl;
        }
    }
    cout<<"Total number of page faults = "<<page_faults<<endl;
}

vector<pair<int, int>>::iterator findPage(vector<pair<int, int>> &page_frame, int page) {
    for(auto itr = page_frame.begin(); itr != page_frame.end(); itr++)
        if(itr->first == page) return itr;
    return page_frame.end();
}

void showPageFrame(const vector<pair<int, int>> &page_frame) {
    for(auto page : page_frame)
        cout<<page.first<<" ";
    cout<<endl;
}

vector<pair<int, int>>::iterator findOptimalPage(vector<pair<int, int>> &page_frame) {
    int optimal_access = 0;
    vector<pair<int, int>>::iterator ans = page_frame.end();
    for(auto itr = page_frame.begin(); itr != page_frame.end(); itr++)
        if(itr->second > optimal_access) {
            ans = itr;
            optimal_access = itr->second;
        }
    return ans;
}

int findNextAccess(const vector<int> &accesses, int page_index) {
    int page = accesses[page_index];
    for(int i = page_index+1; i < accesses.size(); i++)
        if(accesses[i] == page) return i;
    return accesses.size();
}

void ReplacementOptimal(const vector<int> &accesses, int page_frame_size) {
    int page_faults = 0;
    vector<pair<int, int>> page_frame;
    for(int i = 0; i < accesses.size(); i++) {
        int accessed_page = accesses[i];
        auto accessed_page_itr = findPage(page_frame, accessed_page);
        if(accessed_page_itr != page_frame.end()) {
            cout<<"No page fault"<<endl;
            accessed_page_itr->second = findNextAccess(accesses, i) + 1;
            continue;
        }
        page_faults++;
        if(page_frame.size() == page_frame_size) {
            page_frame.erase(findOptimalPage(page_frame));
        }
        page_frame.push_back({accessed_page, findNextAccess(accesses, i)+1});
        showPageFrame(page_frame);
    }
    cout<<"Total number of page faults = "<<page_faults<<endl;
}

vector<pair<int, int>>::iterator findLRUPage(vector<pair<int, int>> &page_frame) {
    int LR_access = INT_MAX;
    vector<pair<int, int>>::iterator ans = page_frame.end();
    for(auto itr = page_frame.begin(); itr != page_frame.end(); itr++)
        if(itr->second < LR_access) {
            ans = itr;
            LR_access = itr->second;
        }
    return ans;
}

void ReplacementLRU(const vector<int> &accesses, int page_frame_size) {
    int page_faults = 0;
    vector<pair<int, int>> page_frame;
    for(int i = 0; i < accesses.size(); i++) {
        int accessed_page = accesses[i];
        auto accessed_page_itr = findPage(page_frame, accessed_page);
        if(accessed_page_itr != page_frame.end()) {
            cout<<"No page fault"<<endl;
            accessed_page_itr->second = i+1;
            continue;
        }
        page_faults++;
        if(page_frame.size() == page_frame_size) {
            page_frame.erase(findLRUPage(page_frame));
        }
        page_frame.push_back({accessed_page, i+1});
        showPageFrame(page_frame);
    }
    cout<<"Total number of page faults = "<<page_faults<<endl;
}

int main() {
    int N;
    cout<<"Sequence Length: ";
    cin>>N;
    vector<int> accesses(N);
    cout<<"Enter Sequence: ";
    for(int i = 0; i < N; i++) cin>>accesses[i];
    int page_frame_size;
    cout<<"Page-frame size: ";
    cin>>page_frame_size;

    cout<<"FIFO - 1"<<endl<<"Optimal - 2"<<endl<<"LRU - 3"<<endl;

    int strat;
    cout<<"Enter Page replacement strategy: ";
    cin>>strat;

    switch(strat) {
    case 1:
        ReplacementFIFO(accesses, page_frame_size);
        break;
    case 2:
        ReplacementOptimal(accesses, page_frame_size);
        break;
    case 3:
        ReplacementLRU(accesses, page_frame_size);
        break;
    }

    return 0;
}