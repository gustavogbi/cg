// (C) Bruno de Oliveira Schneider - DCC / UFLA - A divulgacao deste codigo e' proibida.

#ifndef MAIN_H
#define MAIN_H

#include "wx/glcanvas.h"
#include "matriz.h"
#include "ponto.h"
#include "vertice.h"
#include "transformacao.h"
#include <memory>

typedef unsigned int Uint32;
const int LARGURA_TEXTURA = 256;
const int ALTURA_TEXTURA  = 256;
const Uint32 PRETO = 0x000000FF;
const Uint32 BRANCO = 0xFFFFFFFF;
const Uint32 VERMELHO = 0xFF0000FF;
const Uint32 VERDE = 0x00FF00FF;
const Uint32 AZUL = 0x0000FFFF;

class Textura : public wxImage {
    public:
        Textura();
        wxColour ColourAt(float x, float y);
    private:
};

class PainelDoDesenho : public wxPanel {
    public:
        PainelDoDesenho(wxWindow* ptMae);
        void AlterarRotatacao(int graus);
        void AssociarTextura(Textura* ptTextura) {
            mPtTextura = ptTextura;
        }
        void TrocarTipoDeMapeamento();
    private:
        void DesenharTriangulo(wxPaintDC* ptDC, Vertice v1, Vertice v2, Vertice v3);
        void OnPaint(wxPaintEvent& evento);
        void OnSize(wxSizeEvent& evento);
        Ponto mVertices[4];
        Ponto mVerticesTransformados[4];
        Ponto mNormal; // normal ao plano desenhado em coordenadas do mundo
        CoordTex mVetCoordTex[4];
        wxFrame* mPtMae;
        Textura* mPtTextura;
        Transformacao mEscala; // A parte da escala na projecao
        Transformacao mRotacao; // A parte da rotacao na projecao
        Transformacao mTranslacao; // A parte da translacao na projecao
        Transformacao mPerspectiva; // A parte da deformacao perspectiva na projecao
        Transformacao mAfastamento; // para afastar o poligono do plano de projecao depois da rotacao
        bool mMapeamentoNaoLinear;

        wxDECLARE_EVENT_TABLE();
};

class PainelDaTextura : public wxPanel {
    public:
        PainelDaTextura(wxWindow* ptMae);
        void AssociarTextura(Textura* ptTextura) {
            mPtTextura = ptTextura;
        }
        void AssociarPainelDesenho(PainelDoDesenho* ptPainel);
    private:
        void OnPaint(wxPaintEvent& evento);
        void QuandoMoverSlider(wxScrollEvent& evento);
        Textura* mPtTextura;
        PainelDoDesenho* mPtPainelDesenho;

        wxDECLARE_EVENT_TABLE();
};

// Classe MyApp e' o programa
class MyApp : public wxApp {
    public:
        MyApp() { }
        virtual bool OnInit() override;
        virtual int OnExit() override;
    private:
        Textura mTextura;
};

// Define a new frame type
class Janela : public wxFrame {
    public:
        Janela(bool stereoWindow = false);
        void AssociarTextura(Textura* ptTextura);
    private:
        void AoAbrirArquivo(wxCommandEvent& event);
        void OnClose(wxCommandEvent& event);
        void TrocarTipoDeMapeamento(wxCommandEvent& e);
        PainelDaTextura* mPtPainelTextura;
        PainelDoDesenho* mPtPainelDesenho;
        Textura* mPtTextura;

        wxDECLARE_EVENT_TABLE();
};

#endif // MAIN_H
                                                                                                                                                                                 // (C) Bruno de Oliveira Schneider - Não é permitido divulgar esta implementação - Publishing this code is forbidden
