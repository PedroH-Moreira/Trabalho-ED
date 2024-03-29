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
        void divideArquivo(int &numeroArquivos);
        void intercala(Dados *vetor, int inicio, int meio, int fim);
        void mergeSort(Dados *vetor, int inicio, int fim);
        void ordenaArquivos(int numeroArquivos, int inicio, int fim);
        void mesclaArquivos(int numeroArquivos);
        void mergeSortExterno();
        void limpaDados();
        void exibirOrdenado();
        void ordenadoParaCsv();
};

//tipo específico para comparar corretamente as strings, visto que os eventos podem começar com "

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
		
		if (x > (int)numRecords or y > (int)numRecords or x > y or x < 0 or y < 0)
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

void Dados::ordenadoParaCsv()
{
    ifstream fin;
    fin.open("data_athlete_event_final.bin" , ios_base::in | ios_base::binary);
    if(!fin.is_open())
    {
        throw runtime_error("Arquivo data_athlete_event_final.bin não encontrado!\n");
    }
    Dados d;

    ofstream saida("arquivoOrdenado.csv");
    
    while ((fin.read((char *)&d, sizeof(Dados))))
    {
        saida << d.mEvento << "," << d.mNome << "," << d.mCidade << ",";
        saida << d.mEsporte << "," << d.mId << "," << d.mNoc << endl;
    }
    fin.close();
    saida.close();
}

void Dados::divideArquivo(int &numeroArquivos) 
{
    //esse metodo vai dividir o arquivo data_athlete_event.bin em varios arquivos menores
    cout << "Dividindo o arquivo..." << endl;
    ifstream arquivo;
    arquivo.open("base.bin", ios::binary);
    if (arquivo.is_open()) 
    {
        //calcula o numero de arquivos menores
        arquivo.seekg(0, ios::end);
        int numeroDados = arquivo.tellg() / sizeof(Dados);
        arquivo.seekg(sizeof(Dados), ios::beg);
        numeroArquivos = (numeroDados / 50000) + 1;
        //cria os arquivos
        for (int i = 0; i < numeroArquivos; i++) 
        {
            ofstream arquivo;
            arquivo.open("data_athlete_event" + to_string(i) + ".bin", ios::binary);
            arquivo.close();
        }
        //le os dados do arquivo original e escreve nos arquivos menores
        for (int i = 0; i < numeroArquivos; i++) 
        {
            int j = 0;
            while(arquivo.good() and j < 50000) 
            {
                Dados dado;
                arquivo.read((char*)&dado, sizeof(Dados));
                ofstream arquivo;
                arquivo.open("data_athlete_event" + to_string(i) + ".bin", ios::binary | ios::app);
                if (arquivo.is_open()) 
                {
                    arquivo.write((char*)&dado, sizeof(Dados));
                    arquivo.close();
                } else {
                    cout << "Erro ao abrir o arquivo auxiliar: " << i << endl;
                }
                j++;
            }
        }
        arquivo.close();
    } else {
        cout << "Erro ao abrir o arquivo na divisao" << endl;
    }
    cout << "Arquivo dividido em " << numeroArquivos << " arquivos!" << endl;
}

void Dados::intercala(Dados *vetor, int inicio, int meio, int fim) {
    //esse metodo vai intercalar os dados de dois vetores, usando city como chave de ordenacao principal e ID como secundaria
    int i = inicio, j = meio + 1;
    int tamanho = fim - inicio + 1;
    Dados *vetorAuxiliar = new Dados[tamanho];
    for(int k = 0; k < tamanho; k++) {
        if ((i <= meio) and (j <= fim)) {
            string e1 = vetor[i].mEvento;
            string e2 = vetor[j].mEvento;
            string evento1 = (e1[0] == '"') ? e1.substr(1) : e1;
            string evento2 = (e2[0] == '"') ? e2.substr(1) : e2;
            //compara o evento dos dados
            if (evento1 < evento2) {
                //se o evento do vetor da esquerda for menor, pega o proximo do vetor da esquerda
                vetorAuxiliar[k] = vetor[i];
                i++;
            } else if (evento1 > evento2) {
                //se o evento do vetor da direita for menor, pega o proximo do vetor da direita
                vetorAuxiliar[k] = vetor[j];
                j++;
            } else {
                //se os nomes dos eventos forem iguais, compara os IDs
                if (vetor[i].mId < vetor[j].mId) {
                    //se o ID do vetor da esquerda for menor, pega o proximo do vetor da esquerda
                    vetorAuxiliar[k] = vetor[i];
                    i++;
                } else {
                    //se o ID do vetor da direita for menor, pega o proximo do vetor da direita
                    vetorAuxiliar[k] = vetor[j];
                    j++;
                }
            }
        } else if (i > meio) {
            //se o vetor da esquerda acabou, pega o proximo do vetor da direita
            vetorAuxiliar[k] = vetor[j];
            j++;
        } else {
            //se o vetor da direita acabou, pega o proximo do vetor da esquerda
            vetorAuxiliar[k] = vetor[i];
            i++;
        }
    }
    for (int k = 0; k < tamanho; k++) {
        //escreve os dados ordenados no vetor original
        vetor[inicio + k] = vetorAuxiliar[k];
    }
    delete[] vetorAuxiliar;
}

