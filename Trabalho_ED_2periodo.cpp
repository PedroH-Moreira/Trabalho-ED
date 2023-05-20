#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <cstdlib>
using namespace std;

#ifdef _WIN32
    const std::string CLEAR = "cls"; // Comando para limpar tela no Windows
#else
    const std::string CLEAR = "clear"; // Comando para limpar tela no Linux e macOS
#endif

class Dados
{
    private:
        int mId;
        char mNome[80];
        char mCidade[70];
        char mEsporte[70];
        char mEvento[70];
        char mNoc[4];
    public:
        Dados();
        Dados(string id, string Nome, string Cidade, string Esporte, string Event, string Noc);
        void conversao();
        void exibir();
        //exibir entre o intervalo x e y
        void exibir_intervalo(int x, int y);
        void adicionar();
        void alterar_elemento(string novo);
        //trocar os elementos x e y de lugar
        void trocar_elementos(int x, int y);
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
    //remove ("base.bin");
}

void Dados::conversao()
{
    //começar a leitura do csv
    ifstream ler("baseTest.csv");
    string LINHA;
    getline(ler, LINHA, '\n');
    //pegar a primeira linha da base <Id,Name,City,Sport,Event,NOC>

    //enquanto o arquivo não chegar no fim, com o uso do getline e string string para a conversão
    //os dados são passados sempre no final do arquivo binário
    int cont = 0, total = 271117;
    while(!ler.eof())
    {
        string idString, nome, cidade, esporte, evento, noc;
        getline(ler, idString, ',');
        getline(ler, nome, ',');
        if (nome.front() == '"' and nome.back() != '"') 
        {
            string nomeTemp;
            while(nome.back() != '"')
            {
                getline(ler, nomeTemp, ',');
                nome+= ','+ nomeTemp;
            }
        }

        //"Joseph ""Joe"" Angyal, Jr.",
        //"Tenley Emma Albright (-Gardiner, -Blakely)",
        
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

        //exibindo um contador do progresso na tela
        int porcentagem = (cont + 1) * 100 / total;
        cont++;
        if(cont % 5000 == 0)
        {
            cout << "Convertendo o arquivo para binario..\nProgresso = " << porcentagem << "%";
        } else if(cont % 4999 == 0)
                {
                    system(CLEAR.c_str());
                }
    }

    ler.close();
    system(CLEAR.c_str());
    cout << "Arquivo CSV convertido para binario com sucesso\n";
}

void Dados::exibir_intervalo(int x, int y){}

void Dados::alterar_elemento(string novo){}

void Dados::trocar_elementos(int x, int y)
{

}

void Dados::adicionar(){}

void apresentacao()
{
    cout << " - - - - - - - - - TRABALHO DE ED - - - - - - - - - \n";
    cout << "Segundo periodo --- Universidade Federal de Lavras \n";
    cout << "Participantes: Esther Silva de Magalhaes, \n";
    cout << "Italo Alves Rabelo, \n";
    cout << "Pedro Henrique Cabral Moreira. \n";
    cout << "Professor: Renato Ramos da Silva \n";
    cout << "Base de dados: \"data_athlete_event\"\n";
    cout << "Digite enter para comecar \n";
    char c; cin.get(c);
    system(CLEAR.c_str());
}


int main()
{
    if("base.bin")
        remove("base.bin");

    apresentacao();
    Dados d;
    d.conversao();
    char key = 'l';

    while(key != 's')
    {
        cout << "Escolha a funcao desejada \n";
        cout << "e - Exibir todo o arquivo.\n";
        cout << "i - Exibir um intervalo especifico do arquivo.\n";
        cout << "a - Alterar elemento.\n";
        cout << "t - Trocar elementos.\n";
        cout << "d - Adicionar elemento.\n";
        cout << "s - Fechar o programa.\n";
        cin >> key;
        string novoNome;
        int x, y;
        switch(key)
        {
            case 'e':
                d.exibir();
                break;
            case 'i':
                cout << "Digite o intervalo desejado - Exemplo: de x ate y\n";
                cout << "x = "; cin >> x;
                cout << "y = "; cin >> y;
                cout << endl;
                cin.ignore();
                d.exibir_intervalo(x,y);
                break;
            case 'a':
                cin.ignore();
                cout << "Digite a posicao que deseja alterar o nome.\n";
                cin >> x;

                cin.ignore();
                cout << "Digite o novo nome que deseja atribuir a posicao " << x << endl;
                getline(cin,novoNome);
                d.alterar_elemento(novoNome);
                break;
            case 't':
                cin.ignore();
                cin >> x;
                cin >> y;
                d.trocar_elementos(x,y);
                break;
            case 'd':
                cin.ignore();
                d.adicionar();
            default:

                break;
        }
        system(CLEAR.c_str());
    }
    cout << "\nPrograma encerrado.";
    return 0;
}
