#include<iostream>
#include<fstream>
#include<time.h>
#include<vector>
#include<string>

using namespace std;

const int BLOCK_SIZE = 128;
const string LOAD = "load";
const string DELETE = "delete";
const string PRINT = "print";
const string APPEND = "append";
const string DISPLAY = "display";
const string FREESPACE = "freespace";
const string PRINTINODE = "printinode";
const string EXIT = "exit";

typedef struct inode {
    char file_name[16];
    bool type;
    int file_size;
    char date_created[28];
    char date_last_modified[28];
    int direct_block_addresses[3];
    int index_block_address;
    vector<int> index_block;

    void print_inode() {
        cout<<"Filename: "<<file_name<<endl;
        cout<<"Size: "<<file_size<<" bytes"<<endl;
        cout<<"Date Created: "<<date_created<<endl;
        cout<<"Date Last Modified: "<<date_last_modified<<endl;
        cout<<"Direct block values: ";
        for(int i = 0; i < 3; i++) cout<<direct_block_addresses[i]<<" ";
        cout<<endl;
        cout<<"Index block is stored in: "<<index_block_address<<endl;
        cout<<"Index block contents: ";
        for(int block : index_block) cout<<block<<" ";
        cout<<endl;
    }
} inode;


int main(int argc, char *argv[]) {
    // Handle cmd line args
    while(true) {
        string command, filename;
        cin>>command;
        if(LOAD == command) {
            cin>>filename;

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