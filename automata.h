#pragma once
#include "common.h"
#include "state.h"

class Automata{
    int initialState;
    std::vector<state> states;
    std::vector<bool> stateFinal;
    
public:
    friend std::istream& operator>>(std::istream& ist, Automata& afd);
    friend std::ostream& operator<<(std::ostream& ost, Automata &afd);
    void reverse(Automata &afd);
};


std::istream& operator>>(std::istream& ist, Automata& afd){
    int nStates, firstState, nFinalStates;
    ist >> nStates >> firstState >> nFinalStates;
    afd.initialState = firstState;
    afd.states = std::vector<state>(nStates);
    afd.stateFinal = std::vector<bool>(nStates,false);
    for(int i = 0; i < nFinalStates; i++){
        int sf;
        ist >> sf;
        afd.stateFinal[sf] = true;
    }
    for(int i = 0; i < nStates*2;i++){
        int p, a, q;
        ist >> p >> a >> q;
        afd.states[p].adjacentes[a] = q;
    }

    return ist;
}

std::ostream& operator<<(std::ostream& ost, Automata &afd){
    ost <<'\n';
    int numStateFinal = 0;
    std::vector<int> sf;
    for(int i = 0; i < afd.stateFinal.size(); i++){
        if(afd.stateFinal[i]){
            numStateFinal++;
            sf.push_back(i);
        }  
    }
    ost << afd.states.size() << " " << afd.initialState << " " << numStateFinal << " ";
    for(int i = 0; i < sf.size(); i++){
        ost << sf[i] << " ";
    }
    ost << '\n';
    for(int i = 0; i < afd.states.size()*2;i++){
        ost << i/2 << " " << i%2 << " " << afd.states[i/2].adjacentes[i%2] << '\n';
    }
    return ost;
}

void Automata::reverse(Automata &afd){
    //create temp to store the current initital state
    int temp_current_initial = afd.initialState;
    //create a new initial state
    int new_initial = afd.states.size();
    //convert the old initial state in final state

    //add the new initial state in the automata
    afd.initialState = new_initial;
    //invert the edges
    for(int i = 0; i < afd.states.size();i++){
            afd.states[q].adjacentes[a] = p;
    }
    //convert final state to normal states
    for(int i = 0; i < nFinalStates; i++){
        int sf;
        ist >> sf;
        afd.stateFinal[sf] = false;
    }
    //add initial transition e to the final states

}

