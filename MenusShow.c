#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <locale.h>
#include "MenusShow.h"
 
int loginAdministrador();
void menuAdministrador();
void listarShows();

    // Fun��o que exibe o menu principal do sistema
void menuPrincipal() {
    int opcao;

    do {
        printf("\n------------------------------------------\n");
        printf("             Menu Mega Show \n");
        printf("------------------------------------------\n");
        printf("1 - Ver lista de todos os shows\n");
        printf("2 - Pesquisar shows\n");
        printf("3 - Comprar ingresso\n");
        printf("4 - Acesso do administrador\n");
        printf("5 - Encerrar programa\n");
        printf("------------------------------------------\n");
        printf("Digite sua opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                listarShows();
                break;
            case 2:
                // pesquisarShow();
                break;
            case 3:
                // comprarIngresso();
                break;
            case 4:
                if (loginAdministrador()) {
                    menuAdministrador();
                }
                break;
            case 5:
                printf("\nEncerrando o programa...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while (opcao != 5);
}

    // Menu exclusivo do administrador (aparece s� ap�s login)
void menuAdministrador() {
    int opcao;

    do {
        printf("\n------------------------------------------\n");
        printf("          Menu do Administrador\n");
        printf("------------------------------------------\n");
        printf("1 - Cadastrar show\n");
        printf("2 - Atualizar show\n");
        printf("3 - Excluir show\n");
        printf("4 - Cancelar venda\n");
        printf("5 - Consultar historico de vendas\n");
        printf("6 - Voltar ao menu principal\n");
        printf("------------------------------------------\n");
        printf("Digite sua opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                // cadastrarShow();
                break;
            case 2:
                // atualizarShow();
                break;
            case 3:
                // excluirShow();
                break;
            case 4:
                // cancelarVenda();
                break;
            case 5:
                // consultarHistorico();
                break;
            case 6:
                printf("\nRetornando ao menu principal...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while (opcao != 6);
}

    // Fun��o que realiza o login do administrador
int loginAdministrador() {
    char usuario[30], senha[30];

    printf("\n-------- Login do Administrador --------\n");
    printf("Usuario: ");
    scanf("%s", usuario);
    printf("Senha: ");
    scanf("%s", senha);

    // Verifica usu�rio e senha para liberar acesso ao menu do administrador
    if (strcmp(usuario, "admin") == 0 && strcmp(senha, "123") == 0) {
        printf("\nLogin efetuado com sucesso!\n");
        return 1; // sucesso
    } else {
        printf("\nUsuario ou senha incorretos!\n");
        return 0; // falhou
    }
}

//CADASTRAR SHOW
void cadastrarShow(){
    IngressoShow show, aux;
    int idExiste;

    printf("\n====CADASTRAR SHOW====\n");
    printf("ID: ");
    scanf("%d", &show.id);

    //Verifica se o ID já existe.
    FILE *arquivo;
    arquivo = fopen("showdeBola.bin", "rb");
    while(fread(&aux, sizeof(IngressoShow), 1, arquivo) == 1){
        if(aux.id == show.id){
            idExiste  = 1;
            break;
        }
    }
    fclose(arquivo);
    //Se ID já existe, não deixa cadastrar.
    if(idExiste == 1){
        printf("ERRO! Já existe um show com o ID %d", show.id);
        menu();
    }

    printf("\nNome: ");
    scanf("%s", show.nomeEvento);
    printf("\nPreco: ");
    scanf("%f", &show.preco);
    printf("\nEstá ativo? 1 - SIM | 2 - NAO: ");
    scanf("%d", &show.ativo);

    //Se ID não estiver ocupado, grava normalmente.
    arquivo = fopen("showdeBola.bin", "ab");
    fwrite(&show, sizeof(IngressoShow), 1, arquivo);

    printf("\nSHOW CADASTRADO COM SUCESSO");

    fclose(arquivo);
}

//EXCLUIR SHOW
void excluirShow(){
    IngressoShow show;
    int idProcurado; 
    int achou = 0;
    printf("\n====EXCLUIR SHOW====\n");
    printf("Digite o ID do show que você gostaria de exlcuir: ");
    scanf("%d", &idProcurado);

    FILE *arquivo;
    arquivo = fopen("showdeBola.bin", "r+b"); //Abre o arquivo para leitura e escrita binária.
    
    //Procura o show no arquivo.
    while(fread(&show, sizeof(IngressoShow), 1, arquivo)){
        if (show.id == idProcurado && show.ativo == 1){
            achou = 1;
            show.ativo = 0; //Marca como incativo.
            fseek(arquivo, -sizeof(IngressoShow), SEEK_CUR); //Volta 1 registro.
            fwrite(&show, sizeof(IngressoShow), 1, arquivo);
            printf("\nShow ID %d exlcuído com sucesso!!", idProcurado);
            break;
        }
    }

    if(!achou) //Caso não encontre o ID.
        printf("\nShow não encontrado ou já inativo!\n");

    fclose(arquivo);
}
