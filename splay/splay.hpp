#pragma once
#include <typeinfo.h>
#include "stack.hpp"
#include "queue.hpp"
#define BinNodePosi(T) BinNode<T> *
#define stature(p) ((p) ? (p)->height : -1)
//some common judgement about BinNode instances
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) ((!IsRoot(x)) && (((x).parent->lc) == &(x)))
#define IsRChild(x) ((!IsRoot(x)) && (((x).parent->rc) == &(x)))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).lc) //这里x是一个实例，而不是一个指针，故而不需要考虑x为NULL的情况
#define HasRChild(x) ((x).rc)
#define HasChild(x) (HasLChild(x) || HasRChild(x))
#define HasBothChild(x) (HasLChild(x) && HasRChild(x))
#define IsLeaf(x) (!HasChild(x))
#define sibling(p) \
    (IsLChild(*(p)) ? (p)->parent->rc : (p)->parent->lc) //p can't be NULL or point to _root
#define uncle(x) \
    ((IsLChild((p)->parent)) ? (p)->parent->parent->rc : (p)->parent->parent->lc) //p can't be NULL or point to _root
#define FromParentTo(x) \
    (IsRoot((x)) ? this->_root : (IsLChild((x)) ? (x).parent->lc : (x).parent->rc)) //_root must be defined
#define balFac(x) (stature((x).lc) - stature((x).rc))
#define AvlBalanced(x) ((balFac(x) < 2) && (-2 < balFac(x)))
#define tallChild(p) ( \
    stature((p)->lc) > stature((p)->rc) ? (p)->lc : (stature((p)->rc) > stature((p)->lc) ? (p)->rc : (IsLChild(*(p)) ? (p)->lc : (p)->rc)))
enum RBColor
{
    RB_RED,
    RB_BLACK
};

//Clean function is writen by Deng
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
class BinNode
{
public:
    T data;
    BinNodePosi(T) parent;
    BinNodePosi(T) lc;
    BinNodePosi(T) rc;
    int height;
    int npl;
    RBColor color;
    BinNode() : parent(NULL), lc(NULL), rc(NULL), height(0), npl(1), color(RB_RED) {}
    BinNode(T d, BinNodePosi(T) p = NULL, BinNodePosi(T) l = NULL, BinNodePosi(T) r = NULL,
            int h = 0, int n = 1, RBColor c = RB_RED) : data(d), parent(p), lc(l), rc(r), height(h), npl(n), color(c) {}
    int size() const; //size is computed when needed
    BinNodePosi(T) insertAsLc(const T &);
    BinNodePosi(T) insertAsRc(const T &);
    BinNodePosi(T) succ(); //中序遍历次序下的后继
    BinNodePosi(T) pred(); //中序遍历次序下的前面一个
};

template <typename T>
class BinTree
{
public:
    int _size;
    BinNodePosi(T) _root;
    virtual int updateHeight(BinNodePosi(T) x);
    void updateHeightAbove(BinNodePosi(T) x);
    static int removeAt(BinNodePosi(T) x);
    template <typename VST>
    static void visitAlongLeftBranch(Stack<BinNodePosi(T)> &s, VST &visit);
    static void goAlongLeftBranch(BinNodePosi(T) x, Stack<BinNodePosi(T)> &s);
    static void goToLeftmostLeaf(Stack<BinNodePosi(T)> &s);

