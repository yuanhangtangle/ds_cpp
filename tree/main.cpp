#include <iostream>
#include "bintree.hpp"
#include "queue.hpp"
#include "vector.hpp"
//#include <string>
struct Print
{
    void operator() (int n){ std::cout << n; }
};

int main()
{
    int n;
    BinTree<int> bt;
    BinNodePosi(int) x;
    Print print;
    Queue<int> q;

    bt.insertAsRoot(1);
    bt.insertAsRc(bt.root(), 6);
    x = bt.insertAsLc(bt.root(), 2);
    bt.insertAsRc(x, 5);
    x = bt.insertAsLc(x, 3);
    bt.insertAsRc(x, 4);
    bt.insertAsLc(bt.root() -> rc, 7);
    //preorder traversial, expected output:1234567
    bt.travPre_I1(bt.root(), print);
    std::cout << std::endl;
    bt.travPre_Recur(bt.root(), print);
    std::cout << std::endl;
    bt.travPre_I2(bt.root(), print);
    std::cout << std::endl;
    std::cout << std::endl;
    //inorder traversial, expected output:3425176
    bt.travIn_Recur(bt.root(), print);
    std::cout << std::endl;
    bt.travIn_I1(bt.root(), print);
    std::cout << std::endl;
    bt.travIn_I2(bt.root(), print);
    std::cout << std::endl;
    bt.travIn_I3(bt.root(), print);
    std::cout << std::endl;
    bt.travIn_I4(bt.root(), print);
    std::cout << std::endl;
    std::cout << std::endl;
    //postorder traversial
    bt.travPost_Recur(bt.root(), print);
    std::cout << std::endl;
    bt.travPost_I1(bt.root(), print);
    std::cout << std::endl;
    //traverial by level
    bt.travLevel(bt.root(), print);
    std::cout << std::endl;

    //bt.print_structure(bt.root());
    bt.print_structure_I(bt.root());
    bt.swap();
    bt.print_structure_I(bt.root());
    std::cout << bt.check_cumsum(bt.root()) << std::endl;
    std::cout << bt.check_cumsum_I(bt.root()) << std::endl;

    bt.set_as_max_of_descendant(bt.root());
    bt.print_structure_I(bt.root());
    bt.encode_by_level();
    bt.print_structure_I(bt.root());
    cin >> n;
    return 0;
}