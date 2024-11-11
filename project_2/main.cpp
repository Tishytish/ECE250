#include <iostream>
#include <vector>
#include <string>
#include "Hashing.h"

int main() {
    std::string cmd;
    int N;
    int T;
    int ID;
    int i;
   char payload_new[501]; 
   Hashing* hashTable = nullptr;

    while (std::cin >> cmd) {
        if (cmd == "NEW") {
            std::cin >> N;
            std::cin >> T;
           hashTable = new Hashing(N, T);
            std::cout << "success" << std::endl;
        } 
    else if (cmd == "STORE") {
            std::cin >> ID;
            std::cin.ignore(); 
            std::cin.getline(payload_new, 501, '!'); // Read until the '!' character
            hashTable->store(ID, payload_new); 
        } 
        else if (cmd == "SEARCH") {
            std::cin >> ID;
            hashTable->search(ID); 
        } 
        else if (cmd == "DELETE") {
            std::cin >> ID;
            hashTable->delete_item(ID);
        } 
        else if (cmd == "CORRUPT") {

            std::cin >> ID;
            std::cin.ignore(); 
            std::cin.getline(payload_new, 501, '!'); // Read until the '!' character
            hashTable->corrupt(ID, payload_new);
        } 
        else if (cmd == "VALIDATE") {
            std::cin >> ID;
            hashTable->validate(ID);
        } 
        else if (cmd == "PRINT") {
            std::cin >> i;
            hashTable->print(ID);
        } 
        else if (cmd == "EXIT") {
            break; 
   
    }

}
}

