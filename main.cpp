// Exemplo de mapeamento de texturas

// Criado por Bruno Schneider a partir do exemplo "cube.cpp" do wxWidgets.
// (C) Bruno Schneider - DCC / UFLA - Julho de 2017

// Gustavo Rodrigues Sousa
// método modificado: PainelDoDesenho::DesenharTriangulo

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "main.h"
#include <iostream>

#ifndef wxHAS_IMAGES_IN_RESOURCES
    #include "icone.xpm"
#endif

#include <iomanip>

// IDs para os controles
enum {
    ID_SLIDER = wxID_HIGHEST + 1,
    ID_MAP_NAO_LINEAR
};

using namespace std;

// Converte Ponto para wxPoint
wxPoint WxPoint(const Ponto& v) {
    return wxPoint(v.X(), v.Y());
}

// Classe MyApp representa o programa
wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    if ( !wxApp::OnInit() )
        return false;

    Janela* ptFrame = new Janela();
    ptFrame->AssociarTextura(&mTextura);
    wxImage::AddHandler(new wxPNGHandler);
    wxImage::AddHandler(new wxJPEGHandler);
    return true;
}

int MyApp::OnExit() {
    return wxApp::OnExit();
}

// PainelDaTextura: Parte da janela onde será mostrada a textura e o usuário define as coordenadas
// de textura.

wxBEGIN_EVENT_TABLE(PainelDaTextura, wxPanel)
    EVT_PAINT(PainelDaTextura::OnPaint)
    EVT_COMMAND_SCROLL(ID_SLIDER, PainelDaTextura::QuandoMoverSlider)
wxEND_EVENT_TABLE()

PainelDaTextura::PainelDaTextura(wxWindow* ptMae)
    : wxPanel(ptMae, wxID_ANY, wxDefaultPosition, wxSize(256, -1)) {

    new wxSlider(this, ID_SLIDER, 45, 0, 60, wxPoint(0, 300), wxSize(256, 10));
}
void PainelDaTextura::AssociarPainelDesenho(PainelDoDesenho* ptPainel) {
    mPtPainelDesenho = ptPainel;
}

void PainelDaTextura::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);
    dc.DrawText(wxT("Textura:"), 5, 4);
    // Desenhar a textura com margem superior de 30 pixels
    dc.DrawBitmap(wxBitmap(*mPtTextura), 0, 30);
}
void PainelDaTextura::QuandoMoverSlider(wxScrollEvent& evento) {
    mPtPainelDesenho->AlterarRotatacao(evento.GetInt());
}


// PainelDoDesenho: Parte da janela onde será desenhado um poligono texturizado.

wxBEGIN_EVENT_TABLE(PainelDoDesenho, wxPanel)
    EVT_PAINT(PainelDoDesenho::OnPaint)
    EVT_SIZE(PainelDoDesenho::OnSize)
wxEND_EVENT_TABLE()

PainelDoDesenho::PainelDoDesenho(wxWindow* ptMae)
    : wxPanel(ptMae, wxID_ANY, wxDefaultPosition, wxDefaultSize),
      mVertices{ Ponto(-1, 0, 0), Ponto(-1, 1, 0), Ponto(1, 1, 0), Ponto(1, 0, 0) },
      mVetCoordTex{{0,1}, {0,0}, {1,0}, {1,1}},
      mPtMae(dynamic_cast<wxFrame*>(ptMae)),
      mMapeamentoNaoLinear(true)
{
    SetBackgroundColour(*wxBLACK);
    mRotacao.CarregarRotacaoX(M_PI_4);
    mPerspectiva.CarregarProjecaoPer(1);
    mAfastamento.CarregarTranslacao(0,0,1);
}

void PainelDoDesenho::AlterarRotatacao(int graus) {
    mRotacao.CarregarRotacaoX(graus*0.017453293);
    Refresh();
}

