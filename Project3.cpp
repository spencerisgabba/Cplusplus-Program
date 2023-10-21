#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

class TrieNode { // trie node class
public:
    char data; // store for single char of word in try
    int count; // count int that triggers at end of word
    map<char, TrieNode*> children; //arr map of trie children
    bool isEndOfWord; // end of word flag to end search

    TrieNode(char c) { // initialize try ndoe with char
        data = c;
        count = 0; // sets count to 0 unless end of word flag == true
        isEndOfWord = false;
    }
};

class Trie {
public:
    TrieNode* root; //initialize root node

    Trie() { // make the try node null
        root = new TrieNode('\0');
    }

    void insert(string word) { //insert word into trie
        TrieNode* curr = root; // starts insert on root char
        for (char c : word) { // loop for every character in input word
            if (curr->children.count(c) == 0) { // if char doesn't exist in children add node to children
                curr->children[c] = new TrieNode(c);
            }
            curr = curr->children[c]; // go to next node
        }
        curr->isEndOfWord = true; // when word is finished, add end of word flag
        curr->count++; // when word is finished, increments count
    }

    int search(string word) { // search trie
        TrieNode* curr = root; // cur = root to start at root node
        for (char c : word) { // loop through word
            if (curr->children.count(c) == 0) { // if next char doesn't exist, 0 count
                return 0;
            }
            curr = curr->children[c]; // go to next node
        }
        return curr->isEndOfWord ? curr->count : 0; // if end of word is true, return count else 0 for 0 results
    }
};

void populateData(Trie& trie, const string& filename) { // inserts every line of input file into trie
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        cerr << "Error: Could not open the file." << endl;
        return;
    }

    string line;
    while (getline(inputFile, line)) { // while input file ahs line insert line aka word
        trie.insert(line);
    }

    inputFile.close(); // important to close files
}

void saveTrieData(ofstream& outputFile, TrieNode* node, string prefix) { // saves data to file with output direction, root node, and prefix ("" in this case)
    if (node == nullptr) {
        return;
    }

    if (node->isEndOfWord) {
        outputFile << prefix << " " << node->count << endl;
    }

    for (const auto& pair : node->children) {
        saveTrieData(outputFile, pair.second, prefix + pair.first);
    }
}

void saveData(const Trie& trie, const string& filename) { 
    ofstream outputFile(filename); // opens output file

    if (!outputFile.is_open()) { // err handling
        cerr << "Error: Could not open the file for saving data." << endl;
        return;
    }

    saveTrieData(outputFile, trie.root, "");    // traverse the trie and write data to the file with empty prefix


    outputFile.close(); // important to close file
}


void search(Trie& trie, const string& query) { // Search trie helper
    int count = trie.search(query);
    cout << "Searching for '" << query << "': " << count << endl;
}
void printTrie(TrieNode* node, string prefix, bool isHisto) {
    if (node == nullptr) { // if trie is empty return
        return;
    }

    if (node->isEndOfWord) { // if end of word stop print
        if (isHisto) { // if histogram flag is true, print histogram format
            cout << prefix << ": ";
            for (int i = 0; i < node->count; i++) {
                cout << "*";
            }
            cout << endl;
        }
        else { // if histo false, print regular format
            cout << prefix << ": " << node->count << endl;
        }
    }

    for (const auto& pair : node->children) { // loop through key value pair of children of node
        printTrie(pair.second, prefix + pair.first, isHisto); // call print trie function with key value pairs, "" prefix is added to first key value and pair.second is the child node corrosponding with pair.first
    }
}

int main() {
    Trie trie; // initialize trie
    populateData(trie, "CS210_Project_Three_Input_File.txt"); // populate data with input file
    int input; // int input
    string query; // string query for searches
    saveData(trie, "frequency.dat"); // saves data to .dat file
    bool isHisto = false; // bool to check if histogram

    do { // do while loop for input handling

    cout << "1: Search for an item\n2: Print all items\n3: Print (Histogram)\n4: Exit Program\nPlease enter an option: "; // \n is better than endl, prove me wrong
    cin >> input; // collects user input
        switch (input) { // switch case for every option
            // "cout << "\033[2J\033[1;1H";" is apparently another way to clear screen that works with windows and linux, so I'm trying it out in this project
        case 1:
            cout << "Enter the item you want to search for: ";
            cin >> query;
            search(trie, query); // call search with user entered query
            system("pause"); // pause so user can read input before clearing
            cout << "\033[2J\033[1;1H";
            break;
        case 2:
            cout << "\033[2J\033[1;1H";
            printTrie(trie.root, "", isHisto); // print trie with "" prefix
            system("pause");
            cout << "\033[2J\033[1;1H";
            break;
        case 3:
            isHisto = true;
            cout << "\033[2J\033[1;1H";
            printTrie(trie.root, "", isHisto); // print trie histogram format with "" prefix
            system("pause");
            cout << "\033[2J\033[1;1H";
            break;
        case 4:
            exit(0); // exit the program
            break;
        default: // Error handling
            cout << "Please enter a valid option." << endl;
            system("pause");
            cout << "\033[2J\033[1;1H";
        }
    } while (0<input<4); // Error handling

    return 0;
}
