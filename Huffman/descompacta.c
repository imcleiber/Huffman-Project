#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "suporte.c"
#include "main.c"
#define ALTURA_MAX 31
#define MAX 256

void descompactar()
{
    char nomedoarquivo[50];
    printf("Entre com o nome do arquivo que deseja descompactar: ");
    scanf("%s", nomedoarquivo);

    // Cria um ponteiro para o arquivo
    FILE *compactado = fopen(nomedoarquivo, "rb"); // Criando o arquivo no modo 'rb' de leitura 'read binary'
    descompressao(compactado, nomedoarquivo);
}

int descompressao(FILE *compactado, char *nome)
{
	int tamanho_arquivo = 0;
	short tamanho_lixo, tamanho_arvore = 8191; // Valor maximo de 1 byte "11111111". 
	char nome_saida[50];
    unsigned char primeirobyte, segundobyte;
    NO *raiz = NULL;
    getchar();
    printf("Digite o nome final do arquivo: ");
    scanf("%[^\n]s", nome_saida);
    printf("\nIniciando processo de descompactacao...\n\n");
    FILE *descompactado = fopen(nome_saida, "wb"); // Escreve em binario no novo arquivo
    fseek(compactado, 0, SEEK_END); // Vai ate  o final do arquivo

    tamanho_arquivo = ftell(compactado); // Tamanho_arquivo recebe o tamanho do arquivo em bytes
	rewind(compactado); // Retorna para o inicio do arquivo

    primeirobyte = fgetc(compactado); // F_byte recebe o primeiro byte do arquivo
    
    segundobyte = fgetc(compactado); // S_byte recebe o segundo byte do arquivo
    printf("\nProcesso em andamento...\n\n");
	
    tamanho_lixo = primeirobyte >> 5; // Tamanho_lixo recebe os 3 bits de tamanho do lixo

    tamanho_arvore = ((primeirobyte << 8) | segundobyte) & tamanho_arvore; // Tamanho arvore vai receber o tamanho da arvore

    raiz = montagem_arvore(compactado); // Monta a arvore
	fseek(compactado, tamanho_arvore + 2, SEEK_SET); // Escreve depois da arvore no novo arquivo
	printar_byte(compactado, descompactado, raiz, tamanho_arvore, tamanho_lixo, (tamanho_arquivo-tamanho_arvore-2)); // Escreve tudo no arquivo
	fclose(compactado);
	fclose(descompactado);
    printf("\nArquivo descompactado com sucesso!\n\n");
	return 0;

}

NO *montagem_arvore(FILE *compactado) // Reconstruir a arvore de huffman do arquivo compactado
{
	unsigned char atual;
    NO *novo_no;
    atual = fgetc(compactado);
    // Utilizamos como condicao de analise, valores referentes ao escape(item).
    if (atual == '*')
    {
    	novo_no = criar_no_arvore(atual);
        novo_no->esq = montagem_arvore(compactado);
        novo_no->dir = montagem_arvore(compactado);
    }
    else if (atual == '\\')
    {
        atual = fgetc(compactado);
        novo_no = criar_no_arvore(atual);
    }
    else
    {
        novo_no = criar_no_arvore(atual);
    }
    return novo_no;
}

NO *criar_no_arvore(unsigned char caractere)
{
	unsigned char *aux = (unsigned char *)malloc(sizeof(unsigned char));
    *aux = caractere;
	NO *novo_no = (NO *)malloc(sizeof(NO));
    novo_no->frequencia = 0;
    novo_no->item = aux;
    novo_no->dir = NULL;
    novo_no->esq = NULL;
    novo_no->prox = NULL;
    return novo_no;
}

void printar_byte(FILE* compactado, FILE* descompactado, NO *raiz, short tamanho_arvore, short tamanho_lixo, int tamanho_arquivo) // Escreve o arquivo descompactado
{
    long int i, j, k;
	unsigned char c;
	NO *atual = raiz;
	for (i = 0; i < tamanho_arquivo; ++i)
	{
		c = fgetc(compactado);
		if (i == tamanho_arquivo - 1)
        {
			k = 8 - tamanho_lixo;
        }
		else
        {
			k = 8;
        }
		for (j = 0 ; j < k ; ++j)
		{
			if (bit_esta_setado(c, 7 - j))
            {
				atual = atual->dir;
            }
			else
            {
				atual = atual->esq;
            }
			if (atual->esq == NULL && atual->dir == NULL)//folha
            {
                fprintf(descompactado, "%c", *(unsigned char*)atual->item);
                atual = raiz;
            }
		}
	}
}