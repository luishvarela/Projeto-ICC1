// Inclusão das bibliotecas necessárias para o projeto;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct para tipificação do armazenamento das informações dos produtos;
typedef struct PRODUTO{
    char nome[100];
    int quantidade;
    float preco;
} produto;

// Função que dobra o tamanho do estoque, caso necessário, 
// o que é melhor que fazer realloc para cada produto novo, porque o realloc é custoso;
produto *realoca(produto *estoque, int *tam){
    (*tam) *= 2;
    produto *temp = NULL;
    temp = (produto *) realloc((void *)estoque, (*tam)*sizeof(produto));

    // Prevenção contra memory leak;
    if(temp == NULL){
        free(estoque);
        (*tam) /= 2;
        return NULL;
    }
    else return temp;
}

// Função que insere produto no estoque, obedecendo o comando IP;
int insere_produto(produto **estoque, int *index, int *tam){
    // Checa se o tamanho no estoque é suficiente, se não, chama a função realoca
    // para alocar mais memória para o estoque;
    if(*tam < *index + 1) *estoque = realoca(*estoque, tam);
    if(*estoque == NULL){
        return 1;
    }
    
    // Lê os valores do produto que deve ser inserido;
    scanf(" %s", (*estoque)[*index].nome);
    scanf(" %d", &(*estoque)[*index].quantidade);
    scanf(" %f", &(*estoque)[*index].preco);
    (*index)++;
    return 0;
}

// Função que aumenta a quantidade de um determinado produto no estoque, obedecendo o comando AE;
void aumenta_estoque(produto *estoque, float *saldo, int tamanho){
    int codigo, quantidade;
    scanf(" %d %d", &codigo, &quantidade);

    // Adiciona ao produto de código correspondente a quantidade nova no estoque,
    //  e diminui do saldo o preço dele vezes a quantidade;
    if(codigo >= 0 && codigo < tamanho){
        estoque[codigo].quantidade += quantidade;
        *saldo -= quantidade*estoque[codigo].preco;
    }
}

// Função que modifica o preço de um determinado produto no estoque, obedecendo o comando MP;
void modifica_preco(produto *estoque, int tamanho) {
    int codigo;
    float preco;
    scanf(" %d %f", &codigo, &preco);

    // Altera o preço do produto de código correspondente para o novo preço;
    if(codigo >= 0 && codigo < tamanho){
        estoque[codigo].preco = preco;
    }
}

/// Realiza a venda de um produto do estoque, obedecendo o comando VE;
void realizar_venda(produto *estoque, float *saldo, int tamanho) {
    int codigo;
    float totalVenda = 0;

    // Loop para realizar as vendas enquanto o usuário não digitar -1;
    while(1){
        scanf("%d", &codigo);
        
        if(codigo == -1){
            // Descarta números que sobraram no buffer até o próximo comando;
            while(scanf("%d", &codigo) == 1);
            break;
        }
        
        // Checa se há produto no estoque, e só vende nesse caso.
        if(codigo >= 0 && codigo < tamanho && estoque[codigo].quantidade > 0){
            printf("%s %.2f\n", estoque[codigo].nome, estoque[codigo].preco);

            // O valor do produto atual é adicionado ao total da venda, e o estoque do produto correspondente é diminuído;
            totalVenda += estoque[codigo].preco;
            estoque[codigo].quantidade -= 1;
        }
        
    }

    // O valor total da venda deve ser adicionado ao saldo do caixa;
    *saldo += totalVenda;

    printf("Total: %.2f\n", totalVenda);
    printf("--------------------------------------------------\n");
}

// Realiza uma consulta do estoque, imprimindo sequencialmente, o codigo do produto, o nome e sua quantidade no estoque, obedecendo o comando  CE;
void consulta_estoque(produto *estoque, int tamanho){

    // Loop para percorrer o estoque e imprimir cada item com sua quantidade.
    for (int i = 0; i < tamanho; i++) {
        printf("%d %s %d\n", i, estoque[i].nome, estoque[i].quantidade);
    }
    
    printf("--------------------------------------------------\n");
    
}

// Imprime o saldo atual do caixa, obedecendo o comando CS;
void consulta_saldo(float saldo) {
    printf("Saldo: %.2f\n", saldo);
    printf("--------------------------------------------------\n");
    
}

