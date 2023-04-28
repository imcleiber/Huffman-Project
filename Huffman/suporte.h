#ifndef suporte_h
    #define suporte_h
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include "compactar.h"
    #include "descompactar.h"
    #define MAX 256
    #define ALTURA_MAX 31


    /**
     * @brief Estrutura de um nó da fila de prioridade e da árvore de huffman.
     * 
     * A estrutura NO é utilizada para representar um nó da fila de prioridade e da árvore de huffman.
     * Ela possui um ponteiro para o item, um inteiro para a frequencia, e dois ponteiros para os nós da esquerda e direita.
     * 
     */
    typedef struct NO {
        void *item;
        int frequencia;
        struct NO *prox;
        struct NO *esq;
        struct NO *dir;
    } NO;

    /**
     * @brief Estrutura de uma fila de prioridade.
     * 
     * A estrutura FILA é utilizada para representar uma fila de prioridade.
     * Ela possui um ponteiro para o nó cabeça da fila.
     * 
     */
    typedef struct FILA {
        struct NO *cabeca;
    } FILA;

    /**
     * @brief Estrutura de um elemento da hash table.
     * 
     * A estrutura ELEMENTO é utilizada para representar um elemento da hash table.
     * Ela possui um vetor de char para o caminho na árvore de Huffman,
     * e um inteiro para a frequencia. 
     * 
     */
    typedef struct ELEMENTO {
        char caminho[ALTURA_MAX]; //ALTURA_MAX = 31 = altura máxima
        long long int frequencia;
    } ELEMENTO;

    /**
     * @brief Estrutura da hash table.
     * 
     * A estrutura HT é utilizada para representar uma hash table.
     * Ela possui um vetor de ELEMENTO para a tabela hash.
     * 
     */
    typedef struct HT {
        ELEMENTO* tabela[MAX]; //Numero de elementos na tabela ASCII
    } HT;

    /**
     * @brief Cria uma struct NO (ADT) e retorna seu endereço. 
     * 
     * A função criar_no() aloca um espaço em memória para um NO,
     * e inicializa seus elementos. Atribuindo os valores item e frequencia
     * recebidos como parâmetros para os campos item e frequencia do NO.
     * Atribui NULL para os demais campos, prox, esq, e dir.
     * Retorna o ponteiro para o NO criado. 
     * 
     * @param   item            Dado a ser armazenado no NO
     * @param   frequencia      Frequência desse dado 
     * 
     * @return  NO*             Ponteiro para o NO criado
     */
    NO* criar_no(unsigned char item, int frequencia);

    /**
     * @brief Cria uma fila de prioridade vazia (ADT)
     * 
     * A função criar_fila() aloca um espaço em memória para uma FILA,
     * e inicializa o campo 'cabeca' atribuindo o valor NULL.
     * Retorna o ponteiro para a FILA criada.
     * 
     * @return   FILA*    Ponteiro para a fila criada.
     */
    FILA* criar_fila(); 

    /**
     * @brief Enfileira um NO na fila de prioridade
     * 
     * A função enfileirar() recebe como parâmetro uma FILA e um NO.
     * Se a fila estiver vazia, ou se a frequência do NO for menor ou igual
     * a frequência do primeiro elemento da fila, o NO é inserido no início
     * da fila. Caso contrário, o NO é inserido na posição correta, de forma
     * que a fila permaneça ordenada em ordem crescente de frequência.
     * 
     * @param   fila    Ponteiro para a fila de prioridade 
     * @param   no      Ponteiro para o NO a ser inserido na fila
     */
    void enfileirar(FILA *fila, NO *no); 

    /**
     * @brief Desenfileira um NO da fila de prioridade
     * 
     * A função desenfileirar() recebe como parâmetro uma FILA.
     * Se a fila estiver vazia, retorna NULL. Caso contrário, o primeiro
     * elemento da fila é removido e seu endereço é retornado.
     * 
     * @param   fila    Ponteiro para a fila de prioridade 
     * @return  NO*     Ponteiro para o NO removido da fila
     */
    NO* desenfileirar(FILA *fila); 

    /**
     * @brief Monta a fila de prioridade
     * 
     * A função fila_prioridade() recebe como parâmetro uma HT e uma FILA.
     * Percorre a tabela de frequência da HT e para cada posição que não for
     * nula, cria um NO com o caracter correspondente e sua frequência, e
     * enfileira esse NO na FILA. Retorna a FILA montada.
     * 
     * @param   ht      Ponteiro para uma tabela de frequência (HT)
     * @param   fila    Ponteiro para uma fila de prioridade (FILA)
     * @return  FILA*   Ponteiro para a fila de prioridade montada
     */
    FILA* fila_prioridade(HT *ht, FILA *fila); 

    /**
     * @brief Cria uma árvore de Huffman
     * 
     * A função criar_arvore_huffman() recebe como parâmetro uma FILA.
     * Enquanto a fila não estiver vazia, desenfileira os dois primeiros
     * elementos da fila, cria um novo NO, atribui o caracter '*' ao seu
     * campo item, e a soma das frequências dos dois elementos desenfileirados
     * ao seu campo frequência. Atribui os dois elementos desenfileirados
     * aos campos esq e dir do novo NO. Enfileira o novo NO na fila.
     * Chama a função recursivamente até que a fila tenha apenas um elemento.
     * 
     * @param   fila    Ponteiro para a fila de prioridade (FILA) 
     */
    void criar_arvore_huffman(FILA *fila); 

    /**
     * @brief Calcula o tamanho da árvore de Huffman
     * 
     * A função calcula_tamanho_arvore() recebe como parâmetro um NO e um
     * ponteiro para o tamanho da árvore. Se o NO não for nulo, e se o seu
     * campo item for igual a '\' ou '*', e se os campos esq e dir forem nulos,
     * incrementa o tamanho da árvore em 1. Incrementa o tamanho da árvore em 1.
     * Chama a função recursivamente para o filho esquerdo e para o filho direito.
     * 
     * @param   raiz_arvore     Ponteiro para a raiz da árvore de Huffman
     * @param   tamanho         Ponteiro para o tamanho da árvore 
     */ 
    void calcula_tamanho_arvore(NO *raiz_arvore, unsigned short *tamanho); 

    /**
     * @brief Imprime a árvore de Huffman em pré-ordem
     * 
     * A função imprimir_pre_ordem() recebe como parâmetro um arquivo e um NO.
     * Se o NO não for nulo, e se o seu campo item for igual a '\' ou '*', e se
     * os campos esq e dir forem nulos, imprime '\' antes do caracter. Imprime
     * o caracter. Chama a função recursivamente para o filho esquerdo e para
     * o filho direito.
     * 
     * @param   arquivo         Ponteiro para o arquivo de saída
     * @param   raiz_arvore     Ponteiro para a raiz da árvore de Huffman
     */
    void imprimir_pre_ordem(FILE *arquivo, NO *raiz_arvore);

    /**
     * @brief Cria um elemento
     * 
     * A função criar_elemento() aloca dinamicamente um elemento (ELEMENTO).
     * Atribui 0 ao campo frequência do elemento criado.
     * 
     * @return  ELEMENTO*   Ponteiro para o elemento criado 
     */
    ELEMENTO* criar_elemento(); 

    /**
     * @brief Cria uma hash table
     * 
     * A função criar_hash_table() aloca dinamicamente uma hash table (HT).
     * Atribui a cada posição da tabela um elemento (ELEMENTO) criado pela
     * função criar_elemento().
     * 
     * @return  HT*     Ponteiro para a hash table criada
     */
    HT* criar_hash_table(); 

    /**
     * @brief Adiciona a frequência de cada caracter na hash table
     * 
     * A função adicionar_frequencia() recebe como parâmetro um arquivo e uma
     * hash table. Enquanto não chegar ao final do arquivo, lê um caracter do
     * arquivo e incrementa a frequência do caracter na hash table.
     * 
     * @param   arquivo     Ponteiro para o arquivo de entrada
     * @param   ht          Ponteiro para a hash table
     */
    void adicionar_frequencia(FILE *arquivo, HT *ht); 

    /**
     * @brief Adiciona o caminho de cada caracter na hash table
     * 
     * A função adicionar_string() recebe como parâmetro uma hash table, um
     * caracter e um caminho. Atribui o caminho ao campo caminho do elemento
     * da hash table correspondente ao caracter.
     * 
     * @param ht 
     * @param item 
     * @param caminho 
     */
    void adicionar_string(HT *ht, void *item, char *caminho); 

    /**
     * @brief Cria o caminho de cada caracter na hash table
     * 
     * A função criar_caminho() recebe como parâmetro a raiz da árvore de
     * Huffman, uma hash table, um caminho e um contador. Se o nó for folha,
     * atribui o caminho ao caracter correspondente na hash table. Se não,
     * atribui '0' ao caminho e chama a função recursivamente para o filho
     * 
     * @param raiz_arvore 
     * @param ht 
     * @param caminho 
     * @param contador 
     */
    void criar_caminho(NO *raiz_arvore, HT *ht, char *caminho, int contador); 

    // Setar bit = tornar um bit 1

    /**
     * @brief Seta o bit i de um char para 1
     * 
     * A função setar_um_bit() recebe como parâmetro um char e um inteiro.
     * Inicia uma variável mask com o valor 1 deslocado i vezes para a
     * esquerda. Retorna o resultado da operação OU bit a bit entre mask
     * e o char. 
     * 
     * @param   c               Char a ser modificado
     * @param   i               Indíce do bit
     * @return  unsigned char   Char com o bit i setado para 1
     */
    unsigned char setar_um_bit(unsigned char c, int i); 

    /**
     * @brief Seta os bits de um char
     * 
     * A função setar_bits() recebe como parâmetro um char e um ponteiro
     * para o tamanho. Inicia uma variável mask com o valor do tamanho
     * deslocado 8 vezes para a esquerda. Retorna o resultado da operação
     * OU bit a bit entre mask e o char.
     * 
     * @param       c                   Short a ser modificado
     * @param       tamanho             Ponteiro para o tamanho
     * @return      unsigned short      Short com os bits setados
     */
    unsigned short setar_bits(unsigned short c, unsigned short *tamanho); 

    /**
     * @brief Calcula o tamanho do lixo
     * 
     * A função calcula_tamanho_lixo() recebe como parâmetro uma hash table.
     * Calcula o tamanho do lixo, que é o resto da divisão da soma do número
     * de bits de cada caracter pelo número de bits de um byte.
     * 
     * @param   ht      Ponteiro para a hash table
     * @return  int     Tamanho do lixo
     */
    int calcula_tamanho_lixo(HT *ht); 

    /**
     * @brief Imprime os bits compactados
     * 
     * A função imprimir_bits() recebe como parâmetro um arquivo de entrada,
     * um arquivo de saída e uma hash table. Enquanto não chegar ao final do
     * arquivo de entrada, lê um caracter do arquivo de entrada e imprime os
     * bits correspondentes ao caminho na árvore do caracter no arquivo de saída.
     * 
     * @param entrada 
     * @param saida 
     * @param ht 
     */
    void imprimir_bits(FILE *entrada, FILE *saida, HT *ht); 
    

    //DESCOMPACTAR - FUNCOES

    //Realiza as chamadas das funções para descompactar.
    void descompactar();

    //Realiza a descompressão propriamente dita.
    int descompressao(FILE *compactado);

    //Monta a árvore a partir do arquivo compactado.
    NO *montagem_arvore(FILE *compactado);

    //Cria um NO com um caractere inserido.
    NO *criar_no_arvore(unsigned char caractere);

    //Escreve o arquivo descompactado
    void printar_byte(FILE* compactado, FILE* descompactado, NO *raiz, short tamanho_arvore, short tamanho_lixo, int tamanho_arquivo);

    /**
     * @brief Verifica se um bit i do char c está setado
     * 
     * A função bit_esta_setado() recebe como parâmetro um char e um inteiro.
     * Inicia uma variável mascara com o valor 1 deslocado i vezes para a
     * esquerda. Retorna o resultado da operação E bit a bit entre mascara
     * e o char.
     * 
     * @param       c       // Char a ser verificado
     * @param       i       // Indíce do bit
     * @return      true    // Se o bit i estiver setado
     * @return      false   // Se o bit i não estiver setado
     */
    bool bit_esta_setado(unsigned char c, int i);
#endif