    BinTree() : _size(0), _root(NULL) {}
    ~BinTree()
    {
        if (_size > 0)
            remove(_root);
    } //``remove``?
    int size() const { return _size; }
    bool empty() const { return !_root; }
    BinNodePosi(T) root() const { return _root; }
    BinNodePosi(T) insertAsRoot(const T &e); //一颗树的开端
    BinNodePosi(T) insertAsLc(BinNodePosi(T) x, const T &e);
    BinNodePosi(T) insertAsRc(BinNodePosi(T) x, const T &e);
    BinNodePosi(T) attachAsLc(BinNodePosi(T) x, BinTree<T> *&S);
    BinNodePosi(T) attachAsRc(BinNodePosi(T) x, BinTree<T> *&S);
    int remove(BinNodePosi(T) x);
    BinTree<T> *secede(BinNodePosi(T) x);
    template <typename VST>
    void travPre_Recur(BinNodePosi(T) x, VST &visit);
    template <typename VST>
    void travIn_Recur(BinNodePosi(T) x, VST &visit);
    template <typename VST>
    void travPost_Recur(BinNodePosi(T) x, VST &visit);
    template <typename VST>
    void travPre_I1(BinNodePosi(T) x, VST &visit);
    template <typename VST>
    void travPre_I2(BinNodePosi(T) x, VST &visit);
    template <typename VST>
    void travIn_I1(BinNodePosi(T) x, VST &visit);
    template <typename VST>
    void travIn_I2(BinNodePosi(T) x, VST &visit);
    template <typename VST>
    void travIn_I3(BinNodePosi(T) x, VST &visit);
    template <typename VST>
    void travIn_I4(BinNodePosi(T) x, VST &visit);
    template <typename VST>
    void travPost_I1(BinNodePosi(T) x, VST &visit);
    template <typename VST>
    void travLevel(BinNodePosi(T) x, VST &visit);
    //template<typename VST> void travPost_I2(BinNodePosi(T) x, VST& visit);

    //the following are for exercising
    void print_structure(BinNodePosi(T) x = NULL, int n = -1);
    void print_structure_I(BinNodePosi(T) x = NULL);
    void swap();
    bool check_cumsum(BinNodePosi(T) x, const T &e = 0);
    bool check_cumsum_I(BinNodePosi(T) x, const T &e = 0);
    void set_as_max_of_descendant(BinNodePosi(T) x);
    void encode_by_level();
};

