#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lista.h"

void strToUpper(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = (char)toupper((unsigned char)str[i]);
}

void removerAspas(char *str) {
    if (!str || !*str) return;
    int len = (int)strlen(str);
    while (len > 0 && (str[len-1] == '\r' || str[len-1] == '\n'))
        str[--len] = '\0';
    if (len > 0 && str[len-1] == '"') str[--len] = '\0';
    if (str[0] == '"') memmove(str, str + 1, len);  
}

int splitCSV(char *linha, char *campos[], int maxCampos) {
    int n = 0;
    char *p = linha;

    while (*p && n < maxCampos) {
        if (*p == '"') {

            p++;
            campos[n++] = p;
            while (*p && !(*p == '"' && (*(p+1) == ',' || *(p+1) == '\0'
                                         || *(p+1) == '\r' || *(p+1) == '\n')))
                p++;
            if (*p == '"') *p++ = '\0';
        } else {
            campos[n++] = p;
            while (*p && *p != ',') p++;
        }
        if (*p == ',') *p++ = '\0';
    }
    return n;
}

int buscarTribunal(Tribunal tribunais[], int n, char *sigla) {
    for (int i = 0; i < n; i++)
        if (strcmp(tribunais[i].sigla, sigla) == 0)
            return i;
    return -1;
}

int concatenar(const char *arquivosTribunais[], int tamanho) {
    FILE *f1, *f2;

    f2 = fopen("brasil.csv", "w");
    if (!f2) {
        printf("ERRO: nao foi possivel criar brasil.csv\n");
        return 0;
    }

    char buffer[MAX_LINHA];
    int cabecalho = 0;

    for (int i = 0; i < tamanho; i++) {
        f1 = fopen(arquivosTribunais[i], "r");
        if (!f1) {
            printf("ERRO: nao foi possivel abrir %s\n", arquivosTribunais[i]);
            continue;
        }

        if (fgets(buffer, sizeof(buffer), f1)) {
            if (!cabecalho) {
                fprintf(f2, "%s", buffer);
                cabecalho = 1;
            }
        }

        while (fgets(buffer, sizeof(buffer), f1))
            fprintf(f2, "%s", buffer);

        fclose(f1);
    }

    fclose(f2);
    return 1;
}

void processarCSV(const char *entradaNome, const char *saidaNome) {
    FILE *entrada = fopen(entradaNome, "r");
    FILE *saida   = fopen(saidaNome, "w");

    if (!entrada || !saida) {
        printf("ERRO: nao foi possivel abrir arquivos de processamento\n");
        if (entrada) fclose(entrada);
        if (saida)   fclose(saida);
        return;
    }

    Tribunal tribunais[MAX_TRIBUNAIS];
    memset(tribunais, 0, sizeof(tribunais));
    int qtd = 0;

    char linha[MAX_LINHA];
    fgets(linha, MAX_LINHA, entrada); 

    while (fgets(linha, MAX_LINHA, entrada)) {
        char *campos[MAX_CAMPOS];
        int n = splitCSV(linha, campos, MAX_CAMPOS);
        if (n < 33) continue;

        char sigla[20];
        strncpy(sigla, campos[0], sizeof(sigla) - 1);
        sigla[sizeof(sigla)-1] = '\0';
        removerAspas(sigla);

        double casos_novos_2026    = atof(campos[10]);
        double julgados_2026       = atof(campos[11]);
        double suspensos_2026      = atof(campos[13]);
        double dessobrestados_2026 = atof(campos[14]);

        double distm2_a  = atof(campos[16]);
        double julgm2_a  = atof(campos[17]);
        double suspm2_a  = atof(campos[18]);

        double distm2_ant = atof(campos[20]);
        double julgm2_ant = atof(campos[21]);
        double suspm2_ant = atof(campos[22]);
        double desom2_ant = atof(campos[23]);

        double distm4_a = atof(campos[25]);
        double julgm4_a = atof(campos[26]);
        double suspm4_a = atof(campos[27]);

        double distm4_b = atof(campos[29]);
        double julgm4_b = atof(campos[30]);
        double suspm4_b = atof(campos[31]);

        int idx = buscarTribunal(tribunais, qtd, sigla);
        if (idx == -1) {
            idx = qtd++;
            strncpy(tribunais[idx].sigla, sigla, sizeof(tribunais[idx].sigla) - 1);
        }

        Tribunal *t = &tribunais[idx];

        t->julgados_2026       += julgados_2026;
        t->casos_novos_2026    += casos_novos_2026;
        t->dessobrestados_2026 += dessobrestados_2026;
        t->suspensos_2026      += suspensos_2026;

        t->julgm2_a  += julgm2_a;
        t->distm2_a  += distm2_a;
        t->suspm2_a  += suspm2_a;

        t->julgm2_ant += julgm2_ant;
        t->distm2_ant += distm2_ant;
        t->suspm2_ant += suspm2_ant;
        t->desom2_ant += desom2_ant;

        t->julgm4_a += julgm4_a;
        t->distm4_a += distm4_a;
        t->suspm4_a += suspm4_a;

        t->julgm4_b += julgm4_b;
        t->distm4_b += distm4_b;
        t->suspm4_b += suspm4_b;
    }

    fprintf(saida, "sigla_tribunal,julgados_2026,Meta1,Meta2A,Meta2Ant,Meta4A,Meta4B\n");

    for (int i = 0; i < qtd; i++) {
        Tribunal t = tribunais[i];

        double Meta1 = 0, Meta2A = 0, Meta2Ant = 0, Meta4A = 0, Meta4B = 0;

        double d1    = t.casos_novos_2026 + t.dessobrestados_2026 - t.suspensos_2026;
        double d2A   = t.distm2_a  - t.suspm2_a;
        double d2Ant = t.distm2_ant - t.suspm2_ant - t.desom2_ant;
        double d4A   = t.distm4_a  - t.suspm4_a;
        double d4B   = t.distm4_b  - t.suspm4_b;

        if (d1    != 0) Meta1    = (t.julgados_2026 / d1)    * 100.0;
        if (d2A   != 0) Meta2A   = (t.julgm2_a  / d2A)   * (1000.0 / 7.0);
        if (d2Ant != 0) Meta2Ant = (t.julgm2_ant / d2Ant) * 100.0;
        if (d4A   != 0) Meta4A   = (t.julgm4_a  / d4A)   * 100.0;
        if (d4B   != 0) Meta4B   = (t.julgm4_b  / d4B)   * 100.0;

        fprintf(saida, "%s,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
                t.sigla, t.julgados_2026,
                Meta1, Meta2A, Meta2Ant, Meta4A, Meta4B);
    }

    fclose(entrada);
    fclose(saida);

    printf("Resumo gerado: %s (%d tribunais)\n", saidaNome, qtd);
}

