#include "suporte.h"

//.............................FILA.............................//

NO* criar_no(unsigned char item, int frequencia) //utilizado na criacao da arvore de huffman.
{
    NO *novo_no = (NO*) malloc(sizeof(NO));
    novo_no->item = item;
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
    if((fila->cabeca == NULL) || (no->frequencia <= (fila->cabeca->frequencia)))
    {
        no->prox = fila->cabeca;
        fila->cabeca = no;
    }
    else
    {
        NO *auxiliar = fila->cabeca;
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
    if (fila->cabeca == NULL)
        return NULL;

    NO *auxiliar = fila->cabeca;
    fila->cabeca = fila->cabeca->prox;
    auxiliar->prox = NULL;
    return auxiliar;
}

FILA* fila_prioridade(HT *ht, FILA *fila)
{
    for(int i = 0; i < 256; i++)
    {
        if(ht->tabela[i]->frequencia != 0)
        {
            unsigned char caracter = *(unsigned char*) malloc(sizeof(unsigned char));
            caracter = i;
            NO *no = criar_no(caracter, ht->tabela[i]->frequencia);
            enfileirar(fila, no);
        }
    }
    return fila;
}



//............................ARVORE............................//


void criar_arvore_huffman(FILA *fila) // construcao da arvore com o uso da fila criada.
{
    if(fila->cabeca->prox != NULL && fila->cabeca != NULL)
    {
        NO *no_1 = desenfileirar(fila);
        NO *no_2 = desenfileirar(fila);
        NO *novo_no = (NO*) malloc(sizeof(NO));

        novo_no->item = '*'; //escape *.
        novo_no->frequencia = (no_1->frequencia + no_2->frequencia);
        novo_no->esq = no_1;
        novo_no->dir = no_2;
        enfileirar(fila, novo_no);
        criar_arvore_huffman(fila);
    }
    else
    {
        return;
    }
}

void calcula_tamanho_arvore(NO *raiz_arvore, unsigned short *tamanho)
{
    if(raiz_arvore != NULL)
    {
        if(((char*)(raiz_arvore->item) == '\\' || (char*)(raiz_arvore->item) == '*') 
            && raiz_arvore->dir == NULL && raiz_arvore->esq == NULL)
        { //folha
            *tamanho += 1;
        }
        *tamanho += 1;
        calcula_tamanho_arvore(raiz_arvore->esq, tamanho);
        calcula_tamanho_arvore(raiz_arvore->dir, tamanho);
    }
}

void imprimir_pre_ordem(FILE *arquivo, NO *raiz_arvore)
{
    if(raiz_arvore != NULL)
    {
        if(((unsigned char*)(raiz_arvore->item) == '\\' || (unsigned char*)(raiz_arvore->item) == '*') 
            && raiz_arvore->dir == NULL && raiz_arvore->esq == NULL)
        { //folha
            fputc('\\', arquivo);
        }
        fputc((char*)(raiz_arvore->item), arquivo);
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
    while((num = fgetc(arquivo)) != EOF)
    {
        ht->tabela[num]->frequencia += 1;
    }
}


void adicionar_string(HT *ht, void *item, char *caminho)
{
    int h = (unsigned char*)item;
    strcpy(ht->tabela[h]->caminho, caminho);//concatenacao do caminho.
}


void criar_caminho(NO *raiz_arvore, HT *ht, char *caminho, int contador)
{
    if(raiz_arvore->dir == NULL && raiz_arvore->esq == NULL) // folha
    {
        caminho[contador] = '\0'; 
        adicionar_string(ht, raiz_arvore->item, caminho);
    }
    else
    {
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


int calcula_tamanho_lixo(HT *ht)//retorno para encabecamento.
{
    int i, num_bits, soma_num_bits = 0;
    for(i = 0; i < 256; i++) // 256 = tamanho da tabela ASCII. 
    {
        if(ht->tabela[i]->frequencia>0)
        {
            num_bits = strlen(ht->tabela[i]->caminho);
            num_bits = num_bits*(ht->tabela[i]->frequencia);
            soma_num_bits += num_bits;
        }
    }
    if((soma_num_bits % 8) == 0) 
        return 0;

    return (8 - (soma_num_bits % 8)); 
}


void imprimir_bits(FILE *entrada, FILE *saida, HT *ht) //impressao da codificacao do arquivo.
{
    unsigned char aux, opcao = 0, c = 0;
    int i, contador = 0;

    while (!feof(entrada))
    {
        aux = fgetc(entrada);
        for (i = 0; i < strlen(ht->tabela[aux]->caminho); i++)
        {
            if (ht->tabela[aux]->caminho[i] == '1')
            {
                opcao = setar_um_bit(opcao, 7 - contador);
            }
            contador++;
            if (contador == 8) // 8 = tamanho de um byte.
            {
                fputc(opcao, saida);
                contador = 0;
                opcao = 0;
            }
        }
    }
    if (contador != 0)
    {
        fputc(opcao, saida);
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