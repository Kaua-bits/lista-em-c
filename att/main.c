#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lista.h"

static const char *arquivosTribunais[] = {
    "basedados/teste_TRE-AC.csv",
    "basedados/teste_TRE-AL.csv",
    "basedados/teste_TRE-AM.csv",
    "basedados/teste_TRE-AP.csv",
    "basedados/teste_TRE-BA.csv",
    "basedados/teste_TRE-CE.csv",
    "basedados/teste_TRE-DF.csv",
    "basedados/teste_TRE-ES.csv",
    "basedados/teste_TRE-GO.csv",
    "basedados/teste_TRE-MA.csv",
    "basedados/teste_TRE-MG.csv",
    "basedados/teste_TRE-MS.csv",
    "basedados/teste_TRE-MT.csv",
    "basedados/teste_TRE-PA.csv",
    "basedados/teste_TRE-PB.csv",
    "basedados/teste_TRE-PE.csv",
    "basedados/teste_TRE-PI.csv",
    "basedados/teste_TRE-PR.csv",
    "basedados/teste_TRE-RJ.csv",
    "basedados/teste_TRE-RN.csv",
    "basedados/teste_TRE-RO.csv",
    "basedados/teste_TRE-RR.csv",
    "basedados/teste_TRE-RS.csv",
    "basedados/teste_TRE-SC.csv",
    "basedados/teste_TRE-SE.csv",
    "basedados/teste_TRE-SP.csv",
    "basedados/teste_TRE-TO.csv",
};

int main(void) {

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    int tamanho = (int)(sizeof(arquivosTribunais) / sizeof(arquivosTribunais[0]));
    int opcao;

    /* CONCATENAÇÃO */
    
    FILE *teste = fopen("brasil.csv", "r");
    if (!teste) {
        printf("[INFO] brasil.csv nao encontrado. Concatenando arquivos...\n");
        if (!concatenar(arquivosTribunais, tamanho)) {
            printf("[ERRO] Falha na concatenacao. Encerrando.\n");
            return 1;
        }
    } else {
        fclose(teste);
    }

    do {
        printf("\nMenu:\n");
        printf("  1 - Gerar resumo por tribunal \n");
        printf("  2 - Buscar ocorrencias por municipio\n");
        printf("  0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        getchar(); 

        switch (opcao) {
            case 1:
                processarCSV("brasil.csv", "resumo.csv");
                break;

            case 2: {
                char municipio[100];
                printf("Informe o municipio: ");
                fgets(municipio, sizeof(municipio), stdin);
                municipio[strcspn(municipio, "\n")] = '\0';
                buscarMunicipio("brasil.csv", municipio);
                break;
            }

            case 0:
            
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    return 0;
}
