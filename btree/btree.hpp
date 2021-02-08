#include "vector.hpp"
#include "queue.hpp"
#define BTNodePosi(T) BTNode<T>*

//Clean function by Deng
template <typename T>
struct Cleaner
{
    static void clean(T x)
    { //相当于递归基
#ifdef _DEBUG
        static int n = 0;
        if (7 > strlen(typeid(T).name()))
        { //复杂类型一概忽略，只输出基本类型
            printf("\t<%s>[%d]=", typeid(T).name(), ++n);
            print(x);
            printf(" purged\n");
        }
#endif
    }
};

template <typename T>
struct Cleaner<T *>
{
    static void clean(T *x)
    {
        if (x)
        {
            delete x;
        } //如果其中包含指针，递归释放
#ifdef _DEBUG
        static int n = 0;
        printf("\t<%s>[%d] released\n", typeid(T *).name(), ++n);
#endif
    }
};

template <typename T>
void release(T x) { Cleaner<T>::clean(x); }

template <typename T>
struct BTNode
{
    BTNodePosi(T) parent;
    vector<T> keys;
    vector<BTNodePosi(T)> child;
    BTNode()
    {
        parent = NULL;
        child.insert(0, NULL);
    }
    //BTNode can only be initialized as a root
    BTNode(const T &e, BTNodePosi(T) lc = NULL, BTNodePosi(T) rc = NULL)
    {
        parent = NULL;
        keys.insert(0, e);
        child.insert(0, lc);
        child.insert(1, rc);
        if (lc)
            lc->parent = this;
        if (rc)
            rc->parent = this;
    }
};

template <typename T>
class BTree
{
protected:
    int _size;
    int _order;
    BTNodePosi(T) _root;
    BTNodePosi(T) _hot;
    void solveUnderFlow(BTNodePosi(T));
    void solveOverFlow(BTNodePosi(T));
public:
    BTree(int order = 3) : _order(order), _size(0)
    {
        _root = new BTNode<T>;
    }

    ~BTree()
    {
        if (_root)
            release(_root);
    }

    int order() const { return _order; }
    int size() const { return _size; }
    BTNodePosi(T)& root(){return _root;}
    bool empty() const{return !_root;}
    BTNodePosi(T) search(const T& e);
    bool insert(const T& e);
    bool remove(const T& e);
    void print_structure(BTNodePosi(T));
};

template<typename T>
BTNodePosi(T) BTree<T>::search(const T& e)
{
    BTNodePosi(T) p = _root;
    _hot = NULL;
    while(p)
    {
        Rank r = p -> keys.search(e);
        if(-1 < r && p -> keys[r] == e) return p;
        _hot = p;
        p = p -> child[r+1];
    }
    return p;// p is NULL if e is not found or the tree is empty
}

template<typename T>
bool BTree<T>::insert(const T& e)
{
    BTNodePosi(T) p = search(e);
    if(p) return false;//e found or tree empty
    Rank r = _hot -> keys.search(e);
    _hot -> keys.insert(r+1,e);
    _hot -> child.insert(NULL);
    _size ++;
    solveOverFlow(_hot);
    return true;
} 

template<typename T>
void BTree<T>::solveOverFlow(BTNodePosi(T) v)
{
    if(v -> keys.size() < _order) return;//no need to split

    int s = (_order >> 1);
    BTNodePosi(T) lc = new BTNode<T>();
    lc -> keys = vector<T>(v -> keys, 0, s);
    lc -> child = vector<BTNodePosi(T)>(v -> child, 0, s+1);
    for(int i = 0; i < s+1; i++)
        if(v -> child[i]) v-> child[i] -> parent = lc;

    BTNodePosi(T) rc = new BTNode<T>();
    rc -> keys = vector<T>(v -> keys, s+1, v -> keys.size());
    rc -> child = vector<BTNodePosi(T)>(v -> child, s+1, v->child.size());
    for(int i = s+1; i < v->child.size(); i++)
        if(v -> child[i]) v -> child[i] -> parent = rc;

    if(v == _root)
    {
        _root = new BTNode<T>(v -> keys[s], lc, rc);
        release(v);
    }
    else
    {
        Rank r = v -> parent -> keys.search(v -> keys[s]);
        v -> parent -> keys.insert(r+1, v -> keys[s]);
        v -> parent -> child[r + 1] = lc;
        v -> parent -> child.insert(r + 2, rc);
        BTNodePosi(T) p = v -> parent;
        lc -> parent = p; rc -> parent = p; 
        release(v);
        solveOverFlow(p);
        // v is not _root and has to split
    }
}

