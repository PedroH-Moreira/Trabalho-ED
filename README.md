# Trabalho-ED

## Observações iniciais
  - Foi preciso utilizar uma constante global para adequar o comando clear a todos os sistemas operacionais.
  - Utilizamos string para manipulação correta da base e posteriormente passar para um vetor char.

## Bibliotecas
### Essas foram as bibliotecas utilizadas, para possibilitar:
  - Entrada e saída de dados;
  - Leitura para entrada e escrita no arquivo;
  - Manipulação de string;
  - Conversão de string para vetor de char e conversão de string para int;
  - Utilizar cls para o system no windows e clear para os demais sistemas operacionais;
 ```cpp
  #include <iostream>
  #include <fstream>
  #include <string>
  #include <cstring>
  #include <stdexcept>
  #include <cstdlib>
  ```
  
## Classe Dados
Com o uso de classes, tanto os dados quanto as funções utilizadas ficam mais organizadas, possibilitando modificações no código sem que ele seja completamente comprometido. Logo, criamos a classe dados para a organização dos dados desejados na conversão, sendo eles:
- ID
- NOME
- CIDADE
- ESPORTE
- EVENTO
- NOC

```cpp
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
```
Por mais que o nome de cada função seja sugestivo, será explicado posteriormente o processo envolvido em cada uma delas.

## Construtores
Utilizamos dois construtores, um para definir um valor qualquer ou "em branco" para os objetos e outro para passar os parâmetros necessários para a correta atribuição. Um exemplo disso é a conversão da string para int.

```cpp
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
        // Tratamento de erro em caso de conversão inválida
        // Por exemplo, definir um valor padrão para mId ou lançar uma exceção
        this->mId = -1; // Valor padrão para mId em caso de erro
    }
    strcpy(this->mNome, Nome.c_str());
    strcpy(this->mCidade, Cidade.c_str());
    strcpy(this->mEsporte, Esporte.c_str());
    strcpy(this->mEvento, Event.c_str());
    strcpy(this->mNoc, Noc.c_str());
}
```

## Função de conversão
Nessa função, o arquivo CSV é lido, descartando a linha inicial. Antes do while, cria-se o ofstream, para que o arquivo binário seja aberto e esteja pronto para a escrita posteriormente.
Descrevendo de maneira resumida, enquanto o arquivo não chegar ao fim, ele vai ler diferentes strings com o uso do getline e até a vírgula, lembrando que nos cammpos nome e evento, ocorrem algumas exceções quanto à representação dos mesmos, onde aparecem vígulas e aspas dentro de um só campo(sendo que a vígula é o delimitador de cada um).
Por fim, criamos um pequeno contador para mostrar a porcentagem de como está o carregamento, tendo em vista que a base possui aproximadamente 271 mil linhas, após isso, o arquivo é fechado e é exibida uma mensagem na tela que o arquivo foi convertido com êxito.

```cpp
void Dados::conversao()
{
    //começar a leitura do csv
    ifstream ler("base15K.csv");
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
```

## Alterar dado
- Nesta função, o arquivo binário é aberto de forma a possibilitar entrada e saída de dados. Posteriormente, foi criada a variável numRecords("quantidade de registros") para contabilizar a quantidade de objetos escritos no arquivo. 
- A estrutura de repetição while() foi utilizada para garantir que o user digite somente uma posição válida no arquivo.
- É feito o posicionamento do ponteiro de leitura (seekg) na posição atribuída pelo user, seguido pela alocação dos dados no objeto "d".
- Cria-se um menu (switch) para a seleção de atributo a ser alterado.
- É feito o posicionamento do ponteiro de escrita (seekp) na posição atribuída pelo user, seguido pela escrita do objeto modificado "d" no arquivo.
- Por fim, o arquivo é fechado e encerra-se a função.

```cpp
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
```









