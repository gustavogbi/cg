// Classe Matriz
// (C) Bruno de Oliveira Schneider - DCC - UFLA - Marco de 2017
 
// A classe Tipo deve definir:
// - construtor de copia
// - operador =

#ifndef MATRIZ_H
#define MATRIZ_H

#include <string>
#include <sstream>
#include <stdexcept>

template <class Tipo>
class Matriz {
    public:
        Matriz();
        Matriz(int linhas, int colunas);
        Matriz(const Matriz& matriz);
        ~Matriz();
        void AlterarItem(int linha, int coluna, const Tipo& valor);
        Tipo Item(int linha, int coluna) const;
        Tipo Item(float y, float x) const;
        int NumeroLinhas() const;
        int NumeroColunas() const;
        void operator =(const Matriz& matriz);
        Matriz operator *(const Matriz& matriz);
        void AlterarTamanho(int novoNumLinhas, int novoNumColunas);
        void AlterarTodos(const Tipo& valor);
        const Tipo* Dados() const;
    private:
        int CalcularPosicao(int linha, int coluna) const;
        int numLinhas;
        int numColunas;
        Tipo* vetDados;
};

template <class Tipo>
Matriz<Tipo>::Matriz()
    : numLinhas(0), numColunas(0), vetDados(nullptr) {
}

template <class Tipo>
Matriz<Tipo>::Matriz(int linhas, int colunas)
    : numLinhas(linhas), numColunas(colunas), vetDados(new Tipo[numLinhas*numColunas]) {
}

template <class Tipo>
Matriz<Tipo>::Matriz(const Matriz &matriz)
    : numLinhas(matriz.NumeroLinhas()), numColunas(matriz.NumeroColunas()) {
    int qtdeValores = numLinhas * numColunas;
    vetDados = new Tipo[qtdeValores];
    for(int i = 0; i < qtdeValores; ++i)
        vetDados[i] = matriz.vetDados[i];
}

template <class Tipo>
Matriz<Tipo>::~Matriz() {
    if (vetDados != nullptr)
        delete[] vetDados;
}

template <class Tipo>
int Matriz<Tipo>::NumeroLinhas() const {
    return numLinhas;
}

template <class Tipo>
int Matriz<Tipo>::NumeroColunas() const {
    return numColunas;
}

template <class Tipo>
const Tipo* Matriz<Tipo>::Dados() const {
    return vetDados;
}

template <class Tipo>
Tipo Matriz<Tipo>::Item(int linha, int coluna) const {
// Retorna o item na posicao indicada
// Requer a existencia de um construtor de copia na classe Tipo
    try {
        return vetDados[CalcularPosicao(linha, coluna)]; 
    }
    catch (std::exception e) {
        throw std::runtime_error("Nao existe item nesta posicao.");
    }
}

template <class Tipo>
Tipo Matriz<Tipo>::Item(float y, float x) const {
    if ((y < 0) or (y >= 1) or (x < 0) or (x >= 1)) {
        std::stringstream mensagem;
        mensagem << "Indices (" << y << ", " << x << ") invalidos para a matriz.";
        throw std::runtime_error(mensagem.str());
    }
    int linha  = int(y * float(numLinhas));
    int coluna = int(x * float(numColunas));
//    std::cout <<"y: "<<y<<" linha: "<<linha<<" coluna: "<<coluna<<std::endl;
    return vetDados[CalcularPosicao(linha, coluna)]; 
}

template <class Tipo>
void Matriz<Tipo>::AlterarItem(int linha, int coluna, const Tipo &valor) {
// Altera o valor do item na posicao indicada
// Requer a existencia do operador = na classe Tipo
    try {
        vetDados[CalcularPosicao(linha, coluna)] = valor;
    }
    catch (std::exception e) {
        throw std::runtime_error("Nao existe item nesta posicao para alterar.");
    }
}

template <class Tipo>
void Matriz<Tipo>::AlterarTamanho(int novoNumLinhas, int novoNumColunas) {
// Este metodo altera o numero de linhas e colunas.
// Todos os valores antigos sao perdidos.
    if (vetDados != nullptr)
        delete[] vetDados;
    numLinhas = novoNumLinhas;
    numColunas = novoNumColunas;
    vetDados = new Tipo[novoNumLinhas*novoNumColunas]; 
}

template <class Tipo>
void Matriz<Tipo>::operator=(const Matriz &matriz) {
// Atribuicao entre matrizes
    AlterarTamanho(matriz.NumeroLinhas(), matriz.NumeroColunas());
    int qtdeValores = numLinhas * numColunas;
    for(int i = 0; i < qtdeValores; ++i)
        vetDados[i] = matriz.vetDados[i];
}

template <class Tipo>
Matriz<Tipo> Matriz<Tipo>::operator*(const Matriz& matriz) {
// Implementacao de multiplicacao de matrizes
    if (numColunas != matriz.numLinhas)
        throw std::runtime_error("A multiplicacao de matrizes nao pode ser feita.");

    int& numLinhasResultado = numLinhas;
    int& numColunasResultado = matriz.numColunas;
    Matriz<Tipo> resultado(numLinhasResultado, numColunasResultado);
    Tipo parcial;
    for (int lin=0; lin < numLinhasResultado; ++lin)
        for (int col=0; col < numColunasResultado; ++col) {
            parcial = 0;
            for (int i=0; i < numColunas; ++i) // somatorio
                parcial += Item(lin, i) * matriz.Item(i, col);
            resultado.AlterarItem(lin, col, parcial);
        }
    return resultado;
}

template <class Tipo>
int Matriz<Tipo>::CalcularPosicao(int linha, int coluna) const {
// Retorna o indice do vetDados que corresponde aa linha e coluna indicada
// Linha e coluna sao indices que comecam em 1
// Todo acesso ao vetDados de dados deve ser feito atraves desta funcao
    if (    (linha >= numLinhas) or (coluna >= numColunas) or
            (linha < 0) or (coluna < 0) )
        throw std::runtime_error("Indices invalidos para a matriz.");
    return linha*numColunas + coluna;
}

template <class Tipo>
void Matriz<Tipo>::AlterarTodos(const Tipo& valor) {
// Copia um valor para todas as posicoes da matriz
    int qtdeElementos = numLinhas * numColunas;
    for(int i=0; i < qtdeElementos; ++i)
        vetDados[i] = valor;
}

template <class Tipo>
std::ostream& operator<<(std::ostream& saida, const Matriz<Tipo>& m) {
    int numLinhas = m.NumeroLinhas();
    int numColunas = m.NumeroColunas();
    for (int lin = 0; lin < numLinhas; ++lin) {
        for (int col = 0; col < numColunas; ++col) {
            saida << m.Item(lin, col) << ' ';
        }
        saida << "\n";
    }
    return saida;
}
#endif
                                                                                                                                                                                 // (C) Bruno de Oliveira Schneider - Não é permitido divulgar esta implementação - Publishing this code is forbidden
