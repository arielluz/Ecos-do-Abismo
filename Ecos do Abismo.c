#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <ctype.h>
#include <time.h>

#define LINHAS 15
#define COLUNAS 21
#define PAREDE '#'
#define CAMINHO ' '
#define JOGADOR 'P'
#define SAIDA 'S'
#define CHAVE 'K'
#define FRAGMENTO 'F'

#define MAX_ITENS 20
#define TAM_NOME_ITEM 30

#define PRECO_ARCO 5
#define PRECO_CORDA 2
#define PRECO_LANTERNA 3
#define PRECO_ESCUDO 4
#define PRECO_ESPADA 5
#define PRECO_CHAVE 10
#define PRECO_PEDERNEIRA 1
#define PRECO_POCAO 4
#define PRECO_BARCO 8
#define PRECO_BATERIA_LANTERNA 4
#define PRECO_MAPA 7

typedef struct {
    int x;
    int y;
} Posicao;

void inicializarLabirinto(char labirinto[LINHAS][COLUNAS]);
void mostrarLabirinto(char labirinto[LINHAS][COLUNAS], Posicao jogador);
int moverJogador(char labirinto[LINHAS][COLUNAS], Posicao *jogador, int tecla);
void explorar_labirinto();
void adicionar_item(const char *nome_item);

