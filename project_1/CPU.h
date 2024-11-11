#ifndef CPU_H
#define CPU_H
#include "Cores.h"



class CPU{
    private:
    Cores** cores;
    int number_of_cores;
    bool isactive;



    public:
    CPU(int number_of_cores);
    ~CPU();
    void spawn_task(int id);
    void run_task(int c_id);
    void sleep(int c_id);
    void shutdown();
    void SIZE(int c_id) const;
    void CAPACITY(int c_id) const;
   
};

#endif