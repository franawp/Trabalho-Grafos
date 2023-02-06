/* Algortimos em Grafos
 * Heurística Caixeiro viajante
 * 
 * Parte 1:
 *  - Criação da função e estruturas para armazenar os dados
 *  - Gerar uma solução aleatória e verificar ser uma solução válida
 *
 * Desenvolvido pelos alunos:
 * - Francisco Afonso de Oliveira Neto
 * - Gabriel Raimundo Pereira Barroso
 * - Ingrid Falchi
 */

#include <bits/stdc++.h>
using namespace std;

struct Vertice {
    int identificador = 0;
    double latitude = 0;
    double longitude = 0;
    int demanda = 0;
    int janelaInicial = 0;
    int janelaFinal = 0;
    int duracao = 0;
    int parDeColeta = 0;
    int parDeEntrega = 0;
};

class Grafo {
    private: vector<pair<int,int>> *listaAdjacencia;
    private: vector<Vertice> atributosVertice;
    private: bool *ehPontoDeColeta;

    /* -------------------------------------------------------- */

    private: inline bool verificaSolucaoGerada (vector<int> sequenciaDeVertices, int capacidadeCaminhao) noexcept {
        int acumuladorCapacidade = 0;
        int horarioAtual = 0;
        int verticeAtual;
        int verticeAnterior = 0;

        bool *verticeExplorado = new bool[atributosVertice.size()];

        for (auto i=0; i<(int)sequenciaDeVertices.size(); i++) {
            verticeAtual = sequenciaDeVertices[i];
            verticeExplorado[verticeAtual] = true;

            if (verticeAtual == 0) {
                if (verticeAnterior == 0) {
                    continue;
                }
                else {
                    horarioAtual += listaAdjacencia[verticeAnterior][0].second;
                    return horarioAtual <= atributosVertice[0].janelaFinal && acumuladorCapacidade == 0;
                }
            }
            else {
                if (atributosVertice[verticeAtual].demanda > 0) {
                    if (find(sequenciaDeVertices.begin(), sequenciaDeVertices.end(),atributosVertice[verticeAtual].parDeEntrega) == sequenciaDeVertices.end()) {
                        /* Entrega nao feita pelo caminhao atual */
                        return false;
                    }

                    if (horarioAtual < atributosVertice[verticeAtual].janelaInicial) {
                        horarioAtual = atributosVertice[verticeAtual].janelaInicial + atributosVertice[verticeAtual].duracao + listaAdjacencia[verticeAnterior][verticeAtual].second;
                        acumuladorCapacidade += atributosVertice[verticeAtual].demanda;
                    }
                    else if (horarioAtual > atributosVertice[verticeAtual].janelaInicial && horarioAtual <= atributosVertice[verticeAtual].janelaFinal) {
                        horarioAtual = atributosVertice[verticeAtual].janelaInicial + atributosVertice[verticeAtual].duracao + listaAdjacencia[verticeAnterior][verticeAtual].second;
                        acumuladorCapacidade += atributosVertice[verticeAtual].demanda;
                    }
                    else {
                        return false;
                    }
                    verticeAnterior = verticeAtual;
                }
                else {
                    if (!verticeExplorado[atributosVertice[verticeAtual].parDeColeta]) {
                        return false;
                    }
                    else {
                        if (horarioAtual < atributosVertice[verticeAtual].janelaInicial) {
                            horarioAtual = atributosVertice[verticeAtual].janelaInicial + atributosVertice[verticeAtual].duracao + listaAdjacencia[verticeAnterior][verticeAtual].second;
                            acumuladorCapacidade -= atributosVertice[verticeAtual].demanda;
                        }
                        else if (horarioAtual > atributosVertice[verticeAtual].janelaInicial && horarioAtual <= atributosVertice[verticeAtual].janelaFinal) {
                            horarioAtual = atributosVertice[verticeAtual].janelaInicial + atributosVertice[verticeAtual].duracao + listaAdjacencia[verticeAnterior][verticeAtual].second;
                            acumuladorCapacidade -= atributosVertice[verticeAtual].demanda;
                        }
                        else {
                            return false;
                        }
                        verticeAnterior = verticeAtual;
                    }
                }
            }
        }
        return true;
        delete[] verticeExplorado;
    }

    /* -------------------------------------------------------- */