template <typename T>
class Splay : public BinTree<T>
{
protected:
    BinNodePosi(T) _hot;
    BinNodePosi(T) splay(BinNodePosi(T) v)
    {
        if (!v)
            return v;
        BinNodePosi(T) p;
        BinNodePosi(T) g;
        BinNodePosi(T) gg;
        while ((p = v->parent) && (g = p->parent))
        {
            gg = g->parent;
            if (IsLChild(*v))
            {
                if (IsLChild(*p))
                {
                    g->lc = p->rc;
                    if (g->lc)
                        g->lc->parent = g;
                    p->rc = g;
                    g->parent = p;
                    p->lc = v->rc;
                    if (v->rc)
                        v->rc->parent = p;
                    v->rc = p;
                    p->parent = v;
                }
                else
                {
                    p->lc = v->rc;
                    if (p->lc)
                        p->lc->parent = p;
                    g->rc = v->lc;
                    if (g->rc)
                        g->rc->parent = g;
                    v->lc = g;
                    g->parent = v;
                    v->rc = p;
                    p->parent = v;
                }
            }
            else
            {
                if (IsLChild(*p))
                {
                    g->lc = v->rc;
                    if (g->lc)
                        g->lc->parent = g;
                    p->rc = v->lc;
                    if (p->rc)
                        p->rc->parent = p;
                    v->lc = p;
                    p->parent = v;
                    v->rc = g;
                    g->parent = v;
                }
                else
                {
                    g->rc = p->lc;
                    if (g->rc)
                        g->rc->parent = g;
                    p->rc = v->lc;
                    if (p->rc)
                        p->rc->parent = p;
                    v->lc = p;
                    p->parent = v;
                    p->lc = g;
                    g->parent = p;
                }
            }

            v->parent = gg;
            if (gg)
            {
                if (gg -> lc == g)
                {
                    gg->lc = v;
                }
                else
                {
                    gg->rc = v;
                }
            }

            this->updateHeight(g);
            this->updateHeight(p);
            this->updateHeight(v);
        }

        if (p)
        {
            if (IsLChild(*v))
            {
                p->lc = v->rc;
                if (p->lc)
                    p->lc->parent = p;
                v->rc = p;
                p->parent = v;
            }
            else
            {
                p->rc = v->lc;
                if (p->rc)
                    p->rc->parent = p;
                v->lc = p;
                p->parent = v;
            }
            this->updateHeight(p);
            this->updateHeight(v);
        }

        v->parent = NULL;
        return v;
    };

public:
    BinNodePosi(T) & search(const T &e)
    {
        this->_hot = NULL;
        if (!this->_root || (this->_root->data == e))
            return this->_root; //empty tree, or hit at root

        BinNodePosi(T) v = this->_root;
        bool flag_lc = false;

        while (v && v->data != e)
        {
            this->_hot = v;
            flag_lc = e < v->data; //for returning a reference
            v = flag_lc ? v->lc : v->rc;
        }

        this->_root = splay(v ? v : this->_hot);
        return this->_root;
        //note that the returned pointer may point to _root = _hot
        //if the desired node is not found
    };
    BinNodePosi(T) insert(const T &e)
    {
        if (!this->_root)
        {
            this->_size++;
            return this->_root = new BinNode<T>(e);
        }
        search(e);
        if (e == this->_root->data)
            return this->_root;
        else if (e < this->_root->data)
        {
            BinNodePosi(T) p = new BinNode<T>(e);
            p->lc = this->_root->lc;
            if (p->lc)
                p->lc->parent = p;
            this->_root->lc = NULL;
            p->rc = this->_root;
            this->_root->parent = p;
            this->_root = p;
        }
        else
        {
            BinNodePosi(T) p = new BinNode<T>(e);
            p->rc = this->_root->rc;
            if (p->rc)
                p->rc->parent = p;
            this->_root->rc = NULL;
            p->lc = this->_root;
            this->_root->parent = p;
            this->_root = p;
        }
        this->_size++;
        this->updateHeightAbove(this->_hot);
        return this->_root;
    };
    bool remove(const T& e)
    {
        search(e);
        if((!this -> _root ) || (e != this -> _root -> data)) return false;
        BinNodePosi(T) w = this -> _root;
        if(!this -> _root -> lc)
        {
            this -> _root = this -> _root ->rc;
            if(this -> _root) this -> _root -> parent = NULL;
        }
        else if(!this -> _root -> rc)
        {
            this -> _root = this -> _root ->lc;
            this -> _root -> parent = NULL;
        }
        else
        {
            this -> _root = this -> _root -> rc;
            this -> _root -> parent = NULL;
            this -> _root -> lc = NULL;
            search(e);
            this -> _root -> lc = w -> lc;
            this -> _root -> lc -> parent = this -> _root;
            this -> updateHeight(this -> _root);
        }
        this -> _size --;
        release(w ->data); release(w);
        return true;
    };
};

template <typename T>
int BinNode<T>::size() const
{
    int s = 1;
    if (lc)
        s += lc->size();
    if (rc)
        s += rc->size();
    return s;
}

template <typename T>
BinNodePosi(T) BinNode<T>::insertAsLc(const T &data)
{
    /*
    BinNodePosi(T) bn;
    bn = new BinNode(data);
    bn->parent = this;
    lc = bn;
    return bn;
    */
    return lc = new BinNode<T>(data, this);
}

template <typename T>
BinNodePosi(T) BinNode<T>::insertAsRc(const T &data)
{
    /*
    BinNodePosi(T) bn;
    bn = new BinNode(data);
    bn->parent = this;
    rc = bn;
    return bn;
    */
    return rc = new BinNode<T>(data, this);
}

template <typename T>
BinNodePosi(T) BinNode<T>::succ()
{
    if (rc)
    {
        BinNodePosi(T) bn = rc;
        while (bn->lc)
            bn = bn->lc;
        return bn;
    }
    else
    {
        BinNodePosi(T) bn = this; //注意根节点的特殊性
        while (IsRChild(*bn))
            bn = bn->parent;
        return bn->parent;
    }
}

