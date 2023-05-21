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








