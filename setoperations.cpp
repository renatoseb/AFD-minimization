#include<iostream>
#include<set>
#include<algorithm>

using namespace std;

set<int> intersection(set<int> A, set<int> B){
    set<int> C;
    set_intersection(A.begin(),A.end(), B.begin(),B.end(), inserter(C,C.begin()));
    return C;
}

set<int> diference(set<int> A, set<int> B){
    set<int> C;
    set_difference(A.begin(),A.end(), B.begin(),B.end(), inserter(C,C.begin()));
    return C;
}


int main(){
    set<int> A;
    set<int> B;
    set<set<int>> W;
    A.insert(3);
    A.insert(4);
    A.erase(3);
    A.insert(9);
    B.insert(8);
    B.insert(3);
    B.insert(7);
    W.insert(A);
    W.insert(B);

    set<int> C = intersection(A,B);
    for(auto it= C.begin(); it!=C.end(); ++it){
        cout << *it << " ";
    }
    if(C.empty()){
        cout << "\nvacio";
    }

    for(auto it=W.begin(); it!=W.end(); it++){
        for(auto it1=(it)->begin(); it1!=(it)->end(); it1++){
            cout << *it1 << " ";
        }
        cout << endl;
    }

}