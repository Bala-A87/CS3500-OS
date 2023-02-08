#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<ctime>
#include<chrono>
#include<queue>

using namespace std;

const int BLOCK_SIZE = 128;
const int KILO = 1024;
const string LOAD = "load";
const string DELETE = "delete";
const string PRINT = "print";
const string APPEND = "append";
const string DISPLAY = "display";
const string FREESPACE = "freespace";
const string PRINTINODE = "printinode";
const string EXIT = "exit";

queue<int> free_blocks;

// Gets the current time as a string
string get_time() {
    auto now = chrono::system_clock::now();
    time_t curr_time = chrono::system_clock::to_time_t(now);
    string result = ctime(&curr_time);
    return result.substr(0, result.size()-1);
}

// Checks if there is a free block
bool has_free_blocks() {
    return !free_blocks.empty();
}

// Checks if there are no_blocks free blocks
bool has_free_blocks(int no_blocks) {
    return free_blocks.size() >= no_blocks;
}

// Gets a free block from the queue of free blocks
int get_free_block() {
    int free_block = free_blocks.front();
    free_blocks.pop();
    return free_block;
}

// Prints error message for if disk has insufficient space
void print_no_space_error() {
    cout<<"ERROR: Disk has insufficient space for this operation"<<endl;
}

// Prints error message when file doesn't exist
void print_file_dne_error(string filename) {
    cout<<"ERROR: File "<<filename<<" does not exist"<<endl;
}

// Checks if the given string is a valid file name
bool check_filename_validity(string filename) {
    if(filename[0] != '/') return false;
    if(filename.size() > 16) return false;

    int slash_count = 0, dot_count = 0;
    for(char curr_char : filename) {
        if(curr_char == '/') slash_count++;
        if(curr_char == '.') dot_count++;
    }
    if(slash_count > 2 || dot_count > 1) return false;

    int directory_name_size = 0;
    int file_name_size = 0;
    int extension_size = 0;

    int curr_index = 0;
    if(slash_count == 2) {
        for(curr_index = 1; filename[curr_index] != '/'; curr_index++) {
            directory_name_size++;
            if(filename[curr_index] == '.') return false;
        }
    }

    for(curr_index = curr_index+1; curr_index < filename.size(); curr_index++) {
        if(filename[curr_index] == '.') break;
        file_name_size++;
    }

    if(curr_index < filename.size()) {
        for(curr_index = curr_index+1; curr_index < filename.size(); curr_index++) extension_size++;
    }

    if(directory_name_size > 4 || extension_size > 3) return false;
    return true;
}

// Class to store inode data of a file
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
        file_size = BLOCK_SIZE;
        date_created = get_time();
        date_last_modified = date_created;
        direct_block_addresses[0] = get_free_block();
        direct_block_addresses[1] = direct_block_addresses[2] = index_block_address = -1;
        index_block.clear();
        file_data = "";
        no_blocks_used = 1;
    }

    // Prints inode data of the file
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

    // Prints file contents
    void print_contents() {
        cout<<file_data<<endl;
    }

    // Frees all allocated blocks of the disk for the file
    void delete_file() {
        for(int i = 0; i < 3;  i++) {
            if(direct_block_addresses[i] == -1) break;
            free_blocks.push(direct_block_addresses[i]);
        }
        if(index_block_address != -1) free_blocks.push(index_block_address);
        for(int used_block : index_block) free_blocks.push(used_block);
    }

    // Finds the number of blocks required to append the specified data to present data
    int blocks_reqd_for_append(string append_data) {
        int curr_no_blocks = no_blocks_used;
        int new_no_blocks = 1 + ((file_data+append_data).size() + BLOCK_SIZE-1)/BLOCK_SIZE;
        if(new_no_blocks > 4) new_no_blocks++;
        return new_no_blocks-curr_no_blocks;
    }

    // Allocates a new block to the file, also allocating for index block if required
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
            next_block = get_free_block();
        }
        index_block.push_back(next_block);
    }

    // Appends given data to file data, allocating new blocks as necessary
    void append_contents(string append_data) {
        int final_no_blocks = no_blocks_used + blocks_reqd_for_append(append_data);
        while(no_blocks_used < final_no_blocks) allocate_new_block();
        file_data += append_data;
        file_size += append_data.size();
        date_last_modified = get_time();
    }

    // Displays file info as required in display command
    void display() {
        cout<<"File name: "<<file_name<<", File size: "<<file_size<<" bytes"<<endl;
    }
};

