#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void crypt(char *text, int chav);
void decrypt(char *text, int chav);
void addUsr();
void lstUsr();
void showSenha();
void updtUsr();
void delUsr();

int main() {
    
    int opcao;
    
    do {

        printf("\n1 - Adicionar Usuario\n");
        printf("2 - Listar Usuarios\n");
        printf("3 - Atualizar Usuario\n");
        printf("4 - Deletar Usuario\n");
        printf("5 - Mostrar Senha do Usuario\n");
        printf("0 - Sair\n");

        printf("Digite uma opcao: ");
        scanf("%d", &opcao);
        
        switch(opcao) {

            case 1:
                addUsr();
                break;

            case 2:
                lstUsr();
                break;

            case 3:
                updtUsr();
                break;

            case 4:
                delUsr();
                break;

            case 5:
                showSenha();
                break;

        }

    } while(opcao != 0);
    
    return 0;

}

/*-Funcoes de criptografia e descriptografia da senha-*/
void crypt(char *text, int chav) {

    int i;

    for(i = 0; text[i] != '\0'; i++) {

        text[i] = text[i] + chav;

    }

}

void decrypt(char *text, int chav) {

    int i;

    for(i = 0; text[i] != '\0'; i++) {

        text[i] = text[i] - chav;

    }

}
/*------------Fim Cript e Decript de senha------------*/

/*---------------Funcoes de Gerenciamentos dos usuarios---------------*/
/*--------Adicionar Usuario--------*/
void addUsr() {

    char usuario[100], senha[100];
    int perm;
    
    printf("Nome de Usuario: ");
    scanf("%s", usuario);
    
    printf("Senha: ");
    scanf("%s", senha);
    
    printf("Permissao (1 - Admin, 2 - Usuario padrao): ");
    scanf("%d", &perm);
    
    //Faz a criptografia da senha
    srand(time(NULL));
    int chav = rand() % 100 + 1;
    crypt(senha, chav);
    
    //Salva em um arquivo de nome users.txt
    FILE *file = fopen("users.txt", "a");

    if(file) {

        fprintf(file, "%s %s %d %d %d\n", usuario, senha, perm, 1, chav);
        fclose(file);
        
        printf("Usuario Registrado!\n");

    } else {

        printf("Nao foi possivel adicionar o usuario.\n");

    }

}
/*------------Fim Add Usu----------*/

/*-------------Listagem De Usuarios-------------*/
void lstUsr() {

    char usuario[100], senha[100];
    int perm, ativ, chav;
    
    printf("\nLista de Usuarios:\n");
    printf("Usuario\t\tPermissao\tStatus\n");
    
    FILE *file = fopen("users.txt", "r");

    if(file) {

        while(fscanf(file, "%s %s %d %d %d", usuario, senha, &perm, &ativ, &chav) == 5) {

            if(ativ) {

                printf("%s\t\t%d\t\t%s\n", usuario, perm, ativ ? "Ativo" : "Inativo");

            }

        }

        fclose(file);

    } else {

        printf("Nao existe nenhum usuario registrado.\n");

    }

}
/*-----------------Fim List Usu-----------------*/

/*--------Mostrar senha armazenada descriptografada--------*/
void showSenha() {
    
    char procusr[100], usuario[100], senha[100];
    int perm, ativ, chav;
    int encntrd = 0;
    
    printf("Escreva o usuario que deseja ver a senha: ");
    scanf("%s", procusr);
    
    FILE *file = fopen("users.txt", "r");

    if(file) {

        while(fscanf(file, "%s %s %d %d %d", usuario, senha, &perm, &ativ, &chav) == 5) {

            if(strcmp(usuario, procusr) == 0 && ativ) {

                char decriptSen[100];
                strcpy(decriptSen, senha);
                decrypt(decriptSen, chav);

                printf("Senha do usuario %s: %s\n", usuario, decriptSen);

                encntrd = 1;

                break;

            }

        }

        fclose(file);
        
        if(!encntrd) {

            printf("Nao foi encontrado o usuario, ou o mesmo esta inativo.\n");

        }

    }

}
/*-------------Fim most senha descriptografada-------------*/

/*-------------Atualizacao de Dados do Usuario-------------*/
void updtUsr() {
    char procusr[100], usuario[100], senha[100];
    int perm, ativ, chav;
    int encntrd = 0;
    
    printf("Digite o usuario para alterar: ");
    scanf("%s", procusr);
    
    FILE *file = fopen("users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if(file && temp) {

        while(fscanf(file, "%s %s %d %d %d", usuario, senha, &perm, &ativ, &chav) == 5) {

            if(strcmp(usuario, procusr) == 0 && ativ) {

                printf("Nova senha: ");
                scanf("%s", senha);
                
                printf("Novo nivel de permissao (1-admin, 2-usuario normal): ");
                scanf("%d", &perm);
                
                // Criptografa nova senha
                chav = rand() % 100 + 1;
                crypt(senha, chav);
                encntrd = 1;

            }

            fprintf(temp, "%s %s %d %d %d\n", usuario, senha, perm, ativ, chav);

        }
        
        fclose(file);
        fclose(temp);
        
        remove("users.txt");
        rename("temp.txt", "users.txt");
        
        if(encntrd) {

            printf("Usuario Atualizado!\n");

        } else {

            printf("Nao foi possivel encontrar o usuario\n");

        }

    }

}
/*---------------------Fim Atu Usuario---------------------*/

/*--------------------Deletar Usuario--------------------*/
void delUsr() {

    char procusr[100], usuario[100], senha[100];
    int perm, ativ, chav;
    int encntrd = 0;
    
    printf("Digite o usuario para deletar: ");
    scanf("%s", procusr);
    
    FILE *file = fopen("users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if(file && temp) {

        while(fscanf(file, "%s %s %d %d %d", usuario, senha, &perm, &ativ, &chav) == 5) {

            if(strcmp(usuario, procusr) == 0) {

                ativ = 0;
                encntrd = 1;

            }

            if(ativ) {

                fprintf(temp, "%s %s %d %d %d\n", usuario, senha, perm, ativ, chav);

            }

        }
        
        fclose(file);
        fclose(temp);
        
        remove("users.txt");
        rename("temp.txt", "users.txt");
        
        if(encntrd) {

            printf("Usuario Deletado!\n");

        } else {

            printf("Nao foi possivel encontrar o usuario!\n");

        }

    }

}
/*--------------------Fim Del Usuario--------------------*/

/*---------------------Fim Func Geren de Usuario----------------------*/