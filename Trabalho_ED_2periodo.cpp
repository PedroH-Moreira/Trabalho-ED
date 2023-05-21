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
        char mEvento[80];
        char mNoc[4];
    public:
        Dados();
        Dados(string id, string Nome, string Cidade, string Esporte, string Event, string Noc);
        void conversao();
        void exibir();
        //exibir entre o intervalo x e y
        void exibir_intervalo(int x, int y);
        void adicionar();
        void alterar_dado();
        //trocar os elementos x e y de lugar
        void trocar_elementos();
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
	try {
        this->mId = stoi(id);
    } catch (const exception& e) {
        this->mId = -1; // Valor padrão para mId em caso de conversão inválida
    }
    strcpy(this->mNome, Nome.c_str());
    strcpy(this->mCidade, Cidade.c_str());
    strcpy(this->mEsporte, Esporte.c_str());
    strcpy(this->mEvento, Event.c_str());
    strcpy(this->mNoc, Noc.c_str());
}

void Dados::exibir()
{
    ifstream fin;
    fin.open("base.bin" , ios_base::in | ios_base::binary);
    Dados d;

    while ((fin.read((char *)&d, sizeof(Dados))))
    {
        cout << "ID: " << d.mId << " Nome: " << d.mNome << " Cidade: " << d.mCidade;
        cout << " Esporte: " << d.mEsporte << " Evento: " << d.mEvento << " NOC: " << d.mNoc << endl;
    }
    fin.close();
    //remove ("base.bin");
    //cout << "Digite enter para continuar\n";
    //char c; cin.get(c);
    //system(CLEAR.c_str());
}

void Dados::conversao()
{
    //começar a leitura do csv
    ifstream ler("data_athlete_event.csv");
    string LINHA;
    getline(ler, LINHA, '\n');
    //pegar a primeira linha da base <Id,Name,City,Sport,Event,NOC>

    //enquanto o arquivo não chegar no fim, com o uso do getline e string string para a conversão
    //os dados são passados sempre no final do arquivo binário
    int cont = 0, total = 271117;
    ofstream escrita("base.bin" , ios::binary);
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

        //"Joseph ""Joe"" Angyal, Jr.", -> nesse caso lê até a vírgula e entra no if de cima
        //"Tenley Emma Albright (-Gardiner, -Blakely)", -> e nesse caso também, pq o getline(ler, nome,',') não importa se é ou não uma aspas
        
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
            cout << "Convertendo o arquivo para binario...\nProgresso = " << porcentagem << "%";
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
		file.seekg(0, ios::end);
		streampos fileSize = file.tellg();
		size_t classSize = sizeof(Dados);
		size_t numRecords = (fileSize / classSize) - 1; // -1 UTILIZADO PARA DESCONSIDERAR A PRIMEIRA LINHA DO ARQUIVO
		
		if (x > (int)numRecords and y > (int)numRecords and x > y and x < 0 and y < 0)
		{
			cout << "Intervalo invalido." << endl;
			file.close();
		}
		else{
			
			Dados d;
			int posicao_inicial = x, posicao_final = x;
			
			file.seekg(posicao_inicial * sizeof(Dados));
			
			while (file.read((char *)&d, sizeof(Dados)) and posicao_final <= y and posicao_final <= int(numRecords))
			{
				cout << "ID: " << d.mId << " Nome: " << d.mNome << " Cidade: " << d.mCidade;
				cout << " Esporte: " << d.mEsporte << " Evento: " << d.mEvento << " NOC: " << d.mNoc << endl;
				++posicao_final;
			}
			cout << endl;
		}

		file.close();
	}
}

