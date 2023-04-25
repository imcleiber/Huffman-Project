#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "suporte.c"
#include "compactar.c"
#include "descompactar.c"

int main()
{
    int procedimento;
    printf("Escolha o procedimento a ser realizado:\n1 - Compactar\n2 - Descompactar\n3 - Sair do programa\n");
    scanf("%d", &procedimento);
    if(procedimento == 1)
    {
        compactar();
        main();
    }
    else if(procedimento == 2)
    {
        descompactar();
        main();
    }
    else if(procedimento == 3)
    {
        return 0;
    }
    else
    {
        printf("\nProcedimento Invalido, Vamos tentar de novo!\n\n");
        main();
    }
}