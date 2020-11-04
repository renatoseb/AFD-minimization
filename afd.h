#pragma once
#include "common.h"

class afd{
public:
    int inicialState;
    int numAutomatas;
    std::unordered_map<int,state> states;
public:
    afd brzozowskiAlgorithm();
    friend std::istream& operator>>(std::istream&, afd&);
};

std::istream& operator>>(std::istream& ist, afd& afd_){
    int nAutomatas, inicialState, nFinalStates;
    ist >> nAutomatas >> inicialState >> nFinalStates;
    afd_.numAutomatas = nAutomatas;
    afd_.inicialState = inicialState;
    std::vector<int> finalStates(nAutomatas);
    for(int i = 0; i < nFinalStates; i++){
        int finalState;
        ist >> finalState;
        finalStates.push_back(finalState);
    }
    for(int i = 0; i < nAutomatas*2; i++){
        int p, a, q;
        ist >> p >> a >> q;
        if(afd_.states.find(p) == afd_.states.end()){
            afd_.states[p] = state{a,q};
        }else{
            afd_.states[p].setConexo(a, q);
        }
    }
    for(int i = 0; i < finalStates.size();i++){
        afd_.states[finalStates[i]].end = true;
    }
    return ist;
}