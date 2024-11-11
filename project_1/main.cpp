#include<iostream>
#include"CPU.h"
   
  int main()
{
   int N;
   int P_ID;
   int C_ID;
   CPU* cpu = nullptr;
   std::string cmd;
   while(std::cin >> cmd)
   {
   

   if (cmd == "ON") {
            int N;
            std::cin >> N;
            if (cpu == nullptr) {
                cpu = new CPU(N);
                std::cout << "success" << std::endl;
            } else {
                std::cout << "failure" << std::endl;
            }
        } else if (cmd == "SPAWN") {
            int taskId;
            std::cin >> taskId;
            if (cpu) {
                cpu->spawn_task(taskId);
            } else {
                std::cout << "failure" << std::endl;
            }
        } else if (cmd == "RUN") {
            int coreId;
            std::cin >> coreId;
            if (cpu) {
                cpu->run_task(coreId);
            } else {
                std::cout << "failure" << std::endl;
            }
        } else if (cmd == "SLEEP") {
            int coreId;
            std::cin >> coreId;
            if (cpu) {
                cpu->sleep(coreId);
            } else {
                std::cout << "failure" << std::endl;
            }
        } else if (cmd == "SHUTDOWN") {
            if (cpu) {
                cpu->shutdown();
            }
        } else if (cmd == "SIZE") {
            int coreId;
            std::cin >> coreId;
            if (cpu) {
                cpu->SIZE(coreId);
            } else {
                std::cout << "failure" << std::endl;
            }
        }
        else if (cmd == "CAPACITY") {
         int coreId;
        std::cin >> coreId;
        if (cpu) {
       cpu->CAPACITY(coreId);
            } else {
            std::cout << "failure" << std::endl;
            }
        }
        else if(cmd =="EXIT") {
    break;
   }
   }
}


