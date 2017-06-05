
/*
 *  Trabalho pratico 2- PI
 *  Sistemas de Informacao
 *  UFG
 *
 *  Aluno:
 *    - Guilherme Ferreira Schults
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define LINHA 3
#define COLUNA 3


typedef struct
{
    int    linhas_count;
    int    separadores_count;
    int   *separadores;
    char  *rotulo;
    char **linhas;

}Janela;


// VARIAVEL GLOBAL

int jogada_Da_Vez;
char  jogador1[200];
char  jogador2[200];


// FUNCOES MENU

int menu();
void clear();
void regras_Do_jogo();

// OBTER DADOS DOS JOGADORES

void salvar_jogo(char *entrada);
void historico_jogador(void);
void cadastrar_jogadores(char*, char*);

// FUNCOES DO JOGO

void exibir_Tabuleiro_Jogo(int **);
void jogar(int **);
void jogada(int **);
int** criar_Matriz(int **);
int verificar_Local(int **, int, int);
int verificar_Linha(int **);
int verificar_Coluna(int **);
int verificar_Diagonal(int **);
int verificar_Empate(int **);
int verificar_Termino(int **, int);


// MANIPULACAO DOS DADOS DE EXIBICAO

Janela obter_janela();
void  adicionar_rotulo(Janela*, char*);
void  adicionar_linha(Janela*, char*);
void  adicionar_separador(Janela*);
void  exibir_janela(Janela);
void  exibir_mensagem(char*, char*);

// FUNCAO AUXILIAR

void print_many(char c, int count);
/*
 *
 */
int main()
{
    setlocale(LC_ALL, "Portuguese");

    int **tabuleiro,
          opcao_menu;

    do
    {
        jogada_Da_Vez = 1;
        opcao_menu = menu();

        if(opcao_menu == 1)
        {
            regras_Do_jogo();
        }
        else if(opcao_menu == 2)
        {
            cadastrar_jogadores(jogador1,jogador2);
            jogar(tabuleiro);
        }
        else if(opcao_menu == 3)
        {
            historico_jogador();
        }

    }while(opcao_menu);

    return 0;
}
/*
 *
 */
int menu()
{
    int opcao;

    Janela janela = obter_janela();
    adicionar_rotulo(&janela, "Jogo da Velha");
    adicionar_linha(&janela,  " 1 - Regras do Jogo");
    adicionar_linha(&janela,  " 2 - Jogar");
    adicionar_linha(&janela,  " 3 - Histórico dos Jogos");
    adicionar_linha(&janela,  " 0 - Sair");
    exibir_janela(janela);

    printf("\nOpção: ");
    scanf("%d", &opcao);

    switch(opcao)
    {
        case 3:
        case 2:
        case 1:
        case 0:
            break;
        default:
            clear();
            printf("Opcao inválida. Tente novamente!\n");
    }

    return opcao;
}
/*
 *
 */
void regras_Do_jogo()
{

    Janela janela = obter_janela();;
    adicionar_rotulo(&janela, "Regras - Intruções para o jogo!");
    adicionar_linha(&janela, "Para efetuar uma jogada informa-se a posição ");
    adicionar_linha(&janela, "da jogada ao modelo linha.coluna");
    adicionar_linha(&janela, "Exemplo: linha 2 | coluna 3 ");
    adicionar_linha(&janela, "Essa jogada irá marcar no tabuleiro o local indicado, para");
    adicionar_linha(&janela, "o dono da jogada, o lance na linha 2 coluna 3.");
    exibir_janela(janela);

}
/*
 *
 */
void cadastrar_jogadores(char * jogador1, char * jogador2)
{
    printf("Jogador 1: ");
    scanf(" %[^\n]s", jogador1);
    printf("Jogador 2: ");
    scanf(" %[^\n]s", jogador2);

}
/*
 *
 */
void historico_jogador()
{

    FILE *arquivo;
    char temp[200];

    arquivo = fopen("jogadores.txt", "r");

    if(arquivo == NULL)
    {
        printf("Erro.\nFalha ao abrir o arquivo jogadores.txt");
        exit(-1);
    }
    
    if(fscanf(arquivo," %[^\n]s", temp) == EOF)
    {
        printf("jogue uma partida antes!\n");
    }
    else
    {
        printf("%s\n", temp);

        while(fscanf(arquivo," %[^\n]s", temp) != EOF)
        {
            if(strcmp(temp, "|") == 0)
            {
                printf("\n");
            } else
            {
                printf("%s\n", temp);
            }
        }
    }

    fclose(arquivo);
}
/*
 *
 */
