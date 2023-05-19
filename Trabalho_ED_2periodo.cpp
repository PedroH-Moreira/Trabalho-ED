#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdexcept>
using namespace std;

class Dados
{
    private:
        int mId;
        char mNome[70];
        char mCidade[70];
        char mEsporte[70];
        char mEvento[70];
        char mNoc[4];
    public:
        Dados();
        Dados(string id, string Nome, string Cidade, string Esporte, string Event, string Noc);
        void conversao();
        void exibir();
};

Dados::Dados()
{
    this->mId = -1;
    strcpy(this->mNome, " ");
    strcpy(this->mCidade, " ");
    strcpy(this->mEsporte, " ");
    strcpy(this->mEvento, " ");
    strcpy(this->mNoc, " ");
}

Dados::Dados(string id, string Nome, string Cidade, string Esporte, string Event, string Noc)
{
    this->mId = stoi(id);
    strcpy(this->mNome, Nome.c_str());
    strcpy(this->mCidade, Cidade.c_str());
    strcpy(this->mEsporte, Esporte.c_str());
    strcpy(this->mEvento, Event.c_str());
    strcpy(this->mNoc, Noc.c_str());
}

void Dados::exibir()
{
    ifstream fin("base.bin", ios_base::in | ios_base:: binary);
    Dados d;

    while (fin.read((char *)&d, sizeof(Dados)))
    {
        cout << "ID: " << d.mId << " Nome: " << d.mNome << " Cidade: " << d.mCidade;
        cout << " Esporte: " << d.mEsporte << " Evento: " << d.mEvento << " NOC: " << d.mNoc << endl;
    }
    fin.close();
    remove ("base.bin");
}

void Dados::conversao()
{
    //começar a leitura do csv
    ifstream ler("base15K.csv");
    string LINHA;
    getline(ler, LINHA, '\n');

    //pegar a primeira linha da base <Id,Name,City,Sport,Event,NOC>
    ofstream escrita("base.bin", ios::app | ios::binary);
    char linha[29];
    strcpy(linha, LINHA.c_str());
    escrita.close();

    //enquanto o arquivo não chegar no fim, com o uso do getline e string string para a conversão
    //os dados são passados sempre no final do arquivo binário
    while(!ler.eof())
    {
        string idString, nome, cidade, esporte, evento, noc;
        getline(ler, idString, ',');
        getline(ler, nome, ',');

        while (nome.front() == '"' and nome.back() != '"') 
        {
            // O campo mEvento contém uma vírgula dentro de aspas
            string nomeTemp;
            while (nomeTemp.back() != '"') 
            {
                getline(ler, nomeTemp, ',');
                nome += ',' + nomeTemp;
            }
        }
        
        getline(ler, cidade, ',');
        getline(ler, esporte, ',');
        //observar o problema em <evento>
        getline(ler, evento, ',');
        //caso tenha virgula no meio das aspas
        if (evento.front() == '"' and evento.back() != '"') 
        {
            // O campo mEvento contém uma vírgula dentro de aspas
            string eventoTemp;
            while (eventoTemp.back() != '"') 
            {
                getline(ler, eventoTemp, ',');
                evento += ',' + eventoTemp;
            }
        }
        ler >> noc;

        ler.ignore();

        //usando o construtor com parâmetro para associar aos dados da classe
        Dados d(idString, nome, cidade, esporte, evento, noc);

        ofstream escrita("base.bin" , ios::app | ios::binary);
        escrita.write((char*)&d, sizeof(Dados));
        escrita.close();
    }

    ler.close();

}



int main()
{
    Dados dados;
    dados.conversao();
    dados.exibir();

    return 0;
}
