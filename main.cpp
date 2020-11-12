    #include "automata.h"

    int main(){

        Automata a;
        Automata b;
        std::cin >> a;
        b = a.brzozowski();
        std::cout << b;
        return 0;
    }
