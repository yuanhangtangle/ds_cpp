#pragma once

#include <stack>
#include "./queue.hpp"
#include "./vector.hpp"
#include <iostream>
#include <stdlib.h>

enum VStatus
{
    UNDISCOVERED,
    DISCOVERED,
    VISITED
};
enum EType
{
    UNDETERMINED,
    TREE,
    CROSS,
    FORWARD,
    BACKWARD
};

template <typename Tv, typename Te>
class Graph
{
protected:
    void reset()
    {
        //dtime, ftime,  pri, status, parent
        for (int i = 0; i < n; i++)
        {
            status(i) = UNDISCOVERED;
            priority(i) = INT_MAX;
            dTime(i) = -1;
            fTime(i) = -1;
            parent(i) = -1;
            for (int j = 0; j < n; j++)
                if (exist(i, j))
                    type(i, j) = UNDETERMINED;
        }
    };
    void BFS(int, int &);

public:
    Graph(/* args */);
    ~Graph();
    //node data and operations
    int n;
    virtual int insert(Tv const &) = 0; //return its number
    virtual Tv remove(int) = 0;
    virtual Tv &vertex(int n) = 0;
    virtual int inDegree(int) = 0;
    virtual int outDegree(int) = 0;
    virtual int firstNbr(int) = 0;     //input its number
    virtual int nextNbr(int, int) = 0; // int v, int j
    virtual VStatus &status(int) = 0;
    virtual int &dTime(int) = 0;
    virtual int &fTime(int) = 0;
    virtual int &parent(int) = 0;
    virtual int &priority(int) = 0;

    //edge operation
    int e;
    virtual bool exist(int, int) = 0;
    virtual void insert(Te const &, int w, int v, int u) = 0; //insert a edge of weight w between v and u
    virtual Te remove(int, int) = 0;                          //remove the edge between v and u
    virtual EType &type(int, int) = 0;
    virtual Te &edge(int, int) = 0;
    virtual int &weight(int, int) = 0;

    //alg
    virtual void bfs(int);
    virtual void bfs_p(int);
    virtual void dfs(int);
    virtual void dfs_p(int);
    virtual void dfs_iter(int);
    template <typename PU>
    void pfs(int, PU);

    stack<Tv> *tsort(int);
    void prim(int); // least supporting tree alg
    void dijkstra(int);
};

template <typename Tv>
struct Vertex
{
    Tv data;
    int inDegree;
    int outDegree;
    VStatus status;
    int dTime;
    int fTime;
    int parent;
    int priority;
    Vertex(Tv const &d = (Tv)0) : data(d), inDegree(0), outDegree(0), status(UNDISCOVERED),
                                  dTime(-1), fTime(-1), parent(-1), priority(INT_MAX) {}
};

template <typename Te>
struct Edge
{
    Te data;
    int weight;
    EType type;
    Edge(Te const &d, int w) : data(d), weight(w), type(UNDETERMINED) {}
};

template<typename Tv, typename Te>
struct prio_bfs
{
    virtual void operator() (Graph<Tv, Te>* g, int v, int u)
    {
        static int clock = 0;
        g -> priority(u) = ++clock;
        g -> parent(u) = v;
    }
};
template <typename Tv, typename Te>
class GraphMatrix
{
private:
    vector<Vertex<Tv>> V;
    vector<vector<Edge<Te> *>> E;
    int n;
    int e;
    void reset()
    {
        //dtime, ftime,  pri, status, parent
        for (int i = 0; i < n; i++)
        {
            status(i) = UNDISCOVERED;
            priority(i) = INT_MAX;
            dTime(i) = -1;
            fTime(i) = -1;
            parent(i) = -1;
            for (int j = 0; j < n; j++)
                if (exist(i, j))
                    type(i, j) = UNDETERMINED;
        }
    };
    //breadth first search
    void BFS(int v, int &clock)
    {
        Queue<int> q;
        q.inQueue(v);
        status(v) = DISCOVERED;
        dTime(v) = ++clock;
        while (!q.empty())
        {
            v = q.deQueue();
            for (int j = firstNbr(v); j > -1; j = nextNbr(v, j))
            {
                //start from node, discover it, renew the clock, record dtime
                if (status(j) == UNDISCOVERED)
                {
                    q.inQueue(j);
                    status(j) = DISCOVERED;
                    dTime(j) = ++clock;
                    type(v, j) = TREE;
                    parent(j) = v;
                }
                else
                {
                    type(v, j) = CROSS;
                }
            }
            status(v) = VISITED;
        }

        //check its neighbors, discovered them, mark the edges
    }
    void DFS(int v, int &clock)
    {
        status(v) = DISCOVERED;
        dTime(v) = ++clock;
        for (int u = firstNbr(v); u > -1; u = nextNbr(v, u))
        {
            if (status(u) == UNDISCOVERED)
            {
                parent(u) = v;
                type(v, u) = TREE;
                DFS(u, clock);
            }
            else if (status(u) == DISCOVERED)
                type(v, u) = BACKWARD;
            else if (status(u) == VISITED)
            {
                if (dTime(u) > dTime(v))
                    type(v, u) = FORWARD;
                else
                    type(v, u) = CROSS;
            }
        }
        fTime(v) = ++clock;
        status(v) = VISITED;
    };

