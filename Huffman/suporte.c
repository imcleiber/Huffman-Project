#include "suporte.h"
#include <stdint.h>

//.............................FILA.............................//

NO* criar_no(unsigned char item, int frequencia) //utilizado na criacao da arvore de huffman.
{
    NO *novo_no = (NO*) malloc(sizeof(NO));
    novo_no->item = (void*)(uintptr_t)item; // cast para void*.
    novo_no->frequencia = frequencia;
    novo_no->prox = NULL;
    novo_no->esq = NULL;
    novo_no->dir = NULL;

    return novo_no;
}

FILA* criar_fila()
{
    FILA *nova_fila = (FILA*) malloc(sizeof(FILA));
    nova_fila->cabeca = NULL;
    return nova_fila;
}


void enfileirar(FILA *fila, NO *no)
{
    if((fila->cabeca == NULL) || (no->frequencia <= (fila->cabeca->frequencia))) // insercao no inicio da fila.
    {
        no->prox = fila->cabeca;
        fila->cabeca = no;
    }
    else // insercao no meio da fila.
    {
        NO *auxiliar = fila->cabeca;
        // procura posicao de insercao.
        while ((auxiliar->prox != NULL) && (auxiliar->prox->frequencia < no->frequencia))
        {
            auxiliar = auxiliar->prox;
        }
        no->prox = auxiliar->prox;
        auxiliar->prox = no; 
    }
}

NO* desenfileirar(FILA *fila)
{
    // fila vazia.
    if (fila->cabeca == NULL) {
        return NULL;
    }
   
    NO *auxiliar = fila->cabeca;
    fila->cabeca = fila->cabeca->prox;
    auxiliar->prox = NULL;

    return auxiliar;
}

FILA* montar_fila_prioridade(HT *ht, FILA *fila)
{
    for(int i = 0; i < 256; i++) // percorre a tabela hash e enfileira os caracteres que aparecem no arquivo.
    {
        if(ht->tabela[i]->frequencia != 0) // se o caracter aparece no arquivo, ele eh enfileirado.
        {
            NO *no = criar_no(i, ht->tabela[i]->frequencia); 
            enfileirar(fila, no); 
        }
    }
    return fila;
}




//............................ARVORE............................//


void criar_arvore_huffman(FILA *fila) // construcao da arvore com o uso da fila criada.
{
    if(fila->cabeca != NULL && fila->cabeca->prox != NULL) // enquanto houver mais de um no na fila.
    {
        // desenfileira os dois nos com menor frequencia.
        NO *no_1 = desenfileirar(fila);
        NO *no_2 = desenfileirar(fila);

        // cria um novo no com os dois nos desenfileirados.
        NO *novo_no = (NO*) malloc(sizeof(NO));

        // o novo no recebe o caracter * e a soma das frequencias dos dois nos desenfileirados.
        novo_no->item = (void*)'*'; //escape *.
        novo_no->frequencia = (no_1->frequencia + no_2->frequencia);
        novo_no->esq = no_1;
        novo_no->dir = no_2;

        // enfileira o novo no.
        enfileirar(fila, novo_no);

        // chama a funcao recursivamente até que a fila tenha apenas um no.
        criar_arvore_huffman(fila);
    }
    else
    {
        // quando a fila tiver apenas um no, esse no eh a raiz da arvore.
        return;
    }
}


void calcula_tamanho_arvore(NO *raiz_arvore, unsigned short *tamanho)
{
    if(raiz_arvore != NULL) 
    {
        // se o no for uma folha, o tamanho eh incrementado em 1.
        if(((int)(char*)(raiz_arvore->item) == '\\' || (int)(char*)(raiz_arvore->item) == '*') 
            && raiz_arvore->dir == NULL && raiz_arvore->esq == NULL) 
        { //folha
            *tamanho += 1;
        }
        *tamanho += 1;

        // chama a funcao recursivamente para a esquerda e para a direita
        // até que todos os nos sejam percorridos.
        calcula_tamanho_arvore(raiz_arvore->esq, tamanho);
        calcula_tamanho_arvore(raiz_arvore->dir, tamanho);
    }
}

void imprimir_pre_ordem(FILE *arquivo, NO *raiz_arvore)
{
    if(raiz_arvore != NULL)
    {
        if(((int)(unsigned char*)(raiz_arvore->item) == '\\' || (int)(unsigned char*)(raiz_arvore->item) == '*') 
            && raiz_arvore->dir == NULL && raiz_arvore->esq == NULL)
        { //folha
            fputc('\\', arquivo); // imprime o caracter de escape.
        }
        fputc((int)(char*)(raiz_arvore->item), arquivo); // imprime o caracter.

        // chama a funcao recursivamente para a esquerda e para a direita
        // até que todos os nos sejam percorridos.
        imprimir_pre_ordem(arquivo, raiz_arvore->esq);
        imprimir_pre_ordem(arquivo, raiz_arvore->dir);
    }
}



//..........................HASH TABLE..........................//

ELEMENTO* criar_elemento()
{
    ELEMENTO *novo_elemento = (ELEMENTO*) malloc(sizeof(ELEMENTO));
    novo_elemento->frequencia = 0;
    return novo_elemento;
}

