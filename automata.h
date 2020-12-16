#pragma once
#include "common.h"
#include "state.h"
using namespace std;

class Automata{
    int initialState;
    vector<state> states;
    vector<bool> stateFinal;

    tuple<int,vector<state_afn>,vector<bool>> reverse();
    Automata powerset(tuple<int,vector<state_afn>,vector<bool>> afn);

    public:
        
        // Problema 1
        Automata brzozowski();

        // Problema 2
        vector<vector<bool>> equivalenceAlgorithm();

        // Problema 3
        vector<vector<bool>> secondPart();

        // Problema 4
        Automata huffman_moore();
        
        // Problema 5
        Automata Automata::hopcroft();

        // Adicionales
        friend istream& operator>>(istream& ist, Automata& afd);
        friend ostream& operator<<(ostream& ost, Automata &afd);
        int getSize(){
            return this->states.size();
        }
};


//
// Problema 1
//
Automata Automata::brzozowski(){
    return powerset((powerset(this->reverse())).reverse());
}

tuple<int,vector<state_afn>,vector<bool>> Automata::reverse(){
    //create a new initial state
    int newState = states.size();
    vector<state_afn> afn(newState+1);                                             // Creamos un afn 
    vector<bool> finalstatesAfn(newState+1,false);                                 // Estado finales del AFN
    //create temp to store the current initital state
    int oldNewstate = initialState;                                                     // Guardamos el valor inicial 

    // STEP 1 
    // invertimos transiciones
    for(size_t i=0; i<states.size(); i++){                                              // Recorremos cada estado
        for(size_t j=0; j<2; ++j){                                                      // Recorremos las transiciones de cada estado
            if(i == states[i].adjacentes[j]){
                afn[i].adjacentes[j].push_back(states[i].adjacentes[j]);
            }
            else{
                int destinationState = states[i].adjacentes[j];                         // Guardamos el destino
                afn[destinationState].adjacentes[j].push_back(i);                       // Almacenamos en el destino el valor de salida
            }
        }
    }

    // STEP 2
    // Agregamos un estado y lo unimos a todos los estados de aceptacion
    for(size_t i = 0; i<stateFinal.size(); i++){
        if(stateFinal[i] == true){
            afn[newState].adjacentes[2].push_back(i);
        }
    }
    
    // STEP 3
    // Convertimos el antiguo estado inicial en estado final 
    finalstatesAfn[oldNewstate] = true;
    return {newState, afn, finalstatesAfn};
}




