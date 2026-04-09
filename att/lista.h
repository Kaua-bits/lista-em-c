#ifndef LISTA_H
#define LISTA_H

#define MAX_TRIBUNAIS 100
#define MAX_LINHA     4096
#define MAX_CAMPOS    40

/* CALCULOS DAS METAS */
typedef struct {
    char sigla[20];

    /* Meta 1 */
    double julgados_2026;
    double casos_novos_2026;
    double dessobrestados_2026;
    double suspensos_2026;

    /* Meta 2A */
    double julgm2_a, distm2_a, suspm2_a;

    /* Meta 2Ant */
    double julgm2_ant, distm2_ant, suspm2_ant, desom2_ant;

    /* Meta 4A */
    double julgm4_a, distm4_a, suspm4_a;

    /* Meta 4B */
    double julgm4_b, distm4_b, suspm4_b;

} Tribunal;

/* FILTRAR POR MUNICÍPIOS */

typedef struct {
    char   municipio[100];   
    FILE  *entrada;          
    FILE  *saida;            
    int    col_municipio;    
    long   registros;     
} Municipio;

int concatenar(const char *arquivosTribunais[], int tamanho);

void processarCSV(const char *entradaNome, const char *saidaNome);

void buscarMunicipio(const char *entradaNome, const char *municipio);

int    buscarTribunal(Tribunal tribunais[], int n, char *sigla);
void   removerAspas(char *str);
void   strToUpper(char *str);
int    splitCSV(char *linha, char *campos[], int maxCampos);

#endif 
