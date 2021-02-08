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
    (IsRoot((x)) ? this -> _root : (IsLChild((x)) ? (x).parent->lc : (x).parent->rc)) //_root must be defined
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
class BST : public BinTree<T>
{
protected:
    BinNodePosi(T) _hot;
    BinNodePosi(T) connect34(
        BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c,
        BinNodePosi(T) T0, BinNodePosi(T) T1, BinNodePosi(T) T2, BinNodePosi(T) T3)
    {
        b->lc = a;
        a->parent = b;
        b->rc = c;
        c->parent = b;
        a->lc = T0;
        if (T0) T0->parent = a;
        a->rc = T1;
        if (T1) T1->parent = a;
        c->lc = T2;
        if (T2) T2->parent = c;
        c->rc = T3;
        if (T3) T3->parent = c;
        this->updateHeight(b);
        return b;
    };

    BinNodePosi(T) rotateAt(BinNodePosi(T) v) // v has parent and grandparent
    {
        BinNodePosi(T) p = v->parent;
        BinNodePosi(T) g = p->parent;
        if (IsLChild(*v))
        {
            if (IsLChild(*p))
            {
                p->parent = g->parent;
                return connect34(v, p, g, v->lc, v->rc, p->rc, g->rc);
            }
            else
            {
                v->parent = g->parent;
                return connect34(g, v, p, g->lc, v->lc, v->rc, p->rc);
                //return v;
            }
        }
        else
        {
            if (IsRChild(*p))
            {
                p->parent = g->parent;
                return connect34(g, p, v, g->lc, p->lc, v->lc, v->rc);
                //return p;
            }
            else
            {
                v->parent = g->parent;
                return connect34(p, v, g, p->lc, v->lc, v->rc, g->rc);
                //return v;
            }
        }
    };
    void _searchAllAt(T const &e, Queue<BinNodePosi(T)>& q, BinNodePosi(T) v)
    {
        if(!v)  return;
        if(v -> data == e) 
        {
            q.inQueue(v);
            _searchAllAt(e, q, v -> lc);
            _searchAllAt(e, q, v -> rc);
        }
        else if(v -> data < e) _searchAllAt(e, q, v -> rc);
        else _searchAllAt(e, q, v -> lc);
    }
public:
    virtual BinNodePosi(T) & search(T const &e) //remain
    {
        _hot = NULL;
        if (!this->_root || (this->_root->data == e))
            return this->_root; //empty tree, or hit at root

        BinNodePosi(T) v = this->_root;
        bool flag_lc = false;

        while (v && v->data != e)
        {
            _hot = v;
            flag_lc = e < v->data; //for returning a reference
            v = flag_lc ? v->lc : v->rc;
        }
        if (flag_lc)
            return _hot->lc;
        else
            return _hot->rc;
    };

    virtual Queue<BinNodePosi(T)> searchAll_recur(T const &e)
    {
        Queue<BinNodePosi(T)> q;
        _searchAllAt(e, q, this-> _root);
        return q;
    }

    virtual Queue<BinNodePosi(T)> searchAll_iter(T const &e)
    {
        Queue<BinNodePosi(T)> q;
        Queue<BinNodePosi(T)> qv;
        BinNodePosi(T) v;
        qv.inQueue(this -> _root);
        while(!qv.empty())
        {
            v = qv.deQueue();
            if(!v) continue;
            if(v -> data == e)
            {
                q.inQueue(v);
                qv.inQueue(v -> lc); qv.inQueue(v -> rc);
            }
            else if(v -> data < e) qv.inQueue(v -> rc);
            else qv.inQueue(v -> lc);
        }
        return q;
    }
    virtual BinNodePosi(T) insert(T const &e)
    {
        BinNodePosi(T) &v = search(e);
        if (v)
            return v; //assume no duplicate keys
        v = new BinNode<T>(e, _hot);
        ++this->_size;
        this->updateHeightAbove(_hot);
        return v;
    };

    virtual bool remove(T const &e)
    {
        BinNodePosi(T) &p = search(e);
        if (!p)
            return false; //return false if e is not in BST

        if (!(p->lc) && !(p->rc)) //No child
        {
            p = NULL;
            return true;
        }
        else
        {
            BinNodePosi(T) u;
            if (!(p->lc)) // only rc
            {
                u = p;
                p = p->rc;
                p->parent = _hot;
                //release(u -> data); release(u);
            }
            else if (!(p->rc)) //only lc
            {
                u = p;
                p = p->lc;
                p->parent = _hot;
                //release(u -> data); release(u);
            }
            else
            {
                u = p->succ();
                swap(u->data, p->data);
                BinNodePosi(T) &w = (u->parent == p) ? (u->parent->rc) : (u->parent->lc);
                //(u -> parent == p)? (u -> parent -> rc) : (u -> parent -> lc) = u -> rc;
                //注意条件选择式返回的是副本, 不是原变量的引用, 这里通过&成为引用
                w = u->rc;
                //FromParentTo(*u) =
                _hot = u->parent;
                if (u->rc)
                    u->rc->parent = u->parent;

                /*
                //另一种想法则是使用前驱
                u = p->pred();
                swap(u->data, p->data);
                BinNodePosi(T) &w = (u->parent == p) ? (u->parent->rc) : (u->parent->lc);
                w = u->lc;
                _hot = u->parent;
                if (u->lc)
                    u->lc->parent = u->parent;
                */
            }

            release(u->data);
            release(u);
            this->updateHeightAbove(_hot);
            this->_size--;
            return true;
        }
    };
};

