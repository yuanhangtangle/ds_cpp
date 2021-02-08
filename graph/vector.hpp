#pragma once

#include <iostream>
#define DEFAULT_CAPACITY 100
#include <stdlib.h>
using namespace std;
typedef int Rank;

template <typename T>
class vector
{
private:
    Rank _size;
    int _capacity;
    T *_elem;

    void copyFrom(const T *v, Rank lo, Rank hi);
    void expand();
    void merge2(Rank lo, Rank mi, Rank hi, T *p, T *q);
    void _mergeSort2(Rank lo, Rank hi, T *p, T *q);

public:
    //vector(int c = DEFAULT_CAPACITY);
    vector(const vector<T> &v);
    vector(const vector<T> &v, Rank lo, Rank hi);
    vector(const T *A, int n);
    vector(const T *A, int lo, int hi);
    vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0)
    {
        _elem = new T[_capacity = c];
        for (_size = 0; _size < s; _elem[_size++] = v);
    }
    ~vector();

    int size() const;
    int empty() const;
    Rank insert(Rank r, const T &v);
    Rank insert(const T &v);
    T remove(Rank r);
    int remove(Rank lo, Rank hi);
    int disordered();
    void permute();
    Rank find(const T &v, Rank lo, Rank hi) const;
    Rank find(const T &v) const;
    int deduplicate();
    int deduplicate2();
    int uniquify();
    T &operator[](Rank r) const;
    vector<T> &operator=(const vector<T> &V);
    template <typename VST>
    void traverse(VST visit);
    void mergeSort2(Rank lo, Rank hi);
    void mergeSort2(); //{ mergeSort2(0,_size); }
};

/*
template<typename T> 
vector<T>::vector(int c)
{
    _size = 0;
    _elem = new T[_capacity = c];
}
*/
template <typename T>
vector<T>::vector(const vector<T> &v)
{
    copyFrom(v._elem, 0, v.size());
}

template <typename T>
vector<T>::vector(const vector<T> &v, Rank lo, Rank hi)
{
    copyFrom(v._elem, lo, hi);
}

template <typename T>
vector<T>::vector(const T *A, int n)
{
    copyFrom(A, 0, n);
}

template <typename T>
vector<T>::vector(const T *A, int lo, int hi)
{
    copyFrom(A, lo, hi);
}

template <typename T>
vector<T>::~vector()
{
    delete[] _elem;
}

template <typename T>
int vector<T>::size() const
{
    return _size;
}

template <typename T>
int vector<T>::empty() const
{
    return _size == 0;
}

template <typename T>
void vector<T>::copyFrom(const T *v, Rank lo, Rank hi)
//size, capacity, elem 全部搞定
{
    //delete[] _elem; // 这里是构造函数, _elem本不存在；
    _elem = new T[_capacity = 2 * (hi - lo)];
    for (_size = 0; lo < hi; _elem[_size++] = v[lo++])
        ; //用for搞定 _size 处理以及复制
}


template<typename T> 
vector<T>& vector<T>::operator=(const vector<T>& V)
{
    if (_elem) delete[] _elem;
    copyFrom(V._elem, 0, V.size());
    return *this;
}

template <typename T>
void vector<T>::expand()
{
    if (_size < _capacity)
        return;
    T *oldelem = _elem;
    _elem = new T[_capacity = 2 * _capacity];
    for (int i = 0; i < _size; i++)
        _elem[i] = oldelem[i];
    delete[] oldelem;
}

template <typename T>
Rank vector<T>::insert(Rank r, T const &v)
{
    expand();
    for (int i = _size; i > r; _elem[i] = _elem[i - 1], i--)
        ;
    _elem[r] = v;
    _size++;
    return r;
}

template <typename T>
Rank vector<T>::insert(const T &v)
{
    insert(_size, v);
    return _size - 1;
}

template <typename T>
T vector<T>::remove(Rank r)
{
    T t = _elem[r];
    --_size;
    for (; r < _size; _elem[r] = _elem[r + 1], r++)
        ;
    return t;
}

template <typename T>
int vector<T>::remove(Rank lo, Rank hi)
{
    for (; hi < _size; _elem[lo++] = _elem[hi++])
        ;
    _size = lo;
    return hi - lo;
}

template <typename T>
int vector<T>::disordered()
{
    int s;
    for (int i = 0; i < _size - 1; i++)
        _elem[i] > _elem[i + 1] ? s++ : s;
    return s;
}

template <typename T>
void vector<T>::permute()
{
    for (int i = _size - 1; i > 0; swap(_elem[rand() % (i + 1)], _elem[i--]))
        ;
}

template <typename T>
Rank vector<T>::find(const T &v, Rank lo, Rank hi) const
{
    while ((--hi >= lo) && (_elem[hi] != v))
        ;
    return hi;
}

template <typename T>
Rank vector<T>::find(const T &v) const
{
    return find(v, 0, _size);
}

template <typename T>
T &vector<T>::operator[](Rank r) const
{
    return _elem[r];
}

template <typename T>
int vector<T>::deduplicate() //向后搜索删除，相同元素保留第一个；csacpp给出的是向前搜索，相同元素保留最后一个
{
    int oldSize = _size;
    for (Rank i = 0; i < _size; i++)
        for (Rank j = i + 1; j < _size;)
            (_elem[j] == _elem[i]) ? remove(j) : j++;
    return oldSize - _size;
}

template <typename T>
int vector<T>::deduplicate2() //向后搜索删除，相同元素保留第一个；csacpp给出的是向前搜索，相同元素保留最后一个
{
    int j = 0;
    for (Rank i = 1; i < _size; i++)
        find(_elem[i], 0, i) >= 0 ? j++ : _elem[i - j] = _elem[i];
    _size -= j;
    return j;
}

template <typename T>
int vector<T>::uniquify()
{
    int j = 0;
    for (int i = 1; i < _size; i++)
        (_elem[i - 1] == _elem[i]) ? j++ : _elem[i - j] = _elem[i];
    _size -= j;
    return j;
}

template <typename T>
void vector<T>::merge2(Rank lo, Rank mi, Rank hi, T *p, T *q)
{
    int l = mi - lo;
    int r = hi - mi;
    for (int i = 0; i < l; i++)
        p[i] = _elem[i + lo];
    for (int i = 0; i < r; i++)
        q[i] = _elem[i + mi];
    int i, j, s;
    for (i = j = 0, s = lo; (i < l) || (j < r); s++)
    {
        if (i >= l)
            _elem[s] = q[j++];
        else if (j >= r)
            _elem[s] = p[i++];
        else if (p[i] <= q[j])
            _elem[s] = p[i++];
        else
            _elem[s] = q[j++];
    }
}

template <typename T>
void vector<T>::_mergeSort2(Rank lo, Rank hi, T *p, T *q)
{
    if (hi - lo <= 1)
        return;
    Rank mi = (lo + hi) / 2;
    _mergeSort2(lo, mi, p, q);
    _mergeSort2(mi, hi, p, q);
    merge2(lo, mi, hi, p, q);
}

template <typename T>
void vector<T>::mergeSort2(Rank lo, Rank hi)
{
    int l = ((hi - lo) >> 1) + 1;
    T *p = new T[l];
    T *q = new T[l];
    _mergeSort2(lo, hi, p, q);
    delete[] p;
    delete[] q;
}

template <typename T>
template <typename VST>
void vector<T>::traverse(VST visit)
{
    for (int i = 0; i < _size; i++)
        visit(_elem[i]);
}

template <typename T>
void vector<T>::mergeSort2()
{
    mergeSort2(0, _size);
}
