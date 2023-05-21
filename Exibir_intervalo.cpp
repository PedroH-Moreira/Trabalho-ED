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
    ofstream escrita("base.bin" , ios::app | ios::binary);
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
        getline(ler, noc);

        //usando o construtor com parâmetro para associar aos dados da classe
        Dados d(idString, nome, cidade, esporte, evento, noc);

        escrita.write((char*)&d, sizeof(Dados));

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
    escrita.close();
    ler.close();
    system(CLEAR.c_str());
    cout << "Arquivo CSV convertido para binario com sucesso\n";
}

void Dados::exibir_intervalo(int x, int y)
{
	ifstream file ("base.bin", ios_base::in | ios_base:: binary);
	
	if (!file.is_open())
	{
		cout << "Falha ao abrir o arquivo. " << endl;
	}
	else
	{
		// OPERAÇÕES PARA CALCULAR A QUANTIDADE DE REGISTROS CONTIDOS NO ARQUIVO
		streampos fileSize = file.tellg();
		size_t classSize = sizeof(Dados);
		size_t numRecords = (fileSize / classSize) - 1; // -1 UTILIZADO PARA DESCONSIDERAR A PRIMEIRA LINHA DO ARQUIVO
		Dados d;
		int posicao_inicial = x, posicao_final = x, cont_linhas = 0;
		
		file.seekg(posicao_inicial * sizeof(Dados));
		
		while (file.read((char *)&d, sizeof(Dados)) and posicao_final <= y and cont_linhas < int(numRecords))
		{
			cout << "ID: " << d.mId << " Nome: " << d.mNome << " Cidade: " << d.mCidade;
			cout << " Esporte: " << d.mEsporte << " Evento: " << d.mEvento << " NOC: " << d.mNoc << endl;
			++cont_linhas;
		}
	}

    file.close();
}


void Dados::alterar_elemento(string novo){}

void Dados::trocar_elementos(int x, int y)
{

}

void Dados::adicionar()
{
	fstream file ("base.bin", ios::binary | ios::in | ios::out | ios::ate);
	if (!file.is_open())
	{
		cout << "Falha ao abrir o arquivo. " << endl;
	}
	else
	{
		// OPERAÇÕES PARA CALCULAR A QUANTIDADE DE REGISTROS CONTIDOS NO ARQUIVO
		streampos fileSize = file.tellg();
		size_t classSize = sizeof(Dados);
		size_t numRecords = (fileSize / classSize) - 1; // -1 UTILIZADO PARA DESCONSIDERAR A PRIMEIRA LINHA DO ARQUIVO
		
		
		// GET UMA POSIÇÃO VÁLIDA PARA INSERÇÃO
		int posicaoAInserir;
		cout << "Digite a posição a inserir um novo registro (0 - " << numRecords << ") : ";
		cin >> posicaoAInserir;
		while (posicaoAInserir < 0 or posicaoAInserir > int(numRecords))
		{
			cout << "Digite uma posição válida!\n> ";
			cin >> posicaoAInserir;
		}
		
		
		// CRIA O REGISTRO E RECEBE OS DADOS
		Dados d;
		
		cout << "Digite o Id do atleta: ";
		cin >> d.mId;
		cin.ignore();
		cin.clear();
		
		cout << "Digite o nome do atleta: ";
		cin.getline(d.mNome, 80);
		cin.clear();
		
		cout << "Digite a cidade do atleta: ";
		cin.getline(d.mCidade, 70);
		cin.clear();
		
		cout << "Digite o esporte praticado pelo atleta: ";
		cin.getline(d.mEsporte, 70);
		cin.clear();
		
		cout << "Digite o evento que o atleta participa: ";
		cin.getline(d.mEvento, 70);
		cin.clear();
		
		cout << "Digite a sigla do país do atleta: ";
		cin.getline(d.mNoc, 4);
		cin.clear();
		
		// CRIA UMA AUXILIAR PARA RECEBER OS REGISTROS UM A UM
		Dados aux;
		
		// POSICIONA O PONTEIRO DE LEITURA E DE ESCRITA AO FINAL DO ARQUIVO - REALIZA A LEITURA - ESCREVE O REGISTRO UMA POSIÇÃO A FRENTE ATÉ QUE SE ATINJA A POSIÇÃO DE INSERÇÃO
		for (int i = numRecords; i >= posicaoAInserir; i--)
		{
			file.seekg(i*sizeof(Dados));
			file.read((char*)&aux, sizeof(Dados));
			file.seekp((i+1)*sizeof(Dados));
			file.write((char*)&aux, sizeof(Dados));
		}
		
		file.seekp(posicaoAInserir*sizeof(Dados));
		file.write((char*)&d, sizeof(Dados));
		
		file.close();
		
	}
}

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
