#include <iostream>
#include  <map>
using namespace std;

int main()
{  
    map<int, int> mp;

    cout << ++mp[0] << endl;
    cout << mp[0] << endl;
    cout << ++mp[0] << endl;

    int n;
    cin >> n;
    return n*(n+1)/2;
}  