void buscarMunicipio(const char *entradaNome, const char *municipio) {

    Municipio fm;
    memset(&fm, 0, sizeof(fm));
    fm.col_municipio = 5;  

    
    strncpy(fm.municipio, municipio, sizeof(fm.municipio) - 1);
    strToUpper(fm.municipio);

    removerAspas(fm.municipio);

    char nomeArquivo[120];
    snprintf(nomeArquivo, sizeof(nomeArquivo), "%s.csv", fm.municipio);

    fm.entrada = fopen(entradaNome, "r");
    fm.saida   = fopen(nomeArquivo, "w");

    if (!fm.entrada || !fm.saida) {
        printf("Erro ao abrir arquivos para filtro de municipio\n");
        if (fm.entrada) fclose(fm.entrada);
        if (fm.saida)   fclose(fm.saida);
        return;
    }

    char cabecalho[MAX_LINHA];
    if (!fgets(cabecalho, MAX_LINHA, fm.entrada)) {
        printf("Arquivo de entrada vazio.\n");
        fclose(fm.entrada);
        fclose(fm.saida);
        return;
    }

    fprintf(fm.saida, "%s", cabecalho);

    char linha[MAX_LINHA];
    while (fgets(linha, MAX_LINHA, fm.entrada)) {
        char copia[MAX_LINHA];
        strncpy(copia, linha, MAX_LINHA - 1);

        char *campos[MAX_CAMPOS];
        int n = splitCSV(copia, campos, MAX_CAMPOS);
        if (n <= fm.col_municipio) continue;

        char mun[100];
        strncpy(mun, campos[fm.col_municipio], sizeof(mun) - 1);
        mun[sizeof(mun)-1] = '\0';
        removerAspas(mun);
        strToUpper(mun);

        if (strcmp(mun, fm.municipio) == 0) {
            fprintf(fm.saida, "%s", linha);
            fm.registros++;
        }
    }

    fclose(fm.entrada);
    fclose(fm.saida);

    if (fm.registros > 0)
        printf("Arquivo gerado: %s (%ld registros encontrados)\n",
               nomeArquivo, fm.registros);
    else {
        printf("Nenhum registro encontrado para '%s'. Arquivo removido.\n",
               fm.municipio);
        remove(nomeArquivo);
    }
}
