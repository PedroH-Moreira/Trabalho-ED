#include <iostream>
#include <fstream>
#include <cstdio> // Incluir cabeçalho para std::remove

using namespace std;

// Função para mesclar dois arrays classificados
void mesclar(int* array, int inicio, int meio, int fim) {
    int tamanhoEsquerda = meio - inicio + 1;
    int tamanhoDireita = fim - meio;

    int* arrayEsquerda = new int[tamanhoEsquerda];
    int* arrayDireita = new int[tamanhoDireita];

    for (int i = 0; i < tamanhoEsquerda; i++) {
        arrayEsquerda[i] = array[inicio + i];
    }

    for (int j = 0; j < tamanhoDireita; j++) {
        arrayDireita[j] = array[meio + 1 + j];
    }

    int indiceEsquerda = 0;
    int indiceDireita = 0;
    int indiceMerged = inicio;

    while (indiceEsquerda < tamanhoEsquerda && indiceDireita < tamanhoDireita) {
        if (arrayEsquerda[indiceEsquerda] <= arrayDireita[indiceDireita]) {
            array[indiceMerged] = arrayEsquerda[indiceEsquerda];
            indiceEsquerda++;
        } else {
            array[indiceMerged] = arrayDireita[indiceDireita];
            indiceDireita++;
        }
        indiceMerged++;
    }

    while (indiceEsquerda < tamanhoEsquerda) {
        array[indiceMerged] = arrayEsquerda[indiceEsquerda];
        indiceEsquerda++;
        indiceMerged++;
    }

    while (indiceDireita < tamanhoDireita) {
        array[indiceMerged] = arrayDireita[indiceDireita];
        indiceDireita++;
        indiceMerged++;
    }

    delete[] arrayEsquerda;
    delete[] arrayDireita;
}

// Função para realizar o Merge Sort
void mergeSort(int* array, int inicio, int fim) {
    if (inicio < fim) {
        int meio = inicio + (fim - inicio) / 2;

        mergeSort(array, inicio, meio);
        mergeSort(array, meio + 1, fim);

        mesclar(array, inicio, meio, fim);
    }
}

// Função para criar um bloco de dados classificados usando o Merge Sort
void criarBloco(int* bloco, const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    int valor;
    int indice = 0;

    while (arquivo >> valor) {
        bloco[indice] = valor;
        indice++;
    }

    mergeSort(bloco, 0, indice - 1);

    arquivo.close();
}

// Função para mesclar dois blocos classificados
void mesclarBlocos(const string& arquivo1, const string& arquivo2, const string& arquivoMesclado) {
    ifstream bloco1(arquivo1);
    ifstream bloco2(arquivo2);
    ofstream arquivoSaida(arquivoMesclado);

    int valor1, valor2;

    // Ler o primeiro valor de cada bloco
    bloco1 >> valor1;
    bloco2 >> valor2;

    while (!bloco1.eof() && !bloco2.eof()) {
        if (valor1 < valor2) {
            arquivoSaida << valor1 << "\n";
            bloco1 >> valor1;
        } else {
            arquivoSaida << valor2 << "\n";
            bloco2 >> valor2;
        }
    }

    // Copiar o restante dos valores do bloco 1, se houver
    while (!bloco1.eof()) {
        arquivoSaida << valor1 << "\n";
        bloco1 >> valor1;
    }

    // Copiar o restante dos valores do bloco 2, se houver
    while (!bloco2.eof()) {
        arquivoSaida << valor2 << "\n";
        bloco2 >> valor2;
    }

    bloco1.close();
    bloco2.close();
    arquivoSaida.close();
}

// Função para realizar a ordenação externa
void ordenacaoExterna(const string& arquivoEntrada, const string& arquivoSaida, int tamanhoBloco) {
    ifstream arquivo(arquivoEntrada);
    int* bloco = new int[tamanhoBloco];
    int contador = 0;

    while (!arquivo.eof()) {
        // Criar um novo bloco
        int indice = 0;
        while (indice < tamanhoBloco && !arquivo.eof()) {
            int valor;
            arquivo >> valor;
            bloco[indice] = valor;
            indice++;
        }

        mergeSort(bloco, 0, indice - 1);

        // Salvar o bloco em um arquivo temporário
        ofstream arquivoBloco("bloco_" + to_string(contador) + ".txt");
        for (int i = 0; i < indice; i++) {
            arquivoBloco << bloco[i] << "\n";
        }
        arquivoBloco.close();

        contador++;
    }

    arquivo.close();

    // Mesclar os blocos até que haja apenas um bloco restante
    while (contador > 1) {
        int i = 0;
        int j = contador - 1;

        while (i < j) {
            string arquivo1 = "bloco_" + to_string(i) + ".txt";
            string arquivo2 = "bloco_" + to_string(j) + ".txt";
            string arquivoMesclado = "bloco_" + to_string(i + 1) + ".txt";

            mesclarBlocos(arquivo1, arquivo2, arquivoMesclado);

            remove(arquivo1.c_str());
            remove(arquivo2.c_str());

            i += 2;
            j -= 2;
        }

        contador = (contador + 1) / 2;
    }

    // Renomear o último bloco restante para o arquivo de saída
    rename(("bloco_" + to_string(contador - 1) + ".txt").c_str(), arquivoSaida.c_str());

    delete[] bloco;
}

int main() {
    string arquivoEntrada = "dados.txt";
    string arquivoSaida = "dados_ordenados.txt";
    int tamanhoBloco = 10000; // Tamanho do bloco (pode ser ajustado)

    ordenacaoExterna(arquivoEntrada, arquivoSaida, tamanhoBloco);

    cout << "Ordenação externa concluída.\n";

    return 0;
}
