// Classe Ponto (pontos ou vetores usando coordenadas homogeneas)
// (C) Bruno de Oliveira Schneider - DCC - UFLA - Julho de 2017

#ifndef PONTO_H
#define PONTO_H

#include <iostream>

class Ponto {
    friend std::ostream& operator<<(std::ostream& saida, const Ponto& p);
    public:
        Ponto()
            : mX(0), mY(0), mZ(0), mW(1) {
        }
        Ponto(double x, double y, double z, double w=1)
            : mX(x), mY(y), mZ(z), mW(w) {
        }
        inline double X() const { return mX; }
        inline double Y() const { return mY; }
        inline double Z() const { return mZ; }
        inline double W() const { return mW; }
        Ponto operator-() const;
        Ponto operator-(const Ponto& p) const;
        double Norma() const;
        void Normalizar();
        double ProdutoEscalar(const Ponto& p) const;
        Ponto ProdutoVetorial(const Ponto& p) const;
        double AnguloAte(const Ponto& p) const;
    private:
        double mX;
        double mY;
        double mZ;
        double mW;
};

#endif
                                                                                                                                                                                 // (C) Bruno de Oliveira Schneider - Não é permitido divulgar esta implementação - Publishing this code is forbidden
