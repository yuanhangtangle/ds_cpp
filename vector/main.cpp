#include "./vector.hpp"
using namespace std;

int main(){
    vector<int> v;
    int a[]={7,3,7,4,9,2,6}, n = 7;
    for(int k = 0; k < 7; k++) v.insert(a[k]);
    for(int k = 1; k <= 7; k++) cout << v.quickSelect(k) << ' ';
    return 0;

}