template <typename T>
BinNodePosi(T) BinNode<T>::pred()
{
    BinNodePosi(T) bn;
    if (lc)
    {
        bn = lc;
        while (bn->rc)
            bn = bn->rc;
        return bn;
    }
    else
    {
        bn = this;
        while (IsLChild(*bn))
            bn = bn->parent;
        return bn->parent;
    }
}

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRoot(const T &e)
{
    _size = 1; //_size set as 1
    return _root = new BinNode<T>(e);
}

template <typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x)
{
    int a = stature(x->lc);
    int b = stature(x->rc);
    return x->height = 1 + ((a > b) ? a : b);
}

template <typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x)
{
    while (x)
    {
        updateHeight(x);
        x = x->parent;
    }
    /*
    //modified:
    int h;
    while(x)
    {
        h = x -> height;
        updateHeight(x);
        if(h == x -> height) break;
        x = x -> parent;
    }
    */
}

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsLc(BinNodePosi(T) x, const T &e)
{
    x->insertAsLc(e);
    updateHeightAbove(x);
    _size++;
    return x->lc;
}

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRc(BinNodePosi(T) x, const T &e)
{
    x->insertAsRc(e);
    updateHeightAbove(x);
    _size++;
    return x->rc;
}

template <typename T>
BinNodePosi(T) BinTree<T>::attachAsLc(BinNodePosi(T) x, BinTree<T> *&S)
{ // 注意这里是指针的引用
    if (x->lc = S->_root)
        S->_root->parent = x; //注意 T 有可能是空树
    _size += S->_size;
    updateHeightAbove(x);
    release(S);
    S = NULL; //指针的引用的用处在这里
    return x;
}

template <typename T>
BinNodePosi(T) BinTree<T>::attachAsRc(BinNodePosi(T) x, BinTree<T> *&S)
{ // 注意这里是指针的引用
    if (x->rc = S->_root)
        S->_root->parent = x; //注意 T 有可能是空树
    _size += S->_size;
    updateHeightAbove(x);
    release(S);
    S = NULL; //指针的引用的用处在这里
    return x;
}

template <typename T>
int BinTree<T>::remove(BinNodePosi(T) x)
{
    FromParentTo(*x) = NULL;
    updateHeightAbove(x);
    int n = removeAt(x);
    _size -= n;
    return n;
}

template <typename T>
int BinTree<T>::removeAt(BinNodePosi(T) x)
{
    if (!x)
        return 0;
    int n = 1 + removeAt(x->lc) + removeAt(x->rc);
    release(x->data);
    release(x);
    return n;
}

template <typename T>
BinTree<T> *BinTree<T>::secede(BinNodePosi(T) x)
{
    FromParentTo(x) = NULL;
    //更新高度
    updateHeightAbove(x->parent);
    //下面生成子树，要对_root、_size进行处理
    BinTree<T> *bt = new BinTree<T>;
    bt->insertAsRoot(x);
    bt->_size = x->size();
    //原树的_root不变, 处理原树的_size
    _size -= bt->_size;
    return bt;
}

template <typename T>
template <typename VST>
void BinTree<T>::travPre_Recur(BinNodePosi(T) x, VST &visit)
{
    if (!x)
        return;
    visit(x->data);
    travPre_Recur(x->lc, visit);
    travPre_Recur(x->rc, visit);
}

template <typename T>
template <typename VST>
void BinTree<T>::travIn_Recur(BinNodePosi(T) x, VST &visit)
{
    if (!x)
        return;
    travIn_Recur(x->lc, visit);
    visit(x->data);
    travIn_Recur(x->rc, visit);
}

template <typename T>
template <typename VST>
void BinTree<T>::travPost_Recur(BinNodePosi(T) x, VST &visit)
{
    if (!x)
        return;
    travPost_Recur(x->lc, visit);
    travPost_Recur(x->rc, visit);
    visit(x->data);
}

