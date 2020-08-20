// Classe Vertice pra programas de mapeamento de textura
// (C) Bruno de Oliveira Schneider - DCC - UFLA - Marco de 2017

#ifndef VERTICE_H
#define VERTICE_H

#include "coordtex.h"
#include "ponto.h"
#include <iostream>

class Vertice {
    public:
        void ConverterParaPosicaoNaTela();
        Ponto pos;
        CoordTex tex;
};

#endif
                                                                                                                                                                                 // (C) Bruno de Oliveira Schneider - Não é permitido divulgar esta implementação - Publishing this code is forbidden
