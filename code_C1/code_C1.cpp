#include <iostream>
using namespace std;

long long power2BF_I_recursion(int);
double max_iter(int A[], int);
double max_recur(int A[], int);
double max_recur_half(int A[], int a, int b);
void hanoi(int n, int a, int b, int c);
int ackermann(int m, int n);
int coffee_can_game(int m, int n);
void coffee_can_game_dp(int m, int n);
void hailstone(int m);
void swap(int& a, int& b);
void shift(int A[], int n, int k);
void print_array(int A[], int);
int gcd(int a, int b);
int power_iter(int n);

int main()
{
    int m = 8;
    int n = 10;
    int A[] = {1, 2, 3, 4, 5, 6};

    cout << "P27, ex[14]: " << power2BF_I_recursion(n) << endl;
    cout << "P27, ex[15]:" << power_iter(n) << endl;
    cout << "P28, ex[16]a): " << max_iter(A,6) << endl;
    cout << "P28, ex[16]b): " << max_recur(A,6) << endl;
    cout << "P28, ex[16]b)': " << max_recur_half(A,0,6) << endl;
    cout << "P28, ex[24] Hanoi with 3 disc:" << endl; 
    hanoi(3,1,2,3);
    cout << "P29, ex[27], ackermann: " << ackermann(2,3)<< endl;
    cout << "P29, ex[28], coffee can game, recursion:" << endl;
    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++) cout << '\t' << coffee_can_game(i,j);
        cout << endl;
    }
    cout <<"P29, ex[28], coffee can game, dp: " << endl;
    coffee_can_game_dp(m, n);
    cout<< "P29, ex[29], hailstone, recursion" << endl;
    for(int i = 1; i < m; i++){
        cout << "m = "<<i<<" : ";
        hailstone(i);
        cout << endl;
    }
    cout << "P29, ex[26], shift: " << endl;
    shift(A, 6, 4);
    print_array(A, 6);
    cout << "P29, ex[25], gcd: " << gcd(45, 30) << endl;
    cin >> n;
    return 0;
}

//P27, ex[14]
long long power2BF_I_recursion(int n)
{
    if (n == 0) return 1;
    return 2*power2BF_I_recursion(--n);
}

//P27, ex[15]
int power_iter(int n)
{
    int p = 2;
    int pow = 1;
    while(n > 0)
    {
        if(1 & n) pow *= p;
        p *= p;
        n >>= 1;
    }
    return pow;
}

//P28, ex[16]
double max_iter(int A[], int n)
{
    if (n < 1) {
        cout << "empty array" << endl;
        return -1;
    }

    double m = A[0];
    for(int i = 1; i < n; i++) (A[i] > m)? m = A[i]: m = m;
    return m;
}

//P28, ex[16]
double max_recur(int A[], int n)
{
    if (n < 1){
        cout << "empty array" << endl;
        return -1;
    }
    else if(n == 1) return A[0];
    else{
        double m = max_recur(A, n-1);
        return A[n-1] > m ? A[n-1] : m;
    }
}

//P28, ex[16]
double max_recur_half(int A[], int a, int b)
{
    if (b - a < 1){
        cout << "empty array" << endl;
        return -1;
    }
    else if (b - a == 1) return A[a];
    else{
        int c = (a+b) >> 1;
        double m = max_recur_half(A, a, c);
        double n = max_recur_half(A, c, b);
        return m > n ? m : n;
    }
}

//P28, ex[24] Hanoi
void hanoi(int n, int a, int b, int c)
{
    if (n == 1) cout << "    " << a << "-->" << c << endl;
    else{
        hanoi(n - 1, a, c, b);
        cout<< "    " << a << "-->" << c << endl;
        hanoi(n-1, b, a, c);
    }
}

void swap(int& a, int& b){
    int t = a;
    a = b;
    b = t;
}

//P29, ex[25], gcd
int gcd(int a, int b)
{
    int p = 1;
    while(a % 2 == 0 && b % 2 == 0)
    {
        p *= 2;
        a /= 2;
        b /= 2;
    }
    int t = (a > b) ? a-b : b-a;
    while(t != 0)
    {
        while (t % 2 == 0) t /= 2;
        (a >= b) ? a = t : b = t;
        t = (a > b) ? a-b : b-a;
    }
    return a*p;
}

//P29, ex[26], shift
void shift(int A[], int n, int k)//bugs
{
    if (n % k == 0){
        int c, j;
        int t = n/k;
        for (int i = 0; i < k; i++){
            j = i;
            for(int p = 0; p < t-1; p++){
                int c = (j + k) % n;
                swap(A[j], A[c]);
                j = c;
            }
        }
    }

    else{
        int i = 0;
        int j;
        for(int p = 0; p < n - 1; p++){
            j = (i + k) % n;
            swap(A[i], A[j]);
            i = j;
        }
    }
    
}

//P29, ex[27], ackermann
int ackermann(int m, int n)
{
    if (m == 0) return n + 1;
    else if (m > 0 & n == 0 ) return ackermann(m-1, 1);
    else return ackermann(m-1, ackermann(m, n-1));
}

//P29, ex[28], coffe can game, recursion
//this problem has a simple mathematical solution: when n is odd.
int coffee_can_game(int m, int n) //m black, n white
{
    if (n == 0) return 0;
    else if (n == 1 & m == 0) return 1;
    else if (n < 2 & m != 0) return  coffee_can_game(m - 1, n);
    else if (n >= 2 & m == 0)return  coffee_can_game(m + 1, n - 2);
    else return coffee_can_game(m - 1, n)*coffee_can_game(m + 1, n - 2);
}

//P29, ex[28], coffe can game, dynamic programming
void coffee_can_game_dp(int m, int n)
{
    int A[m + (n-1)/2][n];
    for (int j = 0; j < n; j++)
        for (int i = 0; i < m + (n-1-j)/2; i++){
            if (j == 0) A[i][j] = 0;
            else if (j == 1 & i == 0) A[i][j] = 1;
            else if (j == 1 & i > 0) A[i][j] = A[i-1][j];
            else if (i == 0 & j > 1) A[i][j] = A[i + 1][j-2];
            else  A[i][j] = A[i-1][j]*A[i + 1][j-2];
        }
    
    //print the needed part
    for (int i  = 0; i < m; i++){
        for (int j = 0; j < n; j++)
            cout << '\t' << A[i][j];
        cout << endl;
    }   
}

//P29, ex[29], hailstone, recursion
void hailstone(int m){
    if(m == 1) cout << 1 << ' ';
    else
    {
        cout << m << ' ';
        if (m % 2 == 0) hailstone(m/2);
        else hailstone(3*m + 1);
    }
}

void print_array(int A[], int n)
{
    for(int i = 0; i < n; i++)
        cout << A[i] << ' ';
    cout << endl;
}