// Metodo para desenhar um triangulo com textura
void PainelDoDesenho::DesenharTriangulo(wxPaintDC* ptDC, Vertice v1, Vertice v2, Vertice v3) {
    wxPen pen(*wxBLUE_PEN);

    double denominador_y = (((v2.pos.Y() - v3.pos.Y()) * (v3.pos.X() - v1.pos.X())) + ((v3.pos.X() - v2.pos.X()) * (v3.pos.Y() - v1.pos.Y())));
    double denominador_x = (((v3.pos.X() - v2.pos.X()) * (v3.pos.Y() - v1.pos.Y())) + ((v2.pos.Y() - v3.pos.Y())*(v3.pos.X() - v1.pos.X())));

    double um_zy = (((1/v2.pos.Z()) - (1/v3.pos.Z())) * (v3.pos.X() - v1.pos.X()) + (v3.pos.X() - v2.pos.X()) * ((1/v3.pos.Z()) - (1/v1.pos.Z()))) / denominador_y;
    double um_zx = (((1/v3.pos.Z()) - (1/v2.pos.Z())) * (v3.pos.Y() - v1.pos.Y()) + (v2.pos.Y() - v3.pos.Y()) * ((1/v3.pos.Z()) - (1/v1.pos.Z()))) / denominador_x;

    double uzy = (((v2.tex.u/v2.pos.Z()) - (v3.tex.u/v3.pos.Z())) * (v3.pos.X() - v1.pos.X()) + (v3.pos.X() - v2.pos.X()) * ((v3.tex.u/v3.pos.Z()) - (v1.tex.u/v1.pos.Z()))) / denominador_y;
    double uzx = (((v3.tex.u/v3.pos.Z()) - (v2.tex.u/v2.pos.Z())) * (v3.pos.Y() - v1.pos.Y()) + (v2.pos.Y() - v3.pos.Y()) * ((v3.tex.u/v3.pos.Z()) - (v1.tex.u/v1.pos.Z()))) / denominador_x;

    double vzy = ((((v2.tex.v/v2.pos.Z()) - (v3.tex.v/v3.pos.Z())) * (v3.pos.X() - v1.pos.X())) + ((v3.pos.X() - v2.pos.X()) * ((v3.tex.v/v3.pos.Z()) - (v1.tex.v/v1.pos.Z())))) / denominador_y;
    double vzx = ((((v3.tex.v/v3.pos.Z()) - (v2.tex.v/v2.pos.Z())) * (v3.pos.Y() - v1.pos.Y())) + ((v2.pos.Y() - v3.pos.Y()) * ((v3.tex.v/v3.pos.Z()) - (v1.tex.v/v1.pos.Z())))) / denominador_x;

    if (v2.pos.Y() < v1.pos.Y()) {
        swap(v1, v2);
    }
    if (v3.pos.Y() < v2.pos.Y()) {
        swap(v3, v2);
    }
    if (v2.pos.Y() < v1.pos.Y()) {
        swap(v1, v2);
    }

    double xesq_incl, xdir_incl;

    if (v2.pos.X() >= v1.pos.X()) {
        xesq_incl = double(v3.pos.X() - v1.pos.X()) /double(v3.pos.Y() - v1.pos.Y());
        xdir_incl = double(v2.pos.X() - v1.pos.X())/double(v2.pos.Y() - v1.pos.Y());
    } else {
        xdir_incl = double(v3.pos.X() - v1.pos.X()) /double(v3.pos.Y() - v1.pos.Y());
        xesq_incl = double(v2.pos.X() - v1.pos.X())/double(v2.pos.Y() - v1.pos.Y());
    }

    double xesq = v1.pos.X();
    double xdir = v1.pos.X();

    double ymin = v1.pos.Y();
    double ymax = v2.pos.Y();

    double um_sobre_z = 1 / v1.pos.Z();
    double u_sobre_z = v1.tex.u / v1.pos.Z();
    double v_sobre_z = v1.tex.v / v1.pos.Z();
    double um_sobre_z_cpy = um_sobre_z;
    double u_sobre_z_cpy = u_sobre_z;
    double v_sobre_z_cpy = v_sobre_z;

    double u, v;

    for (int y = ymin; y < ymax; y++) {
        for (int x = int(xesq); x < int(xdir); x++) {
            u = u_sobre_z / um_sobre_z;
            v = v_sobre_z / um_sobre_z;
            pen.SetColour(mPtTextura->ColourAt(u, v));
            ptDC->SetPen(pen);
            ptDC->DrawPoint(x, y);
            um_sobre_z += um_zx;
            u_sobre_z += uzx;
            v_sobre_z += vzx;
        }
        um_sobre_z_cpy += (xesq_incl * um_zx) + um_zy;
        um_sobre_z = um_sobre_z_cpy;
        u_sobre_z_cpy += (xesq_incl * uzx) + uzy;
        u_sobre_z = u_sobre_z_cpy;
        v_sobre_z_cpy += (xesq_incl * vzx) + vzy;
        v_sobre_z = v_sobre_z_cpy;
        xdir += xdir_incl;
        xesq += xesq_incl;
    }


    if (v2.pos.X() >= v1.pos.X()) {
        xesq_incl = double(v3.pos.X() - v1.pos.X()) / double(v3.pos.Y() - v1.pos.Y());
        xdir_incl = double(v2.pos.X() - v3.pos.X()) / double(v2.pos.Y() - v3.pos.Y());
        xdir = v2.pos.X();
    } else {
        xdir_incl = double(v3.pos.X() - v1.pos.X()) / double(v3.pos.Y() - v1.pos.Y());
        xesq_incl = double(v2.pos.X() - v3.pos.X()) / double(v2.pos.Y() - v3.pos.Y());
        xesq = v2.pos.X();
    }

    ymin = v2.pos.Y();
    ymax = v3.pos.Y();

    um_sobre_z = um_sobre_z_cpy;
    u_sobre_z = u_sobre_z_cpy;
    v_sobre_z = v_sobre_z_cpy;

    for (int y = ymin; y < ymax; y++) {
        for (int x = int(xesq); x < int(xdir); x++) {
            u = u_sobre_z / um_sobre_z;
            v = v_sobre_z / um_sobre_z;
            pen.SetColour(mPtTextura->ColourAt(u, v));
            ptDC->SetPen(pen);
            ptDC->DrawPoint(x, y);
            um_sobre_z += um_zx;
            u_sobre_z += uzx;
            v_sobre_z += vzx;
        }
        um_sobre_z_cpy += (xesq_incl * um_zx) + um_zy;
        um_sobre_z = um_sobre_z_cpy;
        u_sobre_z_cpy += (xesq_incl * uzx) + uzy;
        u_sobre_z = u_sobre_z_cpy;
        v_sobre_z_cpy += (xesq_incl * vzx) + vzy;
        v_sobre_z = v_sobre_z_cpy;
        xdir += xdir_incl;
        xesq += xesq_incl;
    }
}