template <typename T>
template <typename VST>
void BinTree<T>::travPre_I1(BinNodePosi(T) x, VST &visit)
{
    Stack<BinNodePosi(T)> s;
    s.push(x);
    while (!s.empty())
    {
        BinNodePosi(T) x = s.pop();
        visit(x->data);
        if (x->rc)
            s.push(x->rc);
        if (x->lc)
            s.push(x->lc);
    }
}

template <typename T>
template <typename VST>
void BinTree<T>::visitAlongLeftBranch(Stack<BinNodePosi(T)> &s, VST &visit)
{
    BinNodePosi(T) x = s.pop();
    while (x)
    {
        visit(x->data);
        if (x->rc)
            s.push(x->rc);
        x = x->lc;
    }
}

template <typename T>
void BinTree<T>::goAlongLeftBranch(BinNodePosi(T) x, Stack<BinNodePosi(T)> &s)
{
    while (x)
    {
        s.push(x);
        x = x->lc;
    }
}

template <typename T>
template <typename VST>
void BinTree<T>::travPre_I2(BinNodePosi(T) x, VST &visit)
{
    Stack<BinNodePosi(T)> s;
    s.push(x);
    while (!s.empty())
        visitAlongLeftBranch(s, visit);
}

template <typename T>
template <typename VST>
void BinTree<T>::travIn_I1(BinNodePosi(T) x, VST &visit)
{
    Stack<BinNodePosi(T)> s;
    while (true)
    {
        goAlongLeftBranch(x, s);
        if (s.empty())
            break;
        x = s.pop();
        visit(x->data);
        x = x->rc;
    }
}

template <typename T>
template <typename VST>
void BinTree<T>::travIn_I2(BinNodePosi(T) x, VST &visit)
{
    Stack<BinNodePosi(T)> s;
    while (true)
    {
        if (x)
        {
            s.push(x);
            x = x->lc;
        }
        else if (!s.empty())
        {
            x = s.pop();
            visit(x->data);
            x = x->rc;
        }
        else
            break;
    }
}

template <typename T>
template <typename VST>
void BinTree<T>::travIn_I3(BinNodePosi(T) x, VST &visit)
{
    bool traceback = false;
    while (true)
    {
        if (!traceback && HasLChild(*x))
            x = x->lc;
        else
        {
            visit(x->data);
            if (HasRChild(*x))
            {
                x = x->rc;
                traceback = false;
            }
            else
            {
                x = x->succ();
                if (!x)
                    break;
                traceback = true;
            }
        }
    }
}

template <typename T>
template <typename VST>
void BinTree<T>::travIn_I4(BinNodePosi(T) x, VST &visit)
{
    while (true)
    {
        if (HasLChild(*x))
            x = x->lc;
        else
        {
            visit(x->data);
            while (!HasRChild(*x))
                if (!(x = x->succ()))
                    return;
                else
                    visit(x->data);
            x = x->rc;
        }
    }
}

template <typename T>
void BinTree<T>::goToLeftmostLeaf(Stack<BinNodePosi(T)> &s)
{
    BinNodePosi(T) x;
    while (x = s.top())
    {
        if (HasLChild(*x))
        {
            if (HasRChild(*x))
                s.push(x->rc);
            s.push(x->lc);
        }
        else
            s.push(x->rc);
    }
    s.pop();
}

template <typename T>
template <typename VST>
void BinTree<T>::travPost_I1(BinNodePosi(T) x, VST &visit)
{
    Stack<BinNodePosi(T)> s;
    if (x)
        s.push(x);
    while (!s.empty())
    {
        if (s.top() != x->parent)
            goToLeftmostLeaf(s);
        x = s.pop();
        visit(x->data);
    }
}

template <typename T>
template <typename VST>
void BinTree<T>::travLevel(BinNodePosi(T) x, VST &visit)
{
    Queue<BinNodePosi(T)> q;
    q.inQueue(x);
    while (!q.empty())
    {
        x = q.deQueue();
        if (x)
        {
            visit(x->data);
            q.inQueue(x->lc);
            q.inQueue(x->rc);
        }
    }
}

