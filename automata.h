#pragma once
#include "common.h"
#include "state.h"
using namespace std;

class Automata{
    int initialState;
    vector<state> states;
    vector<bool> stateFinal;
    vector<state_afn> reverse2;
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
        Automata hopcroft();

        // Adicionales
        friend istream& operator>>(istream& ist, Automata& afd);
        friend ostream& operator<<(ostream& ost, Automata &afd);
        Automata create_automata(Automata,vector<int>, vector<vector<int>>);
        void reverse_2();
        set<int> can_reach(set<int> A, int c);
        int search_state(int destination, set<set<int>> &P);
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
    for(int i = 0; i < nStates; i++){
        if(stateFinal[i]){
            for(int j = 0; j < nStates;j++){
                if(i == j) continue;
                if(!stateFinal[j]){
                    marked[i][j] = 1;
                    marked[j][i] = 1;
                    q.push({i,j});
                }
            }
        }
    }
    set<pair<int,int>> s;
    reverse_2();
    
    while(!q.empty()){
        //cout << q.size() << ' ';
        pair<int,int> e = q.front();
        q.pop();
        for(int i = 0; i < 2; i++){
            vector<int> adjA = reverse2[e.first].adjacentes[i];
            vector<int> adjB = reverse2[e.second].adjacentes[i];
            if(adjA.size() == 0 || adjB.size() == 0) continue;
            else{
                for(int a : adjA){
                    for(int b : adjB){
                        if(a == b || marked[a][b] || marked[b][a]) continue;
                        if(s.find({a,b}) == s.end() && s.find({b,a}) == s.end()){
                            s.insert({a,b});
                            q.push({a,b});
                            marked[a][b] = 1;
                            marked[b][a] = 1;
                        }
                    }
                }
            }
        }
    }
    cout << endl;
    return marked;
}

//
// Problema 4
//
// Funcion que refibe un automata, un vector de indices de la tabla, y los equivalentes de cada indice segun la tabla
Automata Automata::create_automata(Automata MinAutomata,vector<int> back_up, vector<vector<int>> temp_equivalence_states){
    MinAutomata.initialState = initialState;
    MinAutomata.states = vector<state>(back_up.size());
    MinAutomata.stateFinal = vector<bool>(back_up.size(),false);
    //add state finals
    for(int i = 0; i < back_up.size(); i++){
            if(stateFinal[back_up[i]]==true)
                MinAutomata.stateFinal[i] = true;
    }
    //rename states equivalents
    for(int i = 0; i < back_up.size();i++){
        for(int j=0; j<=1;j++){
            for(int l=0; l<temp_equivalence_states.size();l++){
                for (int k=0; k<temp_equivalence_states[l].size();k++){
                    //reemplazamos todos los conjuntos de estados que estan en el indice por el valor del indice
                    if(temp_equivalence_states[l][k]==states[back_up[i]].adjacentes[j]){
                        states[back_up[i]].adjacentes[j]=l;
                    }
                    //verificamos si existe algun estado inicial en ese indice y seteamos el indice como estado inicial
                    if(temp_equivalence_states[l][k]==initialState)
                        MinAutomata.initialState = l;                    
                    //verificamos si existe algun estado final en ese indice y seteamos el indice como estado final
                    if(stateFinal[temp_equivalence_states[l][k]]==true)
                        MinAutomata.stateFinal[l] = true; }
            }
        }
    }
    //add transitions
    for(int i = 0; i < back_up.size();i++){
        for(int j=0; j<=1;j++){
            for(int k = 0; k < back_up.size();k++){
                //verificamos si en nuestro vector existe una transicion y seteamos la transicion con el nuevo estado k
                if(states[back_up[i]].adjacentes[j]==back_up[k]){ 
                    states[back_up[i]].adjacentes[j]=k;
                }
                MinAutomata.states[i].adjacentes[j]= states[back_up[i]].adjacentes[j];
            }
        }
    }    
    return MinAutomata;
}

Automata Automata::huffman_moore(){    
    // Paso 1
    // Armamos la tabla de estados distinguibles
    vector<vector<bool>> tableDistinct = this->equivalenceAlgorithm();
    vector<vector<int>> temp_equivalence_states;
    vector<int> temp_states;
    vector<int> back_up;
    int k=0;

    //pusheamos todos los estados del input
    for(int i=0; i<states.size();i++){
        temp_states.push_back(i);
    }

    // Paso 2
    // Particion de estados    
    for(int i = 0; i < tableDistinct.size(); i++){
        //si encontramos el indice pusehamos y lo borramos para evitar el caso en que se repita
        //si no, no hacemos nada
        if (find(temp_states.begin(), temp_states.end(), i) != temp_states.end()){
            back_up.push_back(i);
            temp_states.erase(remove(temp_states.begin(), temp_states.end(), i), temp_states.end());
            for(int j = tableDistinct.size()-1; j>0+i; j--){
                //verificamos que este en el vector, sino no hacemos nada
                if (find(temp_states.begin(), temp_states.end(), j) != temp_states.end()){
                    // agregar bloques de estados que son equivalentes entre sí
                    if(tableDistinct[j][i]==0){
                        //redefinimos el tamaño del vector donde estaran los equivalentes
                        temp_equivalence_states.resize(back_up.size());
                        //pusheamos todos los equivalentes de un indice
                        temp_equivalence_states[k].push_back(j);
                        //eliminamos del vector para evitar el caso en que se repita
                        temp_states.erase(remove(temp_states.begin(), temp_states.end(), j), temp_states.end());
                    }
                }
            }
        }
        k++;
    }

    //Paso 3
    //Construimos el AFD equivalente con menor numero de estados
    Automata MinAutomata;
    return create_automata(MinAutomata,back_up,temp_equivalence_states);
}