void salvar_jogo(char *temp)
{
        FILE *arquivo;

        arquivo = fopen("jogadores.txt", "a");

        if(arquivo == NULL)
        {
            printf("Erro.\nFalha ao abrir o arquivo jogadores.txt");
            exit(-1);
        }
        
        fprintf(arquivo, "Jogo:\n");
        fprintf(arquivo, "    %s vs %s\n", jogador1, jogador2);
        fprintf(arquivo, "    Resultado: ");
        
        if(strcmp(temp, jogador1) == 0)
        {
            fprintf(arquivo, "O jogador(a) %s é o vencedor(a) desta partida.\n", jogador1);

        }
        if(strcmp(temp, jogador2) == 0)
        {
            fprintf(arquivo, "O jogador(a) %s é o vencedor(a) desta partida.\n", jogador2);
        }
        if(strcmp(temp, "empate")== 0)
        {
            fprintf(arquivo, "Não houve ganhador. Jogo terminou empatado.\n");
        }

        fprintf(arquivo, "|\n");
        fclose(arquivo);
}
/*
 *
 */
void clear() // FUNÇÃO PARA LIMPAR A TELA ANTES DE COMEÇAR O JOGO
{
         int contador = 0;

         while(contador != 25)
         {
             putchar('\n');
             contador++;
         }
}
/*
 *
 */
int** criar_Matriz(int **tabuleiroDoJogo) // CRIANDO A MATRIZ 3X3 PARA O TABULEIRO DO JOGO
{
        int i,
        j;

        tabuleiroDoJogo = (int**)malloc(COLUNA * sizeof(int*)); //ALOCO AS COLUNAS PARA A MATRIZ

        for(i = 0; i < LINHA; i++)  // PERCORRE AS LINHAS DO VETOR DE PONTEIROS.
        {
            tabuleiroDoJogo[i] = (int*)malloc(LINHA * sizeof(int)); //PARA CADA COLUNA ALOCO UMA LINHA
            
            for(j = 0; j < COLUNA; j++)
            {
                tabuleiroDoJogo[i][j] = 0; // ZERANDO TODAS AS LINHAS DE CADA COLUNA
            }
        }
        
        return tabuleiroDoJogo;
}
/*
 *
 */
void exibir_Tabuleiro_Jogo(int **tabuleiroDoJogo) // EXIBE O TABULEIRO DO JOGO ZERADO PRONTO PARA JOGAR
{
        int i,
        j;
        putchar('\n');

        for(i = 0 ; i < LINHA ; i++)
        {
            for(j = 0 ; j < COLUNA ; j++)
            {
                if(tabuleiroDoJogo[i][j] == 0)
                {
                    printf("    ");
                }
                else if(tabuleiroDoJogo[i][j] == 1)
                {
                    printf("  X ");
                } else
                {
                    printf("  O ");
                }

                if(j != (COLUNA - 1)) // SE NAO FOR A ULTIMA COLUNA
                {
                     printf("|");
                }
            }
            putchar('\n');
        }
        putchar('\n');
}
/*
 *
 */
void jogar(int **tabuleiroDoJogo)
{

        tabuleiroDoJogo = criar_Matriz(tabuleiroDoJogo); // VARIVAEL TABULEIRODOJOGO RECEBE A FUNCAO CRIAR MATRIZ PARA CRIAR O TABULEIRO DO JOGO

        do // FAZ
        {
            clear(); // SALTAS AS LINHAS PARA INCIAR O JOGO
            exibir_Tabuleiro_Jogo(tabuleiroDoJogo); // CHAMA A FUNCAO DE EXIBIR O TABULEIRO DO JOGO
            jogada(tabuleiroDoJogo); // CHAMA A FUNCAO JOGADA PARA DAR INICIO AO JOGO

        }while(verificar_Termino(tabuleiroDoJogo, jogada_Da_Vez) != 1); // ENQUANTO O TERMINO DO JOGO NAO ACONTECE
}
/*
 *
 */
int verificar_Local(int **tabuleiroDoJogo, int i, int j) // VALIDAR POSICAO INSERIDA PELO USUARIO
{
    // POSICAO INVALIDA
    if(i < 0 || i > (LINHA - 1) || j < 0 || j > (COLUNA - 1) || tabuleiroDoJogo[i][j] != 0)
    {
        return 0;
    }
    // POSICAO VALIDA
    else
    {
        return 1;
    }
}
/*
 *
 */
int verificar_Linha(int **tabuleiroDoJogo)
{
        int i,
        j,
        soma;

        for(i = 0 ; i < LINHA; i++)
        {
            soma = 0;

            for(j = 0 ; j < COLUNA ; j++)
            {
                 soma += tabuleiroDoJogo[i][j];

                if(soma == LINHA || soma == (-1) * LINHA)
                {
                     return 1;
                }
            }
        }
        return 0;
}
/*
 *
 */
