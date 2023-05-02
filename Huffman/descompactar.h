#ifndef descompactar_h
    #define descompactar_h
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include "suporte.h"

    /**
     * @brief Função que realiza todo o procedimento para descompactar, da entrada à saída.
     * 
     * A função descompactar() irá receber o nome do arquivo que deseja descompactar e irá realizar
     * todo o procedimento para descompactar o arquivo, desde a leitura do arquivo compactado até a
     * escrita do arquivo descompactado.
     * 
     */
    void descompactar();

    /**
     * @brief Funcao que realiza a descompressão de um arquivo.
     * 
     * A funcao descompressao() receberá como parametro um ponteiro para um arquivo compactado
     * e irá realizar o processo de descompactação de Huffman, gerando um novo arquivo descompactado
     * com o nome informado.
     * 
     * @param   compactado  Um ponteiro para um arquivo compactado.
     */
    int descompressao(FILE *compactado);

    /**
     * @brief Função que reconstrói a árvore de huffman do arquivo compactado
     * 
     * A função montagem_arvore() recebe como parâmetro um ponteiro para um arquivo compactado
     * e irá reconstruir a árvore de huffman do arquivo compactado.
     * 
     * @param   compactado  Ponteiro para o arquivo compactado
     * @return  NO*         Retorna um ponteiro para a raiz da árvore de huffman
     */
    NO *montagem_arvore(FILE *compactado);


    /**
     * @brief Função que escreve o arquivo descompactado
     * 
     * A função escrever_arquivo() recebe como parâmetro um ponteiro para o arquivo compactado, um ponteiro
     * para o arquivo descompactado, a raiz da árvore de huffman, o tamanho da árvore, o tamanho do lixo
     * e o tamanho do arquivo compactado. A função irá escrever o arquivo descompactado.
     * 
     * @param       compactado              Ponteiro para o arquivo compactado
     * @param       descompactado           Ponteiro para o arquivo descompactado
     * @param       raiz                    Raiz da árvore de huffman
     * @param       tamanho_lixo            Tamanho do lixo
     * @param       tamanho_arquivo_final   Tamanho do arquivo compactado
     */
    void escrever_arquivo(FILE* compactado, FILE* descompactado, NO *raiz, short tamanho_lixo, int tamanho_arquivo_final);
#endif