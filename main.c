#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
// Constantes
#define ROWS 5
#define COLS 5
#define MAX_BILHETES 100
#define FILMES 4 // Número de filmes disponíveis

// Struct para armazenar informações de um bilhete
typedef struct {
    int bilheteNum;
    int linha;
    int coluna;
    char filme[50];
    int meiaEntrada; // 1 para meia-entrada, 0 para inteiro
    int combo;       // 1 para incluir combo, 0 para não
    float preco;
    char data[20];
} Bilhete;

// Variáveis globais
char matriz[FILMES][ROWS][COLS]; // Matriz tridimensional para cada filme
Bilhete bilhetes[MAX_BILHETES];  // Bilhetes vendidos
int totalBilhetes = 0;           // Contador de bilhetes
float totalArrecadado = 0;       // Total arrecadado
int totalCombos = 0;             // Total de combos vendidos

// Funções auxiliares
void inicializarSalas() {
    for (int filme = 0; filme < FILMES; filme++) {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                matriz[filme][i][j] = 'D'; // 'D' para disponível
            }
        }
    }
}

void exibirSala(int filme) {
    setlocale(LC_ALL, "Portuguese");
    printf("\n--- Disposição dos Lugares para o Filme %d ---\n", filme + 1);
    printf("   ");
    for (int j = 0; j < COLS; j++) {
        printf(" %d ", j);
    }
    printf("\n");

    for (int i = 0; i < ROWS; i++) {
        printf(" %d ", i);
        for (int j = 0; j < COLS; j++) {
            printf("[%c]", matriz[filme][i][j]);
        }
        printf("\n");
    }
}

void exibirFilmes() {
    setlocale(LC_ALL, "Portuguese");
    printf("\n--- Filmes Disponíveis ---\n");
    printf("1. Terrifier 3\n");
    printf("2. Star Wars\n");
    printf("3. As Aventuras de PI\n");
    printf("4. Interestellar\n");
}

void obterDataAtual(char *buffer) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer, "%02d/%02d/%04d %02d:%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
}

int reservarLugar() {
    setlocale(LC_ALL, "Portuguese");
    int linha, coluna, meiaEntrada, combo, escolhaFilme;
    float preco = 20.0;
    char filmeEscolhido[50];

    exibirFilmes();
    printf("Escolha um filme (1-4): ");
    scanf("%d", &escolhaFilme);

    if (escolhaFilme < 1 || escolhaFilme > FILMES) {
        printf("Filme inválido.\n");
        return -1;
    }

    int filmeIndex = escolhaFilme - 1;
    strcpy(filmeEscolhido, escolhaFilme == 1 ? "Terrifier 3" :
                           escolhaFilme == 2 ? "Star Wars" :
                           escolhaFilme == 3 ? "As Aventuras de PI" : "Interestellar");

    
    exibirSala(filmeIndex);
    // Adicionando opção para cancelar e voltar ao menu principal
    printf("Se desejar cancelar a operação, digite '-1 -1'.\n");
    printf("Escolha um lugar (linha e coluna): ");
    scanf("%d %d", &linha, &coluna);

    // Verifica se o usuário deseja cancelar
    if (linha == -1 && coluna == -1) {
    printf("Operação cancelada. Retornando ao menu principal.\n");
    return -1;
    }
    

    if (linha < 0 || linha >= ROWS || coluna < 0 || coluna >= COLS || matriz[filmeIndex][linha][coluna] == 'X') {
        printf("Lugar inválido ou já ocupado.\n");
        return -1;
    }

    matriz[filmeIndex][linha][coluna] = 'X'; // Marca como ocupado

    printf("Escolha o tipo de ingresso: (1-Meia Entrada(10 reais), 0-Inteiro(20 reais)): ");
    scanf("%d", &meiaEntrada);
    if (meiaEntrada) preco /= 2;

    printf("Deseja incluir combo (pipoca + refrigerante)? por 10 reais (1-Sim, 0-Não): ");
    scanf("%d", &combo);
    if (combo) {
        preco += 10.0;
        totalCombos++;
    }

    char data[20];
    obterDataAtual(data);

    // Criação do bilhete
    Bilhete novoBilhete = {totalBilhetes + 1, linha, coluna, "", meiaEntrada, combo, preco, ""};
    strcpy(novoBilhete.filme, filmeEscolhido);
    strcpy(novoBilhete.data, data);
    bilhetes[totalBilhetes++] = novoBilhete;

    totalArrecadado += preco;

    printf("\nReserva realizada com sucesso!\n");
    printf("Número do Bilhete: %d\n", novoBilhete.bilheteNum);
    printf("Filme: %s\n", novoBilhete.filme);
    printf("Data: %s\n", novoBilhete.data);
    return novoBilhete.bilheteNum;
}

