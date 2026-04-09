#include <stdio.h>
#include <stdlib.h>

typedef struct Item{
    
    int Chave;
    struct Item * Proximo; // responsavel encadeamento 
} Item;

Item * CriarItem(int Chave);

int main() {

    system ("cls");

    Item * A = CriarItem(25);
    Item * B = CriarItem(30);
    if (A == NULL) {
        printf("ERRO: nao ha memoria para armazenar um item da lista!\n");
        exit(1);
    }

    A->Chave = 25;
    A->Proximo = NULL; // nao ha ninguem depois de A

    printf("%X \t %d \t %X \n", A, A->Chave, A->Proximo);

    free(A);

    return 0;

}

Item * CriarItem(int Chave) {
    Item * I = (Item *) malloc (sizeof(Item));

    if (I == NULL) {
        printf("ERRO: nao ha memoria para armazenar um item da lista!\n");
        return NULL;
    }

    I->Chave = Chave;
    I->Proximo = NULL;
    printf("%X \t %d \t %X \n", I, I->Chave, I->Proximo);
    return I;
}