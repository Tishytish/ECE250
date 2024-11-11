#include<iostream>
#include "classes.h"
#include <cmath>



Tasks::Tasks(int task_num){
    id = task_num;
    }


Tasks::~Tasks(){

}

int Tasks::find_ID() const{
    return id;
}

