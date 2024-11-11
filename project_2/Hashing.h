#ifndef HASHING_H
#define HASHING_H
#include <vector>
#include "Fileblock.h"

class Hashing{
    private:
    int size;
    int hashtype; 
    std::vector<std::vector<Fileblock> > Chain;
    std::vector<Fileblock> doublehashing;
    std::vector<bool> occupied;
    int Primary_hashfunction(int key);
    int Secondary_hashfunction(int key);


    public:
    Hashing(int bucket, int type);
    ~Hashing();
    void store(int id, const char* payload);
    int search (int id);
    void insert_item_chaining(int id);
    void delete_item(int key);
    void corrupt(int id, const char* payload);
    void validate(int id);
    void print(int index);
};

#endif
