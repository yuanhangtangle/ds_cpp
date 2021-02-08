#include <iostream>
using namespace std;

class Bitmap
{
    private:
        char* M;
        int N;
    public:
        Bitmap(int n):N(n) { M = new char[n]; }
        ~Bitmap(){ delete[] M; }
        bool test(int n)
        {
            return M[n >> 3] & (0x80 >> (n & 0x07));
        }
        void set(int n)
        {
            M[n >> 3] |= 0x80 >> (n & 0x07);
        }
        void clear(int n)
        {
            M[n >> 3] &= ~(0x80 >> (n & 0x07));
        }
        void memset(int lo, int hi)
        {
            static unsigned int A[] = {0,128,192,224,240,248,252,254};
            int a = lo >> 3;
            int b = hi >> 3;
            if ( a == b)
                M[a] &= (A[lo & 0x07] | ~A[hi & 0x07]);
            else
            {
                M[a] &= A[lo & 0x07];
                M[b] &= ~A[hi & 0x07];
                for(int i = a + 1; i < b; i++)
                    M[i] &= 0;
            }
            
        }
        char* toString(int n)
        {
            char *s = new char[n + 1];
            s[n] = '\0';
            for(int i = 0; i < n; i++)
                s[i] = test(i)? '1':'0';
            return s;  //allocation of s is determined by the user
        }
        int size(){return N;}
};


int main()
{
    Bitmap bm(28); int N = 28;
    for(int i = 0; i < N; i++)
    {
        if(i % 2) bm.set(i);
        else bm.clear(i);
    }
    char *s = bm.toString(N);
    for(int i = 0; s[i]; i++)
        cout << s[i];
    cin >> N;
    return 0;
}