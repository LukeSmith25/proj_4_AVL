/**
 * file: driver-proj4.cpp
 * author: Luke Smith
 * course: CSI 3334
 * assignment: project 4
 * due date: October 23, 2022
 *
 * This file contains the driver for AVL Tree.
 */

#include <iostream>
#include <sstream>
#include "avl-tree-student-proj4.h"
using namespace std;

/**
 * main()
 *
 * This is the main driver for the AVL classes.
 *
 * Parameters: None.
 *
 * Return value:
 *      int: 0 representing successful run.
 */
int main() {
    EncryptionTree<string> encryptTree;
    string line, input;
    char comm;
    bool end = false;

    while (cin >> comm && !end) {
        // Last word for encrypt & decrypt
        bool lastWord = false;
        char next;
        // Stringstream init
        stringstream curLine;
        curLine.clear();
        curLine.str("");

        switch(comm){
            case 'l':
                encryptTree.printLevelOrder(cout);

                if (cin.peek() != '\n'){
                    cin >> ws;
                    if (cin.peek() == 'q') {
                        end = true;
                        break;
                    }
                }

                break;

            case 'p':
                encryptTree.printPreorder(cout);

                if (cin.peek() != '\n'){
                    cin >> ws;
                    if (cin.peek() == 'q') {
                        end = true;
                        break;
                    }
                }

                break;

            case 'i':
                getline(cin, line);
                curLine.str(line);
                curLine >> input;
                encryptTree.insert(input);

                break;

            case 'r':
                getline(cin, line);
                curLine.str(line);
                curLine >> input;
                encryptTree.remove(input);

                break;

            case 'e':
                // Reads until first cleartext
                getline(cin, line, '\'');
                // Reads until last cleartext
                getline(cin, line, '\'');

                // Clear and initialize stringstream
                curLine.clear();
                curLine.str(line);

                while (curLine >> input) {
                    cout << encryptTree.encrypt(input);
                    if (!curLine.eof()){
                        curLine >> ws;
                        if (!curLine.eof()) {
                            cout << " ";
                        }
                    }
                }
                cout << endl;

                // Peak to see if line is empty
                if (cin.peek() != '\n') {
                    cin >> ws;
                    getline(cin, line);
                    if (line.at(0) == 'q') {
                        end = true;
                    }
                }

                break;

            case 'd':
                // Reads until first cleartext
                getline(cin, line, '\'');
                // Reads until last cleartext
                getline(cin, line, '\'');

                // Clear and initialize stringstream
                curLine.clear();
                curLine.str(line);

                // While cleartext is not empty
                while (curLine >> input) {
                    if (encryptTree.decrypt(input)) {
                        cout << *encryptTree.decrypt(input);
                    } else {
                        cout << "?";
                    }
                    if (!curLine.eof()){
                        curLine >> ws;
                        if (!curLine.eof()) {
                            cout << " ";
                        }
                    }
                }
                cout << endl;

                // Read in whitespace
                // Peak to see if line is empty
                if (cin.peek() != '\n') {
                    cin >> ws;
                    getline(cin, line);
                    if (line.at(0) == 'q') {
                        end = true;
                    }
                }

                break;
        }
        /*
        if (!curLine.eof()){
            curLine >> ws;
            if (curLine.peek() == 'q') {
                end = true;
            }
        }
        */
    }
    return 0;
}