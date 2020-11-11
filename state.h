#pragma once

struct state{
    int adjacentes[2];
    /*
    adjacentes[0]->transitions 0
    adjacentes[1]->transitions 1
    */
};


struct state_afn{
    std::vector<int> adjacentes[3];
    /*
    adjacentes[0]->transitions 0
    adjacentes[1]->transitions 1
    adjacentes[2]->transitions e
    */
};