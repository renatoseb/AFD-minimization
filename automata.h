#pragma once
#include "common.h"
#include "state.h"
using namespace std;
class Automata{
    int initialState;
    std::vector<state> states;
    std::vector<bool> stateFinal;
    
    std::tuple<int,std::vector<state_afn>,std::vector<bool>> reverse();
    Automata powerset(std::tuple<int,std::vector<state_afn>,std::vector<bool>> afn);

    public:
        /*Automata(int numStates){
            states = std::vector<state>(numStates);
            stateFinal = std::vector<bool>(numStates,false);
        }*/
        friend std::istream& operator>>(std::istream& ist, Automata& afd);
        friend std::ostream& operator<<(std::ostream& ost, Automata &afd);
        void pushState(state st){
            states.push_back(st);
        }
        Automata brzozowski();
};

/*class afn{
    
    int initialState;
    std::vector<state_afn> states;
    std::vector<bool> stateFinal;
    
    public:
        afn(int initialState, int cantStates, int cantFinals);
        afn(Automata afd);
};*/


Automata Automata::brzozowski(){
    return powerset((powerset(this->reverse())).reverse());
}


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

/*int Automata::get_initialState(){
    return initialState;
}

std::vector<state> Automata::get_states(){
    return states;
}

std::vector<bool> Automata::get_stateFinal(){
    return stateFinal;
}

afn::afn(int initialState, int cantStates, int cantFinals){
    this->initialState = initialState;
    states = std::vector<state_afn>(cantStates);
    stateFinal = std::vector<bool>(cantFinals);
}

afn::afn(Automata afd){
    initialState = afd.get_initialState();
    
}*/

std::tuple<int,std::vector<state_afn>,std::vector<bool>> Automata::reverse(){
    int newState = states.size();
    std::cout << newState << '\n';
    std::vector<state_afn> afn(newState+1);                                               // Creamos un afn 
    std::vector<bool> finalstatesAfn(newState+1,false);                                                   // Estado finales del AFN
    //create temp to store the current initital state
    int oldNewstate = initialState;                                                     // Guardamos el valor inicial 
    //create a new initial state
    int new_initial = states.size();
    // 3 0 1 1
    // 0 0 1
    // 0 1 2
    // 1 0 1
    // 1 1 0
    // 2 0 2
    // 2 1 1
    // STEP 1
    // invertimos transiciones
    for(size_t i=0; i<states.size(); i++){                                              // Recorremos cada estado
        std::cout << "state: " << i << '\n';
        for(size_t j=0; j<2; ++j){                                                      // Recorremos las transiciones de cada estado
            std::cout << "Transicion: " <<  j <<'\n';
            if(i == states[i].adjacentes[j]){
                std::cout << '\t' << "Bucle: " << states[i].adjacentes[j] << '\n';
                afn[i].adjacentes[j].push_back(states[i].adjacentes[j]);
            }
            else{
                std::cout << '\t' << "A otro estado: " << states[i].adjacentes[j] << '\n';
                int destinationState = states[i].adjacentes[j];                         // Guardamos el destino
                afn[destinationState].adjacentes[j].push_back(i);                       // Almacenamos en el destino el valor de salida
            }
        }
    }
    std::cout << "Finish STEP 1" << '\n';
    // STEP 2
    // Agregamos un estado y lo unimos a todos los estados de aceptacion
    for(size_t i = 0; i<stateFinal.size(); i++){
        std::cout << i << '\n';
        if(stateFinal[i] == true){
            std::cout << "\t Estado de aceptacion: "<<i << '\n';
            afn[newState].adjacentes[2].push_back(i);
        }
    }
    std::cout << "Finish STEP 2" <<'\n';
    // STEP 3
    // Convertimos el antiguo estado inicial en estado final 
    finalstatesAfn[oldNewstate] = true;
    
    std::cout << afn.size() << '\n';
    return {newState, afn, finalstatesAfn};
}

void printAfn(std::tuple<int,std::vector<state_afn>,std::vector<bool>> &afn){
    int numfinalStates = 0;
    std::cout << std::get<1>(afn).size() << " " << std::get<0>(afn) << " ";
    for(int i = 0; i< std::get<2>(afn).size(); ++i){
        if(std::get<2>(afn)[i] == true){
            numfinalStates++;
        }
    }
    std::cout << numfinalStates << " "; 
    for(int i = 0; i< std::get<2>(afn).size(); ++i){
        if(std::get<2>(afn)[i] == true){
            std::cout << i << " ";
        }
    }

    std::cout << "\n";
    for(int i = 0; i < std::get<1>(afn).size(); i++){
        for(int j = 0; j<3; ++j){
            std::cout << i << " " << j << " ";
            for(auto k: std::get<1>(afn)[i].adjacentes[j]){
                std::cout << k << " ";
            }
            std::cout << "\n";
        }
    }
}


