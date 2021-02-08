#include <iostream>
#include "bintree.hpp"
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
    AVL<int> bt;

    bt.insert(3);
    bt.insert(6);
    bt.insert(2);
    bt.insert(5);
    //bt.insert(1);
    bt.insert(4);
    bt.insert(7);
    /**/
    
    //bt.print_structure(bt.root());
    bt.print_structure_I(bt.root());
    cout << endl;
    //std::cout << bt.search(2) -> data << std::endl;

    /*
    bt.zig(bt.search(3));
    bt.print_structure_I(bt.root());
    */

    bt.remove(7);
    bt.remove(6);
    bt.print_structure_I(bt.root());

    bt.zig(bt.search(4));
    bt.print_structure_I(bt.root());

    cin >> n;
    return 0;
}