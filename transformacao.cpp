// (C) Bruno de Oliveira Schneider - DCC / UFLA - A divulgacao deste codigo e' proibida.

#include "transformacao.h"
#include <cstring> // para memcpy
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

ostream& operator<<(ostream& saida, const Transformacao& t) {
    saida.setf(ios::showpoint|ios::fixed);
    saida.precision(5);
    for (int i=0; i<4; ++i) {
        for (int j=0; j<4; ++j)
            saida << setw(10) << t.matriz[i][j] << ' ';
        saida << "\n";
    }
    saida.unsetf(ios::showpoint|ios::fixed);
    return saida;
}

Transformacao::Transformacao() {
    CarregarIdentidade();
}

Transformacao::Transformacao(const Transformacao& trans) {
    for (int i=0; i<4; ++i)
        for (int j=0; j<4; ++j)
            matriz[i][j] = trans.matriz[i][j];
}

void Transformacao::CarregarIdentidade() {
    static int tamanho = sizeof(double) * 16;
    static double identidade[] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
    memcpy(matriz, identidade, tamanho);
}

void Transformacao::CarregarTranslacao(double x, double y, double z) {
    CarregarIdentidade();
    matriz[0][3] = x;
    matriz[1][3] = y;
    matriz[2][3] = z;
}

void Transformacao::CarregarTranslacao(const Ponto& vetorTranslacao) {
    CarregarIdentidade();
    matriz[0][3] = vetorTranslacao.X();
    matriz[1][3] = vetorTranslacao.Y();
    matriz[2][3] = vetorTranslacao.Z();
}

void Transformacao::CarregarRotacaoX(double radianos) {
    CarregarIdentidade();
    matriz[1][1] =  cos(radianos);
    matriz[1][2] = -sin(radianos);
    matriz[2][1] =  sin(radianos);
    matriz[2][2] =  cos(radianos);
}

void Transformacao::CarregarRotacaoY(double radianos) {
    CarregarIdentidade();
    matriz[0][0] =  cos(radianos);
    matriz[0][2] =  sin(radianos);
    matriz[2][0] = -sin(radianos);
    matriz[2][2] =  cos(radianos);
}

void Transformacao::CarregarRotacaoZ(double radianos) {
    CarregarIdentidade();
    matriz[0][0] =  cos(radianos);
    matriz[0][1] = -sin(radianos);
    matriz[1][0] =  sin(radianos);
    matriz[1][1] =  cos(radianos);
}

void Transformacao::CarregarEscala(double escX, double escY, double escZ) {
    CarregarIdentidade();
    matriz[0][0] = escX;
    matriz[1][1] = escY;
    matriz[2][2] = escZ;
}

void Transformacao::CarregarCisalhamento(double cisX, double cisY) {
    CarregarIdentidade();
    matriz[0][2] = cisX;
    matriz[1][2] = cisY;
}

Ponto Transformacao::operator*(const Ponto& ponto) {
    double x = matriz[0][0]*ponto.X() + matriz[0][1]*ponto.Y() +
               matriz[0][2]*ponto.Z() + matriz[0][3]*ponto.W();
    double y = matriz[1][0]*ponto.X() + matriz[1][1]*ponto.Y() +
               matriz[1][2]*ponto.Z() + matriz[1][3]*ponto.W();
    double z = matriz[2][0]*ponto.X() + matriz[2][1]*ponto.Y() +
               matriz[2][2]*ponto.Z() + matriz[2][3]*ponto.W();
    double w = matriz[3][0]*ponto.X() + matriz[3][1]*ponto.Y() +
               matriz[3][2]*ponto.Z() + matriz[3][3]*ponto.W();
    return Ponto(x, y, z, w);
}

Transformacao Transformacao::operator*(const Transformacao& t) {
    double parcial;
    Transformacao resultado;
    for (int lin=0; lin<4; ++lin)
        for (int col=0; col<4; ++col) {
            parcial = 0;
            for (int i=0; i<4; ++i)
                parcial += matriz[lin][i] * t.matriz[i][col];
            resultado.matriz[lin][col] = parcial;
        }
    return resultado;
}

void Transformacao::CarregarProjecaoPer(double d) {
    CarregarIdentidade();
    matriz[3][2] = 1/d;
    matriz[3][3] = 0;
}

void Transformacao::CarregarRotacao(const Ponto& vetRef, const float radianos) {
    double anguloY;
    if (vetRef.Z()) {
        Ponto projEmY(vetRef.X(), 0, vetRef.Z());
        if (vetRef.Z() < 0)
            anguloY = -(projEmY.AnguloAte(Ponto(1,0,0)));
        else
            anguloY = projEmY.AnguloAte(Ponto(1,0,0));
    }
    else
        anguloY = 0;
    CarregarRotacaoY(anguloY);
    Ponto vetTemp = (*this) * vetRef;
    double anguloZ;
    if (vetTemp.Y() < 0)
        anguloZ = vetTemp.AnguloAte(Ponto(1,0,0));
    else
        anguloZ = -(vetTemp.AnguloAte(Ponto(1,0,0)));
    Transformacao tTemp;
    tTemp.CarregarRotacaoZ(anguloZ);
    *this = tTemp * (*this);
    tTemp.CarregarRotacaoX(radianos);
    *this = tTemp * (*this);
    tTemp.CarregarRotacaoZ(-anguloZ);
    *this = tTemp * (*this);
    tTemp.CarregarRotacaoY(-anguloY);
    *this = tTemp * (*this);
}
                                                                                                                                                                                 // (C) Bruno de Oliveira Schneider - Não é permitido divulgar esta implementação - Publishing this code is forbidden