    void DFS_iter(int v, int &clock)
    {
        status(v) = DISCOVERED;
        dTime(v) = ++clock;
        stack<int> s;
        s.push(v);
        while (!s.empty())
        {
            v = s.top();
            for (int u = firstNbr(v); u > -1; u = nextNbr(v, u))
            {
                if (status(u) == UNDISCOVERED)
                {
                    status(u) = DISCOVERED;
                    dTime(u) = ++clock;
                    type(v, u) = TREE;
                    parent(u) = v;
                    s.push(u);
                    break;
                }
                else if (status(u) == DISCOVERED)
                {
                    if (type(v, u) == UNDETERMINED)
                        type(v, u) == BACKWARD;
                }
                else
                {
                    if (type(v, u) == UNDETERMINED)
                    {
                        if (dTime(u) > dTime(v))
                            type(v, u) = FORWARD;
                        else
                            type(v, u) = CROSS;
                    }
                }
            }
            if (v == s.top())
            {
                fTime(v) = ++clock;
                status(v) = VISITED;
                s.pop();
            }
        }
    }

    bool Tsort_iter(int v, stack<Tv> *S)
    {
        status(v) = DISCOVERED;
        stack<int> s;
        s.push(v);
        while (!s.empty())
        {
            v = s.top();
            for (int u = firstNbr(v); u > -1; u = nextNbr(v, u))
            {
                if (status(u) == UNDISCOVERED)
                {
                    status(u) = DISCOVERED;
                    s.push(u);
                    break;
                }
                else if (status(v) == DISCOVERED)
                {
                    return false;
                }
            }
            if (v == s.top())
            {
                status(v) = VISITED;
                S->push(vertex(s.top()));
                s.pop();
            }
        }
        return true;
    };

    bool Tsort(int v, stack<Tv> *S)
    {
        status(v) = DISCOVERED;
        for (int u = firstNbr(v); u > -1; u = nextNbr(v, u))
        {
            if (status(u) == UNDISCOVERED)
            {
                status(u) = DISCOVERED;
                Tsort(u, S);
            }
            else if (status(u) == DISCOVERED)
                return false;
        }
        status(v) = VISITED;
        S->push(vertex(v));
        return true;
    }

    template <typename PU>
    void PFS(int v, PU prio)
    {
        //update the status
        status(v) = VISITED;
        type(parent(v), v) = TREE;
        while (1)
        {
            //check the neighbors
            for (int u = firstNbr(v); u > -1; u = nextNbr(v, u))
            {
                prio(this, v, u);
                //prio should update the priority and parent of u.
            }

            //check all the nodes
            for (int p = INT_MAX, w = 0; w < n; w++)
            {
                if (status(w) == UNDISCOVERED)
                    if (priority(w) < p)
                    {
                        p = priority(w);
                        v = w;
                    }
            }
            if (status(v) = VISITED)
                break;
        }
    };

    char type2name(EType et)
    {
        switch (et)
        {
        case UNDETERMINED:
            return 'U';
            break;

        case TREE:
            return 'T';
            break;

        case CROSS:
            return 'C';
            break;

        case BACKWARD:
            return 'B';
            break;

        case FORWARD:
            return 'F';
            break;
        default:
            return 'U';
            break;
        }
    }

public:
    GraphMatrix()
    {
        this->n = 0;
        this->e = 0;
    };
    ~GraphMatrix()
    {
        for (int i = 0; i < this->n; i++)
            for (int j = 0; j < this->n; j++)
                delete E[i][j];
    };
    //Vertex op
    virtual Tv &vertex(int n)
    {
        return V[n].data;
    }

    virtual int inDegree(int n)
    {
        return V[n].inDegree;
    }

    virtual int outDegree(int n)
    {
        return V[n].outDegree;
    }

    virtual int &dTime(int n)
    {
        return V[n].dTime;
    }

    virtual int &fTime(int n)
    {
        return V[n].fTime;
    }

    virtual int &parent(int n)
    {
        return V[n].parent;
    }

    virtual int &priority(int n)
    {
        return V[n].priority;
    }

    virtual VStatus &status(int n)
    {
        return V[n].status;
    }

    virtual int nextNbr(int v, int u)
    {
        while ((-1 < u) && (!exist(v, --u)))
            ;
        return u;
    }

