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

bool has_free_blocks(int no_blocks) {
    return free_blocks.size() >= no_blocks;
}

int get_free_block() {
    int free_block = free_blocks.front();
    free_blocks.pop();
    return free_block;
}

void print_no_space_error() {
    cout<<"ERROR: Disk has insufficient space for this operation"<<endl;
}

void file_dne_error(string filename) {
    cout<<"ERROR: File "<<filename<<" does not exist"<<endl;
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
    int no_blocks_used;

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
        no_blocks_used = 1;
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

    void delete_file() {
        for(int i = 0; i < 3;  i++) {
            if(direct_block_addresses[i] == -1) break;
            free_blocks.push(direct_block_addresses[i]);
        }
        for(int used_block : index_block) free_blocks.push(used_block);
    }

    void blocks_reqd_for_append(string append_data) {
        int curr_no_blocks = no_blocks_used;
        // Check!! Need to check whether index block is allocated too 
    }

    void allocate_new_block() {
        int next_block = get_free_block();
        no_blocks_used++;
        for(int i = 0; i < 3; i++) {
            if(direct_block_addresses[i] == -1) {
                direct_block_addresses[i] = next_block;
                return;
            }
        }
        if(index_block_address == -1) {
            index_block_address = next_block;
            file_size += BLOCK_SIZE;
            no_blocks_used++;
        }
        next_block = get_free_block();
        // Check if file size exceeds limit
        index_block.push_back(next_block);
    }

    void append_contents(string append_data) {
        file_data += append_data;
        file_size += append_data.size();
        int req_no_blocks = append_data.size()/BLOCK_SIZE;
        for(int i = 0; i < req_no_blocks; i++) allocate_new_block();
    }

    void display() {
        cout<<"File name: "<<file_name<<", File size: "<<file_size<<endl;
    }
};


int main(int argc, char *argv[]) {
    int D;
    int no_blocks;
    // Handle cmd line args
    if(argc != 2) {
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
            if(has_free_blocks())
                directory[filename] = new inode(filename);
            else 
                print_no_space_error();
        }
        else if(DELETE == command) {
            cin>>filename;
            // Check if file exists
            inode *del_file = directory[filename];
            directory.erase(filename);
            del_file->delete_file();
            free(del_file);
        }
        else if(PRINT == command) {
            cin>>filename;
            // Check if file exists
            directory[filename]->print_contents();
        }
        else if(APPEND == command) {
            cin>>filename;
            string append_data;
            cout<<"Enter data to append to "<<filename<<endl;
            cin.ignore();
            getline(cin, append_data);
            // Check if file exists
            // Is there enough space to append?
            directory[filename]->append_contents(append_data);
        }
        else if(DISPLAY == command) {
            // Check if empty and print special message?
            for(auto file : directory) {
                file.second->display();
            }
        }
        else if(FREESPACE == command) {
            int free_blocks_count = free_blocks.size();
            cout<<"Number of free blocks: "<<free_blocks_count<<endl;
            cout<<"Total free size: "<<free_blocks_count * BLOCK_SIZE<<endl;
        }
        else if(PRINTINODE == command) {
            cin>>filename;
            // Check if file exists
            directory[filename]->print_inode();
        }
        else if(EXIT == command)
            break;
        else {
            cout<<"Unknown command "<<command<<". Please enter a valid command"<<endl;
        }
    }
    return 0;
}