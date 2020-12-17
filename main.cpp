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
    Automata c;
    cin >> a;
    cout << endl;
    cout << "Tabla de Myhill Nerode Optimizado";
    auto matriz = a.secondPart();
    for(int i = 1; i < matriz.size(); i++){
        for(int j = 0; j < i; j++){
            cout<< matriz[i][j] << " ";
        }
        cout <<'\n';

    }  
    cout << endl;

    cout << "Minización Huffman-Moore";
    b = a.huffman_moore();
    cout << b << endl;

    cout << "Minización Hopcroft";
    c = a.hopcroft();
    cout << c << endl;
    
    return 0;
}