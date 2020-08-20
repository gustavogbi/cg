// (C) Bruno de Oliveira Schneider - DCC / UFLA - A divulgacao deste codigo e' proibida.

#include "ponto.h"
#include <cmath>

std::ostream& operator<<(std::ostream& saida, const Ponto& p) {
    saida << '(' << p.mX << ", " << p.mY << ", " << p.mZ << ", " << p.mW << ')';
    return saida;
}

Ponto Ponto::operator-() const {
    return Ponto(-mX, -mY, -mZ, -mW);
}

Ponto Ponto::operator-(const Ponto& p) const {
    return Ponto(mX-p.mX, mY-p.mY, mZ-p.mZ, mW-p.mW);
}

double Ponto::Norma() const {
    return sqrt(mX*mX + mY*mY + mZ*mZ);
}

void Ponto::Normalizar() {
    if (mW != 0) {
        // ponto
        mX /= mW;
        mY /= mW;
        mZ /= mW;
        mW = 1;
    }
    else {
        // vetor
        double norma = Norma();
        mX /= norma;
        mY /= norma;
        mZ /= norma;
    }
}

double Ponto::ProdutoEscalar(const Ponto& p) const {
    return mX*p.mX + mY*p.mY + mZ*p.mZ;
}

Ponto Ponto::ProdutoVetorial(const Ponto& p) const {
    return Ponto(mY*p.mZ-mZ*p.mY, mZ*p.mX-mX*p.mZ, mX*p.mY-mY*p.mX, 0);
}

double Ponto::AnguloAte(const Ponto& p) const {
    return acos(ProdutoEscalar(p) / (Norma() * p.Norma()));
}
                                                                                                                                                                                 // (C) Bruno de Oliveira Schneider - Não é permitido divulgar esta implementação - Publishing this code is forbidden