    public: inline bool funcaoGeraSolucaoAleatoria (int capacidadeCaminhao) noexcept {
        int quantidadeVertice = listaAdjacencia->size();
        int quantidadeCaminhoes = rand() % (quantidadeVertice/4) ;
        int numeroVerticesPorCaminhao = quantidadeVertice/quantidadeCaminhoes;
        int restante = quantidadeVertice % quantidadeCaminhoes;
        int aux;
        vector<int> verticesJaSelecionados;
        vector<int> *sequenciaVerticesPorCaminhao = new vector<int>[quantidadeCaminhoes];

        verticesJaSelecionados.push_back(0);

        for (auto i=0; i<quantidadeCaminhoes; i++) {
            sequenciaVerticesPorCaminhao[i].push_back(0);

            for (auto j=0; j<numeroVerticesPorCaminhao; j++) {
                aux = rand() % quantidadeVertice;

                if (find(verticesJaSelecionados.begin(),verticesJaSelecionados.end(), aux) != verticesJaSelecionados.end()) {
                    j--;
                    continue;
                }
                sequenciaVerticesPorCaminhao[i].push_back(aux);
            }
            sequenciaVerticesPorCaminhao[i].push_back(0);
        }

        if (restante != 0) {
            sequenciaVerticesPorCaminhao[0].pop_back();
            
            for (auto i=0; i<restante; i++) {
                aux = rand() % quantidadeVertice;

                if (find(verticesJaSelecionados.begin(),verticesJaSelecionados.end(), aux) != verticesJaSelecionados.end()) {
                    i--;
                    continue;
                }
                sequenciaVerticesPorCaminhao[0].push_back(aux);
            }
            sequenciaVerticesPorCaminhao[0].push_back(0);
        }

        cout << "Solucao aleatoria gerada:\n";
        cout << "Quantidade de caminhoes: " << quantidadeCaminhoes << "\n";

        for (auto i=0; i<quantidadeCaminhoes; i++) {
            cout << "Caminhao " << i << ": ";
            for (auto j=0; j<(int)sequenciaVerticesPorCaminhao[i].size(); j++) {
                cout << sequenciaVerticesPorCaminhao[i][j] << "->";
            }
            cout << "\n";
        }

        for (auto i=0; i<quantidadeCaminhoes; i++) {
            if (verificaSolucaoGerada(sequenciaVerticesPorCaminhao[i], capacidadeCaminhao)) {
                continue;
            }
            else {
                return false;
            }
        }
        return true;
        
    }

    /* -------------------------------------------------------- */

    public: void adicionaVertice(Vertice umVertice) {
        atributosVertice.push_back(umVertice);
    }

    /* -------------------------------------------------------- */

    public: void adicionaAresta (int origem, pair<int,int> umaAresta) {
        listaAdjacencia[origem].push_back(umaAresta);
    }

    /* -------------------------------------------------------- */

    public: Grafo (int numVertice) {
        listaAdjacencia = new vector<pair<int,int>>[numVertice];
        ehPontoDeColeta = new bool[numVertice];
    }

    /* -------------------------------------------------------- */

    public: ~Grafo () {
        delete[] ehPontoDeColeta;
        delete[] listaAdjacencia;
    }

};

class Empresa {
    private: const string listaInstancias[30] = {
        "instancias/bar-n100-1.txt", "instancias/bar-n100-2.txt",
        "instancias/ber-n100-3.txt", "instancias/ber-n100-4.txt",
        "instancias/nyc-n100-4.txt", "instancias/nyc-n100-5.txt",
        "instancias/poa-n100-1.txt", "instancias/poa-n100-2.txt",
        "instancias/poa-n100-6.txt", "instancias/poa-n100-7.txt",
        "instancias/bar-n200-1.txt", "instancias/bar-n200-2.txt",
        "instancias/bar-n200-3.txt", "instancias/bar-n200-4.txt",
        "instancias/ber-n200-5.txt", "instancias/ber-n200-6.txt",
        "instancias/nyc-n200-3.txt", "instancias/nyc-n200-4.txt",
        "instancias/poa-n200-1.txt", "instancias/poa-n200-2.txt",
        "instancias/bar-n400-1.txt", "instancias/ber-n600-1.txt",
        "instancias/nyc-n800-3.txt", "instancias/poa-n1000-1.txt",
        "instancias/poa-n1500-6.txt", "instancias/nyc-n2000-4.txt",
        "instancias/ber-n2500-3.txt", "instancias/bar-n3000-6.txt",
        "instancias/poa-n4000-2.txt", "instancias/poa-n5000-3.txt"
    };