HT* criar_hash_table()
{
    HT *nova_ht = (HT*) malloc(sizeof(HT)); //Alocacao Dinâmica.
    int i;
    for(i = 0; i < MAX; i++)
    {
        nova_ht->tabela[i] = criar_elemento();
    }
    return nova_ht;
}


void adicionar_frequencia(FILE *arquivo, HT *ht)
{   
    int num;
    while((num = fgetc(arquivo)) != EOF) // enquanto nao chegar no fim do arquivo.
    {
        ht->tabela[num]->frequencia += 1;
    }
}


void adicionar_string(HT *ht, void *item, char *caminho)
{
    // h recebe o valor do caracter.
    int h = (int)(unsigned char*)item; 
    strcpy(ht->tabela[h]->caminho, caminho); // copia o caminho para a tabela hash.
}

// Cria caminhos para todas as folhas da arvore.
// O caminho eh criado com 0 para a esquerda e 1 para a direita.
// Isto é feito para poder printar o caminho em binario no arquivo compactado.
void criar_caminho(NO *raiz_arvore, HT *ht, char *caminho, int contador)
{
    // se o no for uma folha, o caminho é terminado com \0. 
    if (raiz_arvore->dir == NULL && raiz_arvore->esq == NULL) 
    {
        caminho[contador] = '\0'; 
        adicionar_string(ht, raiz_arvore->item, caminho);
    }
    else
    {
        // ele percorre pelos nos da arvore e adiciona 0 para a esquerda e 1 para a direita.
        // até que chegue em uma folha.
        caminho[contador] = '0';
        criar_caminho(raiz_arvore->esq, ht, caminho, contador + 1); 
        caminho[contador] = '1';
        criar_caminho(raiz_arvore->dir, ht, caminho, contador + 1);
    }
}



//..........................AUXILIARES..........................//


unsigned char setar_um_bit(unsigned char c, int i)
{
    unsigned char mask = 1 << i; // 1 = 00000001 << i = 00000010; 
                                // Move o bit 1 i vezes para a esquerda. 

    return mask | c; // 00000010 | 00000001 = 00000011; 
                    // Seta o bit i para 1.
}


unsigned short setar_bits(unsigned short c, unsigned short *tamanho)
{
    unsigned short mask = *tamanho; // 
    return mask | c;
}


int calcula_tamanho_lixo(HT *ht) //retorno para encabecamento.
{
    int i, num_bits, soma_num_bits = 0; // num_bits = numero de bits de cada caracter.
                                        // soma_num_bits = soma dos bits de todos os caracteres.

    /* percorre a tabela hash e calcula o numero de bits de cada caracter.
    *  multiplica o numero de bits pelo numero de vezes que o caracter aparece.
    *  soma todos os bits de todos os caracteres.
    * se a soma for divisivel por 8, retorna 0.
    * se nao, retorna o resto da divisao de 8 pela soma. (quantidade de bits que faltam para completar 1 byte)
    */
    for (i = 0; i < 256; i++) // 256 = tamanho da tabela ASCII. 
    {
        if (ht->tabela[i]->frequencia > 0) 
        {
            num_bits = strlen(ht->tabela[i]->caminho); 
            num_bits = num_bits * (ht->tabela[i]->frequencia); 
            soma_num_bits += num_bits; 
        }
    }
    if ((soma_num_bits % 8) == 0) 
    {
        return 0;
    }

    return (8 - (soma_num_bits % 8)); 
}


void imprimir_bits(FILE *entrada, FILE *saida, HT *ht) //impressao da codificacao do arquivo.
{
    unsigned char buffer; // buffer = byte que sera lido do arquivo.
    unsigned char temp = 0; // byte que sera impresso no arquivo.
    int contador = 0; // contador para saber quando o byte esta completo.

    /*
    Percorre o arquivo de entrada e para cada caracter lido, percorre o caminho
    na tabela hash e seta 1 para a direita e 0 para a esquerda em um byte temporario.
    Quando o byte temporario estiver completo, ele eh impresso no arquivo de saida.
    O byte temporario é necessário pois os caminhos podem ter tamanhos diferentes.
    */
    while (!feof(entrada))
    {
        buffer = fgetc(entrada);
        for (int i = 0; i < strlen(ht->tabela[buffer]->caminho); i++)
        {
            if (ht->tabela[buffer]->caminho[i] == '1')
            {
                temp = setar_um_bit(temp, 7 - contador);
            }
            contador++;

            if (contador == 8) // 8 = tamanho de um byte.
            {
                fputc(temp, saida);
                contador = 0;
                temp = 0;
            }
        }
    }
    if (contador != 0) // se acabar o arquivo e o byte nao estiver completo, imprime o byte incompleto.
    {
        fputc(temp, saida);
    }
    fclose(entrada);
    fclose(saida);
}


bool bit_esta_setado(unsigned char c, int i)
{
    unsigned char mascara = 1 << i; // 1 = 00000001 << i = 00000010; 
                                    // Move o bit 1 i vezes para a esquerda.
    return mascara & c; // 00000010 & 00000001 = 00000000; 
                        // Verifica se o bit i está setado.
}