#pragma once
#include "entry.hpp"
#include <iostream>
#include "Bitmap.h"
#include "release.h"
#define lazilyRemoved(x) (lazyRemovel -> test(x));
#define markAsRemoved(x) (lazyRemoval -> set(x));
template<typename K, typename V>
class HashTable
{
    private:
        Entry<K,V>** ht;//词条指针的数组，表示为词条指针数组的指针
        int M;
        int N;

        Bitmap* lazyRemoval;

        int primeNLT(int n){
            int p;
            for(p = n; !checkPrime(p); ++p);
            return p;
        }

        bool checkPrime(int n){
            if(n < 2) return false;
            int m;
            for(m = 2; m < n && (n % m); ++m);
            if(m == n) return true;
            else return false;
        }

        //for testing only; Assume K is int; hash by division
        int hascode(int n){
            return n % M;
        }

    protected:
        int probe4Hit(K key);
    public:
        HashTable(int c = 5);
        ~HashTable();
        int size() const { return N };
        V* get(K key);
        bool remove(K key);


};

template<typename K, typename V>
HashTable<K,V>::HashTable(int c){
    M = primeNLT(c);
    N = 0;
    ht = new Entry<K,V>*[M];
    memset(ht, 0, sizeof(Entry<K,V>*) * M);
    lazyRemoval = new Bitmap(M);
}

template<typename K, typename V>
HashTable<K,V>::~HashTable(){
    for(int i = 0; i < M; i++)
        if(ht[i]) release(ht[i]);
    release(ht);
    release(lazyRemoval);
}

template<typename K, typename V>
int HashTable<K,V>::probe4Hit(K key){
    int r = hascode(key) % M;
    for(; lazilyRemoved(r) || (ht[r] && ht[r] != key); r = (r+1) % M);
    return r;
}

template<typename K, typename V>
V* HashTable<K,V>::get(K key){
    int r = probe4Hit(key);
    return ht[r]? &(ht[r]-> value) : NULL;
}

template<typename K, typename V>
bool HashTable<K,V>::remove(K key){
    int r = probe4Hit(key);
    if(!ht[r]) return false;
    else{
        markAsRemoved(r);
        release(ht[r]);
        ht[r] = NULL;
        --N;
        return true;
    }
    
}









