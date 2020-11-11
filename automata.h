#pragma once
#include "common.h"
#include "state.h"

class Automata{
    int initialState;
    std::vector<state> states;
    std::vector<bool> stateFinal;
    
    std::tuple<int,std::vector<state_afn>,std::vector<bool>> reverse();
    Automata powerset(std::tuple<int,std::vector<state_afn>,std::vector<bool>> afn);

    public:
        friend std::istream& operator>>(std::istream& ist, Automata& afd);
        friend std::ostream& operator<<(std::ostream& ost, Automata &afd);
        void pushState(state st){
            states.push_back(st);
        }
        //Automata brzozowski();
        std::vector<std::vector<bool>> equivalenceAlgorithm();
};

std::vector<std::vector<bool>> Automata::equivalenceAlgorithm(){
    int nStates = states.size();
    std::vector<std::vector<bool>> marked(nStates,std::vector<bool>(nStates));
    for(int i = 0; i < nStates;i++){
        for(int j = 0; j < nStates;j++){
            marked[i][j] = false;
        }
    }
    for(int i = 0; i < nStates; i++){
        if(stateFinal[i]){
            for(int j = 0; j < nStates;j++){
                if(i == j) continue;
                if(!stateFinal[j]){
                    marked[i][j] = 1;
                    marked[j][i] = 1;
                }
            }
        }
    }
    for(int i = 0; i < nStates; i++){
        for(int j = 0; j < nStates; j++){
            if(i == j) continue;
            for(int k = 0; k < 2; k++){
                if(marked[states[i].adjacentes[k]][states[j].adjacentes[k]] || marked[states[j].adjacentes[k]][states[i].adjacentes[k]])
                    marked[i][j] = 1;
            }
        }
    }
    // unmarked 
    for(int i = 0; i < nStates; i++){
        for(int j = 0; j < nStates; j++){
            if(i == j) continue;
            if(!marked[i][j]){
                for(int u = 0; u < 2; u++){
                    if(marked[states[i].adjacentes[u]][states[j].adjacentes[u]] ||marked[states[j].adjacentes[u]][states[i].adjacentes[u]])
                        marked[i][j] = 1;
                }
            }
        }
    }

    std::cout <<'\n';
    for (int i = 0; i < nStates; i++) {
        for (int j = 0; j < nStates; j++) {
            std::cout << marked[i][j] << " ";
        }
        std::cout << '\n';
    }
    std::cout <<'\n';
    
    return marked;
}


/*class afn{
    
    int initialState;
    std::vector<state_afn> states;
    std::vector<bool> stateFinal;
    
    public:
        afn(int initialState, int cantStates, int cantFinals);
        afn(Automata afd);
};*/

/*
Automata Automata::brzozowski(){
    // auto reverse = this->reverse();
    // Automata tempAutomata = powerset(reverse);   
    // reverse = tempAutomata.reverse(); 
    // tempAutomata = powerset(reverse);

    return powerset((powerset(this->reverse())).reverse());
}
*/

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
/*
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

std::vector<state_afn> clausura(std::vector<int> states, std::vector<state_afn> &afn){
    std::vector<state_afn> c; 
    for(auto i:states){
        recClausura(afn[i].adjacentes[2],afn,c);
    }
}

state_afn recClausura(std::vector<state_afn> states,std::vector<state_afn> &afn, std::vector<state_afn> c){
    
}

std::vector<int> delta(std::vector<state_afn> q, int c, std::vector<state_afn> &afn){
    std::vector<int> result_delta;
    for(size_t i=0; i<afn.size(); i++){                                              // Recorremos cada estado
            std::cout << "state: " << i << '\n';
        for (size_t j=0; j<q.size(); j++){ 
                if(i == q[j])
                    result_delta.push_back(afn[i].adjacentes[c]);
            }
    }

        return result_delta;
}

Automata Automata::powerset(std::tuple<int,std::vector<state_afn>,std::vector<bool>> afn){
    Automata automata; 

    std::vector<state_afn> initialState;
    initialState.push_back(std::get<1>(afn)[std::get<0>(afn)]);
    std::vector<state_afn> q_0 = clausura(initialState,std::get<1>(afn));
    std::vector<std::vector<std::vector<state_afn>>> T;

    std::vector<std::vector<state_afn>> Q; 
    Q.push_back(q_0);

    std::queue<std::vector<state_afn>> workList;
    workList.push(q_0);

    while(!workList.empty()){
        std::vector<state_afn> q = workList.front();
        workList.pop();
        std::vector<std::vector<state_afn>> transitions(2);
        for(size_t i = 0; i<2; i++){
            std::vector<state_afn> t = clausura(delta(q,i));
            transitions[i] = t;
            bool flag = false;
            for(auto j: Q){
                if(j == t){
                    flag = true;
                    break;
                }
            }
            if(flag == false){
                Q.push_back(t);
                workList.push(t);
            }
            T.push_back(transitions);
        } 
    }

}*/

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