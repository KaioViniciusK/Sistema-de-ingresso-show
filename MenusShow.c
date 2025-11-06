#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <locale.h>
#include "MenusShow.h"
 
void menuAdministrador();
void listarShows();

void pesquisarShow() {
    printf("\n[EM DESENVOLVIMENTO] pesquisarShow()\n");
}

void comprarIngresso() {
    printf("\n[EM DESENVOLVIMENTO] comprarIngresso()\n");
}

void sairPrograma() {
    printf("\nEncerrando o programa...\n");
    exit(0);
}

void atualizarShow() {
    printf("\n[EM DESENVOLVIMENTO] atualizarShow()\n");
}

void consultarHistorico() {
    printf("\n[EM DESENVOLVIMENTO] consultarHistorico()\n");
}

void cancelarVenda() {
    printf("\n[EM DESENVOLVIMENTO] cancelarVenda()\n");
}

void loginAdministrador() {
    int senha;

    printf("\n===== LOGIN ADMINISTRADOR =====\n");
    printf("Digite a senha: ");
    scanf("%d", &senha);

    if (senha == 123) {
        printf("\nLogin bem-sucedido!\n");
        menuAdministrador();
    } else {
        printf("\nSenha incorreta!\n");
    }
}




    // Fun��o que exibe o menu principal do sistema
void menuPrincipal() {
    int escolha;

    // Vetor de ponteiros para funções do usuário
    void (*operacoesUsuario[])() = {listarShows, pesquisarShow, comprarIngresso, loginAdministrador, sairPrograma};

    do {
        printf("\n=========== MENU USUÁRIO ===========\n");
        printf("1 - Listar shows\n");
        printf("2 - Pesquisar show\n");
        printf("3 - Comprar ingresso\n");
        printf("4 - Login como administrador\n");
        printf("5 - Encerrar programa\n");
        printf("Escolha uma opção: ");
        scanf("%d", &escolha);

        if (escolha >= 1 && escolha <= 5) {
            (*operacoesUsuario[escolha - 1])(); // Chama função correspondente
        } else {
            printf("\nOpção inválida!\n");
        }

    } while (escolha != 5);
}

    // Menu exclusivo do administrador (aparece s� ap�s login)
void menuAdministrador() {
    int escolha;

    void (*operacoesAdm[])() = {cadastrarShow, atualizarShow, excluirShow, menuPrincipal};

    do {
        printf("\n=========== MENU ADMINISTRADOR ===========\n");
        printf("1 - Cadastrar show\n");
        printf("2 - Atualizar show\n");
        printf("3 - Excluir show\n");
        printf("4 - Voltar ao menu do usuário\n");
        printf("Escolha uma opção: ");
        scanf("%d", &escolha);

        if (escolha >= 1 && escolha <= 6) {
            (*operacoesAdm[escolha - 1])();
        } else {
            printf("\nOpção inválida!\n");
        }

    } while (escolha != 6);
}


void listarShows() {
    IngressoShow show;
    FILE *arquivo;

    arquivo = fopen("showdeBola.bin", "rb");

    if (arquivo == NULL) {
        printf("\nNenhum show cadastrado ainda!\n");
        return;
    }

    rewind(arquivo);
    printf("\n===== LISTA DE SHOWS =====\n");

    while (fread(&show, sizeof(IngressoShow), 1, arquivo) == 1) {
        if (show.ativo == 1) { // Exibe apenas shows ativos
            printf("ID: %d | Nome: %s | Preço: %.2f\n", show.id, show.nomeEvento, show.preco);
        }
    }

    fclose(arquivo);
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
        menuAdministrador();;
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