int verificar_Coluna(int **tabuleiroDoJogo)
{
        int i, j,
        soma;


        for(j = 0 ; j < COLUNA ; j++)
        {
            soma = 0;

            for(i = 0 ; i < LINHA ; i++)
            {
                soma += tabuleiroDoJogo[i][j];

                if(soma == COLUNA || soma == (-1) * COLUNA)
                {
                    return 1;
                }
            }
        }
        return 0;
}
/*
 *
 */
int verificar_Diagonal(int **tabuleiroDoJogo)
{
        int i,
        diagonal_Principal = 0,
        diagonal_Secundaria = 0;

        for(i = 0; i < LINHA ; i++)
        {
            diagonal_Principal += tabuleiroDoJogo[i][i];
            diagonal_Secundaria += tabuleiroDoJogo[i][LINHA - i - 1];
        }

        if(diagonal_Principal == LINHA || diagonal_Principal == (-1) * LINHA ||
           diagonal_Secundaria == LINHA || diagonal_Secundaria == (-1) * LINHA)
       {
             return 1;
       }

        return 0;
}
/*
 *
 */
int verificar_Empate(int **tabuleiroDoJogo)
{
        int i,
        j;

        for(i = 0 ; i < LINHA ; i++)
        {
             for(j = 0 ; j < COLUNA ; j++)
             {
                 if(tabuleiroDoJogo[i][j] == 0)
                 {
                     return 0;
                 }
             }
        }

        return 1;
}
/*
 *
 */
int verificar_Termino(int **tabuleiroDoJogo, int vez)
{
        int tmp;

        if(verificar_Linha(tabuleiroDoJogo))
        {
            if(((jogada_Da_Vez % 2) +1) == 1)
            {
                printf("Jogo finalizado. Jogador(a) %s venceu a partida. Parabéns !!!\n", jogador1);
            }
            else
            {
                printf("Jogo finalizado. Jogador(a) %s venceu a partida. Parabéns !!!\n", jogador2);
            }

            exibir_Tabuleiro_Jogo(tabuleiroDoJogo);

            tmp = (vez % 2) + 1;
            if(tmp == 1)
            {
                salvar_jogo(jogador1);
            } else
            {
                salvar_jogo(jogador2);
            }
            return 1;
        }

        if(verificar_Coluna(tabuleiroDoJogo))
        {

            if(((jogada_Da_Vez % 2) +1) == 1)
            {
                printf("Jogo finalizado. Jogador(a) %s venceu a partida. Parabéns !!!\n", jogador1);
            }else
            {
                printf("Jogo finalizado. Jogador(a) %s venceu a partida. Parabéns !!!\n", jogador2);
            }

            exibir_Tabuleiro_Jogo(tabuleiroDoJogo);

            tmp = (vez % 2) + 1;
            if(tmp == 1)
            {
                salvar_jogo(jogador1);
            } else
            {
                salvar_jogo(jogador2);
            }
            return 1;
        }

        if(verificar_Diagonal(tabuleiroDoJogo))
        {
            if(((jogada_Da_Vez % 2) +1) == 1)
            {
            printf("Jogo finalizado. Jogador(a) %s venceu a partida. Parabéns !!!\n", jogador1);
            }else
            {
            printf("Jogo finalizado. Jogador(a) %s venceu a partida. Parabéns !!!\n", jogador2);
            }

            exibir_Tabuleiro_Jogo(tabuleiroDoJogo);

            tmp = (vez % 2) + 1;
            if(tmp == 1)
            {
                salvar_jogo(jogador1);
            } else
            {
                salvar_jogo(jogador2);
            }
            return 1;
        }

        if(verificar_Empate(tabuleiroDoJogo))
        {
            printf("Jogo finalizado. Ocorreu um empate! !\n\n");
            exibir_Tabuleiro_Jogo(tabuleiroDoJogo);
            salvar_jogo("empate");
            return 1;
        }

        return 0;
}
/*
 *
 */
void jogada(int **tabuleiroDoJogo)
{
    int i,
    j;

    jogada_Da_Vez++;

    printf("\n--> Jogador: ");

    if(((jogada_Da_Vez % 2) +1) == 1)
    {
        printf("%s \n", jogador1);
    }else
    {
        printf("%s \n", jogador2);
    }

    do
    {
        printf("Linha: ");
        scanf("%d", &i);
        i--;

        printf("Coluna: ");
        scanf("%d", &j);
        j--;

        if(verificar_Local(tabuleiroDoJogo, i, j) == 0)  //verifica se o local escolhido pelo jogador esta ocupado ou nao existe
       {
            printf("Posição já está ocupada ou é inexistente. Escolha outra.\n");
       }


    } while(verificar_Local(tabuleiroDoJogo, i, j) == 0);

    if(jogada_Da_Vez % 2)
    {
         tabuleiroDoJogo[i][j] = -1;
    }else
    {
        tabuleiroDoJogo[i][j] = 1;
    }

}
/*
 *
 */

