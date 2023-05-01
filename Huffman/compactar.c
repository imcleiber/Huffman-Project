#include "compactar.h"

#define MAX_FILENAME_SIZE 100

void compactar()
{
    char nome_do_arquivo[MAX_FILENAME_SIZE]; //nome do arquivo a ser compactado. 
    char caminho[ALTURA_MAX]; // 
    int header; // cabecalho do arquivo compactado.
    int aux_byte_header; // auxiliar para a criacao do cabecalho.
    unsigned short tamanho_lixo; // quantidade de bits que nao serao utilizados no ultimo byte.
                                // ocupa 3 bits no cabecalho.
    unsigned short tamanho_arvore = 0; // tamanho da arvore de huffman.
                                    // ocupa 13 bits no cabecalho.

    
    
    printf("\nDigite o nome do arquivo a ser compactado:\n");
    scanf("%s", nome_do_arquivo);

    FILE *arquivo = fopen(nome_do_arquivo, "rb");

    if(arquivo == NULL)
    {
        printf("\nErro no arquivo.\nArquivo Vazio ou inexistente\n\n");
        return;
    }

    printf("\nIniciando processo de compactacao...\n\n");

    /*
     * Cria uma tabela hash vazia
     * e adiciona as frequencias de cada caracter do arquivo.
     */
    HT *ht = criar_hash_table();
    adicionar_frequencia(arquivo, ht);
    rewind(arquivo);
    
    // Cria uma fila de prioridade com os itens da tabela hash.
    FILA *fila = criar_fila();
    fila = montar_fila_prioridade(ht, fila); 

    // Cria a arvore de huffman com a fila de prioridade criada.
    criar_arvore_huffman(fila);
    NO *arvore = fila->cabeca; // cabeca da fila eh a raiz da arvore.

    // Cria o caminho de cada caracter na arvore.
    criar_caminho(arvore, ht, caminho, 0);

    printf("\nProcesso em andamento...\n\n"); //codificacao do arquivo construida.

    // Calcula o tamanho do lixo e da arvore para o cabecalho (header).
    tamanho_lixo = calcula_tamanho_lixo(ht);
    calcula_tamanho_arvore(arvore, &tamanho_arvore);

    // Cria o cabecalho (header):
    header = tamanho_lixo; 
    header = header << 13; 
    header = setar_bits(header, &tamanho_arvore); // concatena os bits do lixo com o tamanho da arvore

    // Abertura do arquivo de saida:
    *nome_do_arquivo = *strtok(nome_do_arquivo, "."); // separa o nome do arquivo da extensao.
    *nome_do_arquivo = *strcat(nome_do_arquivo, ".huff"); // concatena o nome do arquivo com a extensao .huff.
    FILE *saida = fopen(nome_do_arquivo, "wb"); // abre o arquivo de saida.

    // Separa em dois bytes o inteiro e escreve no arquivo
    aux_byte_header = header >> 8; // seta os 8 primeiros bits do cabecalho. 
    fputc(aux_byte_header, saida); // imprime o primeiro byte do cabecalho.
    fputc(header, saida); // imprime o segundo byte do cabecalho.

    // Imprime a arvore de huffman no arquivo de saida.
    imprimir_pre_ordem(saida, arvore); 

                // 2 bytes do lixo e tamanho da arvore + arvore
    fseek(saida, (2 + tamanho_arvore), SEEK_SET); // Desloca o ponteiro do arquivo para após o cabecalho.

    // Imprime o arquivo compactado no arquivo de saida.
    imprimir_bits(arquivo, saida, ht); 

    printf("\nProcesso Finalizado!\n\n");
}