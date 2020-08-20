// Exemplo de mapeamento de texturas

// Criado por Bruno Schneider a partir do exemplo "cube.cpp" do wxWidgets.
// (C) Bruno Schneider - DCC / UFLA - Julho de 2017

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
    // O mapeamento de texturas e' feito junto da rasterizacao, por isso este metodo e' ao mesmo
    // tempo rasterizacao de triangulos e mapeamento de texturas.
    // Para acessar as coordenadas dos vertices use o atributo publico pos. Ex.: v1.pos.X()
    // A interface permite implementacao de mapeamento linear e nao-linear. O atributo booleano
    // mMapeamentoNaoLinear indica se o mapeamento nao linear está selecionado na interface.
    // Mapeamento nao linear e' a opcao padrao na interface. O enunciado define quais mapeamentos
    // devem ser implementados.
    // Para desenhar um pixel, mude a "caneta" usada no DC e depois use o metodo DrawPoint(int,int).
    wxPen pen(*wxBLUE_PEN); // caneta local para escolher a cor do pixel
    // Ex.:
    //      pen.SetColour(mPtTextura->ColourAt(uinvz/invz, vinvz/invz)); // selecionar cor pela textura
    //      ptDC->SetPen(pen); // mudar a cor corrente
    //      ptDC->DrawPoint(x, y); // desenhar com a cor corrente

    //se o ponto p2 estiver embaixo do ponto p1, troque
    if(v2.pos.Y() < v1.pos.Y()) swap(v1, v2);
    // Se o ponto p3 estiver embaixo do ponto p2, troque
    if(v3.pos.Y() < v2.pos.Y()) swap(v3, v2);
    // Se novo ponto p2 estiver embaixo do ponto p1, troque
    if(v2.pos.Y() < v1.pos.Y()) swap(v1, v2);

    
    double incXesq, incXdir;

    //Se o ponto p2 estiver a direita do ponto p1
    if(v2.pos.X() >= v1.pos.X()){
        incXesq = double(v3.pos.X() - v1.pos.X())/double(v3.pos.Y() - v1.pos.Y());
        incXdir = double(v2.pos.X() - v1.pos.X())/double(v2.pos.Y() - v1.pos.Y());
    } else {
        incXdir = double(v3.pos.X() - v1.pos.X())/double(v3.pos.Y() - v1.pos.Y());
        incXesq = double(v2.pos.X() - v1.pos.X())/double(v2.pos.Y() - v1.pos.Y());
    }

    double xEsq = v1.pos.X();
    double xDir = v1.pos.X();

    // Definição do valor minimo de Y para a parte inferior do triangulo
    int yMin = v1.pos.Y();
    int yMax = v2.pos.Y();

    
    // calculate start and end scanlines
	const int yStart = (int)ceil( v1.pos.Y() - 0.5f );
	const int yEnd = (int)ceil( v3.pos.Y() - 0.5f ); // the scanline AFTER the last line drawn
    // coisas fora do for
    // calulcate dVertex / dy juntamente com o interpolant prestep
    const float delta_y = v3.pos.Y() - v1.pos.Y();
    double dv1X = ((v3.pos.X() - v1.pos.X()) / delta_y)*2 * (float( yStart ) + 0.5f - v1.pos.Y());
    double dv1Y = ((v3.pos.Y() - v1.pos.Y()) / delta_y)*2 * (float( yStart ) + 0.5f - v1.pos.Y());
    double dv1Z = ((v3.pos.Z() - v1.pos.Z()) / delta_y)*2 * (float( yStart ) + 0.5f - v1.pos.Y());
    Ponto* dv1 = new Ponto(dv1X, dv1Y, dv1Z);
    double dv2X = ((v3.pos.X() - v2.pos.X()) / delta_y)*2 * (float( yStart ) + 0.5f - v1.pos.Y());
    double dv2Y = ((v3.pos.Y() - v2.pos.Y()) / delta_y)*2 * (float( yStart ) + 0.5f - v1.pos.Y());
    double dv2Z = ((v3.pos.Z() - v2.pos.Z()) / delta_y)*2 * (float( yStart ) + 0.5f - v1.pos.Y());
    Ponto* dv2 = new Ponto(dv2X, dv2Y, dv2Z);
    // create right edge interpolant
    double itEdge2X = v2.pos.X();
    double itEdge2Y = v2.pos.Y();
    double itEdge2Z = v2.pos.Z();
	// create edge interpolant for left edge (always v0)
	double itEdge1X = v1.pos.X();
    double itEdge1Y = v1.pos.Y();
    double itEdge1Z = v1.pos.Z();


	// init tex width/height and clamp values
	const float tex_width = 255;
	const float tex_height = 255;
	const float tex_clamp_x = tex_width - 1.0f;
	const float tex_clamp_y = tex_height - 1.0f;


    // Desenha os pixels da parte inferior do triangulo
    for (int y = yStart; y < yEnd; y++) {
        //int x = int(xEsq);
        itEdge1X += dv1->X();
        itEdge1Y += dv1->Y();
        itEdge1Z += dv1->Z();

        itEdge2X += dv2->X();
        itEdge2Y += dv2->Y();
        itEdge2Z += dv2->Z();

		// calculate start and end pixels
		int xStart = (int)ceil( itEdge1X - 0.5f );
		int xEnd = (int)ceil( itEdge2X - 0.5f ); // the pixel AFTER the last pixel drawn
		// calculate scanline dTexCoord / dx
		double dtcLine = (itEdge2Z - itEdge1Z) / (itEdge2X - itEdge1X);
		// create scanline tex coord interpolant and prestep
		double itcLine = itEdge1Z + dtcLine * (float( xStart ) + 0.5f - itEdge1X);
        for( int x = xStart; x < xEnd; x++,itcLine += dtcLine ) {
            pen.SetColour(mPtTextura->ColourAt(int( std::fmod( itcLine * tex_width,tex_clamp_x)), int( std::fmod( itcLine * tex_height,tex_clamp_y))));
            ptDC->SetPen(pen); // mudar a cor corrente
            ptDC->DrawPoint(x, y); // desenhar com a cor corrente
        }

        // while(x < int(xDir)){
            
        //     //PutPixel( x,y,tex.GetPixel(
		// 	//int( std::fmod( itcLine.v * tex_width,tex_clamp_x ) ),
		// 	//int( std::fmod( itcLine.u * tex_height,tex_clamp_y ) ) ) );
            

        //     x++;
        // }
        // Incrementa o xDir e o xEsq
        //xDir += incXdir;
        //xEsq += incXesq;
    }

    // Se o X do ponto 2 for maior que o X do ponto 1
    if(v2.pos.X() >= v1.pos.X()){
        incXesq = double(v3.pos.X() - v1.pos.X())/double(v3.pos.Y() - v1.pos.Y());
        incXdir = double(v2.pos.X() - v3.pos.X())/double(v2.pos.Y() - v3.pos.Y());
        xDir = v2.pos.X();
    } else { // se o x do ponto2 for menor que o x do ponto 1
        incXdir = double(v3.pos.X() - v1.pos.X())/double(v3.pos.Y() - v1.pos.Y());
        incXesq = double(v2.pos.X() - v3.pos.X())/double(v2.pos.Y() - v3.pos.Y());
        xEsq = v2.pos.X();
    }

    yMin = v2.pos.Y();
    yMax = v3.pos.Y();

    // Desenha os pixels da metade superior do triangulo
    for (int y = yMin; y < yMax; y++) {
        int x = int(xEsq);



        while(x < int(xDir)){




            //pen.SetColour(mPtTextura->ColourAt(x/xEsq, y/xDir));
            ptDC->SetPen(pen); // mudar a cor corrente
            ptDC->DrawPoint(x, y); // desenhar com a cor corrente
            x++;
        }
        xDir += incXdir;
        xEsq += incXesq;
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