void Dados::mergeSort(Dados *vetor, int inicio, int fim) 
{
    //esse metodo vai ordenar os dados de um vetor
    if (inicio < fim) 
    {
        int meio = (inicio + fim) / 2;
        mergeSort(vetor, inicio, meio);
        mergeSort(vetor, meio + 1, fim);
        intercala(vetor, inicio, meio, fim);
    }
}

void Dados::limpaDados() 
{
    mId = '\0';
    for (int i = 0; i < 90; i++) {
        mNome[i] = '\0';
    }
    for (int i = 0; i < 50; i++) {
        mCidade[i] = '\0';
    }
    for (int i = 0; i < 50; i++) {
        mEsporte[i] = '\0';
    }
    for (int i = 0; i < 180; i++) {
        mEvento[i] = '\0';
    }
    for (int i = 0; i < 4; i++) {
        mNoc[i] = '\0';
    }
}

void Dados::ordenaArquivos(int numeroArquivos, int inicio, int fim) 
{
    //esse metodo vai ordenar os arquivos auxiliares
    Dados *vetor = new Dados[50000];
    for (int i = 0; i < numeroArquivos; i++) 
    {
        for (int j = 0; j < 50000; j++) 
        {
            vetor[j].limpaDados();
        }
        ifstream arquivo;
        arquivo.open("data_athlete_event" + to_string(i) + ".bin", ios::binary);
        if (arquivo.is_open()) 
        {
            for (int j = 0; j < 50000; j++) 
            {
                arquivo.read((char*)&vetor[j], sizeof(Dados));
            }
            //ordenar o vetor
            mergeSort(vetor, inicio, fim);
            arquivo.close();
            //abrir o arquivo para escrever
            ofstream arquivo;
            arquivo.open("data_athlete_event" + to_string(i) + ".bin", ios::binary);
            if (arquivo.is_open()) 
            {
                //escrever o vetor ordenado no arquivo
                arquivo.seekp(0, ios::beg);
                for (int j = 0; j < 50000; j++) 
                {
                    if (vetor[j].mEvento[0] != '\0') 
                    {
                        arquivo.write((char*)&vetor[j], sizeof(Dados));
                    }
                }
                arquivo.close();
                cout << "Arquivo " << i << " ordenado!" << endl;
            } else {
                cout << "Erro ao abrir o arquivo auxiliar: " << i << "para escrever na ordenacao" << endl;
            }
        } else {
            cout << "Erro ao abrir o arquivo auxiliar: " << i << "para ler na ordenacao" << endl;
        }
    }
}