    /* -------------------------------------------------------- */

    private: string NAME;
    private: string LOCATION;
    private: string COMMENT;
    private: string TYPE;
    private: int SIZE;
    private: string DISTRIBUTION;
    private: string DEPOT;
    private: int ROUTTIME;
    private: int TIMEWINDOW;
    private: int CAPACITY;

    /* -------------------------------------------------------- */

    public: inline void lerInstancias () {
        int auxEdge;
        string coletarInstancias;
        string chave = "", valor = "";
        ifstream instancias;
        Vertice auxVertice;

        for (auto i=0; i<30; i++) {
            cout << "Abrindo instanciancia...\n";
            instancias.open(listaInstancias[i]);

            if (instancias) {
                /* Ler o cabeçalho */
                for (auto j=0; j<10; j++) {
                    getline (instancias,coletarInstancias);
                    int k = 0;
                    char aux = '0';

                    while (aux != ':') {
                        aux = coletarInstancias[k];
                        chave += aux;
                        k++;
                    }
                    k++;
                    for (;k<int(coletarInstancias.size()); k++) {
                        valor += coletarInstancias[k];
                    }

                    if (chave == "NAME:") {
                        this->NAME = valor;
                    }
                    else if (chave == "LOCATION:") {
                        this->LOCATION = valor;
                    }
                    else if (chave == "COMMENT:") {
                        this->COMMENT = valor;
                    }
                    else if (chave == "TYPE:") {
                        this->TYPE = valor;
                    }
                    else if (chave == "SIZE:") {
                        this->SIZE = stoi(valor);
                    }
                    else if (chave == "DISTRIBUTION:") {
                        this->DISTRIBUTION = valor;
                    }
                    else if (chave == "DEPOT:") {
                        this->DEPOT = valor;
                    }
                    else if (chave == "ROUTE-TIME:") {
                        this->ROUTTIME = stoi(valor);
                    }
                    else if (chave == "TIME-WINDOW:") {
                        this->TIMEWINDOW = stoi(valor);
                    }
                    else if (chave == "CAPACITY:") {
                        this->CAPACITY = stoi(valor);
                    }
                    chave = valor = "";
                }
                instancias >> coletarInstancias; // Descartando a palavra NODES

                Grafo roteirizacao (this->SIZE);

                /* Lendo todas as informacoes dos vertices */
                for (auto k=0; k<this->SIZE; k++) {
                    instancias >> auxVertice.identificador;
                    instancias >> auxVertice.latitude;
                    instancias >> auxVertice.longitude;
                    instancias >> auxVertice.demanda;
                    instancias >> auxVertice.janelaInicial;
                    instancias >> auxVertice.janelaFinal;
                    instancias >> auxVertice.duracao;
                    instancias >> auxVertice.parDeColeta;
                    instancias >> auxVertice.parDeEntrega;

                    roteirizacao.adicionaVertice(auxVertice);
                }

                instancias >> coletarInstancias; // Descartando a palavra EDGES
                
                /* Montando a lista de adjacencias */
                for (auto f=0; f<this->SIZE; f++) {
                    for (auto x=0; x<this->SIZE; x++) {
                        instancias >> auxEdge;
                        if (f == x) {
                            continue;
                        }
                        roteirizacao.adicionaAresta(f,{x,auxEdge});
                    }
                }

                instancias.close();

                roteirizacao.funcaoGeraSolucaoAleatoria(this->CAPACITY) ? cout << "Solucao viavel\n" : cout << "Solucao inviavel\n";
            } 
            else {
                throw "ERRO DE ABERTURA\n";
            }
            
            char temp;
            cout << "Pressione qualquer letra para abrir nova instancia\n";
            cin >> temp;
        }
    }

    /* -------------------------------------------------------- */

    public: Empresa () {
        NAME = "";
        LOCATION = "";
        COMMENT = "";
        TYPE = "";
        SIZE = 0;
        DISTRIBUTION = "";
        DEPOT = "";
        ROUTTIME = 0;
        TIMEWINDOW = 0;
        CAPACITY = 0;
    }
};


int main (int argc, char **argv) {
    /* Projetando rotas de entrega 
     * Empresa de logística
     */

    Empresa FedExExpress;
    FedExExpress.lerInstancias();

    return 0;
}