//
// Problema 5
//
set<int> difference(set<int> A, set<int> B){
    set<int> C;
    set_difference(A.begin(),A.end(), B.begin(),B.end(), inserter(C,C.begin()));
    return C;
}

set<int> intersection(set<int> A, set<int> B){
    set<int> C;
    set_intersection(A.begin(),A.end(), B.begin(),B.end(), inserter(C,C.begin()));
    return C;
}

int Automata::search_state(int destination, set<set<int>> &P){
    int i = 0;
    int pos;
    for(auto it=P.begin(); it!=P.end(); it++){
        for(auto it1=(it)->begin(); it1!=(it)->end(); it1++){
            if(destination == *it1) return i;
        }
        i++;
    }

}

set<int> Automata::can_reach(set<int> A, int c){
    set<int> X;
    for(auto it = A.begin(); it != A.end(); it++){
        vector<int> reachableStates = reverse2[*it].adjacentes[c];
        for(auto &i: reachableStates){
            X.insert(i);
        }
    }   

    return X;
}


Automata Automata::hopcroft(){
    
    // Creamos los conjuntos P y W
    set<set<int>> P;
    set<set<int>> W;
    
    reverse_2();
    
    set<int> notFinals;
    set<int> Finals;

    // Primera particion
    // Partimos los estados de aceptacion de los no aceptacion
    for(int i = 0; i < this->getSize(); i++){
        if(this->stateFinal[i] == true) Finals.insert(i);
        else notFinals.insert(i);
    }
    P.insert(Finals);
    P.insert(notFinals);
    W.insert(Finals);
    W.insert(notFinals);

    while(!W.empty()){
        // Escogemos y eliminamos un conjunto de W
        // y lo guardamos en A
        set<int> A = *(W.begin());
        W.erase(W.begin());

        for(int c = 0; c < 2; c++){

            // Creamos el conjunto X
            // X: Conjunto de estados que llegan a algun estado de A
            //    con transicion c.
            set<int> X = this->can_reach(A, c);
            for(auto Y = P.begin(); Y!=P.end(); Y++){
                set<int> Intersect = intersection(X,*Y);
                set<int> Difference = difference(*Y,X);
                if(!Intersect.empty() && !Difference.empty()){
                    P.erase(Y);
                    P.insert(Intersect);
                    P.insert(Difference);
                    if(W.find(*Y) != W.end()){
                        W.erase(Y);
                        W.insert(Intersect);
                        W.insert(Difference);
                    }
                    else{
                        if(Intersect.size() <= Difference.size()){
                            W.insert(Intersect);
                        }
                        else{
                            W.insert(Difference);
                        }
                        
                    }
                }
            }
        }
    }

    Automata MinAutomata;
    MinAutomata.states = vector<state>(P.size());
    MinAutomata.stateFinal = vector<bool>(P.size(),false);

    // añadimos estado inicial y estados finales
    int i = 0;
    for(auto it = P.begin(); it!=P.end(); it++){
        for(auto it1=(it)->begin(); it1!=(it)->end(); it1++){
            if(*it1 == initialState){
                MinAutomata.initialState = i;
            } 
            else if(stateFinal[*it1] == true){
                MinAutomata.stateFinal[i] = true;
            } 
        }
        i++;
    }
    // añadimos los estados finales
    i = 0;
    for(auto it = P.begin(); it!=P.end(); it++){
        auto it1 = (it)->begin();
        for(int transition: {0,1}){
            int destination = states[*it1].adjacentes[transition];
            int pos = search_state(destination, P);
            MinAutomata.states[i].adjacentes[transition] = pos;
        }
        i++;
    }
    return MinAutomata;

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

void Automata::reverse_2(){
    vector<state_afn> afn(states.size());                                             // Creamos un afn

    for(int i=0; i < states.size(); i++){                                              // Recorremos cada estado
        for(int j=0; j < 2; ++j){                                                      // Recorremos las transiciones de cada estado
            if(i == states[i].adjacentes[j]){
                afn[i].adjacentes[j].push_back(i);
            }
            else{
                int destinationState = states[i].adjacentes[j];                         // Guardamos el destino
                afn[destinationState].adjacentes[j].push_back(i);                       // Almacenamos en el destino el valor de salida
            }
        }
    }
    reverse2 = afn;
}