void Dados::mesclaArquivos(int numeroArquivos) 
{
    //esse metodo vai mesclar os arquivos auxiliares em um arquivo final
    int indices[numeroArquivos];
    for (int i = 0; i < numeroArquivos; i++) 
    {
        indices[i] = 0;
    }
    //abrir os arquivos auxiliares
    ifstream arquivos[numeroArquivos];
    for (int i = 0; i < numeroArquivos; i++) 
    {
        arquivos[i].open("data_athlete_event" + to_string(i) + ".bin", ios::binary);
        if (!arquivos[i].is_open()) 
        {
            cout << "Erro ao abrir o arquivo auxiliar: " << i << "na mescla" << endl;
        }
    }
    //abrir o arquivo final
    ofstream arquivoFinal;
    arquivoFinal.open("data_athlete_event_final.bin", ios::binary);
    if (!arquivoFinal.is_open()) 
    {
        cout << "Erro ao abrir o arquivo final na mescla" << endl;
        return;
    }
    //mesclar os arquivos auxiliares 
    bool arquivosAbertos = true;
    while (arquivosAbertos) {
        arquivosAbertos = false;
        Dados menor;
        int indiceMenor = -1;
        for (int i = 0; i < numeroArquivos; i++) 
        {
            arquivos[i].seekg(0, ios::end);
            int tamanho = arquivos[i].tellg() / sizeof(Dados);
            arquivos[i].seekg(0, ios::beg);
            if (indices[i] < tamanho) 
            {
                arquivosAbertos = true;
                Dados aux;
                arquivos[i].seekg(indices[i] * sizeof(Dados), ios::beg);
                arquivos[i].read((char*)&aux, sizeof(Dados));
                if (indiceMenor == -1) 
                {
                    menor = aux;
                    indiceMenor = i;
                } else {
                    string e1 = aux.mEvento;
                    string e2 = menor.mEvento;
                    string evento1 = (e1[0] == '"') ? e1.substr(1) : e1;
                    string evento2 = (e2[0] == '"') ? e2.substr(1) : e2;

                    //string evento1 = aux.mEvento;
                    //string evento2 = menor.mEvento;
                    //compara os eventos dos dados
                    if (evento1 < evento2) 
                    {
                        //se o evento do vetor da esquerda for menor, pega o proximo do vetor da esquerda
                        menor = aux;
                        indiceMenor = i;
                    } else if (evento1 > evento2) {
                        //se o evento do vetor da direita for menor, pega o proximo do vetor da direita
                    } else {
                        //se os eventos forem iguais, compara os IDs
                        if (aux.mId < menor.mId) {
                            //se o ID do vetor da esquerda for menor, pega o proximo do vetor da esquerda
                            menor = aux;
                            indiceMenor = i;
                        } else {
                            //se o ID do vetor da direita for menor, pega o proximo do vetor da direita
                        }
                    }
                }
            }
        }
        if (indiceMenor != -1) 
        {
            arquivoFinal.write((char*)&menor, sizeof(Dados));
            indices[indiceMenor]++;
        }
    }
    //fechar os arquivos
    for (int i = 0; i < numeroArquivos; i++) 
    {
        arquivos[i].close();
    }
    arquivoFinal.close();
    cout << "Arquivos mesclados, arquivo final gerado com sucesso!" << endl;
}

void Dados::mergeSortExterno() 
{
    //esse metodo vai chamar os metodos necessarios para ordenar o arquivo
    int numeroArquivos;
    divideArquivo(numeroArquivos);
    if (numeroArquivos > 1) 
    {
        ordenaArquivos(numeroArquivos, 0, 49999);
        mesclaArquivos(numeroArquivos);
        for (int i = 0; i < numeroArquivos; i++)
            remove(("data_athlete_event" + to_string(i) + ".bin").c_str());

        cout << "Deseja exibir o arquivo?\n";
        cout << "Digite 's' para sim ou 'n' para nao\n";
        char key;
        cin >> key;
        if(key == 's')
        {
            exibirOrdenado();
        }
        key = 'k';
        cout << "Deseja converter o arquivo para CSV ?\n";
        cout << "Digite 's' para sim ou 'n' para nao\n";
        cin >> key;
        if(key == 's')
        {
            system(CLEAR.c_str());
            ordenadoParaCsv();
        }
    } else {
        cout << "Arquivo pequeno demais" << endl;
    }
}

void Dados::exibirOrdenado()
{
    ifstream fin;
    fin.open("data_athlete_event_final.bin" , ios_base::in | ios_base::binary);
    if(!fin.is_open())
    {
        throw runtime_error("Arquivo ordenado não encontrado.\n");
    }
    Dados d;

    while ((fin.read((char *)&d, sizeof(Dados))))
    {
        cout << "Evento: " << d.mEvento << " Nome: " << d.mNome << " Cidade: " << d.mCidade;
        cout << " Esporte: " << d.mEsporte << " ID: " << d.mId << " NOC: " << d.mNoc << endl;
    }
    fin.close();


}


int main()
{
    if("base.bin")
        remove("base.bin");

    if("data_athlete_event_final.bin")
        remove("data_athlete_event_final.bin");

    if("arquivoOrdenado.csv")
        remove("arquivoOrdenado.csv");

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
        cout << "o - ordenar o arquivo pelo nome do evento.\n";
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
            case 'o':
                d.mergeSortExterno();
                break;
            default:
		cout << "Digite um comando valido!\n> ";
                break;
        }
    }

    cout << "\nPrograma encerrado.";
    return 0;
}
