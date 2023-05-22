# Trabalho-ED

## Observações iniciais
  - Foi preciso utilizar uma constante global para adequar o comando clear a todos os sistemas operacionais.
  	```cpp
	#ifdef _WIN32
   	 const std::string CLEAR = "cls"; // Comando para limpar tela no Windows
	#else
   	 const std::string CLEAR = "clear"; // Comando para limpar tela no Linux e macOS
	#endif
	```
  - Utilizamos string para manipulação correta da base e posteriormente passar para um vetor char.
  - Pode ser que ao baixar a base de dados, uma quebra de linha seja adicionada ao final do arquivo, isso pode causar problemas na execução do programa. Caso isso aconteça, recomendamos remover essa linha.


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

## Módulo de conversão
Nessa função, o arquivo CSV é lido, descartando a linha inicial. Antes do while, cria-se o ofstream, para que o arquivo binário seja aberto e esteja pronto para a escrita posteriormente.
Descrevendo de maneira resumida, enquanto o arquivo não chegar ao fim, ele vai ler diferentes strings com o uso do getline e até a vírgula, lembrando que nos campos nome e evento, ocorrem algumas exceções quanto à representação dos mesmos, onde aparecem vígulas e aspas dentro de um só campo(sendo que a vígula é o delimitador de cada um).
Por fim, criamos um pequeno contador para mostrar a porcentagem de como está o carregamento, tendo em vista que a base possui aproximadamente 271 mil linhas, após isso, o arquivo é fechado e é exibida uma mensagem na tela que o arquivo foi convertido com êxito.

```cpp
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

## Módulo exibir
Como o próprio nome diz, usamos um loop com condição de não fim, para que vá até o final do arquivo imprimindo todos os dados.
```cpp
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
}
``` 


## Módulo para alterar dado
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

## Módulo para exibir intervalo
- Deixamos um if else para verificação da abertura correta do arquivo logo no começo, para caso a abertura seja bem sucedida, iniciamos a operação. Nela, configura-se um ponteiro para percorrer até o final do arquivo e verificar o tamanho do mesmo.
- Após isso, adequa-se o número de linhas pela divisão do tamanho do arquivo pelo tamanho ocupado pela classe.
- Novamente, com o uso de if else verificamos se o intervalo digitado é de fato válido.
- Caso ele seja válido, cria-se duas variáveis, posicao_inicial e posicao_final, para indicar de onde até onde deve ser feita a leitura.
- Por fim, fecha-se o arquivo e dá segmento ao programa.

```cpp
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
```

## Módulo de trocar elementos
- Iniciamos o ponteiro para que possamos direcionar futuramente com os valores fornecidos.
- Ao criar os dois objetos procura-se os dois por meio de seekg.
- Em seguida posiciona-se o ponteiro na posição desejada x para que seja inserido o segundo elemento(y) e faz-se o mesmo com y.
- Após a escrita de ambos, o arquivo é fechado e indicamos ao usuário que a troca foi feita com sucesso.

```cpp
void Dados::trocar_elementos()
{
    fstream file("base.bin", ios_base::binary | ios_base::ate | ios_base:: in | ios_base::out);
    int posx, posy;
    cout << "Digite a posicoes em que deseja fazer a troca\n";
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
```

## Módulo para adicionar elementos
- Criamos um ponteiro utilizando ios::ate para inserir na posição desejada.
- Com o uso do if else verificamos a abertura do arquivo.
- Após isso calculamos a quantidade de registros.
- Usamos un cin com while para garantir que o usuário digite uma posição válida.
- Criamos um registro para receber os dados e posteriormente a inserção dos dados desejados.
- Após assimilar os dados ao objeto, posicionamos o ponteiro ao final do arquivo e adicionamos uma posição a cada elemento para que o objeto seja adicionado com êxito.
- Fechamos o arquivo e finalizamos o módulo.


## Módulo de apresentação
Basicamente uma descrição detalhada das pessoas envolvidas no trabalho.
```cpp
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
```

## Int main
- Caso o arquivo binário já exista ele será apagado, para que não exista uma sopreposição de dados.
- Logo em seguida, é feita a apresentação, detalhando os envolvidos no trabalho.
- Chamamos o módulo de conversão para que o usuário possa fazer as manipulações desejadas no arquivo binário
### Organização
- Utilizamos um loop com a condição de key != s para que o programa continue rodando caso o usuário não queira sair e usamos um switch dentro dele em prol de adequar cada comando à sua função.
- Além disso, como foi citado nas bibliotecas, utilizamos o clear ou cls para deixar o terminal "clean" durante a execução.
- Por fim, como deafult do switch colocamos uma saída para pedir que o usuário digite um comando válido e caso o comando seja s, o programa é então encerrado.

```cpp
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
                cout << "\nDigite o intervalo desejado:\n\n" <<
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
				cout << "Digite um comando válido!\n> ";
                break;
        }
    }

    cout << "\nPrograma encerrado.";
    return 0;
}
```