template <typename T>
void BinTree<T>::print_structure(BinNodePosi(T) x, int n)
{
    //if(!x) x = _root;
    for (int i = 0; i < n; i++)
        std::cout << "|  "; //<< std::endl;
    if (!x)
        std::cout << "|--X" << std::endl;
    else
    {
        if (n >= 0)
            std::cout << "|--" << x->data << "(" << x->height << ")" << std::endl;
        else
            std::cout << "----------Structure of the binary tree:----------\n"
                      << x->data << "(" << x->height << ")" << std::endl;
    }
    if (x)
    {
        print_structure(x->rc, n + 1);
        print_structure(x->lc, n + 1);
    }
}

template <typename T>
void BinTree<T>::print_structure_I(BinNodePosi(T) x)
{
    Stack<BinNodePosi(T)> s;
    Stack<int> si;
    int n;
    std::cout << "----------Structure of the binary tree:----------\n"
              << std::endl;
    if (x)
        std::cout << x->data << std::endl;
    s.push(x->lc);
    s.push(x->rc);
    si.push(0);
    si.push(0);

    while (!s.empty())
    {
        x = s.pop();
        n = si.pop();
        for (int i = 0; i < n; i++)
            std::cout << "|  "; //<< std::endl;
        if (x)
        {
            std::cout << "|--" << x->data << std::endl;
            s.push(x->lc);
            s.push(x->rc);
            si.push(n + 1);
            si.push(n + 1);
        }
        else
            std::cout << "|--X" << std::endl;
    }
}

template <typename T>
void BinTree<T>::swap()
{
    Stack<BinNodePosi(T)> s;
    s.push(_root);
    BinNodePosi(T) x;
    while (!s.empty())
    {
        x = s.pop();
        BinNodePosi(T) t = x->lc;
        x->lc = x->rc;
        x->rc = t;
        if (x->rc)
            s.push(x->rc);
        if (x->lc)
            s.push(x->lc);
    }
}

template <typename T> //remain
bool BinTree<T>::check_cumsum(BinNodePosi(T) x, const T &e)
{
    if (!x)
        return true;
    return (x->data >= e) &&
           (check_cumsum(x->lc, e + x->data)) &&
           (check_cumsum(x->rc, e + x->data));
}

template <typename T> //remain
bool BinTree<T>::check_cumsum_I(BinNodePosi(T) x, const T &e)
{
    Stack<BinNodePosi(T)> s;
    Stack<T> st;
    s.push(x);
    st.push(e);
    T p;
    while (!s.empty())
    {
        x = s.pop();
        p = st.pop();
        if (x)
        {
            if (x->data < p)
                return false;
            s.push(x->rc);
            s.push(x->lc);
            st.push(p + x->data);
            st.push(p + x->data);
        }
    }
    return true;
}

template <typename T>
void BinTree<T>::set_as_max_of_descendant(BinNodePosi(T) x)
{
    Stack<BinNodePosi(T)> s;
    if (x)
        s.push(x);
    while (!s.empty())
    {
        if (s.top() != x->parent)
            goToLeftmostLeaf(s);
        x = s.pop();
        if (HasBothChild(*x))
            x->data = (x->lc->data > x->rc->data) ? x->lc->data : x->rc->data;
        else if (HasLChild(*x))
            x->data = x->lc->data;
        else if (HasRChild(*x))
            x->data = x->rc->data;
    }
}

template <typename T>
void BinTree<T>::encode_by_level()
{
    Stack<BinNodePosi(T)> s;
    _root->data = 0;
    if (_root->rc)
        s.push(_root->rc);
    if (_root->lc)
        s.push(_root->lc);

    while (!s.empty())
    {
        BinNodePosi(T) x = s.pop();
        if (IsLChild(*x))
            x->data = x->parent->data * 2 + 1;
        else if (IsRChild(*x))
            x->data = x->parent->data * 2 + 2;

        if (x->rc)
            s.push(x->rc);
        if (x->lc)
            s.push(x->lc);
    }
}
