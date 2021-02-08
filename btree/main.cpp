#include "vector.hpp"
#include <iostream>
#include "btree.hpp"
using namespace std;

int main()
{
    int n;
    BTree<int> bt;
    bt.insert(0);
    bt.print_structure(bt.root());
    bt.insert(1);
    bt.print_structure(bt.root());
    bt.insert(2);
    bt.print_structure(bt.root());
    bt.insert(3);
    bt.print_structure(bt.root());
    bt.insert(4);
    bt.print_structure(bt.root());
    bt.insert(-1);
    bt.print_structure(bt.root());
    bt.insert(-2);
    bt.print_structure(bt.root());

    bt.remove(4);
    cin >> n;
    return 0;
}