#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void cripto(char *texto, int chav);
void decripto(char *texto, int chav);
void addUsua();
void listUsua();
void mostrarSenha();
void atuaUsua(char *usuaLogad, int permUsua);
void deletUsua();
int login(char *nomeUsua, int *perm);
void menuAdmin();
void menuUsua(char *nomeUsua);
int Admin();

int main() {

    int opcao;
    char nomeUsua[100];
    int perm;
    int primeiroAcess;

    do{
        primeiroAcess = !Admin();
        
        printf("\n1 - Login\n");
        if(primeiroAcess) {
            printf("2 - Primeiro Acesso\n");
        }
        printf("0 - Sair\n");
        printf("Digite uma opcao: ");
        scanf("%d", &opcao);
        
        switch(opcao) {
        case 1:
            printf("Nome de Usuário: ");
            scanf("%s", nomeUsua);
                
                if(login(nomeUsua, &perm)){
                    if(perm == 1) {
                    menuAdmin();
                    } else {
                    menuUsua(nomeUsua);
                    }
                } else {
                printf("Login falhou!\n");
                }
            break;
                
        case 2:
                if(primeiroAcess){
                    addUsua();
                }
                break;
        }
    } 
    while(opcao != 0);
    
    return 0;

}

int Admin(){

    char usuario[100], senha[100];
    int perm, ativ, chav;
    
    FILE *file = fopen("arquivo.txt", "r");
    if(!file) return 0;
    
    int encontrd = 0;
    while(fscanf(file, "%s %s %d %d %d", usuario, senha, &perm, &ativ, &chav) == 5) {
        if(perm == 1 && ativ == 1) {
        encontrd = 1;
        break;
        }

    }
    
    fclose(file);
    return encontrd;
}


void cripto(char *texto, int chav){
    int i;
    for(i = 0; texto[i] != '\0'; i++) {
    texto[i] = texto[i] + chav;

    }

}

void decripto(char *texto, int chav){
    int i;
    for(i = 0; texto[i] != '\0'; i++) {
    texto[i] = texto[i] - chav;
    }

}

int login(char *nomeUsua, int *perm){
    char senha[100],guardSenha[100];
    char guardUsua[100];
    int guardPerm, ativ, chav;
    
    printf("Senha: ");
    scanf("%s", senha);
    
    FILE *file = fopen("arquivo.txt", "r");
    if(!file) return 0;

    
    while(fscanf(file, "%s %s %d %d %d", guardUsua, guardSenha, &guardPerm, &ativ, &chav) == 5) {
        if(strcmp(nomeUsua, guardUsua) == 0 && ativ) {
            char decriptoSenha[100];
            strcpy(decriptoSenha, guardSenha);
            decripto(decriptoSenha, chav);
            
            if(strcmp(senha, decriptoSenha) == 0) {
                *perm = guardPerm;
                fclose(file);
                return 1;
            }

        }

    }
    
    fclose(file);
    return 0;


}

void menuAdmin(){
    int opcao;
    do{
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
                addUsua();
                break;
            case 2:
                listUsua();
                break;
            case 3:
                atuaUsua(NULL, 1);
                break;
            case 4:
                deletUsua();
                break;
            case 5:
                mostrarSenha();
                break;
        }

    }while(opcao != 0);

}

void menuUsua(char *nomeUsua){
    int opcao;
    do {
        printf("\n1 - Atualizar Senha\n");
        printf("2 - Listar Usuarios\n");
        printf("0 - Sair\n");

        printf("Digite uma opcao: ");
        scanf("%d", &opcao);
        
        switch(opcao) {
            
            case 1:
            atuaUsua(nomeUsua, 2);
            break;
                
            case 2:
            listUsua();
            break;
        }
    } while(opcao != 0);

}

void addUsua(){
    char usuario[100], senha[100];
    int perm;
    
    printf("Nome de Usuario: ");
    scanf("%s", usuario);
    printf("Senha: ");
    scanf("%s", senha);
    
    if(!Admin()) {
        perm = 1;
        printf("Usuario criado como administrador (primeiro acesso)\n");

    } else {
        printf("Permissao (1 - Admin, 2 - Usuario padrao): ");
        scanf("%d", &perm);
}
    

    srand(time(NULL));
    int chav = rand() % 100 + 1;
    cripto(senha, chav);
    
    FILE *file = fopen("arquivo.txt", "a");
    if(file) {
        fprintf(file, "%s %s %d %d %d\n", usuario, senha, perm, 1, chav);
        fclose(file);
        printf("Usuario Registrado!\n");
    } else {
        printf("Nao foi possivel adicionar o usuario.\n");
}

}

void listUsua(){
    char usuario[100], senha[100];
    int perm, ativ, chav;
    
    printf("\nLista de Usuarios:\n");
    printf("Usuario\t\tPermissao\tStatus\n");
    
    FILE *file = fopen("arquivo.txt", "r");
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

void mostrarSenha(){
    char procusr[100], usuario[100], senha[100];
    int perm, ativ, chav;
    int encntrd = 0;
    
    printf("Escreva o usuario que deseja ver a senha: ");
    scanf("%s", procusr);
    
    FILE *file = fopen("arquivo.txt", "r");
    if(file) {
        while(fscanf(file, "%s %s %d %d %d", usuario, senha, &perm, &ativ, &chav) == 5) {
            if(strcmp(usuario, procusr) == 0 && ativ) {
            char decriptSen[100];
                strcpy(decriptSen, senha);
                decripto(decriptSen, chav);
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

void atuaUsua(char *usuaLogad, int permUsua){
    char procusr[100], usuario[100], senha[100];
    int perm, ativ, chav;
    int encntrd = 0;
    
    if(permUsua == 2) { 
        strcpy(procusr, usuaLogad);
    } else {
        printf("Digite o usuario para alterar: ");
        scanf("%s", procusr);
    }
    
    FILE *file = fopen("arquivo.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if(file && temp) {
        while(fscanf(file, "%s %s %d %d %d", usuario, senha, &perm, &ativ, &chav) == 5) {
            if(strcmp(usuario, procusr) == 0 && ativ) {
                printf("Nova senha: ");
                scanf("%s", senha);
                
            if(permUsua == 1) {
                printf("Novo nivel de permissao (1-admin, 2-usuario normal): ");
                scanf("%d", &perm);
            }
                
            srand(time(NULL));
            chav = rand() % 100 + 1;
            cripto(senha, chav);
            encntrd = 1;
        }

        fprintf(temp, "%s %s %d %d %d\n", usuario, senha, perm, ativ, chav);
}
        
        fclose(file);
        fclose(temp);
        
    remove("arquivo.txt");
    rename("temp.txt", "arquivo.txt");
        
    if(encntrd) {
        printf("Usuario Atualizado!\n");
} else {
        printf("Nao foi possivel encontrar o usuario\n");
    }
    }

}

void deletUsua(){
    char procusr[100], usuario[100], senha[100];
    int perm, ativ, chav;
    int encntrd = 0;
    
    printf("Digite o usuario para deletar: ");
    scanf("%s", procusr);
    
    FILE *file = fopen("arquivo.txt", "r");
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
        
    remove("arquivo.txt");
    rename("temp.txt", "arquivo.txt");
        
        if(encntrd) {
        printf("Usuario Deletado!\n");
        } else {
        printf("Nao foi possivel encontrar o usuario!\n");
    }

}

}