template <typename T>
class AVL : public BinTree<T>
{
protected:
    BinNodePosi(T) _hot;
    BinNodePosi(T) connect34(
        BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c,
        BinNodePosi(T) T0, BinNodePosi(T) T1, BinNodePosi(T) T2, BinNodePosi(T) T3)
    {
        b->lc = a;
        a->parent = b;
        b->rc = c;
        c->parent = b;
        a->lc = T0;
        if (T0) T0->parent = a;
        a->rc = T1;
        if (T1) T1->parent = a;
        this -> updateHeight(a);
        c->lc = T2;
        if (T2) T2->parent = c;
        c->rc = T3;
        if (T3) T3->parent = c;
        this->updateHeight(c);
        this->updateHeight(b);
        return b;
    };
    virtual bool removeAt(T const &e)
    {
        BinNodePosi(T) &p = search(e);
        if (!p)
            return false; //return false if e is not in BST

        if (!(p->lc) && !(p->rc)) //No child
        {
            p = NULL;
            return true;
        }
        else
        {
            BinNodePosi(T) u;
            if (!(p->lc)) // only rc
            {
                u = p;
                p = p->rc;
                p->parent = _hot;
                //release(u -> data); release(u);
            }
            else if (!(p->rc)) //only lc
            {
                u = p;
                p = p->lc;
                p->parent = _hot;
                //release(u -> data); release(u);
            }
            else
            {
                u = p->succ();
                swap(u->data, p->data);
                BinNodePosi(T) &w = (u->parent == p) ? (u->parent->rc) : (u->parent->lc);
                //(u -> parent == p)? (u -> parent -> rc) : (u -> parent -> lc) = u -> rc;
                //注意条件选择式返回的是副本, 不是原变量的引用, 这里通过&成为引用
                w = u->rc;
                //FromParentTo(*u) =
                _hot = u->parent;
                if (u->rc)
                    u->rc->parent = u->parent;
            }
            release(u->data);
            release(u);
            this -> updateHeightAbove(_hot);
            return true;
        }
    };
    BinNodePosi(T) rotateAt(BinNodePosi(T) v) 
    // v has parent and grandparent
    {
        BinNodePosi(T) p = v->parent;
        BinNodePosi(T) g = p->parent;
        if (IsLChild(*v))
        {
            if (IsLChild(*p))
            {
                p->parent = g->parent;
                return connect34(v, p, g, v->lc, v->rc, p->rc, g->rc);
            }
            else
            {
                v->parent = g->parent;
                return connect34(g, v, p, g->lc, v->lc, v->rc, p->rc);
                //return v;
            }
        }
        else
        {
            if (IsRChild(*p))
            {
                p->parent = g->parent;
                return connect34(g, p, v, g->lc, p->lc, v->lc, v->rc);
                //return p;
            }
            else
            {
                v->parent = g->parent;
                return connect34(p, v, g, p->lc, v->lc, v->rc, g->rc);
                //return v;
            }
        }
    };

public:
    virtual BinNodePosi(T) & search(T const &e) //remain
    {
        _hot = NULL;
        if (!this->_root || (this->_root->data == e))
            return this->_root; //empty tree, or hit at root

        BinNodePosi(T) v = this->_root;
        bool flag_lc = false;

        while (v && v->data != e)
        {
            _hot = v;
            flag_lc = e < v->data; //for returning a reference
            v = flag_lc ? v->lc : v->rc;
        }
        if (flag_lc)
            return _hot->lc;
        else
            return _hot->rc;
    };

    bool remove(const T &e)
    {
        BinNodePosi(T) &p = this->search(e);
        if (!p) return false;

        this -> removeAt(e); this -> _size --;
        for(BinNodePosi(T) g = _hot; g; g = g -> parent)
        {
            if(!AvlBalanced(*g))
            {
                if(g == this -> _root) g = this -> _root = this->rotateAt(tallChild(tallChild(g)));
                else if(IsLChild(*g)) g = g -> parent -> lc = this->rotateAt(tallChild(tallChild(g)));
                else g = g -> parent ->rc = this->rotateAt(tallChild(tallChild(g)));
            }
            this -> updateHeight(g);
        }
        return true;
    };

    BinNodePosi(T) insert(const T &e)
    {
        BinNodePosi(T) &p = this->search(e);
        if (p)
            return p;

        p = new BinNode<T>(e, this->_hot);
        this->_size++;
        for (BinNodePosi(T) x = this -> _hot; x; x = x->parent)
        {
            if (AvlBalanced(*x))
                this->updateHeight(x);
            else
            {
                BinNodePosi(T)& pp = FromParentTo(*x);
                pp = this->rotateAt(tallChild(tallChild(x)));
                /*
                if(x == this -> _root) this -> _root = this->rotateAt(tallChild(tallChild(x)));
                else if(IsLChild(*x)) x -> parent -> lc = this->rotateAt(tallChild(tallChild(x)));
                else x -> parent ->rc = this->rotateAt(tallChild(tallChild(x)));
                */
                break;
            }
        }
        return p;
    };
    
    void zig(BinNodePosi(T) v)
    {
        BinNodePosi(T) p = v -> parent;
        BinNodePosi(T)& w = FromParentTo(*p);
        w = v; v -> parent = p -> parent; //v 和p的父亲连接
        p -> lc = v -> rc;
        if(p -> lc) p -> lc -> parent = p; //p 左孩子
        v -> rc = p; p ->parent = v; //v 右孩子
    }
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
            std::cout << "|--" << x->data << std::endl;
        else
            std::cout << "----------Structure of the binary tree:----------\n"
                      << x->data << std::endl;
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