template<typename T>
void BTree<T>::print_structure(BTNodePosi(T) v)
{
    cout << "--------------------BTree Structure--------------------" << endl;
    queue<BTNodePosi(T)> q;
    q.enQueue(v);
    int m = 1;
    int n = 1;
    int count = 0;
    while(!q.empty())
    {
        //cout << "--------------------BTree Structure--------------------" << endl;
        for(;count < m; count ++)
        {
            v = q.top();
            if(!v -> keys.empty())
            {
                for(int i = 0; i < v -> keys.size(); i ++)//print keys in v
                cout << v -> keys[i] << "_";
                if(v -> parent)//print its parent in a parenthesis
                {
                    BTNodePosi(T) p = v -> parent;
                    cout << "(";
                    for(int i = 0; i < p -> keys.size(); i++)
                    {
                        cout << p -> keys[i] << "_";
                    }
                    cout << ")";
                }
                else// N if parent is NULL
                {
                    cout << "(N)";
                }
                
                cout << ' ';
                for(int i = 0; i < v -> child.size(); i ++, n++)//push child into q, n +
                    if(v -> child[i]) 
                        q.enQueue(v -> child[i]);
            }
            q.deQueue();
        }
        m = n;
        cout << endl;
    }
    cout << endl;
}

template<typename T>
bool BTree<T>::remove(const T& e)
{
    BTNodePosi(T) v = search(e);
    if(!v) return false; //empty tree or e not found

    Rank r = v ->keys.search(e);
    BTNodePosi(T) u = v ->child[r+1];
    if(!u)
    {
        v -> keys.remove(r);
        v -> child.remove(v->child.size() - 1);
        solveUnderFlow(v);
    }
    else
    {
       while(u -> child[0]) u = u -> child[0];
       T t = u -> keys[0];
       u -> keys[0] = v -> keys[r];
       v -> keys[r] = t; //swap there value
       u -> keys.remove(0);
       u -> child.remove(u -> child.size()-1);
       solveUnderFlow(u);
    }
    _size--;
    return true;
}

template<typename T>
void BTree<T>::solveUnderFlow(BTNodePosi(T) v)
{
    int s = _order % 2?(_order >> 1 ) + 1 : (_order >> 1);
    while(1)
    {
        if(v->child.size() >= s ) return;
        else if(v == _root && _root->keys.size() > 0) return;
        else if(v == _root && _root->keys.size() == 0)
        {
            BTNodePosi(T) w = _root;
            _root = w ->child[0];
            _root -> parent = NULL;
            release(w);
            return; 
        }
        else
        {
            BTNodePosi(T) p = v -> parent;
            Rank r = 0;//
            for(;p -> child[r] != v; r++);//p -> keys.search(v->keys[0]);
            if(p ->child[r] && p -> child[r] -> child.size() > s)
            {
                BTNodePosi(T) lc = p -> child[r];
                v -> keys.insert(0, p -> keys[r]);
                v -> child.insert(v -> child.size()-1, NULL);

                p -> keys[r] = lc -> keys[lc->keys.size()-1];

                lc -> keys.remove( lc -> keys.size()-1 );
                lc -> child.remove(lc->child.size()-1);
            }
            else if(p ->child[r+2] && p -> child[r+2] -> child.size() > s)
            {
                BTNodePosi(T) rc = p -> child[r+2];
                v -> keys.insert(v -> keys.size()-1, p -> keys[r]);
                v -> child.insert(v -> child.size()-1, NULL);

                p -> keys[r] = rc -> keys[0];

                rc -> keys.remove(0);
                rc -> child.remove(rc->child.size()-1);
            }
            else if(p -> child[r]) //merge lc
            {
                BTNodePosi(T) lc = p -> child[r];
                lc -> keys.insert(p -> keys[r]);
                lc -> child.insert(NULL);

                Rank lo = lc -> keys.size();
                Rank length = v -> keys.size();
                for(int i = 0; i < length; lc -> keys[i + lo] = v -> keys[i], lc -> child.insert(NULL), i++);

                p -> keys.remove(r); 
                p -> child.remove(r+1);
                release(v);
                v = lc;
            }
            else //merge rc
            {
                r = r+1;
                BTNodePosi(T) rc = p -> child[r+1];
                v -> keys.insert(p -> keys[r]);
                v -> child.insert(NULL);

                Rank lo = v -> keys.size();
                Rank length = rc -> keys.size();
                for(int i = 0; i < length; v -> keys[i + lo] = rc -> keys[i], v -> child.insert(NULL), i++);

                p -> keys.remove(r); 
                p -> child.remove(r+1);
                release(rc);
            }
            v = p;
        }
    }    
}



