#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void crypt(char *text, int chav);
void decrypt(char *text, int chav);
void addUsr();
void lstUsr();
void showSenha();
void updtUsr(char *loggedUser, int userPerm);
void delUsr();
int login(char *username, int *perm);
void menuAdmin();
void menuUser(char *username);
int hasAdmin();

int main() {
    int opcao;
    char username[100];
    int perm;
    int firstAccess;
    
    do {
        firstAccess = !hasAdmin(); // Verifica no início de cada loop
        
        printf("\n1 - Login\n");
        if(firstAccess) {
            printf("2 - Primeiro Acesso\n");
        }
        printf("0 - Sair\n");
        printf("Digite uma opcao: ");
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1:
                printf("Username: ");
                scanf("%s", username);
                
                if(login(username, &perm)) {
                    if(perm == 1) {
                        menuAdmin();
                    } else {
                        menuUser(username);
                    }
                } else {
                    printf("Login falhou!\n");
                }
                break;
                
            case 2:
                if(firstAccess) {
                    addUsr();
                }
                break;
        }
    } while(opcao != 0);
    
    return 0;
}

int hasAdmin() {
    char usuario[100], senha[100];
    int perm, ativ, chav;
    
    FILE *file = fopen("users.txt", "r");
    if(!file) return 0; // Se o arquivo não existe, retorna 0
    
    int found = 0;
    while(fscanf(file, "%s %s %d %d %d", usuario, senha, &perm, &ativ, &chav) == 5) {
        if(perm == 1 && ativ == 1) { // Verifica se existe admin ativo
            found = 1;
            break;
        }
    }
    
    fclose(file);
    return found;
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

int login(char *username, int *perm) {
    char senha[100], storedUser[100], storedPass[100];
    int storedPerm, ativ, chav;
    
    printf("Senha: ");
    scanf("%s", senha);
    
    FILE *file = fopen("users.txt", "r");
    if(!file) return 0;
    
    while(fscanf(file, "%s %s %d %d %d", storedUser, storedPass, &storedPerm, &ativ, &chav) == 5) {
        if(strcmp(username, storedUser) == 0 && ativ) {
            char decryptedPass[100];
            strcpy(decryptedPass, storedPass);
            decrypt(decryptedPass, chav);
            
            if(strcmp(senha, decryptedPass) == 0) {
                *perm = storedPerm;
                fclose(file);
                return 1;
            }
        }
    }
    
    fclose(file);
    return 0;
}

void menuAdmin() {
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
                updtUsr(NULL, 1); // NULL indica admin
                break;
            case 4:
                delUsr();
                break;
            case 5:
                showSenha();
                break;
        }
    } while(opcao != 0);
}

void menuUser(char *username) {
    int opcao;
    
    do {
        printf("\n1 - Atualizar Senha\n");
        printf("2 - Listar Usuarios\n");
        printf("0 - Sair\n");

        printf("Digite uma opcao: ");
        scanf("%d", &opcao);
        
        switch(opcao) {
            
            case 1:
                updtUsr(username, 2);
                break;
                
            case 2:
                lstUsr();
                break;
        }
    } while(opcao != 0);
}

void addUsr() {
    char usuario[100], senha[100];
    int perm;
    
    printf("Nome de Usuario: ");
    scanf("%s", usuario);
    
    printf("Senha: ");
    scanf("%s", senha);
    
    if(!hasAdmin()) {
        perm = 1; // Se é primeiro acesso, força criação de admin
        printf("Usuario criado como administrador (primeiro acesso)\n");
    } else {
        printf("Permissao (1 - Admin, 2 - Usuario padrao): ");
        scanf("%d", &perm);
    }
    
    srand(time(NULL));
    int chav = rand() % 100 + 1;
    crypt(senha, chav);
    
    FILE *file = fopen("users.txt", "a");
    if(file) {
        fprintf(file, "%s %s %d %d %d\n", usuario, senha, perm, 1, chav);
        fclose(file);
        printf("Usuario Registrado!\n");
    } else {
        printf("Nao foi possivel adicionar o usuario.\n");
    }
}

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

void updtUsr(char *loggedUser, int userPerm) {
    char procusr[100], usuario[100], senha[100];
    int perm, ativ, chav;
    int encntrd = 0;
    
    if(userPerm == 2) { // Usuário normal
        strcpy(procusr, loggedUser);
    } else { // Admin
        printf("Digite o usuario para alterar: ");
        scanf("%s", procusr);
    }
    
    FILE *file = fopen("users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if(file && temp) {
        while(fscanf(file, "%s %s %d %d %d", usuario, senha, &perm, &ativ, &chav) == 5) {
            if(strcmp(usuario, procusr) == 0 && ativ) {
                printf("Nova senha: ");
                scanf("%s", senha);
                
                if(userPerm == 1) { // Apenas admin pode mudar permissões
                    printf("Novo nivel de permissao (1-admin, 2-usuario normal): ");
                    scanf("%d", &perm);
                }
                
                srand(time(NULL));
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