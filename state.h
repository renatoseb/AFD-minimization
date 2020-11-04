#pragma once

struct state{
    bool end;
    int conexo[2];
    state(int a, int q, bool end = false){
        this->end = end;
        conexo[a] = q;
    }
    void setConexo(int a, int q){
        conexo[a] = q;
    }
};