char labirinto[LINHAS][COLUNAS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
    {1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1},
    {1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,1},
    {1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1},
    {1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
    {1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

int chave_coletada = 0;
int fragmento_coletado = 0;

void inicializarLabirinto(char labirinto[LINHAS][COLUNAS]) {
    labirinto[3][8] = CHAVE;     
    labirinto[10][15] = FRAGMENTO; 
}

void mostrarLabirinto(char labirinto[LINHAS][COLUNAS], Posicao jogador) {
    system("cls");
    printf("\n=== LABIRINTO DE CRISTAL ===\n\n");
    
    for(int i = 0; i < LINHAS; i++) {
        for(int j = 0; j < COLUNAS; j++) {
            if (i == jogador.y && j == jogador.x) {
                printf("%c ", JOGADOR); 
            } else {
                switch(labirinto[i][j]) {
                    case 0: printf("%c ", CAMINHO); break;  
                    case 1: printf("%c ", PAREDE); break;  
                    case 2: printf("%c ", SAIDA); break;   
                    case CHAVE: 
                        if (!chave_coletada) printf("%c ", CHAVE);
                        else printf("%c ", CAMINHO);
                        break;
                    case FRAGMENTO: 
                        if (!fragmento_coletado) printf("%c ", FRAGMENTO);
                        else printf("%c ", CAMINHO);
                        break;
                    default: printf("? "); break;
                }
            }
        }
        printf("\n");
    }
    
    printf("\nLEGENDA:\n");
    printf("%c = Voce\n", JOGADOR);
    printf("%c = Parede\n", PAREDE);
    printf("%c = Caminho\n", CAMINHO);
    printf("%c = Saida\n", SAIDA);
    printf("%c = Chave\n", CHAVE);
    printf("%c = Fragmento do Mapa\n", FRAGMENTO);
    
    printf("\nCONTROLES:\n");
    printf("W - Cima\n");
    printf("A - Esquerda\n");
    printf("S - Baixo\n");
    printf("D - Direita\n");
    printf("Q - Sair do labirinto\n");
    
    printf("\nSTATUS:\n");
    printf("Chave: %s\n", chave_coletada ? "Coletada" : "Nao encontrada");
    printf("Fragmento: %s\n", fragmento_coletado ? "Coletado" : "Nao encontrado");
}

int moverJogador(char labirinto[LINHAS][COLUNAS], Posicao *jogador, int tecla) {
    int novoX = jogador->x;
    int novoY = jogador->y;
    
    switch(tecla) {
        case 'w': case 'W': novoY--; break;
        case 's': case 'S': novoY++; break;
        case 'a': case 'A': novoX--; break;
        case 'd': case 'D': novoX++; break;
        default: return 0; 
    }
    
    if (novoX < 0 || novoX >= COLUNAS || novoY < 0 || novoY >= LINHAS) {
        return 0; 
    }
    
    if (labirinto[novoY][novoX] == 1) {
        return 0;
    }
    
    if (labirinto[novoY][novoX] == CHAVE && !chave_coletada) {
        chave_coletada = 1;
        printf("\n VOCE ENCONTROU A CHAVE!\n");
        Sleep(1000);
    }
    
    if (labirinto[novoY][novoX] == FRAGMENTO && !fragmento_coletado) {
        fragmento_coletado = 1;
        printf("\n  VOCE ENCONTROU UM FRAGMENTO DO MAPA!\n");
        Sleep(1000);
    }
    
    if (labirinto[novoY][novoX] == 2) {
        if (chave_coletada) {
            printf("\n VOCE CHEGOU NA SAIDA!\n");
            return 2;
        } else {
            printf("\n A SAIDA ESTA TRANCADA! PRECISA DA CHAVE.\n");
            Sleep(1000);
            return 0;
        }
    }
    
    jogador->x = novoX;
    jogador->y = novoY;
    return 1; 
}

void explorar_labirinto() {
    char labirinto_copia[LINHAS][COLUNAS];
    Posicao jogador = {1, 1}; 
    int tecla;
    int resultado;
    
    chave_coletada = 0;
    fragmento_coletado = 0;
    
    memcpy(labirinto_copia, labirinto, sizeof(labirinto));
    inicializarLabirinto(labirinto_copia);
    
    system("cls");
    printf("\n=== LABIRINTO DE CRISTAL ===\n\n");
    printf("Voce entrou em um misterioso labirinto de cristal!\n");
    printf("Encontre a chave para abrir a saida e procure pelo fragmento do mapa.\n\n");
    printf("Pressione ENTER para comecar...");
    getchar();
    
    while(1) {
        mostrarLabirinto(labirinto_copia, jogador);
        
        printf("\nDigite seu movimento (WASD) ou Q para sair: ");
        tecla = _getch();
        tecla = tolower(tecla);
        
        if (tecla == 'q') {
            printf("\nSaindo do labirinto...\n");
            Sleep(1000);
            return;
        }
        
        resultado = moverJogador(labirinto_copia, &jogador, tecla);
        
        if (resultado == 2) { 
            printf("\n PARABENS! VOCE CONSEGUIU SAIR DO LABIRINTO!\n");
            
            if (fragmento_coletado) {
                printf("Voce encontrou um fragmento do mapa do tesouro!\n");
                adicionar_item("Fragmento do Labirinto");
            }
            
            printf("\nPressione ENTER para continuar...");
            getchar();
            getchar();
            return;
        }
    }
}

typedef struct {
    char nome[TAM_NOME_ITEM];
    int quantidade;
} Item;

Item inventario[MAX_ITENS];
int num_itens = 0;
char local_escolhido[TAM_NOME_ITEM] = "vila";
int moedas = 6;
int barco_desbloqueado = 0;
int bateria_desbloqueada = 0;
int fragmentos_encontrados = 0;
int tesouro_encontrado = 0;
int vida_jogador = 10;
int max_vida = 10;

void printletraporletra(const char *mensagem, int delay_padrao, int delay_pausa);
void mostrar_inventario();
void adicionar_item(const char *nome_item);
int verificar_item_inventario(const char *nome_item);
void remover_item(const char *nome_item);
void desbloquear_item_loja(const char* item);
void chamarMenu();
void explorar_castelo();
void explorar_pantano();
void explorar_piramide();
void explorar_ilha_secreta();
void explorar_floresta();
void explorar_montanha();
void explorar_tesouro_final();
void batalha_com_monstro(const char* tipo_monstro, int forca_monstro);
void final_do_jogo();
void cena_pos_creditos();
void debug_contadores();
int tem_todos_fragmentos();
void mostrar_status();
void usar_pocao();
void mostrar_mapa();

int tem_todos_fragmentos() {
    int fragmentos_diferentes = 0;
    char fragmentos_coletados[6][TAM_NOME_ITEM] = {0};
    int count = 0;
    
    for (int i = 0; i < num_itens; i++) {
        if (strstr(inventario[i].nome, "Fragmento") != NULL) {
            int ja_tem = 0;
            for (int j = 0; j < count; j++) {
                if (strcmp(fragmentos_coletados[j], inventario[i].nome) == 0) {
                    ja_tem = 1;
                    break;
                }
            }
            if (!ja_tem) {
                strcpy(fragmentos_coletados[count], inventario[i].nome);
                count++;
                fragmentos_diferentes++;
            }
        }
    }
    
    return fragmentos_diferentes >= 4;
}

void debug_contadores() {
    printf("\n=== DEBUG CONTADORES ===\n");
    printf("Moedas: %d\n", moedas);
    printf("Vida: %d/%d\n", vida_jogador, max_vida);
    printf("Fragmentos encontrados: %d/4\n", fragmentos_encontrados);
    printf("Fragmentos no inventario: ");
    
    int fragmentos_diferentes = 0;
    char fragmentos_coletados[6][TAM_NOME_ITEM] = {0};
    int count = 0;
    
    for (int i = 0; i < num_itens; i++) {
        if (strstr(inventario[i].nome, "Fragmento") != NULL) {
            int ja_tem = 0;
            for (int j = 0; j < count; j++) {
                if (strcmp(fragmentos_coletados[j], inventario[i].nome) == 0) {
                    ja_tem = 1;
                    break;
                }
            }
            if (!ja_tem) {
                strcpy(fragmentos_coletados[count], inventario[i].nome);
                count++;
                fragmentos_diferentes++;
            }
            printf("%s (x%d) ", inventario[i].nome, inventario[i].quantidade);
        }
    }
    
    printf("\nFragmentos diferentes: %d/4\n", fragmentos_diferentes);
    printf("Local atual: %s\n", local_escolhido);
    printf("========================\n");
}

void printletraporletra(const char *mensagem, int delay_padrao, int delay_pausa) {
    for (int i = 0; mensagem[i] != '\0'; i++) {
        printf("%c", mensagem[i]);
        fflush(stdout);
        
        if (_kbhit()) {
            char tecla = _getch();
            if (tecla == '\r' || tecla == '\n') { 
                printf("%s", &mensagem[i + 1]); 
                fflush(stdout);
                return; 
            }
        }

        if (mensagem[i] == ',' || mensagem[i] == '.' || mensagem[i] == '?' || mensagem[i] == '!') {
            Sleep(delay_pausa);
        } else {
            Sleep(delay_padrao);
        }
    }
}

void mostrar_status() {
    printf("\n=== STATUS ===\n");
    printf("Vida: %d/%d\n", vida_jogador, max_vida);
    printf("Moedas: %d\n", moedas);
    printf("Fragmentos: %d/4\n", fragmentos_encontrados);
    printf("Local: %s\n", local_escolhido);
    printf("==============\n");
}

void mostrar_inventario() {
    system("cls");
    printf("\n=== INVENTARIO ===\n");
    mostrar_status();
    
    if (num_itens == 0) {
        printletraporletra("Seu inventario esta vazio.\n", 50, 300);
    } else {
        printf("Itens:\n");
        for (int i = 0; i < num_itens; i++) {
            printf("- %s", inventario[i].nome);
            if (inventario[i].quantidade > 1) printf(" (x%d)", inventario[i].quantidade);
            printf("\n");
        }
    }
    
    printf("\nPressione ENTER para continuar...");
    getchar();
}

void adicionar_item(const char *nome_item) {
    
    if (strstr(nome_item, "Fragmento") != NULL) {
        for (int i = 0; i < num_itens; i++) {
            if (strcmp(inventario[i].nome, nome_item) == 0) {
                printf("\nVoce ja tem este fragmento!\n");
                Sleep(1000);
                return;
            }
        }
        fragmentos_encontrados++;
    }
    
    for (int i = 0; i < num_itens; i++) {
        if (strcmp(inventario[i].nome, nome_item) == 0) {
            inventario[i].quantidade++;
            return;
        }
    }
    
    if (num_itens < MAX_ITENS) {
        strncpy(inventario[num_itens].nome, nome_item, TAM_NOME_ITEM-1);
        inventario[num_itens].nome[TAM_NOME_ITEM-1] = '\0';
        inventario[num_itens].quantidade = 1;
        num_itens++;
    }
}

int verificar_item_inventario(const char *nome_item) {
    for (int i = 0; i < num_itens; i++) {
        if (strcmp(inventario[i].nome, nome_item) == 0) {
            return inventario[i].quantidade;
        }
    }
    return 0;
}

void remover_item(const char *nome_item) {
    for (int i = 0; i < num_itens; i++) {
        if (strcmp(inventario[i].nome, nome_item) == 0) {
            if (inventario[i].quantidade > 1) {
                inventario[i].quantidade--;
            } else {
                for (int j = i; j < num_itens - 1; j++) {
                    strcpy(inventario[j].nome, inventario[j+1].nome);
                    inventario[j].quantidade = inventario[j+1].quantidade;
                }
                num_itens--;
            }
            return;
        }
    }
}

void usar_pocao() {
    if (verificar_item_inventario("Pocao de Cura") > 0) {
        vida_jogador = (vida_jogador + 5 > max_vida) ? max_vida : vida_jogador + 5;
        remover_item("Pocao de Cura");
        printf("Voce usou uma Pocao de Cura e recuperou 5 pontos de vida!\n");
        printf("Vida atual: %d/%d\n", vida_jogador, max_vida);
    } else {
        printf("Voce nao tem Pocao de Cura!\n");
    }
    Sleep(1500);
}

void desbloquear_item_loja(const char* item) {
    if (strcmp(item, "Barco Pequeno") == 0) {
        barco_desbloqueado = 1;
    } else if (strcmp(item, "Bateria Lanterna") == 0) {
        bateria_desbloqueada = 1;
    }
}

void mostrar_mapa() {
    system("cls");
    printf("\n=== MAPA DO MUNDO ===\n\n");
    printf("Locais disponiveis:\n");
    printf("1. Vila - Sua base inicial\n");
    printf("2. Floresta - Encontre recursos e fragmentos\n");
    printf("3. Castelo Sombrio - Enfrente guardas e encontre fragmentos\n");
    printf("4. Piramide Antiga - Resolva enigmas e encontre artefatos\n");
    printf("5. Pantano Proibido - Enfrente criaturas aquaticas\n");
    printf("6. Montanha dos Deuses - Desafio final antes do tesouro\n");
    printf("7. Ilha Secreta - Requer barco para acesso\n");
    printf("8. Tesouro Final - Local do tesouro de Kael\n");
    printf("9. Labirinto de Cristal - Encontre a chave e fragmentos\n");
    
    printf("\nFragmentos encontrados: %d/4\n", fragmentos_encontrados);
    printf("\nPressione ENTER para continuar...");
    getchar();
}

void chamarMenu() {
    int opcao;
    char input[10];
    
    while(1) {
        system("cls");
        printf("\n=== LOJA DO AMBROSIO ===\n");
        mostrar_status();
        
        printf("\nItens disponiveis:\n");
        printf("1 - Arco e flecha (%d moedas)\n", PRECO_ARCO);
        printf("2 - Corda (%d moedas)\n", PRECO_CORDA);
        printf("3 - Lanterna (%d moedas)\n", PRECO_LANTERNA);
        printf("4 - Escudo (%d moedas)\n", PRECO_ESCUDO);
        printf("5 - Espada (%d moedas)\n", PRECO_ESPADA);
        printf("6 - Chave (%d moedas)\n", PRECO_CHAVE);
        printf("7 - Pederneira (%d moedas)\n", PRECO_PEDERNEIRA);
        printf("8 - Pocao de Cura (%d moedas)\n", PRECO_POCAO);
        printf("9 - Bateria da Lanterna (%d moedas)\n", PRECO_BATERIA_LANTERNA);
        printf("10 - Ver inventario\n"); 
        printf("11 - Usar Pocao de Cura\n");
        printf("12 - Ver Mapa do Mundo\n");
        printf("13 - Voltar para %s\n", local_escolhido);
        printf("14 - Sair do jogo\n");
        
        if (barco_desbloqueado && !verificar_item_inventario("Barco Pequeno")) {
            printf("15 - Barco Pequeno (%d moedas)\n", PRECO_BARCO);
        }
        
        if (fragmentos_encontrados >= 2 && !verificar_item_inventario("Mapa Antigo")) {
            printf("16 - Mapa Antigo (%d moedas)\n", PRECO_MAPA);
        }
        
        printf("\nSua escolha: ");
        fgets(input, sizeof(input), stdin);
        opcao = atoi(input);
        
        switch(opcao) {
            case 1: 
                if (moedas >= PRECO_ARCO) {
                    moedas -= PRECO_ARCO;
                    adicionar_item("Arco e flecha");
                    printf("Arco e flecha comprado!\n");
                } else {
                    printf("Moedas insuficientes!\n");
                }
                break;
            case 2:
                if (moedas >= PRECO_CORDA) {
                    moedas -= PRECO_CORDA;
                    adicionar_item("Corda");
                    printf("Corda comprada!\n");
                } else {
                    printf("Moedas insuficientes!\n");
                }
                break;
            case 3:
                if (moedas >= PRECO_LANTERNA) {
                    moedas -= PRECO_LANTERNA;
                    adicionar_item("Lanterna");
                    printf("Lanterna comprada!\n");
                } else {
                    printf("Moedas insuficientes!\n");
                }
                break;
            case 4:
                if (moedas >= PRECO_ESCUDO) {
                    moedas -= PRECO_ESCUDO;
                    adicionar_item("Escudo");
                    printf("Escudo comprado!\n");
                } else {
                    printf("Moedas insuficientes!\n");
                }
                break;
            case 5:
                if (moedas >= PRECO_ESPADA) {
                    moedas -= PRECO_ESPADA;
                    adicionar_item("Espada");
                    printf("Espada comprada!\n");
                } else {
                    printf("Moedas insuficientes!\n");
                }
                break;
            case 6:
                if (moedas >= PRECO_CHAVE) {
                    moedas -= PRECO_CHAVE;
                    adicionar_item("Chave");
                    printf("Chave comprada!\n");
                } else {
                    printf("Moedas insuficientes!\n");
                }
                break;
            case 7:
                if (moedas >= PRECO_PEDERNEIRA) {
                    moedas -= PRECO_PEDERNEIRA;
                    adicionar_item("Pederneira");
                    printf("Pederneira comprada!\n");
                } else {
                    printf("Moedas insuficientes!\n");
                }
                break;
            case 8:
                if (moedas >= PRECO_POCAO) {
                    moedas -= PRECO_POCAO;
                    adicionar_item("Pocao de Cura");
                    printf("Pocao de Cura comprada!\n");
                } else {
                    printf("Moedas insuficientes!\n");
                }
                break;
            case 9:
                if (moedas >= PRECO_BATERIA_LANTERNA) {
                    moedas -= PRECO_BATERIA_LANTERNA;
                    adicionar_item("Bateria Lanterna");
                    printf("Bateria comprada!\n");
                } else {
                    printf("Moedas insuficientes!\n");
                }
                break;
            case 10:
                mostrar_inventario();
                break;
            case 11:
                usar_pocao();
                break;
            case 12:
                mostrar_mapa();
                break;
            case 13:
                return;
            case 14:
                printf("Saindo do jogo...\n");
                exit(0);
            case 15:
                if (barco_desbloqueado && !verificar_item_inventario("Barco Pequeno")) {
                    if (moedas >= PRECO_BARCO) {
                        moedas -= PRECO_BARCO;
                        adicionar_item("Barco Pequeno");
                        printf("Barco Pequeno comprado!\n");
                    } else {
                        printf("Moedas insuficientes! (Custa %d moedas)\n", PRECO_BARCO);
                    }
                } else {
                    printf("Opcao invalida ou barco ja adquirido!\n");
                }
                break;
            case 16:
                if (fragmentos_encontrados >= 2 && !verificar_item_inventario("Mapa Antigo")) {
                    if (moedas >= PRECO_MAPA) {
                        moedas -= PRECO_MAPA;
                        adicionar_item("Mapa Antigo");
                        printf("Mapa Antigo comprado!\n");
                    } else {
                        printf("Moedas insuficientes! (Custa %d moedas)\n", PRECO_MAPA);
                    }
                } else {
                    printf("Opcao invalida ou mapa ja adquirido!\n");
                }
                break;
            default:
                printf("Opcao invalida!\n");
        }
        Sleep(1000);
    }
}

void batalha_com_monstro(const char* tipo_monstro, int forca_monstro) {
    char escolha[10];
    int vida_monstro = forca_monstro * 2;
    int tem_pocao = verificar_item_inventario("Pocao de Cura");
    int tem_escudo = verificar_item_inventario("Escudo");
    int tem_espada = verificar_item_inventario("Espada");
    
    system("cls");
    printletraporletra("\nUm ", 50, 300);
    printletraporletra(tipo_monstro, 50, 300);
    printletraporletra(" aparece diante de voce!\n", 50, 700);
    
    if (tem_espada) {
        printletraporletra("Voce pega sua espada e se prepara para a batalha!\n", 50, 700);
        
        while (vida_jogador > 0 && vida_monstro > 0) {
            printf("\nStatus: Sua vida: %d/%d | Vida do %s: %d\n", 
                  vida_jogador, max_vida, tipo_monstro, vida_monstro);
            printletraporletra("\nO que voce faz?\n", 50, 300);
            printletraporletra("1 - Atacar com a espada\n", 50, 300);
            printletraporletra("2 - Defender\n", 50, 300);
            
            if (tem_pocao) {
                printletraporletra("3 - Usar Pocao de Cura (+5 de vida)\n", 50, 300);
            }
            
            printf("\nSua escolha: ");
            fgets(escolha, sizeof(escolha), stdin);
            escolha[strcspn(escolha, "\n")] = '\0';
            
            int dano_jogador = 0;
            int dano_monstro = 0;
            int defesa = 0;
            
            if (strcmp(escolha, "1") == 0) {
                dano_jogador = 2 + (rand() % 3);
                dano_monstro = 1 + (rand() % 2);
                printletraporletra("\nVoce ataca o ", 50, 300);
                printletraporletra(tipo_monstro, 50, 300);
                printf("!\n");
            } 
            else if (strcmp(escolha, "2") == 0) {
                defesa = 1;
                if (tem_escudo && rand() % 100 < 70) {
                    defesa = 2;
                    printletraporletra("\nVoce se defende com sucesso usando seu escudo!\n", 50, 300);
                } else {
                    printletraporletra("\nVoce tenta se defender!\n", 50, 300);
                }
                dano_monstro = 1 + (rand() % 3) - defesa;
                if (dano_monstro < 0) dano_monstro = 0;
            }
            else if (strcmp(escolha, "3") == 0 && tem_pocao) {
                usar_pocao();
                tem_pocao = verificar_item_inventario("Pocao de Cura");
                continue;
            }
            else {
                printletraporletra("\nAcao invalida! O monstro aproveita para atacar!\n", 50, 300);
                dano_monstro = 2 + (rand() % 2);
            }
            
            vida_monstro -= dano_jogador;
            vida_jogador -= dano_monstro;
            
            if (dano_jogador > 0) {
                printf("Voce causou %d de dano!\n", dano_jogador);
            }
            if (dano_monstro > 0) {
                printf("O %s causou %d de dano!\n", tipo_monstro, dano_monstro);
            }
            
            Sleep(1500);
        }
        
        if (vida_jogador > 0) {
            printletraporletra("\nVoce derrota o ", 50, 300);
            printletraporletra(tipo_monstro, 50, 300);
            printf("!\n");
            
            int moedas_ganhas = forca_monstro * 3 + (rand() % 8);
            moedas += moedas_ganhas;
            printf("Voce encontrou %d moedas!\n", moedas_ganhas);
            
            if (forca_monstro >= 3 && rand() % 100 < 50 && fragmentos_encontrados < 4) {
                char fragmentos[4][TAM_NOME_ITEM] = {
                    "Fragmento do Castelo",
                    "Fragmento do Pantano", 
                    "Fragmento da Montanha",
                    "Fragmento da Floresta"
                };
                adicionar_item(fragmentos[rand() % 4]);
                printletraporletra("Voce encontra um fragmento do mapa!\n", 50, 700);
            }
        } else {
            printletraporletra("\nVoce foi derrotado...\n", 50, 700);
            printletraporletra("\n========= VOCE REVIVEU NA VILA =========\n\n", 50, 900);
            
            vida_jogador = max_vida / 2;
            moedas = moedas / 2;
            if (moedas < 5) moedas = 5;
            printf("Voce acorda na vila com %d de vida e %d moedas!\n", vida_jogador, moedas);
            
            strcpy(local_escolhido, "vila");
            printf("\nPressione ENTER para continuar...");
            getchar();
        }
    } else {
        printletraporletra("Voce nao tem uma arma para enfrentar a criatura!\n", 50, 700);
        printletraporletra("\n========= VOCE REVIVEU NA VILA =========\n\n", 50, 900);
        
        vida_jogador = max_vida;
        int moedas_ajuda = 5 + (rand() % 6);
        moedas += moedas_ajuda;
        printf("Um anciao te resgata e te da %d moedas!\n", moedas_ajuda);
        
        printletraporletra("\nVoce precisara de uma espada!\n", 50, 700);
        strcpy(local_escolhido, "vila");
        printf("\nPressione ENTER para continuar...");
        getchar();
    }
}

void explorar_castelo() {
    char escolha_caminho[10];
    char escolha_vila[10];
    static int monstro_derrotado = 0;
    
    system("cls");
    printletraporletra("\n=== CASTELO SOMBRIO ===\n", 50, 300);
    printletraporletra("Voce adentra o castelo em ruinas...\n", 50, 700);
    
    if (!monstro_derrotado) {
        batalha_com_monstro("Guarda do Castelo", 3);
        if (vida_jogador > 0) {
            monstro_derrotado = 1;
        } else {
            return;
        }
    }

    printletraporletra("Voce encontra duas passagens a frente!\n", 50, 700);
    printletraporletra("Escolha: direita ou esquerda\n", 50, 700);
    
    printf("\nSua escolha: ");
    fgets(escolha_caminho, sizeof(escolha_caminho), stdin);
    escolha_caminho[strcspn(escolha_caminho, "\n")] = '\0';
    
    if (strcmp(escolha_caminho, "direita") == 0 || strcmp(escolha_caminho, "Direita") == 0) {
        printletraporletra("\nAo vasculhar o ambiente voce encontra 8 moedas brilhando!\n", 50, 700);
        moedas += 8;
        printletraporletra("Moedas adicionadas ao seu inventario!\n", 50, 700);
        
        if (rand() % 100 < 70) {
            printletraporletra("Voce tambem encontra um fragmento de mapa antigo!\n", 50, 700);
            adicionar_item("Fragmento do Castelo");
            printletraporletra("O fragmento mostra o caminho para o Pantano Proibido!\n", 50, 700);
        }
        
        while(1) {
            printletraporletra("\nPara onde deseja ir agora?\n", 50, 300);
            printletraporletra("1 - Ir para a piramide\n", 50, 300);
            printletraporletra("2 - Voltar para a vila\n", 50, 300);
            
            if (verificar_item_inventario("Fragmento do Castelo")) {
                printletraporletra("3 - Ir para o Pantano\n", 50, 300);
            }
            
            printf("\nSua escolha: ");
            fgets(escolha_vila, sizeof(escolha_vila), stdin);
            escolha_vila[strcspn(escolha_vila, "\n")] = '\0';
            
            if (strcmp(escolha_vila, "1") == 0) {
                strcpy(local_escolhido, "piramide");
                return;
            }
            else if (strcmp(escolha_vila, "2") == 0) {
                strcpy(local_escolhido, "vila");
                return;
            }
            else if (strcmp(escolha_vila, "3") == 0 && verificar_item_inventario("Fragmento do Castelo")) {
                strcpy(local_escolhido, "pantano");
                return;
            }
            else {
                printletraporletra("Opcao invalida! Tente novamente.\n", 50, 300);
            }
        }
    } 
    else if (strcmp(escolha_caminho, "esquerda") == 0 || strcmp(escolha_caminho, "Esquerda") == 0) {
        printletraporletra("\nUma flecha misteriosa te atinge!\n", 50, 700);
        vida_jogador -= 3;
        printf("Voce perdeu 3 pontos de vida! Vida atual: %d/%d\n", vida_jogador, max_vida);
        
        if (vida_jogador <= 0) {
            printletraporletra("Voce morreu...\n", 50, 700);
            printletraporletra("\n========= VOCE REVIVEU NA VILA =========\n\n", 50, 900);
            vida_jogador = max_vida / 2;
            moedas = moedas / 2;
            printf("Voce acorda na vila com %d de vida e %d moedas!\n", vida_jogador, moedas);
        }
        
        strcpy(local_escolhido, "vila");
        printf("\nPressione ENTER para continuar...");
        getchar();
        return;
    }
    else {
        printletraporletra("Opcao invalida! Voltando para a vila.\n", 50, 300);
        strcpy(local_escolhido, "vila");
        Sleep(1000);
    }
}

void explorar_piramide() {
    char escolha[30];
    system("cls");
    printletraporletra("\n=== PIRAMIDE ANTIGA ===\n", 50, 300);
    printletraporletra("Voce entra na piramide misteriosa...\n", 50, 700);
    
    if (verificar_item_inventario("Lanterna")) {
        printletraporletra("Sua lanterna revela hieroglifos misteriosos nas paredes.\n", 50, 700);
        
        if (rand() % 100 < 60) {
            printletraporletra("Voce decifra os hieroglifos e encontra um mapa antigo!\n", 50, 700);
            adicionar_item("Mapa Antigo");
        }
    } else {	
        printletraporletra("Esta muito escuro para ver anything. Voce precisa de uma lanterna.\n", 50, 700);
    }
    
    batalha_com_monstro("Guardiao da Piramide", 4);
    
    if (vida_jogador > 0) {
        printletraporletra("\nVoce encontra uma sala secreta com um pedestal antigo...\n", 50, 700);
        
        if (rand() % 100 < 70) {
            printletraporletra("Sobre o pedestal, ha um artefato dourado reluzente!\n", 50, 700);
            adicionar_item("Artefato Dourado");
            printletraporletra("Parabens! Voce encontrou o artefato escondido da piramide!\n", 50, 700);
            
            if (verificar_item_inventario("Mapa Antigo")) {
                printletraporletra("\nO artefato reage com o mapa e revela um novo destino!\n", 50, 800);
                printletraporletra("Nova area desbloqueada: ILHA SECRETA\n", 50, 700);
                desbloquear_item_loja("Barco Pequeno");
            }
        }
        
        if (rand() % 100 < 50) {
            printletraporletra("Voce tambem encontra um fragmento do mapa!\n", 50, 700);
            adicionar_item("Fragmento da Piramide");
        }
    }
    
    strcpy(local_escolhido, "vila");
    printf("\nPressione ENTER para voltar a vila...");
    getchar();
}

void explorar_pantano() {
    system("cls");
    printletraporletra("\n=== PANTANO PROIBIDO ===\n", 50, 300);
    printletraporletra("Voce adentra o pantano escuro e nebuloso...\n", 50, 700);
    
    batalha_com_monstro("Criatura do Pantano", 5);
    
    if (vida_jogador > 0) {
        if (rand() % 100 < 80) {
            printletraporletra("Voce encontra um fragmento do mapa!\n", 50, 700);
            adicionar_item("Fragmento do Pantano");
        }
        
        int tesouro_pantano = 10 + (rand() % 15);
        moedas += tesouro_pantano;
        printf("Voce encontrou %d moedas escondidas no pantano!\n", tesouro_pantano);
    }
    
    strcpy(local_escolhido, "vila");
    printf("\nPressione ENTER para voltar a vila...");
    getchar();
}

void explorar_ilha_secreta() {
    system("cls");
    printletraporletra("\n=== ILHA SECRETA ===\n", 50, 300);

    if (!(verificar_item_inventario("Barco Pequeno") || verificar_item_inventario("Barco Improvisado"))) {
        printletraporletra("Voce precisa de um barco para chegar na ilha!\n", 50, 700);
        printf("\nPressione ENTER para voltar...");
        getchar();
        return;
    }

    printletraporletra("Voce navega ate a ilha secreta...\n", 50, 700);
    batalha_com_monstro("Guardiao da Ilha", 6);
    
    if (vida_jogador > 0) {
        printletraporletra("Voce explora a ilha e encontra tesouros escondidos!\n", 50, 700);
        
        int tesouro_ilha = 20 + (rand() % 30);
        moedas += tesouro_ilha;
        printf("Voce encontrou %d moedas!\n", tesouro_ilha);
        
        if (rand() % 100 < 70) {
            printletraporletra("Voce encontra um fragmento do mapa muito antigo!\n", 50, 700);
            adicionar_item("Fragmento da Ilha");
        }
        
        if (rand() % 100 < 40) {
            printletraporletra("Voce encontra uma pocao rara!\n", 50, 700);
            adicionar_item("Pocao de Cura");
        }
    }
    
    strcpy(local_escolhido, "vila");
    printf("\nPressione ENTER para voltar a vila...");
    getchar();
}

void explorar_floresta() {
    system("cls");
    printletraporletra("\n=== FLORESTA ===\n", 50, 300);
    printletraporletra("Voce explora a floresta...\n", 50, 700);
    
    if (rand() % 100 < 40) {
        batalha_com_monstro("Animal Selvagem", 2);
    }
    
    if (vida_jogador > 0) {
        int recursos = 3 + (rand() % 7);
        moedas += recursos;
        printf("Voce encontrou %d moedas na floresta!\n", recursos);
        
        if (rand() % 100 < 30) {
            printletraporletra("Voce encontra um fragmento do mapa!\n", 50, 700);
            adicionar_item("Fragmento da Floresta");
        }
        
        if (rand() % 100 < 20) {
            printletraporletra("Voce encontra madeira para construir um barco!\n", 50, 700);
            adicionar_item("Barco Improvisado");
        }
    }
    
    strcpy(local_escolhido, "vila");
    printf("\nPressione ENTER para voltar a vila...");
    getchar();
}

void explorar_montanha() {
    system("cls");
    printletraporletra("\n=== MONTANHA DOS DEUSES ===\n", 50, 300);
    
    if (!tem_todos_fragmentos()) {
        printletraporletra("Voce precisa de pelo menos 4 fragmentos diferentes para escalar a montanha!\n", 50, 700);
        printf("\nPressione ENTER para voltar...");
        getchar();
        return;
    }
    
    printletraporletra("Voce escala a montanha sagrada...\n", 50, 700);
    batalha_com_monstro("Guardiao da Montanha", 8);
    
    if (vida_jogador > 0) {
        printletraporletra("No topo da montanha, voce encontra o templo sagrado!\n", 50, 700);
        
        if (rand() % 100 < 80) {
            printletraporletra("Dentro do templo, ha o ultimo fragmento do mapa!\n", 50, 700);
            adicionar_item("Fragmento da Montanha");
        }
        
        int tesouro_montanha = 30 + (rand() % 40);
        moedas += tesouro_montanha;
        printf("Voce encontrou %d moedas no templo!\n", tesouro_montanha);
        
        if (tem_todos_fragmentos()) {
            printletraporletra("\nVoce agora tem todos os fragmentos necessarios!\n", 50, 700);
            printletraporletra("Juntando-os, voce forma o Mapa do Tesouro Final!\n", 50, 700);
            adicionar_item("Mapa do Tesouro Final");
        }
    }
    
    strcpy(local_escolhido, "vila");
    printf("\nPressione ENTER para voltar a vila...");
    getchar();
}

void explorar_tesouro_final() {
    system("cls");
    
    if (!verificar_item_inventario("Mapa do Tesouro Final")) {
        printletraporletra("Voce precisa do Mapa do Tesouro Final!\n", 50, 700);
        printf("\nPressione ENTER para voltar...");
        getchar();
        return;
    }
    
    printletraporletra("\n=== TESOURO FINAL DE KAEL ===\n", 50, 300);
    printletraporletra("Seguindo o mapa, voce chega a caverna do tesouro...\n", 50, 700);
    
    batalha_com_monstro("Espirito de Kael", 10);
    
    if (vida_jogador > 0) {
        printletraporletra("Voce derrotou o espirito guardiao!\n", 50, 700);
        printletraporletra("A frente, uma sala repleta de ouro e joias!\n", 50, 700);
        printletraporletra("VOCE ENCONTROU O TESOURO PERDIDO DE KAEL!\n", 50, 700);
        
        int tesouro_final = 100 + (rand() % 200);
        moedas += tesouro_final;
        printf("Voce encontrou %d moedas no tesouro!\n", tesouro_final);
        
        tesouro_encontrado = 1;
        
        char escolha[10];
        printletraporletra("\nDeseja explorar os segredos finais? (sim/nao): ", 50, 300);
        fgets(escolha, sizeof(escolha), stdin);
        escolha[strcspn(escolha, "\n")] = 0;
        
        if (strcmp(escolha, "sim") == 0 || strcmp(escolha, "Sim") == 0) {
            cena_pos_creditos();
        } else {
            final_do_jogo();
        }
    }
}

void final_do_jogo() {
    system("cls");
    
    printf("\n\n");
    printf("  _________________________________________________________\n");
    printf(" /                                                         \\\n");
    printf("|                   TESOURO PERDIDO DE KAEL                 |\n");
    printf("|___________________________________________________________|\n");
    printf("|                                                           |\n");
    printf("|    Parabens! Voce completou a jornada e encontrou o       |\n");
    printf("|    tesouro perdido de Kael. Sua coragem e determinacao    |\n");
    printf("|    foram recompensadas com riquezas e gloria eterna.      |\n");
    printf("|                                                           |\n");
    printf("|    Que sua lenda seja cantada por todas as geracoes!      |\n");
    printf("|___________________________________________________________|\n\n");
    
    printf("  _________________________________________________________\n");
    printf(" /                                                         \\\n");
    printf("|                    ESTATISTICAS FINAIS                    |\n");
    printf("|___________________________________________________________|\n");
    printf("|                                                           |\n");
    printf("|     Fragmentos encontrados: %d/4                          |\n", fragmentos_encontrados);
    printf("|     Moedas acumuladas: %-10d                     |\n", moedas);
    printf("|     Itens coletados: %-15d                  |\n", num_itens);
    printf("|     Tesouro encontrado: %-10s                |\n", tesouro_encontrado ? "Sim" : "Nao");
    printf("|                                                           |\n");
    printf("|___________________________________________________________|\n\n");
    
    printletraporletra("Obrigado por jogar ECOS DO ABISMO!\n", 50, 700);
    
    printf("\nPressione ENTER para sair...");
    getchar();
    exit(0);
}

void cena_pos_creditos() {
    system("cls");
    
    printf("\n\n");
    printf("  _________________________________________________________\n");
    printf(" /                                                         \\\n");
    printf("|                   CENA POS-CREDITOS                      |\n");
    printf("|___________________________________________________________|\n\n");
    
    printletraporletra("Anos se passaram desde que voce descobriu o tesouro...\n\n", 50, 700);
    printletraporletra("Voce se tornou uma lenda, e sua historia e contada para todas as criancas.\n\n", 50, 700);
    printletraporletra("Mas uma nova ameaca surge no horizonte, e a aventura continua...\n\n", 50, 700);
    
    printletraporletra("FIM... ou COMECO?\n\n", 50, 1000);
    
    printf("Pressione ENTER para sair...");
    getchar();
    exit(0);
}

int main() {
    char escolha[10];
    char nome[20];
    
    num_itens = 0;
    moedas = 10;
    fragmentos_encontrados = 0;
    tesouro_encontrado = 0;
    vida_jogador = 10;
    max_vida = 10;
    srand(time(NULL));
    
    system("cls");
    printf("\n\n");
    printf("  _________________________________________________________\n");
    printf(" /                                                         \\\n");
    printf("|                    ECOS DO ABISMO                        |\n");
    printf("|___________________________________________________________|\n\n");
    
    printletraporletra("Bem-vindo a uma aventura epica em busca do tesouro perdido de Kael!\n\n", 50, 500);
    
    printf("Digite seu nome: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;
    
    printf("\nOla, %s! Eu sou Ambrosio, o pirata.\n", nome);
    printletraporletra("Preciso de sua ajuda para encontrar o tesouro perdido de Kael!\n\n", 50, 500);
    
    printf("Voce aceita esta missao? (sim/nao): ");
    fgets(escolha, sizeof(escolha), stdin);
    escolha[strcspn(escolha, "\n")] = 0;
    
    if (strcmp(escolha, "nao") == 0 || strcmp(escolha, "Nao") == 0) {
        printf("\nQue pena! Talvez outra hora...\n");
        return 0;
    }
    
    printletraporletra("\nExcelente! Sua jornada comeca agora.\n", 50, 500);
    printletraporletra("Voce recebeu 10 moedas para comecar.\n", 50, 500);
    printletraporletra("Visite a loja para equipamentos antes de explorar.\n\n", 50, 500);
    
    printf("Pressione ENTER para continuar...");
    getchar();
    
    
    while (1) {
        system("cls");
        printf("\n=== MUNDO DE KAEL ===\n");
        mostrar_status();
        
        printf("\nOnde voce quer ir?\n");
        printf("1 - Loja do Ambrosio\n");
        printf("2 - Floresta\n");
        printf("3 - Castelo Sombrio\n");
        printf("4 - Piramide Antiga\n");
        printf("5 - Pantano Proibido\n");
        printf("6 - Montanha dos Deuses\n");
        printf("7 - Ilha Secreta\n");
        printf("8 - Tesouro Final\n");
        printf("9 - Labirinto de Cristal\n");
        printf("10 - Ver Inventario\n");
        printf("11 - Ver Mapa\n");
        printf("12 - Sair do Jogo\n");
        
        printf("\nSua escolha: ");
        fgets(escolha, sizeof(escolha), stdin);
        int opcao = atoi(escolha);
        
        switch(opcao) {
            case 1:
                chamarMenu();
                break;
            case 2:
                explorar_floresta();
                break;
            case 3:
                explorar_castelo();
                break;
            case 4:
                explorar_piramide();
                break;
            case 5:
                explorar_pantano();
                break;
            case 6:
                explorar_montanha();
                break;
            case 7:
                explorar_ilha_secreta();
                break;
            case 8:
                explorar_tesouro_final();
                break;
            case 9:
                explorar_labirinto();
                break;
            case 10:
                mostrar_inventario();
                break;
            case 11:
                mostrar_mapa();
                break;
            case 12:
                printf("Saindo do jogo...\n");
                return 0;
            default:
                printf("Opcao invalida!\n");
                Sleep(1000);
        }
        
        if (tesouro_encontrado) {
            break;
        }
    }
    
    return 0;
}