void PainelDoDesenho::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);
    Transformacao projecao = mTranslacao * mEscala * mPerspectiva * mAfastamento * mRotacao;
    Vertice vetVertices[4];
    for (int i = 0; i < 4; ++i) {
        vetVertices[i].pos = projecao * mVertices[i];
        vetVertices[i].tex = mVetCoordTex[i];
        vetVertices[i].ConverterParaPosicaoNaTela();
    }
    DesenharTriangulo(&dc, vetVertices[0], vetVertices[1], vetVertices[2]);
    DesenharTriangulo(&dc, vetVertices[2], vetVertices[3], vetVertices[0]);
}

void PainelDoDesenho::OnSize(wxSizeEvent& event) {
    wxSize tamanho = GetClientSize();
    mEscala.CarregarEscala(tamanho.x/2.0, -tamanho.y, 1.0);
    mTranslacao.CarregarTranslacao(tamanho.x/2.0, tamanho.y, 0);
    wxLogStatus(wxT("Tamanho da área de desenho: %dx%d"), tamanho.x, tamanho.y);
}

void PainelDoDesenho::TrocarTipoDeMapeamento() {
    mMapeamentoNaoLinear = not mMapeamentoNaoLinear;
    Refresh();
}

Textura::Textura()
    : wxImage(256, 256, false) {
    // Inicializar a textura com um quadriculado vermelho/verde/azul
    unsigned char* ptData = GetData();
    int valor;
    for (int lin = 0; lin < 256; ++lin) {
        for (int col = 0; col < 256; ++col) {
            valor = (lin/8 + col/8) % 3;
            if (valor == 0) {
                *ptData++ = 255; *ptData++ = 0; *ptData++ = 0;
            }
            else if (valor == 1) {
                *ptData++ = 0; *ptData++ = 255; *ptData++ = 0;
            }
            else {
                *ptData++ = 0; *ptData++ = 0; *ptData++ = 255;
            }
        }
    }
}

