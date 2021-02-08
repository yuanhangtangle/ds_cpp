#include <iostream>
#define Posi(T) ListNode<T>*
using namespace std;

template <typename T>
class ListNode
{
    public:
        T data;
        Posi(T) pred;
        Posi(T) succ;
        ListNode() {}
        ListNode(const T& e, Posi(T) pred = NULL, Posi(T) succ = NULL): // remain
            data(e),pred(pred),succ(succ) {}

        Posi(T) insertAsPred(T const& e)
        {
            Posi(T) p = new ListNode(e, this->pred, this);
            this->pred->succ = p; this->pred = p;
            return p;
        }

        Posi(T) insertAsSucc(T const& e)
        {
            Posi(T) p = new ListNode(e, this, this->succ);
            this->succ->pred = p; this -> succ  = p;
            return p;
        }
};

template <typename T>
class List
{
    private:
        int _size;
        Posi(T) header;
        Posi(T) trailer;

    protected:
        void init()
        {
            _size = 0;
            header = new ListNode<T>;
            trailer = new ListNode<T>;
            header->succ = trailer;
            trailer->pred = header;
            header->pred = NULL;
            trailer->succ = NULL;
        };
        void copyNodes(const Posi(T) p, int n)
        {
            init();
            while(n-- > 0)
            {
                insertAsLast(p->data);
                p = p->succ;
            }
        }
        void clear() { while(_size > 0) remove(header -> succ); }
    
    public:
        List(){ init(); }
        List(const Posi(T) p, int n){ copyNodes(p, n); }
        List(const List<T>& L, int n)
        {
            Posi(T) p = L.first();
            copyNodes(p, n);
        }
        List(const List<T>& L)
        {
            copyNodes(L.first(), L.size());
        }
        ~List()
        {
            while(_size > 0) remove(header -> succ);
            delete header; delete trailer;
        }
        
        bool valid(Posi(T) p) { return p && (header != p) && (trailer != p); }
        int size() { return _size; }
        Posi(T) first() { return header->succ; }
        Posi(T) last() { return trailer->pred; }
        Posi(T) insertAsFirst(const T& e) 
        {
            ++_size;
            return header->insertAsSucc(e);
        }
        Posi(T) insertAsLast(const T& e)
        {
            ++_size;
            return trailer->insertAsPred(e);
        }
        Posi(T) insertA(Posi(T) p, const T& e)
        {
            ++_size;
            return p->insertAsSucc(e);
        }
        Posi(T) insertB(Posi(T) p, const T& e) 
        {
            ++_size;
            return p->insertAsPred(e);
        }
        Posi(T) find(const T& e, int n, Posi(T) p) const
        {
            while( n-- > 0)
                if( e == (p = p -> pred) -> data ) return p;
            return NULL;
            /*
            while((p = p -> pred) && (n-- > 0) && (p != header)) 
                if (p -> data == e) return p;
            return NULL;
            */
        }
        Posi(T) find(const T& e, Posi(T) p, int n)
        {
            while((p != trailer) && p && (n-- > 0))
            {
                if (p -> data == e) return p;
                p = p -> succ;
            }
            return NULL;
        }
        Posi(T) find(const T& e) {  return find(e, _size, last()); }
        Posi(T) search(const T& e, int n, Posi(T) p)
        {
            for(;(p = p -> pred) && (p != header) && (n > 0); n--)
                if (p -> data <= e) return p;
            return p;
        }
        T remove(Posi(T) p)
        {
            T e = p->data;
            p->pred->succ = p->succ;
            p->succ->pred = p->pred;
            delete p; --_size;
            return e;
        }
        int deduplicate()
        {
            if(_size < 2) return 0;
            int oldSize = _size;
            Posi(T) p = header -> succ;
            while(p = p->succ)
            {
                for(Posi(T) q = p->pred; q != header; q = q-> pred)
                {
                    if (q->data == p->data) 
                    {
                        remove(q);
                        break;
                    }
                }
            }
            return oldSize - _size;
        }
        int uniquify()
        {
            int oldSize = _size;
            Posi(T) p = header -> succ;
            Posi(T) q = p -> succ;
            for(; q && (q != trailer); q = p -> succ)
            {
                if (p -> data == q -> data) remove(q);
                else p = p -> succ;
            }
            return oldSize - _size;
        }
        template<typename VST> 
        void traverse(VST& vst) 
        {
            for(Posi(T) p = header->succ; p != trailer; p = p->succ)
                vst(p->data);
        }
        void insertionSort()
        {
            if( _size > 1 )
            {
                Posi(T) p = header -> succ -> succ;
                for(int n = 1; p != trailer; ++n)
                {
                    insertA(search(p -> data, n, p), p -> data);
                    p = p -> succ; remove(p -> pred);
                }
            }
        }
        bool empty(){return trailer->pred == header;}
};