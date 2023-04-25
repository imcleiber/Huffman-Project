#include "compactar.h"

//Realiza todo o procedimento para compactar.
void compactar()
{
    char nome_do_arquivo[100], caminho[ALTURA_MAX];
    int inteiro, auxiliar;
    unsigned short lixo, tamanho=0; //referencia para uso binario.

    printf("\nDigite o nome do arquivo a ser compactado:\n");
    scanf("%s", nome_do_arquivo);

    FILE *arquivo = fopen(nome_do_arquivo, "rb");

    if(arquivo == NULL)
    {
        printf("\nErro no arquivo.\nArquivo Vazio ou inexistente\n\n");
        return;
    }

    printf("\nIniciando processo de compactacao...\n\n");

    HT *ht = criar_hash_table();
    adicionar_frequencia(arquivo, ht); //tabela preenchida com frequencia do arquivo.
    rewind(arquivo);
    
    FILA *fila = criar_fila();
    fila = fila_prioridade(ht, fila); //Fila com a ordenacao das frequencias.

    criar_arvore_huffman(fila);
    NO *arvore = fila->cabeca; //determinacao da raiz da arvore.


    criar_caminho(arvore, ht, caminho, 0);
    printf("\nProcesso em andamento...\n\n"); //codificacao do arquivo construida.

    //Inicio do encabecamento do arquivo comprimido.
    lixo = calcula_tamanho_lixo(ht);
    inteiro = lixo;
    calcula_tamanho_arvore(arvore, &tamanho);

    inteiro = inteiro << 13; //deslocamento de 13 por conta do tamanho armazenado de 2 bytes.
    inteiro = setar_bits(inteiro, &tamanho); 

    *nome_do_arquivo = *strtok(nome_do_arquivo, ".");  //Manipulacao de leitura e concatenacao do novo arquivo.
    *nome_do_arquivo = *strcat(nome_do_arquivo, ".huff");
    FILE *saida = fopen(nome_do_arquivo, "wb"); //criacao e abertura do arquivo de saida em binario.

    auxiliar = inteiro>>8; //colocacao do caractere inteiro deslocando em um byte.
    fputc(auxiliar, saida);//No final do arquivo.
    fputc(inteiro, saida);
    imprimir_pre_ordem(saida, arvore);//impressao da arvore.
    fseek(saida, (2 + tamanho), SEEK_SET); //deslocamento sem mudanca dentro do arquivo.
    imprimir_bits(arquivo, saida, ht); // Printa todos os bits dentro do arquivo.
    printf("\nProcesso Finalizado!\n\n");
}