    virtual int firstNbr(int v)
    {
        for (int i = this->n - 1; i > -1; i--)
            if (exist(v, i))
                return i;
        return -1;
        //or nextNbr(v, n)
    }

    virtual int insert(Tv const &d)
    {
        for (int i = 0; i < this->n; i++)
            E[i].insert((Edge<Te> *)NULL);
        this->n++;
        E.insert(vector<Edge<Te> *>(this->n, this->n, (Edge<Te> *)NULL));
        return V.insert(Vertex<Tv>(d));
    }

    virtual Tv remove(int i)
    {

        for (int j = 0; j < this->n; j++)
            if (exist(i, j))
            {
                delete E[i][j];
                V[j].inDegree -= 1;
            }
        E.remove(i);
        this->n--;
        Tv d = V[i].data;
        V.remove(i);

        //outdegree
        for (int j = 0; j < this->n; j++)
        {
            if (exist(j, i))
            {
                delete E[j][i];
                V[j].outDegree -= 1;
            }
            E[j].remove(i);
        }
        return d;
    }

    //edge op
    virtual bool exist(int i, int j)
    {
        //range of i and j are not examined
        return E[i][j] != NULL;
    }

    virtual void insert(Te const &d, int w, int i, int j)
    {
        if (!exist(i, j))
        {
            Edge<Te> *ep = new Edge<Te>(d, w);
            E[i].insert(j, ep);
            this->e++;
            V[i].outDegree += 1;
            V[j].inDegree += 1;
        }
    }

    virtual Te remove(int i, int j)
    {
        //we assume that exist(i,j) == 1,
        this->e--;
        Te d = E[i][j]->data;
        delete E[i][j];
        E[i][j] = NULL;
        V[i].outDegree -= 1;
        V[j].inDegree -= 1;
        return d;
    }

    virtual EType &type(int i, int j)
    {
        return E[i][j]->type;
    }

    virtual Te &edge(int i, int j)
    {
        return E[i][j]->data;
    }

    virtual int &weight(int i, int j)
    {
        return E[i][j]->weight;
    }

    void print_matrix()
    {
        std::cout << "------------------- Adjacency matrix -------------------" << std::endl;
        for (int i = 0; i < this->n; i++)
        {
            for (int j = 0; j < this->n; j++)
                std::cout << (int)exist(i, j) << ' ';
            std::cout << endl;
        }
        std::cout << endl;
    }

    void print_tree_structure()
    {
        std::cout << "------------------- Tree Structure -------------------" << std::endl;
        for (int i = 0; i < this->n; i++)
        {
            for (int j = 0; j < this->n; j++)
                if (exist(i, j))
                    printf("%c ", type2name(type(i, j)));
                else
                {
                    printf("0 ");
                }

            std::cout << endl;
        }
        std::cout << endl;
    }
    //breadth first search
    //queue version
    virtual void bfs(int v)
    {
        //reset the graph
        this->reset();
        int u = v;
        int clock = 0;
        do
            BFS(u, clock);
        while ((u = (++u % this->n)) != v && status(u) == UNDISCOVERED);
    };
    virtual void dfs(int v)
    {
        reset();
        int u = v;
        int clock = 0;
        do
            if (status(u) == UNDISCOVERED)
                DFS(u, clock);
        while ((u = (++u) % n) != v);
    };

    virtual void dfs_iter(int v)
    {
        reset();
        int u = v;
        int clock = 0;
        do
            if (status(u) == UNDISCOVERED)
                DFS_iter(u, clock);
        while ((u = (++u) % n) != v);
    };

    stack<Tv> *tsort_iter(int v)
    {
        stack<int> *S = new stack<int>;
        reset();
        int u = v;
        do
            if (status(u) == UNDISCOVERED)
                if (!Tsort_iter(v, S))
                {
                    while (!S->empty())
                        S->pop();
                    break;
                }
        while ((u = (++u) % n) != v);
        return S;
    };

    stack<Tv> *tsort(int v)
    {
        stack<int> *S = new stack<int>;
        reset();
        int u = v;
        do
            if (status(u) == UNDISCOVERED)
                if (!Tsort(v, S))
                {
                    while (!S->empty())
                        S->pop();
                    break;
                }
        while ((u = (++u) % n) != v);
        return S;
    };

    template<typename PU>
    void pfs(int v, PU prio)
    {
        reset();
        int u = v;
        do
        {
            if (status(u) == UNDISCOVERED)
                PFS(u, prio);
        } while (v != (u = ++u % n));
    };

    virtual void bfs_p(int v)
    {
        //prio_bfs<Tv, Te> prio;
        pfs(v, prio_bfs<Tv, Te>());
    };
    /*
    
    virtual void dfs_p(int);
    
    template <typename PU>
    

    void prim(int); // least supporting tree alg
    void dijkstra(int);
    */
};