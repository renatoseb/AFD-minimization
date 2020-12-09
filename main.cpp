#include "automata.h"

    // Autores: 
    //     - Barrueta Aspajo, Frings Douglas
    //     - Riveros Soto, Andres
    //     - Rodríguez Llanos, Renato Sebastian

    // GitHub: 
    //     - https://github.com/renatoseb/AFD-minimization.git
int main(){
    Automata a;
    Automata b;
    // cout << "PROBLEMA 1\n";
    // std::cin >> a;
    // b = a.brzozowski();
    // std::cout << b;

    cout << "PROBLEMA 2\n";
    cin >> a;
    cout << "\n";
    auto matriz = a.equivalenceAlgorithm();
    for(int i = 1; i < matriz.size(); i++){
        for(int j = 0; j < i; j++){
            cout<< matriz[i][j] << " ";
        }
        cout <<'\n';

    }  
    cout <<'\n';
    return 0;
}