map<string, inode*> directory;

// Returns if the file filename is present in the directory
bool is_file_present(string filename) {
    return directory.find(filename) != directory.end();
}

// Removes file filename from directory
void erase_file_from_directory(string filename) {
    inode *del_file = directory[filename];
    directory.erase(filename);
    del_file->delete_file();
    delete del_file;
}

int main(int argc, char *argv[]) {
    int D;
    int no_blocks;
    if(argc != 2) {
        cout<<"Please stick to specifications"<<endl;
        cout<<"Usage: ./disk-directory <D>"<<endl;
        cout<<"Where D is the disk size in KB"<<endl;
        exit(1);
    }
    else {
        D = atoi(argv[1]);
        no_blocks = (D * KILO)/BLOCK_SIZE;
    }
    for(int i = 0; i < no_blocks; i++) free_blocks.push(i);
    directory.clear();

    cout<<"List of available commands:"<<endl;
    cout<<"\tload <filename>"<<endl;
    cout<<"\t\tCreates file with name <filename> and loads it onto disk"<<endl;
    cout<<"\tdelete <filename>"<<endl;
    cout<<"\t\tDeletes file with name <filename> from disk"<<endl;
    cout<<"\tprint <filename>"<<endl;
    cout<<"\t\tPrints contents of file <filename>"<<endl;
    cout<<"\tappend <filename>"<<endl;
    cout<<"\t\tGets data to append from user, and appends it to <filename>"<<endl;
    cout<<"\tdisplay"<<endl;
    cout<<"\t\tPrints name and size of files in the disk"<<endl;
    cout<<"\tfreespace"<<endl;
    cout<<"\t\tPrints available space in the disk"<<endl;
    cout<<"\tprintinode <filename>"<<endl;
    cout<<"\t\tPrints inode contents of file <filename>"<<endl;
    cout<<"\texit"<<endl;
    cout<<"\t\tExit the simulation"<<endl;

    while(true) {
        string command, filename;
        cout<<endl<<"Enter a command to execute"<<endl;
        cin>>command;

        if(LOAD == command) {
            cin>>filename;
            // First check if the given file name is valid
            if(check_filename_validity(filename)) {
                // If file is already present, empties the file (deletes it)
                if(is_file_present(filename))
                    erase_file_from_directory(filename);
                // Check if there is enough space to load the file onto the disk
                if(has_free_blocks()) 
                    directory[filename] = new inode(filename);
                else 
                    print_no_space_error();
            }
            else
                cout<<"ERROR: File name "<<filename<<" is invalid"<<endl;
        }

        else if(DELETE == command) {
            cin>>filename;
            if(is_file_present(filename))
                erase_file_from_directory(filename);
            else
                print_file_dne_error(filename);
        }

        else if(PRINT == command) {
            cin>>filename;
            if(is_file_present(filename))
                directory[filename]->print_contents();
            else
                print_file_dne_error(filename);
        }

        else if(APPEND == command) {
            cin>>filename;
            if(is_file_present(filename)) {
                string append_data;
                cout<<"Enter data to append to "<<filename<<endl;
                cin.ignore();
                getline(cin, append_data);
                // Check if disk has enough space to append all the given data
                if(has_free_blocks(directory[filename]->blocks_reqd_for_append(append_data)))
                    directory[filename]->append_contents(append_data);
                else
                    print_no_space_error();
            }
            else
                print_file_dne_error(filename);
        }

        else if(DISPLAY == command) {
            if(!directory.empty()) {
                for(auto file : directory) {
                    file.second->display();
                }
            }
            else {
                cout<<"The directory is empty"<<endl;
            }
        }

        else if(FREESPACE == command) {
            int free_blocks_count = free_blocks.size();
            cout<<"Number of free blocks: "<<free_blocks_count<<endl;
            cout<<"Total free size: "<<free_blocks_count * BLOCK_SIZE<<" bytes"<<endl;
        }

        else if(PRINTINODE == command) {
            cin>>filename;
            if(is_file_present(filename))
                directory[filename]->print_inode();
            else
                print_file_dne_error(filename);
        }

        else if(EXIT == command)
            break;

        else {
            cout<<"Unknown command "<<command<<". Please enter a valid command"<<endl;
        }
    }

    return 0;
}
