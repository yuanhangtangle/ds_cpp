#include <iostream>
#include "graph.hpp"
#include <string>
#include <map>
#include <utility>
int main()
{
    GraphMatrix<int, int> gm;
    int n = 8;
    for (int i = 0; i < n; i++)
        gm.insert(i);
    
    int A[11][2] = {{0,2},
                    {0,4},
                    {2,1},
                    {3,1},
                    {4,5},
                    {4,6},
                    {6,1},
                    {6,5},
                    {7,0},
                    {7,2},
                    {7,3}};
    
    for(int i = 0; i < 11; i++)
    {
        int u = A[i][0];
        int v = A[i][1];
        gm.insert(1,0,u,v);
    }

    gm.print_matrix();
    gm.bfs(7);
    std::cout<< "-------------------- standard breadth first search --------------------" << std::endl;
    gm.print_tree_structure();

    std::cout<< "-------------------- PFS breadth first search --------------------" << std::endl;
    //gm.bfs_p(7);
    gm.print_tree_structure();
    GraphMatrix<int, int> gm2;
    n = 7;
    for (int i = 0; i < n; i++)
        gm2.insert(i);
    
    int B[10][2] = {{0,1},
                    {0,2},
                    {0,5},
                    {1,2},
                    {3,0},
                    {3,4},
                    {4,5},
                    {5,6},
                    {6,0},
                    {6,2}};

    for(int i = 0; i < 10; i++)
    {
        int u = B[i][0];
        int v = B[i][1];
        gm2.insert(1,0,u,v);
    }
    gm2.print_matrix();
    gm2.dfs(0);
    gm2.print_tree_structure();
    gm2.dfs_iter(0);
    gm2.print_tree_structure();
    //std::cout << gm2.type(6,0)<< endl;

    GraphMatrix<int, int> gm3;
    n = 7;
    for (int i = 0; i < n; i++)
        gm3.insert(i);
    
    int C[9][2] = {{0,1},
                    {0,2},
                    {0,5},
                    {1,2},
                    {3,0},
                    {3,4},
                    {4,5},
                    {5,6},
                    //{6,0},
                    {6,2}};

    for(int i = 0; i < 9; i++)
    {
        int u = C[i][0];
        int v = C[i][1];
        gm3.insert(1,0,u,v);
    }
    gm3.print_matrix();

    stack<int>* S = gm3.tsort(3);
    while(!S->empty())
    {
        std:: cout << S -> top() << std::endl;
        S->pop();
    }

    S = gm3.tsort_iter(3);
    while(!S->empty())
    {
        std:: cout << S -> top() << std::endl;
        S->pop();
    }

    std::cin >> n;
    return 0;
}