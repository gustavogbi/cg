// transformacao.h  (C) Bruno de Oliveira Schneider - DCC - UFLA - Abril de 2003

// Esta classe representa uma transformacao geometrica qualquer (rotacao,
// translacao, escala, etc...). Usa-se a pre-multiplicacao, ou seja, T3 * T2 * T1 e'
// a transformacao T1 seguida de T2, seguida de T3.

#ifndef TRANSFORMACAO_H
#define TRANSFORMACAO_H

#include <cmath>
#include <iostream>
#include "ponto.h"

class Transformacao {
    friend std::ostream& operator<<(std::ostream& saida, const Transformacao& t);
    public:
        Transformacao();
        Transformacao(const Transformacao& trans);
        Ponto operator*(const Ponto& ponto);
        Transformacao operator*(const Transformacao& t);
        void CarregarTranslacao(double x, double y, double z);
        void CarregarTranslacao(const Ponto& vetorTranslacao);
        void CarregarRotacaoX(double radianos);
        void CarregarRotacaoY(double radianos);
        void CarregarRotacaoZ(double radianos);
        // Rotacao em torno de um VETOR arbitrario
        void CarregarRotacao(const Ponto& vetRef, const float radianos);
        void CarregarEscala(double escX, double escY, double escZ);
        void CarregarCisalhamento(double cisX, double cisY);
        // Projecao Perspectiva Especial (CP na origem, x e y alinhados)
        void CarregarProjecaoPer(double d);
        void CarregarIdentidade();
    protected:
    private:
        double matriz[4][4];
};

#endif
                                                                                                                                                                                 // (C) Bruno de Oliveira Schneider - Não é permitido divulgar esta implementação - Publishing this code is forbidden