// Retorna a cor da textura nas coordenadas (reais) indicadas
wxColour Textura::ColourAt(float x, float y) {
    // Colocado teste para reclamar uma unica vez de coordenadas invalidas para textura, auxiliando
    // na depuracao.
    static bool aindaNaoReclamou = true;
    if (x > 1) {
        if (aindaNaoReclamou) {
            cerr << "Erro: Coordenadas (" << x << ", " << y << ") invalidas para a textura.\n";
            aindaNaoReclamou = false;
        }
        x = 1;
    }
    else if (x < 0) {
        if (aindaNaoReclamou) {
            cerr << "Erro: Coordenadas (" << x << ", " << y << ") invalidas para a textura.\n";
            aindaNaoReclamou = false;
        }
        x = 0;
    }
    if (y > 1) {
        if (aindaNaoReclamou) {
            cerr << "Erro: Coordenadas (" << x << ", " << y << ") invalidas para a textura.\n";
            aindaNaoReclamou = false;
        }
        y = 1;
    }
    else if (y < 0) {
        if (aindaNaoReclamou) {
            cerr << "Erro: Coordenadas (" << x << ", " << y << ") invalidas para a textura.\n";
            aindaNaoReclamou = false;
        }
        y = 0;
    }
    int largura = GetWidth();
    int linha  = int(y * float(GetHeight()-1));
    int coluna = int(x * float(largura-1));
    unsigned char* ptDados = GetData() + ((linha * largura + coluna) * 3);
    return wxColour(*ptDados, *(ptDados+1), *(ptDados+2));
}

// ----------------------------------------------------------------------------
// Janela: janela principal do programa

// A tabela de eventos só pode ter métodos da própria classe.
wxBEGIN_EVENT_TABLE(Janela, wxFrame)
    EVT_MENU(wxID_OPEN, Janela::AoAbrirArquivo)
    EVT_MENU(ID_MAP_NAO_LINEAR, Janela::TrocarTipoDeMapeamento)
    EVT_MENU(wxID_CLOSE, Janela::OnClose)
wxEND_EVENT_TABLE()

Janela::Janela(bool stereoWindow)
       : wxFrame(nullptr, wxID_ANY, wxT("Mapeamento de Texturas")) {

    // criar um painel de fundo para colocar a area de desenho e a imagem
    mPtPainelTextura = new PainelDaTextura(this);
    // colocar a área de desenho sobre o fundo
    mPtPainelDesenho = new PainelDoDesenho(this);
    wxSizer* ptSizer = new wxBoxSizer(wxHORIZONTAL);
    ptSizer->Add(mPtPainelDesenho, 1, wxEXPAND | wxALL, 5);
    ptSizer->Add(mPtPainelTextura, 0, wxEXPAND | wxALL, 5);
    this->SetSizer(ptSizer);

    SetIcon(wxICON(ICONE));

    // Criar a barra de menus
    wxMenu *menu = new wxMenu;
    menu->Append(wxID_OPEN, "Abrir &Textura\tCtrl-T");
    menu->AppendCheckItem(ID_MAP_NAO_LINEAR, wxT("Mapeamento não &linear\tCtrl-L"),
                          wxT("Liga ou desliga o mapeamento não linear."));
    menu->AppendSeparator();
    menu->Append(wxID_CLOSE, wxT("&Encerrar\tCtrl-Q"));
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menu, wxT("&Arquivo"));

    SetMenuBar(menuBar);

    CreateStatusBar();
    SetClientSize(800, 600);

    mPtPainelTextura->AssociarPainelDesenho(mPtPainelDesenho);
    Show();

}

void Janela::AssociarTextura(Textura* ptTextura) {
    mPtTextura = ptTextura;
    mPtPainelTextura->AssociarTextura(ptTextura);
    mPtPainelDesenho->AssociarTextura(ptTextura);
}
void Janela::AoAbrirArquivo(wxCommandEvent& event) {
    wxFileDialog dialogo(this, wxT("Abrir Imagem"), "", "",
        "Imagens|*.png;*.jpg;*.jpeg", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (dialogo.ShowModal() == wxID_CANCEL)
        return;
    if (not mPtTextura->LoadFile(dialogo.GetPath()))
        cerr << "Erro ao abrir o arquivo '" << dialogo.GetPath() << "'.\n";
    Refresh();
}

void Janela::TrocarTipoDeMapeamento(wxCommandEvent& e) {
    mPtPainelDesenho->TrocarTipoDeMapeamento();
}

void Janela::OnClose(wxCommandEvent& WXUNUSED(event)) {
    Close(true);
}
                                                                                                                                                                                 // (C) Bruno de Oliveira Schneider - Não é permitido divulgar esta implementação - Publishing this code is forbidden
