#include <iostream>
#include "Cores.h"

Cores::Cores(){
    capacity=4;
    size=0;
    tasks = new Tasks*[capacity] ;
    front=0;
    rear=-1;
    core_id = 0;
 }


Cores::~Cores(){
    for (int i=0; i<size; i++) {
        delete tasks[(front + i) % capacity];
    }
    delete[] tasks;
    tasks = nullptr;
}

int Cores::current_size(){
    //size = (capacity - front + rear) % capacity;
    return size;
}
bool Cores::isEmpty(){
    return size == 0;
}
Tasks* Cores::front_element(){
    if (isEmpty()){
        return nullptr; 
    }
    else{
        return tasks[front]; 
    }
}
Tasks* Cores::dequeue(){
    if(isEmpty()){
        return nullptr;
    }
    else{
        Tasks* task = tasks[front];
        tasks[front] = nullptr;
        front = (front +1) % capacity;
        size--;

        if (size < capacity / 4 && capacity > 2) {
            resize(); 
        }

        return task;
    }
}

void Cores::enqueue(Tasks* id){
    if(current_size() == capacity){
        resize();
    }
    rear = (rear + 1) % capacity;
    tasks[rear] = id;
    size++;
}

Tasks* Cores::steal_task(){ //may need to add c_id as an argument to check what task they are stealing from
    if (current_size()== 0){
        return nullptr;
    }
    else{
        rear = (rear - 1 + capacity)% capacity;
        Tasks* stolenTask = tasks[rear];

        if (stolenTask == nullptr) {
            return nullptr;
        }

        tasks[rear] = nullptr;
        size--;

        return stolenTask;
    }
}


void Cores::cleartasks(){
    for(int i=0; i<size; ++i){
        delete tasks[front];
        front = (front + 1) % capacity;
    }
    size =0;
}

int Cores::getSize() const {
    return size;
}

int Cores::getCapacity() const {
    return capacity;
}

void Cores::resize(){

    int new_capacity;
    if (size == capacity-1){
        new_capacity = capacity * 2;
    }
    else if (size == (capacity / 4) && (capacity > 2)){
        new_capacity = capacity/2;
    }
    else{
        return;
    }

    Tasks** newTasks = new Tasks*[new_capacity];

    for(int i=0; i<size; ++i){
        newTasks[i]= tasks[front];
        front = (front +1) % capacity;
    }

    delete[] tasks;

    tasks = newTasks;
    capacity = new_capacity;
    front =0;
    rear = size;
}

Tasks* Cores::POP() {
    if (size == 0) {
        return nullptr; 
    }

    Tasks* task = tasks[rear]; 
    tasks[rear] = nullptr; 
    size--; 
    rear = (rear - 1 + capacity) % capacity; 
    
    if (size < capacity / 4 && capacity > 2) {
        resize(); 
    }

    return task; 
}