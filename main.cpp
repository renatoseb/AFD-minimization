    #include "automata.h"

    int main(){

        Automata a;
        Automata b;
        std::cin >> a;
        //std::cout << a;
        b = a.brzozowski();
        std::cout << b;
        
        return 0;
    }
