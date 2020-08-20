// (C) Bruno de Oliveira Schneider - DCC / UFLA - A divulgacao deste codigo e' proibida.

#include "vertice.h"
#include <stdexcept>

using namespace std;

void Vertice::ConverterParaPosicaoNaTela() {
    double w = pos.W();
    if (w == 0)
        throw runtime_error("Vertice::PosicaoNaTela: Impossível transformar direção em posição.");
    pos = Ponto(pos.X()/w, pos.Y()/w, w);
}

std::ostream& operator<<(std::ostream& saida, const Vertice& v) {
    saida<<'('<<v.pos.X()<<", "<<v.pos.Y()<<", "<<v.pos.Z()<<", "<<v.pos.W()
         <<"; "<<v.tex.u<<", "<<v.tex.v<<')';
    return saida;
}
                                                                                                                                                                                 // (C) Bruno de Oliveira Schneider - Não é permitido divulgar esta implementação - Publishing this code is forbidden
