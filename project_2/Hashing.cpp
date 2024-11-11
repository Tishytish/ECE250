#include <iostream>
#include "Hashing.h"
#include <cmath>
#include <vector>



//constructor which initializes size of hashtable with input bucket and hashtype with type
Hashing::Hashing(int bucket, int type){
    hashtype = type;
    size = bucket;
    occupied = std::vector<bool>(bucket, false); 
    //setting size of hashtable to be bucket depending on which hashing type the user wants to use
    if (hashtype == 1){
        Chain.resize(bucket); 
    }
   if (hashtype == 0 ){
    doublehashing.resize(bucket);
   }
}

Hashing::~Hashing(){

}

//the first hashfunction
int Hashing::Primary_hashfunction(int key){
  return key % size;
}

//second hash function
int Hashing::Secondary_hashfunction(int key){
  int h_two;
  h_two = (key/size) % size;
  if((h_two % 2)== 0){
  h_two = h_two + 1;
  }
  return h_two;
}


void Hashing::store(int id, const char* payload) {
    if (hashtype == 0) { 
        int index = Primary_hashfunction(id);

       //checking if the table is free
int trues = 0;
while (occupied[index] && trues < size) {
    if (doublehashing[index].getID() == id) {
        std::cout << "failure" << std::endl;
        return;
    }
    index = (index + Secondary_hashfunction(id)) % size;
    trues++;
}

if (trues >= size) {
    std::cout << "failure" << std::endl;
    return;
}

        
        Fileblock new_Fileblock;  
        new_Fileblock.id = id;    
        new_Fileblock.set_payload(payload); 
        new_Fileblock.setChecksum(new_Fileblock.compute_checksum()); 
        doublehashing[index] = new_Fileblock; 
        occupied[index] = true;
        std::cout << "success" << std::endl; 
    } else { 
        int index = Primary_hashfunction(id);

        // check if key was already there
        for (int i = 0; i < Chain[index].size(); ++i) {
            if (Chain[index][i].getID() == id) {
                std::cout << "failure" << std::endl; //print failure if it was
                return; 
            }
        }

        
        Fileblock new_Fileblock;
        new_Fileblock.id = id;
        new_Fileblock.set_payload(payload);
        new_Fileblock.setChecksum(new_Fileblock.compute_checksum());
        //after setting up new filebloc with the id, payload and checksome, add to the chain with push_back(vector predefined-function)
        Chain[index].push_back(new_Fileblock); 
        std::cout << "success" << std::endl;
    }
}

int Hashing::search(int id){
    int index = Primary_hashfunction(id);

    if(hashtype ==  1){
//know the index for the hashtable, but not the chain, so loop/ check through chain
        for(int i = 0;i < Chain[index].size(); ++i){
            if(Chain[index][i].id == id){
                std::cout << "found " << id << " in " << index << std::endl;
                return index;
            }
        }
        std::cout << "not found" << std::endl;
    }
else { 
        int original_index = index;
        do {
            if (occupied[index] && doublehashing[index].getID() == id) {
                std::cout << "found " << id << " in " << index << std::endl;
                return index;
            }
            index = (index + Secondary_hashfunction(id)) % size;
        } while (index != original_index && occupied[index]);

        std::cout << "not found" << std::endl;
    }
    return -1; // Return a value to indicate not found
}


void Hashing::delete_item(int id){
    if(hashtype == 1){
        int index = Primary_hashfunction(id);

        for(int i=0; i< Chain[index].size(); ++i){
            if (Chain[index][i].getID() == id){
                Chain[index].erase(Chain[index].begin() + i);
                std::cout << "success" << std::endl;
                return;
            }
        }

        std::cout << "failure" << std::endl;
    }
else { 
        int index = Primary_hashfunction(id);
        int original_index = index;
        int offset = Secondary_hashfunction(id);

        do {
            if (occupied[index] && doublehashing[index].getID() == id) {
                doublehashing[index] = Fileblock(); // Replace with an empty Fileblock
                std::cout << "success" << std::endl;
                return;
            }
            index = (index + offset) % size;
        } while (index != original_index && occupied[index]);

        std::cout << "failure" << std::endl;
    }
}



//does basically the same thing as store, but on a fileblock thta has already been created
void Hashing::corrupt(int id, const char* payload) {
    if (hashtype == 0) { 
        int index = Primary_hashfunction(id);
        int original_index = index;
        int step = Secondary_hashfunction(id);
        int trues = 0;

        while (trues < size) {
            if (occupied[index]) { 
                if (doublehashing[index].getID() == id) { 
                    // the set payload function in the file block already sets everything to zero first before setting it with the new payload
                    doublehashing[index].set_payload(payload); 
                    std::cout << "success" << std::endl;
                    return;
                }
            } else { 
                break;
            }
            
            index = (index + step) % size;
            trues++;

            if (index == original_index) {
                break;
            }
        }
        std::cout << "failure" << std::endl;
    } else if (hashtype == 1) { 
        int index = Primary_hashfunction(id);

        for (int i = 0; i < Chain[index].size(); ++i) {
            if (Chain[index][i].getID() == id) {
                Chain[index][i].set_payload(payload);
                std::cout << "success" << std::endl;
                return;
            }
        }
        std::cout << "failure" << std::endl;
    }
}

//validate_checksum called compute checksum and then compares with storedChecksum
void Hashing::validate(int id) {
    int index = Primary_hashfunction(id);

    if (hashtype == 0) { 
        int initial_index = index; 
        while (occupied[index]) {
            if (doublehashing[index].getID() == id) {
                if (doublehashing[index].validate_checksum()) {
                    std::cout << "valid" << std::endl;
                } else {
                    std::cout << "invalid" << std::endl;
                }
                return;
            }
            index = (index + Secondary_hashfunction(id)) % size;
            if (index == initial_index) break; 
        }
    } else { 
        for (int i = 0; i < Chain[index].size(); ++i) {
            if (Chain[index][i].getID() == id) {
                if (Chain[index][i].validate_checksum()) {
                    std::cout << "valid" << std::endl;
                } else {
                    std::cout << "invalid" << std::endl;
                }
                return;
            }
        }
    }

    std::cout << "failure" << std::endl;
}




void Hashing::print(int index) {
    //print chain is empty if chain is empty
    if (Chain[index].empty()) {
        std::cout << "chain is empty" << std::endl;
        return;
    }
//places the contents of Chain[index] into this newly created vector called sorted_chain
    std::vector<Fileblock> sorted_chain = Chain[index];

    //sorting the chaiing using insertion sort
    for (int i = 1; i < sorted_chain.size(); ++i) {
        Fileblock key = sorted_chain[i];
        int j = i - 1;

       
        while (j >= 0 && sorted_chain[j].getID() > key.getID()) {
            sorted_chain[j + 1] = sorted_chain[j];
            j--;
        }
        sorted_chain[j + 1] = key;
    }

    //printing of IDs
    for (int i = 0; i < sorted_chain.size(); ++i) {
        std::cout << sorted_chain[i].getID();
        if (i < sorted_chain.size() - 1) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}