std::set<int> clausura(std::set<int> states, std::vector<state_afn>& afn){
    std::queue<int> q;
    std::set<int> clausuraResult;
    for(auto it=states.begin(); it!=states.end(); ++it){
        q.push(*it);
        clausuraResult.insert(*it);
        while(!q.empty()){
            int currState = q.front();
            q.pop();
            for(size_t j=0; j<afn[currState].adjacentes[2].size(); j++){
                auto temp = (afn[currState].adjacentes[2])[j];
                if(clausuraResult.find(temp) == clausuraResult.end()){
                    clausuraResult.insert(temp);
                    q.push(temp);
                }
            }
        }
    }
    return clausuraResult;
}

std::set<int> delta(std::set<int> q, int c, std::vector<state_afn> &afn){
    std::set<int> result_delta;
    auto it = q.begin();
    while (it != q.end()){                                                      // Recorremos el set
        for(size_t j=0; j< afn[*it].adjacentes[c].size(); j++){                 // Recoremos para ver a que estados llega desde *it con transiciones c
            result_delta.insert((afn[*it].adjacentes[c])[j]);
        }
        it++;
    }
    return result_delta;
}

Automata Automata::powerset(std::tuple<int,std::vector<state_afn>,std::vector<bool>> afn){
    std::set<int> initialState;
    initialState.insert(std::get<0>(afn));                                                          // Estado inicial
    std::set<int> q_0 = clausura(initialState,std::get<1>(afn));
    
    std::vector<std::set<int>> Q;                                                                   // Vector de estados
    Q.push_back(q_0);
    
    std::map<std::set<int>,std::tuple<std::set<int>,std::set<int>>> transitions;                    // Mapa de transiciones
    
    std::queue<std::set<int>> workList;                                                             // Vector de control de estados
    workList.push(q_0);

    while(!workList.empty()){
        std::set<int> q = workList.front();
        workList.pop();
        
        std::tuple<std::set<int>,std::set<int>> trans;
        for(int i = 0; i<2; i++){
            std::set<int> t = clausura(delta(q,i,std::get<1>(afn)),std::get<1>(afn));
            
            if(i == 0) std::get<0>(trans) = t;
            else std::get<1>(trans) = t;
            
            auto it = std::find(Q.begin(),Q.end(),t);
            if(it == Q.end()){
                Q.push_back(t);
                workList.push(t);
            }
        } 
        transitions.insert(std::pair<std::set<int>,std::tuple<std::set<int>,std::set<int>>> (q,trans));
    }

    Automata afdResult;
    afdResult.initialState = 0;
    afdResult.states = std::vector<state>(Q.size());
    afdResult.stateFinal = std::vector<bool>(Q.size(),false);
    for(size_t i = 0; i<Q.size(); ++i){
        std::tuple<std::set<int>,std::set<int>> trans = transitions[Q[i]];
        auto transZero = std::find(Q.begin(),Q.end(),  std::get<0>(trans));
        auto transOne = std::find(Q.begin(),Q.end(),  std::get<1>(trans));
        afdResult.states[i].adjacentes[0] = transZero - Q.begin();
        afdResult.states[i].adjacentes[1] = transOne - Q.begin();

        // Pasamos los valores finales
        for(size_t j = 0; j<std::get<2>(afn).size(); ++j){
            if(std::get<2>(afn)[j] == true){
                auto itFinal = std::find(Q[i].begin(),Q[i].end(), j);
                if(itFinal != Q[i].end()){
                    afdResult.stateFinal[i] = true;
                }
            }
        }
    }

    return afdResult;
}





// 4 3 1 0
// 0 0
// 0 1 1 
// 0 2
// 1 0 0 1
// 1 1 2
// 1 2
// 2 0 2
// 2 1 0
// 2 2
// 3 0
// 3 1
// 3 2 1


// 6 5 1 0
// 5 2 0 2 4
// 5 0
// 5 1
// 4 0 4
// 4 1 4 2
// 4 2
// 2 0 2 3 1
// 2 1
// 2 2
// 3 0
// 3 1 3 1
// 3 2
// 1 0
// 1 1 0
// 1 2
// 0 0 0
// 0 1
// 0 2