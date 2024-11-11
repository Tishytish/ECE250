#ifndef CORES_H
#define CORES_H
#include "classes.h"

class Cores{
    private:
    Tasks** tasks;
    int size;
    int core_id;
    int front;
    int rear;
    int capacity;
    void resize();


    public:
    Cores();
    ~Cores();
    Tasks* POP();
    void enqueue(Tasks* id);
    Tasks* dequeue();
    bool isEmpty(); 
    void distribute_tasks(Cores** allCores, int numCores);
    Tasks* steal_task();
    int current_size(); 
    Tasks* front_element();
    void cleartasks();
    int getSize() const;
   int getCapacity() const;
};

#endif