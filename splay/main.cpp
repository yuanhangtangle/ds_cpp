#include <iostream>
#include "splay.hpp"
#include "queue.hpp"
#include "vector.hpp"
using namespace std;
//#include <string>
struct Print
{
    void operator() (int n){ std::cout << n; }
};

int main()
{
    int n = 1;
    int m = 2;
    Splay<int> s;
    s.insert(0);
    //s.print_structure(s.root());
    s.insert(3);
    s.insert(2);
    //s.search(2);
    s.insert(4);
    s.insert(5);
    s.search(0);
    s.insert(1);
    //s.search(3);
    //s.remove(3);
    s.remove(1);
    s.print_structure(s.root());
    cin >> n;
    return 0;
}