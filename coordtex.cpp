// Classe CoordTex (coordenadas de textura)
// (C) Bruno de Oliveira Schneider - DCC - UFLA - Julho de 2017

#include <iostream>
#include "coordtex.h"

std::ostream& operator<<(std::ostream& saida, const CoordTex& c) {
    saida << '(' << c.u << ", " << c.v << ')';
    return saida;
}
                                                                                                                                                                                 // (C) Bruno de Oliveira Schneider - Não é permitido divulgar esta implementação - Publishing this code is forbidden