set<int> clausura(set<int> states, vector<state_afn>& afn){
    queue<int> q;
    set<int> clausuraResult;
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

set<int> delta(set<int> q, int c, vector<state_afn> &afn){
    set<int> result_delta;
    auto it = q.begin();
    while (it != q.end()){                                                                          // Recorremos el set
        for(size_t j=0; j< afn[*it].adjacentes[c].size(); j++){                                     // Recorremos para ver a que estados llega desde *it con transiciones c
            result_delta.insert((afn[*it].adjacentes[c])[j]);
        }
        it++;
    }
    return result_delta;
}

Automata Automata::powerset(tuple<int,vector<state_afn>,vector<bool>> afn){
    set<int> initialState;
    initialState.insert(get<0>(afn));                                                          // Estado inicial
    set<int> q_0 = clausura(initialState,get<1>(afn));
    vector<set<int>> Q;                                                                   // Vector de estados
    Q.push_back(q_0);
    
    map<set<int>,tuple<set<int>,set<int>>> transitions;                    // Mapa de transiciones
    
    queue<set<int>> workList;                                                             // Vector de control de estados
    workList.push(q_0);

    while(!workList.empty()){
        set<int> q = workList.front();
        workList.pop();
        bool flagNull = false;
        tuple< set<int>,set<int>> trans;
        for(int i = 0; i<2; i++){
            set<int> t = clausura(delta(q,i,get<1>(afn)),get<1>(afn));
            
            if(t.size() == 0){
                flagNull = true;
                continue;
            }
            if(i == 0) get<0>(trans) = t;
            else get<1>(trans) = t;
            
            auto it = find(Q.begin(),Q.end(),t);
            if(it == Q.end()){
                Q.push_back(t);
                workList.push(t);
            }
        } 
        if(!flagNull) transitions[q] = trans;
    }

    // Parseamos el afn 
    Automata afdResult;
    afdResult.initialState = 0;
    afdResult.states = vector<state>(Q.size());
    afdResult.stateFinal = vector<bool>(Q.size(),false);
    for(size_t i = 0; i<Q.size(); ++i){
        tuple<set<int>,set<int>> trans = transitions[Q[i]];
        auto transZero = find(Q.begin(),Q.end(),  get<0>(trans));
        auto transOne = find(Q.begin(),Q.end(),  get<1>(trans));
        afdResult.states[i].adjacentes[0] = transZero - Q.begin();
        afdResult.states[i].adjacentes[1] = transOne - Q.begin();

        // Pasamos los valores finales
        for(size_t j = 0; j<get<2>(afn).size(); ++j){
            if(get<2>(afn)[j] == true){
                auto itFinal = find(Q[i].begin(),Q[i].end(), j);
                if(itFinal != Q[i].end()){
                    afdResult.stateFinal[i] = true;
                }
            }
        }
    }

    return afdResult;
}

// 
// Problema 2
//
vector<vector<bool>> Automata::equivalenceAlgorithm(){
    int nStates = states.size();
    vector<vector<bool>> marked(nStates,vector<bool>(nStates));
    for(int i = 0; i < nStates;i++){
        for(int j = 0; j < nStates;j++){
            marked[i][j] = false;
        }
    }
    // mark pairs Qi ∈ F and Qj ∉ F
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
    
    int changes = 1;
    while(changes != 0){
        changes = 0;
        for(int i = 0; i < nStates; i++){
            for(int j = 0; j < nStates; j++){
                if(i == j) continue;
                for(int k = 0; k < 2; k++){
                    if(marked[states[i].adjacentes[k]][states[j].adjacentes[k]] && (!marked[i][j] && !marked[j][i])){
                        marked[i][j] = 1;
                        marked[j][i] = 1;
                        changes++;
                    }
                }
            }
        }
    }
    return marked;
}


//
// Problema 3
//
vector<vector<bool>> Automata::secondPart(){
    int nStates = (int)states.size();
    vector<vector<bool>> marked(nStates,vector<bool>(nStates));
    queue<pair<int,int>> q;
    for(int i = 0; i < nStates;i++){
        for(int j = 0; j < nStates;j++){
            marked[i][j] = false;
        }
    }
    // mark pairs Qi ∈ F and Qj ∉ F
    for(int i = 0; i < nStates; i++){
        if(stateFinal[i]){
            for(int j = 0; j < nStates;j++){
                if(i == j) continue;
                if(!stateFinal[j]){
                    marked[i][j] = 1;
                    marked[j][i] = 1;
                }
            }
        }else{
            for(int j = 0; j < i;j++){
                if(i == j) continue;
                if(!stateFinal[j]){
                    q.push({i,j});
                }
            }
        }
    }
    while(!q.empty()){
        vector<pair<int,int>> vec;
        pair<int,int> fState = q.front();
        q.pop();
        vec.push_back(fState);
        bool flag = false;
        while(!marked[fState.first][fState.second]){
            int changes = 0;
            for(int i = 0; i < 2; i++){
                if(states[fState.first].adjacentes[i] == states[fState.second].adjacentes[i]) continue;
                if(!marked[states[fState.first].adjacentes[i]][states[fState.second].adjacentes[i]] && !marked[fState.first][fState.second]){
                    fState = make_pair(states[fState.first].adjacentes[i], states[fState.second].adjacentes[i]);
                    vec.push_back(fState);
                    changes++;
                }else{
                    flag = true;
                    vec.push_back(fState);
                    changes = 0;
                    break;
                }
            }
            if(!changes){
                break;
            }
        }
        if(flag){
            for(int i = 0; i < vec.size();i++){
                marked[vec[i].first][vec[i].second] = 1;
                marked[vec[i].second][vec[i].first] = 1;
            }
        }
    }
    return marked;
}

// Problema 3
vector<std::vector<bool>> Automata::secondPart(){
    int nStates = (int)states.size();
    std::vector<std::vector<bool>> marked(nStates,std::vector<bool>(nStates));
    queue<pair<int,int>> q;
    for(int i = 0; i < nStates;i++){
        for(int j = 0; j < nStates;j++){
            marked[i][j] = false;
        }
    }
    // mark pairs Qi ∈ F and Qj ∉ F
    for(int i = 0; i < nStates; i++){
        if(stateFinal[i]){
            for(int j = 0; j < nStates;j++){
                if(i == j) continue;
                if(!stateFinal[j]){
                    marked[i][j] = 1;
                    marked[j][i] = 1;
                }
            }
        }else{
            for(int j = 0; j < i;j++){
                if(i == j) continue;
                if(!stateFinal[j]){
                    q.push({i,j});
                }
            }
        }
    }
    while(!q.empty()){
        vector<pair<int,int>> vec;
        pair<int,int> fState = q.front();
        q.pop();
        vec.push_back(fState);
        bool flag = false;
        while(!marked[fState.first][fState.second]){
            int changes = 0;
            for(int i = 0; i < 2; i++){
                if(states[fState.first].adjacentes[i] == states[fState.second].adjacentes[i]) continue;
                if(!marked[states[fState.first].adjacentes[i]][states[fState.second].adjacentes[i]] && !marked[fState.first][fState.second]){
                    fState = make_pair(states[fState.first].adjacentes[i], states[fState.second].adjacentes[i]);
                    vec.push_back(fState);
                    changes++;
                }else{
                    flag = true;
                    vec.push_back(fState);
                    changes = 0;
                    break;
                }
            }
            if(!changes){
                break;
            }
        }
        if(flag){
            for(int i = 0; i < vec.size();i++){
                marked[vec[i].first][vec[i].second] = 1;
                marked[vec[i].second][vec[i].first] = 1;
            }
        }
    }
    return marked;
}

// Problema 4
Automata Automata::huffman_moore(){
    Automata automata;
    
    // Paso 1
    // Armamos la tabla de estados distinguibles
    vector<vector<bool>> tableDistinct = this->equivalenceAlgorithm();

    // Paso 2
    // Particion de estados
    vector<set<int>> states;
    for(int i = 1; i < tableDistinct.size(); i++){
        for(int j = 0; j < i; j++){
            
        }
    }
}


//
// Problema 5
//
Automata Automata::hopcroft(){
    
    // Creamos los conjuntos P y W
    set<set<state>> P;
    set<set<state>> W;

    set<state> notFinals;
    set<state> Finals;
    for(int i = 0; i < this->getSize(); i++){
        if(this->stateFinal[i] == true) Finals.insert(this->states[i]);
        else notFinals.insert(this->states[i]);
    }
    P.insert(Finals);
    P.insert(notFinals);
    W.insert(Finals);
    W.insert(notFinals);

    while

}

//
// ADICIONALES
//
void printAfn(tuple<int,vector<state_afn>,vector<bool>> &afn){
    int numfinalStates = 0;
    cout << get<1>(afn).size() << " " << get<0>(afn) << " ";
    for(int i = 0; i< get<2>(afn).size(); ++i){
        if(get<2>(afn)[i] == true){
            numfinalStates++;
        }
    }
    cout << numfinalStates << " "; 
    for(int i = 0; i< get<2>(afn).size(); ++i){
        if(get<2>(afn)[i] == true){
            cout << i << " ";
        }
    }

    cout << "\n";
    for(int i = 0; i < get<1>(afn).size(); i++){
        for(int j = 0; j<3; ++j){
            cout << i << " " << j << " ";
            for(auto k: get<1>(afn)[i].adjacentes[j]){
                cout << k << " ";
            }
            cout << "\n";
        }
    }
}



istream& operator>>(istream& ist, Automata& afd){
    int nStates, firstState, nFinalStates;
    ist >> nStates >> firstState >> nFinalStates;
    afd.initialState = firstState;
    afd.states = vector<state>(nStates);
    afd.stateFinal = vector<bool>(nStates,false);
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

ostream& operator<<(ostream& ost, Automata &afd){
    ost <<'\n';
    int numStateFinal = 0;
    vector<int> sf;
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


