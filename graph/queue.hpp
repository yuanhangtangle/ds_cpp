#pragma once
#include "list.hpp"
template<typename T>
class Queue: public List<T>
{
    public:
        void inQueue(const T& e){this -> insertAsFirst(e);}
        T deQueue( ){return this -> remove(this -> last());}
        T top( ) {return this -> last() -> data;}
};