// Finaliza o dia e guarda as informações em um arquivo para uso futuro, obedecendo o comando FE;
void finalizar_dia(FILE **binF, produto **estoque, int *num, float *saldo){

    // Abre os dois arquivos para escrita em binário;

    *binF= fopen("estoque.bin", "wb");

    // Salva os dados atuais para uso futuro;
    fwrite(saldo, sizeof(float), 1, *binF);

    // Fecha o arquivo e abre para modo de adição;
    fclose(*binF);
    *binF= fopen("estoque.bin", "ab");

    // Adiciona ao arquivo o número de produtos no estoque e as informações do estoque;
    fwrite(num, sizeof(int), 1, *binF);
    fwrite(*estoque, sizeof(produto), *num, *binF);

    // Fecha o arquivo aberto;
    fclose(*binF);

    // Libera a memória alocada para o estoque e faz o ponteiro apontar para NULL para evitar dangling;
    free(*estoque);
    *estoque = NULL;
}

// Função que lê os dados do dia anterior, caso existam, ou cria
int entrada(FILE **binF, produto **estoque, int *tam, float *saldo, int *num){
    // Abre o arquivo para leitura;
    *binF = fopen("estoque.bin", "rb");

    // Se não existir, inicializa como o saldo base de 100, e lê o tamanho do estoque;
    if(*binF == NULL){
        *saldo = 100;
        scanf(" %d", tam);
        *estoque = (produto *) malloc((*tam) * sizeof(produto));
        if(*estoque == NULL) return 1;
    }
    
    // Se existir arquivos anteriores, faz sua leitura e fechamento;
    else {
        // Lê o saldo anterior;
        fread(saldo,sizeof(float), 1, *binF);
        // Lê o tamanho do estoque anterior;
        fread(tam,sizeof(int), 1, *binF);
        *num = *tam;

        // Aloca o espaço para o estoque correspondente ao que havia antes.
        *estoque = (produto *) malloc((*tam) * sizeof(produto));
        
        // Se der erro na alocação, encerra a função imediatamente;
        if(*estoque == NULL) return 1;
        
        // Lê as informações do estoque anterior e fecha o arquivo logo após;
        fread(*estoque, sizeof(produto), (*tam), *binF);
        fclose(*binF);
    }

    

    return 0;
}   

//Função que determina qual comando será executado
int comando(char* comando){
    // Função que facilita o switch dentro da main associando as strings de comando a
    // números chave;
    if(!strcmp(comando, "IP")) return 1;
    if(!strcmp(comando, "AE")) return 2;
    if(!strcmp(comando, "MP")) return 3;
    if(!strcmp(comando, "VE")) return 4;
    if(!strcmp(comando, "CE")) return 5;
    if(!strcmp(comando, "CS")) return 6;
    if(!strcmp(comando, "FE")) return 7;

    return 0;
}

int main(void){
    // Inicialização das variáveis primárias;
    char comandoAtual[3];
    produto *estoque = NULL;
    int tamEstoque = 0, numProdutos = 0;
    float saldo;

    // Inicializa o ponteiro do arquivo que pode ou não ser lido;
    FILE *binF = NULL;

    // Chama a função de entrada para iniciar o programa;
    if(entrada(&binF, &estoque, &tamEstoque, &saldo, &numProdutos)){
        exit(1);
    }
    
    // Loop principal de leitura dos comandos que utiliza um switch para chamar a função adequada
    // a depender do comando escrito pelo usuário;
    while(1){
        scanf(" %s", comandoAtual);
        switch(comando(comandoAtual)){

            // IP -> Inserir produto;
            case 1:
                if(insere_produto(&estoque, &numProdutos, &tamEstoque) == 1){
                    exit(1);
                }
                break;

            // AE -> Aumento do estoque;
            case 2:
                aumenta_estoque(estoque,&saldo, numProdutos);
                break;
            
            // MP -> Modifica Preco
            case 3:
                modifica_preco(estoque, numProdutos);
                break;

            // VE -> Venda
            case 4:
                realizar_venda(estoque, &saldo, numProdutos);
                break;
                
            // CE -> Consulta Estoque
            case 5:
                consulta_estoque(estoque, numProdutos);
                break;

            // CS -> Consulta saldo
            case 6:
                consulta_saldo(saldo);
                break;

            // FE -> Finalizar o Dia
            case 7:
                finalizar_dia(&binF, &estoque, &numProdutos, &saldo);
                return (0);
                break;

            default:
                exit(1);
                break;
        }
    }

    return (0);
}