Janela obter_janela()
{
        Janela janela;
        janela.rotulo = "";
        janela.linhas = (char**) malloc( (janela.linhas_count = 0) );
        janela.separadores = (int*) malloc( (janela.separadores_count = 0) );

        return janela;
}

/*
 *
 */
void adicionar_rotulo(Janela *janela, char *rotulo)
{
        size_t rotulo_size = strlen(rotulo);

        janela->rotulo = (char*) malloc(sizeof(char) * (rotulo_size + 1));

        int i;
        for(i = 0; i < rotulo_size; i++)
            janela->rotulo[i] = rotulo[i];

        janela->rotulo[rotulo_size] = '\0';
}

/*
 *
 */
void adicionar_linha(Janela *janela, char *linha)
{
        size_t linha_size = strlen(linha),
        newline_count = 0,
        *newline_positions = (size_t*) malloc(0),
        i,
        j;

        char **linhas = (char**) malloc(sizeof(char*)),
        *linha_atual = (char*) malloc(sizeof(char) * linha_size);

        int linhas_size = 0,
        linha_atual_index = 0;

        for(i = 0; i <= linha_size; i++)
        {
            if(linha[i] == '\n' || i == linha_size)
            {
                linhas = (char**) realloc(linhas, ++linhas_size * sizeof(char*));
                linhas[linhas_size-1] = (char*) malloc(sizeof(char) * (linha_atual_index + 1));

                for(j = 0; j < linha_atual_index; j++)
                    linhas[linhas_size-1][j] = linha_atual[j];

                linhas[linhas_size-1][j] = '\0';
                linha_atual_index = 0;
            }
            else
            {
                linha_atual[linha_atual_index++] = linha[i];
            }
        }

        for(i = 0; i < linhas_size; i++)
        {

            int linhas_count = janela->linhas_count++,
            linha_size = strlen(linhas[i]);

            janela->linhas = (char**) realloc(janela->linhas, sizeof(char*) * janela->linhas_count);
            janela->linhas[linhas_count] = (char*) malloc(sizeof(char) * (linha_size + 1));

            for(j = 0; j < linha_size; j++)
                janela->linhas[linhas_count][j] = linhas[i][j];

            janela->linhas[linhas_count][linha_size] = '\0';
    }
}

/*
 *
 */
void adicionar_separador(Janela *janela)
{
        janela->separadores = (int*) realloc(janela->separadores, ++janela->separadores_count * sizeof(int));
        janela->separadores[janela->separadores_count-1] = janela->linhas_count - 1;
}

/*
 *
 */
void exibir_janela(Janela janela)
{
        size_t rotulo_size = strlen(janela.rotulo),
        linha_size = 0,
        rotulo_offset;

        int i, j;

        for(i = 0; i < janela.linhas_count; i++)
            if(strlen(janela.linhas[i]) > linha_size)
                linha_size = strlen(janela.linhas[i]);

        rotulo_offset = (linha_size - rotulo_size) / 2;

        if(rotulo_size >= linha_size)
            linha_size = rotulo_size;

        if((linha_size - rotulo_size) % 2 == 0)
            rotulo_offset = (linha_size - rotulo_size) / 2;
        else
            rotulo_offset = (++linha_size - rotulo_size) / 2;

        printf("\n +");
        print_many('-', linha_size+2);
        printf("+\n");

        if(rotulo_size > 0)
        {
            printf(" | ");
            print_many(' ', rotulo_offset);
            printf("%s", janela.rotulo);
            print_many(' ', rotulo_offset);
            printf(" |\n");
            printf(" +");
            print_many('-', linha_size+2);
            printf("+\n");
        }

        for(i = 0; i < janela.linhas_count; i++)
        {
            printf(" | %s", janela.linhas[i]);
            print_many(' ', linha_size - strlen(janela.linhas[i]));
            printf(" | \n");

            for(j = 0; j < janela.separadores_count; j++)
            {
                if(i == janela.separadores[j])
                {
                    printf(" +");
                    print_many('-', linha_size+2);
                    printf("+\n");
                }
            }
        }

        printf(" +");
        print_many('-', linha_size+2);
        printf("+\n");
}

/*
 *
 */
void exibir_mensagem(char *rotulo, char *mensagem)
{
        Janela janela = obter_janela();

        if(strlen(rotulo) > 0)
            adicionar_rotulo(&janela, rotulo);

        adicionar_linha(&janela, mensagem);
        exibir_janela(janela);
}

/*
 *
 */
void print_many(char c, int count)
{
        int i;
        for(i = 0; i < count; i++)
            printf("%c", c);
}
