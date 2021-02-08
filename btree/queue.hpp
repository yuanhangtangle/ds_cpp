#pragma once
#include "list.hpp"
template<typename T>
class queue: public List<T>
{
    public:
        void enQueue(const T& e){this -> insertAsFirst(e);}
        T deQueue( ){return this -> remove(this -> last());}
        T top( ) {return this -> last() -> data;}
};