void Dados::alterar_dado()
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
		
		
		// GET UMA POSIÇÃO VÁLIDA PARA ALTERAÇÃO
		int posicaoAlterar;
		cout << "Digite a posicao a se alterar (0 - " << numRecords << ")\n> ";
		cin >> posicaoAlterar;
		while (posicaoAlterar < 0 or posicaoAlterar > int(numRecords))
		{
			cout << "Digite uma posicao valida!\n> ";
			cin >> posicaoAlterar;
		}
		cin.ignore();
		cin.clear();
		
		
		// CRIA UMA VARIÁVEL PARA RECEBER O REGISTRO A SER ALTERADO
		Dados d;
		
		
		// POSICIONA O PONTEIRO DE LEITURA PARA A POSIÇÃO DESEJADA PARA ALTERAÇAO
		file.seekg(posicaoAlterar * sizeof(Dados));
		file.read((char*)&d, sizeof(Dados));
		
		
		// SWITCH CASE PARA O USER DECIDIR QUAL ATRIBUTO DO REGISTRO IRÁ ALTERAR
		int opt;
		do
		{
			cout << "\n(1) Alterar Id\n(2) Alterar nome\n(3) Alterar cidade\n(4) Alterar esporte\n(5) Alterar evento\n(6) Alterar noc\n(0) Sair\n> ";
			cin >> opt;
			cin.ignore();
			cin.clear();
			
			switch (opt)
			{
				case 1:
					cout << "Entre com o novo Id : ";
					cin >> d.mId;
					cin.ignore();
					cin.clear();
					break;
					
				case 2:
					cout << "Entre com o novo nome : ";
					cin.getline(d.mNome, 80);
					cin.clear();
					break;
					
				case 3:
					cout << "Entre com a nova cidade : ";
					cin.getline(d.mCidade, 70);
					cin.clear();
					break;
					
				case 4:
					cout << "Entre com o novo esporte : ";
					cin.getline(d.mEsporte, 70);
					cin.clear();
					break;
					
				case 5:
					cout << "Entre com o novo evento : ";
					cin.getline(d.mEvento, 70);
					cin.clear();
					break;
					
				case 6:
					cout << "Entre com a nova sigla do pais : ";
					cin.getline(d.mNoc, 4);
					cin.clear();
					
				case 0:
					break;
				
				default:
					cout << "Digite um comando valido. \n";
					break;
			}
		} while (opt != 0);
		
		
		// ESCREVE O REGISTRO COM OS DADOS ALTERADOS NA POSIÇÃO DESEJADA
		file.seekp(posicaoAlterar * sizeof(Dados));
		file.write((char*)&d, sizeof(Dados));
		
		
		file.close();
	}
}

void Dados::trocar_elementos()
{
    fstream file("base.bin", ios_base::binary | ios_base::ate | ios_base:: in | ios_base::out);
    int posx, posy;
    cout << "Digite as posicoes em que deseja fazer a troca\n";
    cout << "x > ";
    cin >> posx; cout << endl;
    cout << "y > ";
    cin >> posy; cout << endl;
    cin.ignore();

    Dados aux1;
    Dados aux2;

    file.seekg(posx * sizeof(Dados));
    file.read((char*)&aux1, sizeof(Dados));

    file.seekg(posy * sizeof(Dados));
    file.read((char*)&aux2, sizeof(Dados));

    file.seekp(posx * sizeof(Dados));
    file.write((char*)&aux2, sizeof(Dados));

    file.seekp(posy * sizeof(Dados));
    file.write((char*)&aux1, sizeof(Dados));
    file.close();
    cout << "Troca feita com sucesso!\n";
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
		cout << "Digite a posicao a inserir um novo registro (0 - " << numRecords << ") : ";
		cin >> posicaoAInserir;
		while (posicaoAInserir < 0 or posicaoAInserir > int(numRecords))
		{
			cout << "Digite uma posicao valida!\n> ";
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
		
		cout << "Digite a sigla do pais do atleta: ";
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
	cout << "\n\n==================== MENU =====================\n\n";
        cout << "Escolha a funcao desejada: \n";
        cout << "e - Exibir todo o arquivo.\n";
        cout << "i - Exibir um intervalo especifico do arquivo.\n";
        cout << "a - Alterar elemento.\n";
        cout << "t - Trocar elementos.\n";
        cout << "d - Adicionar elemento.\n";
        cout << "s - Fechar o programa.\n";
        cin >> key;
        string novoNome;
        int x, y;
        system(CLEAR.c_str());
        switch(key)
        {
            case 'e':
                d.exibir();
                break;
            case 'i':
                cout << "Digite o intervalo desejado:\n\n" <<
                        "{" << 
                        " Exemplo: intervalo de 5 a 15\n" <<
                        "  x = 5\n" <<
                        "  y = 15\n" <<
                        "}\n\n" <<
                        "Insira abaixo:\n";
                cout << "x = "; cin >> x;
                cout << "y = "; cin >> y;
                cout << "\n\n";
                cin.ignore();
                d.exibir_intervalo(x,y);
                break;
            case 'a':
                d.alterar_dado();
                break;
            case 't':
                cin.ignore();
                d.trocar_elementos();
                break;
            case 'd':
                cin.ignore();
                d.adicionar();
                break;
            case 's':
                break;
            default:
		cout << "Digite um comando valido!\n> ";
                break;
        }
    }

    cout << "\nPrograma encerrado.";
    return 0;
}
