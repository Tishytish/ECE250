#include <iostream>
#include "CPU.h"



CPU::CPU(int number_of_cores): number_of_cores(number_of_cores), isactive(true){
   cores = new Cores*[number_of_cores];
   for (int i =0; i< number_of_cores; ++i){
    cores[i] = new Cores();
   }
}


CPU::~CPU(){
    for (int i = 0; i < number_of_cores; ++i) {
        delete cores[i];
    }
    delete[] cores;
    cores = nullptr;
}

void CPU::spawn_task(int id) {
    if (!isactive || id <= 0) {
        std::cout << "failure" << std::endl;
        return;
    }

    int smallest_index = -1; //index with smallest number of tasks
    int qs_for_si = 100000; //stores queue size(number of tasks) for the index with the smallest number of tasks


   
    for (int i = 0; i < number_of_cores; ++i) {
        int currentSize = cores[i]->current_size();
        if (currentSize < qs_for_si) {
            qs_for_si = currentSize;
            smallest_index = i; 
        }
    }

  
    if (smallest_index != -1){
        cores[smallest_index]->enqueue(new Tasks(id)); 
        std::cout << "core " << smallest_index << " assigned task " << id << std::endl;
    } else{

    }
}


void CPU::run_task(int coreId) {
    if (coreId < 0 || coreId >= number_of_cores) {
        std::cout << "failure" << std::endl;
        return;
    }

  
    Tasks* task = cores[coreId]->dequeue();

    if (task) {
        std::cout << "core " << coreId << " is running task " << task->find_ID() << std::endl;
        delete task; 

        //check if the core I just dequeued is empty, if it is, steal from core with smallest index with the biggest size
        if (cores[coreId]->isEmpty()) {
           
            int biggest_size = -1;
            int index_w_bs = -1; //index with biggest size(don't forget it has to be smallest index)

       
            for (int i = 0; i < number_of_cores; ++i) {
                if (i != coreId) { 
                    int currentSize = cores[i]->current_size();
                    if (currentSize > biggest_size) {
                        biggest_size = currentSize;
                        index_w_bs = i;
                    } else if (currentSize == biggest_size && index_w_bs > i) {
                        index_w_bs = i;
                    }
                }
            }

            if (index_w_bs != -1) {
                Tasks* stolen_task = cores[index_w_bs]->dequeue();
                if (stolen_task) {
                    cores[coreId]->enqueue(stolen_task); 
                }
            }
        }
    } else {
        // If core has no tasks to run
        std::cout << "core " << coreId << " has no tasks to run" << std::endl;
    }
}



void CPU::sleep(int c_id) {
    if (c_id < 0 || c_id >= number_of_cores) {
        std::cout << "failure" << std::endl;
        return;
    }

    if (cores[c_id]->isEmpty()) {
        std::cout << "core " << c_id << " has no tasks to assign" << std::endl;
        return;
    }

    bool first_output= true; 

    while (!cores[c_id]->isEmpty()) {
        int core_wlt = -1; 
        int size_of_core_wlt = 1000000; 

        for (int i = 0; i < number_of_cores; ++i) {
            if (i != c_id && cores[i]->current_size() < size_of_core_wlt) {
                size_of_core_wlt = cores[i]->current_size();
                core_wlt = i;
            }
        }

  
        Tasks* task_to_assign = cores[c_id]->POP();
        if (task_to_assign != nullptr) {
            cores[core_wlt]->enqueue(task_to_assign);
            if (!first_output) {
              std::cout<<" ";
            }
           std::cout<< "task " << task_to_assign->find_ID() << " assigned to core " << core_wlt;
           first_output= false;
        }
    }

  
   std::cout << std::endl;
}


void CPU::shutdown() {
    bool first_output = true;
    bool has_tasks = false; 

    for (int i = 0; i < number_of_cores; ++i) {
        while (!cores[i]->isEmpty()) {
            Tasks* task = cores[i]->dequeue(); 
            if (task) {
                has_tasks = true; 
                if (!first_output) {
                   std::cout<<" ";
                }
                std::cout << "deleting task " << task->find_ID() << " from core " << i;
                first_output = false; 
                delete task;
            }
        }
    }

    if (has_tasks) { 
        std::cout << std::endl;
    } else { 
        std::cout << "no tasks to remove" << std::endl; 
    }

    isactive = false; 
}

void CPU::SIZE(int c_id) const {
    if (c_id < 0 || c_id >= number_of_cores) {
        std::cout << "failure" << std::endl;
    } else {
        std::cout << "size is " << cores[c_id]->getSize() << std::endl;
    }
}

void CPU::CAPACITY(int c_id) const {
    if (c_id < 0 || c_id >= number_of_cores) {
        std::cout << "failure" << std::endl;
    } else {
        std::cout << "capacity is " << cores[c_id]->getCapacity() << std::endl;
    }
}