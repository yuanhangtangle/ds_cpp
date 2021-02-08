#pragma once
#include <iostream>
#include "vector.hpp"
#include <stdlib.h>
using namespace std;
typedef int Rank;
enum Operator {ADD,SUB,MUL,DIV,POW,FAC,L_P,R_P,EOE};
struct Queen
{
    int x;
    int y;
    Queen(int xx, int yy):x(xx),y(yy) {}
    Queen(){}
    bool operator== (Queen const& q) const
    {
        return (x == q.x) ||
            (y == q.y)||
            (q.y - y == q.x - x)||
            (q.y - y == x - q.x);
    }

    bool operator!= (Queen const& q) const
    {
        return !(*this == q);
    }
};

template <typename T> 
class Stack: public vector<T>
{
    public:
        void push(T const& e){ this -> insert(this->size(), e); }
        T pop() {return this -> remove( this -> size() - 1 ); }
        T& top() {return (*this)[ this -> size() - 1 ]; }
};

void convert(int n, int b);
void visualize_queen(Stack<Queen> &s);
void placeQueen(int N);
bool test_permutation(int* A, int n);
char orderBetween(char top, char read);
double calc(double b, double a, char s);
double evaluate(char* s, char*& RPN);
int frac(int n);

//P90, 4.3.1
void convert(int n, int b)
{
    cout << "P90, 4.3.1, base conversion:" << endl;
    Stack<int> s;
    while(n > 0)
    {
        s.push(n % b);
        n = n / b;
    }
    while(!s.empty())
        cout << s.pop();
    cout << endl << endl;;
}

//P94, 4.3.3
void readNumber(char* &s, Stack<double> &opnd, char* &RPN)
{
    double d = 0;
    double base = 10;
    bool flag = false;
    while((('0'<= *s) && (*s <= '9')) || (*s == '.'))
    {
        if(('0'<= *s) && (*s <= '9'))
        {
            if(flag)
            {
                d = d + (*s - '0')*base;
                base *= 0.1;
            }

            else
            {
                d = d*base + (*s - '0');
            }
        }
        else if(*s == '.')
        {
            flag = true;
            base = 0.1;
        }
        *(RPN++) = *(s++);
    }
    opnd.push(d);
}
char orderBetween(char top, char read)
{
    const int N_OPTR = 9;
    static char pri[N_OPTR][N_OPTR] = {
        '>', '>', '<', '<', '<', '<', '<', '>', '>',
        '>', '>', '<', '<', '<', '<', '<', '>', '>',
        '>', '>', '>', '>', '<', '<', '<', '>', '>',
        '>', '>', '>', '>', '<', '<', '<', '>', '>',
        '>', '>', '>', '>', '>', '<', '<', '>', '>',
        '>', '>', '>', '>', '>', '<', ' ', '>', '>',
        '<', '<', '<', '<', '<', '<', '<', '=', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        '<', '<', '<', '<', '<', '<', '<', ' ', '='
    };
    Operator a;
    Operator b;
    switch (top)
    {
    case '+':
        a = ADD;
        break;
    
    case '-':
        a = SUB;
        break;

    case '*':
        a = MUL;
        break;

    case '/':
        a = DIV;
        break;
    
    case '^':
        a = POW;
        break;
    case '!':
        a = FAC;
        break;
    case '(':
        a = L_P;
        break;
    case ')':
        a = R_P;
        break;
    case '\0':
        a = EOE;
        break;
    }
    switch (read)
    {
    case '+':
        b = ADD;
        break;
    
    case '-':
        b = SUB;
        break;

    case '*':
        b = MUL;
        break;

    case '/':
        b = DIV;
        break;
    
    case '^':
        b = POW;
        break;
    case '!':
        b = FAC;
        break;
    case '(':
        b = L_P;
        break;
    case ')':
        b = R_P;
        break;
    case '\0':
        b = EOE;
        break;
    }
    return pri[a][b];
}
int frac(int n)
{
    if(n == 0) return 1;
    for(int s = n - 1; s >= 2; n *= s, s--);
    return n;
}
double calc(double b, double a, char s)
{
    switch (s)
    {
    case '+':
        {
            return a + b;
            break;    
        }
    case '-':
        {return a - b;}
    case '*':
        {return a*b;}
    case '/':
        {return a/b;}
    case '^':
        {
            double p = a;
            for(int i = 1; i < b ;  a *= p, i++);
            return a;
        }
    default:
        {
            cout << "unsurpported operator" << endl;
            exit(-1);
            break;
        }
    }
}
double evaluate(char* s, char*& RPN)
{
    Stack<double> opnd;
    Stack<char> optr;
    optr.push('\0');
    while(!optr.empty())
    {   
        /*
        cout << "optr:";
        for(int i = 0; i< optr.size();i++)
            cout << optr[i] << ' ';
        cout << endl;
        cout << "opnd:";
        for(int i = 0; i< opnd.size();i++)
            cout << opnd[i] << ' ';
        cout << endl;
        */
        //cout << "top of opnd" << opnd.top() << endl;
        if(isdigit(*s))
            readNumber(s, opnd, RPN);
        else
        {
            switch(orderBetween(optr.top(),*s))
            {
            case '>':
            {
                char t = optr.pop();
                //optr.push(*(s++));
                if(t == '!')
                    opnd.push(frac(opnd.pop()));
                else
                {
                    double b = opnd.pop();
                    double a = opnd.pop();
                    opnd.push(calc(b,a,t)); //cpp的参数处理是从右边到左边的
                    *(RPN++) = t;
                }
                break;
            }
            case '<':
            {
                optr.push(*s); 
                s++;
                break;
            }
            case '=':
            {
                optr.pop();
                s++;
                break;
            }
            case ' ':
            {
                cout << "illegal expression" << endl;
                //exit(-1);
                break;
            }
            }
        }
    }
    return opnd.pop();
}

//P100, 4.4.2, 8 queen problem
void placeQueen(int N)
{
    cout <<"//P100, 4.4.2, N queen problem" << endl;
    Queen q(0,0);
    Stack<Queen> s;
    int nSolu = 0;

    s.push(q);
    q.x ++;
    while((q.x > 0) || (q.y < N))
    {
        while((q.y < N) && (s.find(q) >= 0))
        {
            q.y ++;
        }
        if(q.y >= N)
        {
            q = s.pop();
            q.y ++;
        }
        else if(q.y < N)
        {
            s.push(q);
            if(s.size() >= N)
            {
                visualize_queen(s);
                ++ nSolu;
                q = s.pop();
                q.y ++;
            }
            else if(s.size() < N)
            {
                q.x ++;
                q.y = 0;
            }
        }
    }
    cout << nSolu << " solutions to " << N << " queens problem found" << endl;
}
void visualize_queen(Stack<Queen> &s)
{
    int n = s.size();
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            cout << ((s[i].y == j) ? "O ":"X "); 
        }
        cout << endl;
    }
    cout << endl;
}

//X89, 4-3-a 
bool test_permutation(int* A, int n)
{
    cout << "X89, 4-3-a " << endl;
    Stack<int> s;
    int k = 1;
    for(int i = 0; i < n; i++)
    {
        while(s.empty() || (s.top() != A[i]))
        {
            if(k <= n) s.push(k++);
            else return false;
        }
        s.pop();
    }
    return true;
}
