    #include "automata.h"

    int main(){

        Automata a;
        Automata b;
        std::cin >> a;
        //std::cout << a;
        b = a.brzozowski();
        std::cout << b;

        auto reverse_  = a.reverse();
        a = a.powerset(reverse_);
        reverse_ = a.reverse();

        std::cout << "AFD REVERSED: \n";
        printAfn(reverse_);
        return 0;
    }