#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<stdlib.h>
#include<string.h>
#include<ctime>
#include<chrono>
#include<queue>

using namespace std;

const int BLOCK_SIZE = 128;
const int KILO = 1024;
const int INODE_INFO_SIZE = 93;
const string LOAD = "load";
const string DELETE = "delete";
const string PRINT = "print";
const string APPEND = "append";
const string DISPLAY = "display";
const string FREESPACE = "freespace";
const string PRINTINODE = "printinode";
const string EXIT = "exit";

queue<int> free_blocks;

string get_time() {
    auto now = chrono::system_clock::now();
    time_t curr_time = chrono::system_clock::to_time_t(now);
    string result = ctime(&curr_time);
    return result.substr(0, result.size()-1);
}

bool has_free_blocks() {
    return !free_blocks.empty();
}

int get_free_block() {
    int free_block = free_blocks.front();
    free_blocks.pop();
    return free_block;
}

class inode {
    string file_name;
    bool type;
    int file_size;
    string date_created;
    string date_last_modified;
    int direct_block_addresses[3];
    int index_block_address;
    vector<int> index_block;
    string file_data;

public:
    inode(string file_name) {
        this->file_name = file_name;
        type = false;
        file_size = INODE_INFO_SIZE;
        date_created = get_time();
        date_last_modified = date_created;
        direct_block_addresses[0] = get_free_block();
        direct_block_addresses[1] = direct_block_addresses[2] = index_block_address = -1;
        index_block.clear();
        file_data = "";
    }

    void print_inode() {
        cout<<"Filename: "<<file_name<<endl;
        cout<<"Size: "<<file_size<<" bytes"<<endl;
        cout<<"Date Created: "<<date_created<<endl;
        cout<<"Date Last Modified: "<<date_last_modified<<endl;
        cout<<"Direct block values: ";
        for(int i = 0; i < 3; i++) {
            if(direct_block_addresses[i] == -1) break;
            cout<<direct_block_addresses[i]<<" ";
        }
        cout<<endl;
        if(index_block_address != -1) {
            cout<<"Index block is stored in: "<<index_block_address<<endl;
            cout<<"Index block contents: ";
            for(int block : index_block) cout<<block<<" ";
            cout<<endl;
        }
    }

    void print_contents() {
        cout<<file_data<<endl;
    }
};


int main(int argc, char *argv[]) {
    int D;
    int no_blocks;
    // Handle cmd line args
    if(argc != 2) { //check later
        cout<<"Please stick to specifications"<<endl;
        exit(1);
    }
    else {
        D = atoi(argv[1]);
        no_blocks = (D * KILO)/BLOCK_SIZE;
    }
    for(int i = D; i > 0; i--) free_blocks.push(i);
    map<string, inode*> directory;
    directory.clear();
    while(true) {
        string command, filename;
        cin>>command;
        if(LOAD == command) {
            cin>>filename;
            // What if filename already exists?
            directory[filename] = new inode(filename);
        }
        else if(DELETE == command) {
            cin>>filename;

        }
        else if(PRINT == command) {
            cin>>filename;
            
        }
        else if(APPEND == command) {
            cin>>filename;

        }
        else if(DISPLAY == command) {

        }
        else if(FREESPACE == command) {

        }
        else if(PRINTINODE == command) {
            cin>>filename;

        }
        else if(EXIT == command)
            break;
        else {
            cout<<"Unknown command "<<command<<". Please enter a valid command"<<endl;
        }
    }
    return 0;
}