void imprimirBilhete() {
    setlocale(LC_ALL, "Portuguese");
    int bilheteNum;
    printf("Digite o número do bilhete: ");
    scanf("%d", &bilheteNum);

    for (int i = 0; i < totalBilhetes; i++) {
        if (bilhetes[i].bilheteNum == bilheteNum) {
            printf("\n--- Bilhete #%d ---\n", bilhetes[i].bilheteNum);
            printf("Filme: %s\n", bilhetes[i].filme);
            printf("Lugar: Linha %d, Coluna %d\n", bilhetes[i].linha, bilhetes[i].coluna);
            printf("Tipo: %s\n", bilhetes[i].meiaEntrada ? "Meia-Entrada" : "Inteiro");
            printf("Combo: %s\n", bilhetes[i].combo ? "Sim" : "Não");
            printf("Preço: R$ %.2f\n", bilhetes[i].preco);
            printf("Data: %s\n", bilhetes[i].data);
            return;
        }
    }
    printf("Bilhete não encontrado.\n");
}

void cancelarCompra() {
    setlocale(LC_ALL, "Portuguese");
    int bilheteNum;
    printf("Digite o número do bilhete a ser cancelado: ");
    scanf("%d", &bilheteNum);

    for (int i = 0; i < totalBilhetes; i++) {
        if (bilhetes[i].bilheteNum == bilheteNum) {
            int filmeIndex = -1;
            if (strcmp(bilhetes[i].filme, "Terrifier 3") == 0) filmeIndex = 0;
            else if (strcmp(bilhetes[i].filme, "Star Wars") == 0) filmeIndex = 1;
            else if (strcmp(bilhetes[i].filme, "As Aventuras de PI") == 0) filmeIndex = 2;
            else if (strcmp(bilhetes[i].filme, "Interestellar") == 0) filmeIndex = 3;

            if (filmeIndex >= 0) {
                matriz[filmeIndex][bilhetes[i].linha][bilhetes[i].coluna] = 'D'; // Libera o lugar
            }

            totalArrecadado -= bilhetes[i].preco;
            if (bilhetes[i].combo) totalCombos--;

            printf("Compra cancelada com sucesso! Valor R$ %.2f será reembolsado.\n", bilhetes[i].preco);
            bilhetes[i] = bilhetes[--totalBilhetes]; // Remove o bilhete
            return;
        }
    }
    printf("Bilhete não encontrado.\n");
}

void exibirRelatorio() {
    setlocale(LC_ALL, "Portuguese");
    printf("\n--- Relatório Final ---\n");
    printf("Total de ingressos vendidos: %d\n", totalBilhetes);
    printf("Total de combos vendidos: %d\n", totalCombos);
    printf("Total arrecadado: R$ %.2f\n", totalArrecadado);
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    int opcao;
    inicializarSalas();

    do {
        printf("\n--- Sistema de Bilheteria ---\n");
        printf("1. Reservar Lugar\n");
        printf("2. Imprimir Bilhete\n");
        printf("3. Cancelar Compra\n");
        printf("4. Exibir Relatório Final\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: reservarLugar(); break;
            case 2: imprimirBilhete(); break;
            case 3: cancelarCompra(); break;
            case 4: exibirRelatorio(); break;
            case 5: printf("Encerrando o sistema. Até logo!\n"); break;
            default: printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 5);

    return 0;
}
