#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <locale.h>
#include "MenusShow.h"
 
void menuAdministrador();
void listarShows();

void pesquisarShow() {
    FILE *arquivo = fopen("showdeBola.bin", "rb");;
    IngressoShow show;
    int idBusca;
    int encontrado = 0;

    system("cls");
    printf("\n Pesquisar show por ID\n");
    printf("Digite o ID do show que deseja pesquisar: ");
    scanf("%d", &idBusca);

    if(arquivo == NULL){
        printf("\n Pesquisar show por ID\n");
        system("pause");
        return;
    }

    while(fread(&show, sizeof(IngressoShow), 1, arquivo) == 1){
        if(show.id == idBusca && show.ativo == 1){
            encontrado = 1;
            printf("\n--- Show Entrado ---\n");
            printf("ID: %d\n", show.id);
            printf("Nome: %s\n", show.nomeEvento);
            printf("Preco: R$%2.f\n", show.preco);
            break;
        }
    }

    fclose(arquivo);

    if(!encontrado){
        printf("\nO show com ID %d nao foi encontrado", idBusca);
    }

    system("pause");
}

void comprarIngresso() {
    printf("\n[EM DESENVOLVIMENTO] comprarIngresso()\n");
}

void consultarHistorico() {
    printf("\n[EM DESENVOLVIMENTO] consultarHistorico()\n");
}

void cancelarVenda() {
    printf("\n[EM DESENVOLVIMENTO] cancelarVenda()\n");
}


    // Função que exibe o menu principal do sistema
void menuPrincipal() {
    int escolha;

    // Vetor de ponteiros para funções do usuário
    void (*operacoesUsuario[])() = {listarShows, pesquisarShow, comprarIngresso, loginAdministrador, sairPrograma};

    do {
        system("cls");
        printf("\n=========== MENU USUARIO ===========\n");
        printf("1 - Listar shows\n");
        printf("2 - Pesquisar show\n");
        printf("3 - Comprar ingresso\n");
        printf("4 - Login como administrador\n");
        printf("5 - Encerrar programa\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &escolha);

        if (escolha >= 1 && escolha <= 5) {
            (*operacoesUsuario[escolha - 1])(); // Chama função correspondente
        } else {
            printf("\nOpção inválida!\n");
        }
         system("pause");
    } while (escolha != 5);
}

    // Menu exclusivo do administrador (aparece s� ap�s login)
void menuAdministrador() {
    int escolha;

    void (*operacoesAdm[])() = {cadastrarShow, atualizarShow, excluirShow, menuPrincipal};

    do {
        system("cls");
        printf("\n=========== MENU ADMINISTRADOR ===========\n");
        printf("1 - Cadastrar show\n");
        printf("2 - Atualizar show\n");
        printf("3 - Excluir show\n");
        printf("4 - Voltar ao menu do usuario\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &escolha);

        if (escolha >= 1 && escolha <= 4) { // Bug caso usuário digitasse 5 ou 6 corrigido
            (*operacoesAdm[escolha - 1])();
        } else {
            printf("\nOpcao invalida!\n");
        }
         system("pause");
    } while (escolha != 6);
}

void loginAdministrador() {
    int senha;

    system("cls");
    printf("\n===== LOGIN ADMINISTRADOR =====\n");
    printf("Digite a senha: ");
    scanf("%d", &senha);

    if (senha == 123) {
        printf("\nLogin bem-sucedido!\n");
        menuAdministrador();
    } else {
        printf("\nSenha incorreta!\n");
    }

     system("pause");
}

void atualizarShow() {
    FILE *arquivo;
    IngressoShow *shows = NULL;
    int total = 0, i, idBusca, encontrado = 0;

    system("cls");
    printf("\n==== ATUALIZAR SHOW ====\n");

    arquivo = fopen("showdeBola.bin", "r+b");
    if (arquivo == NULL) {
        printf("\nNenhum show cadastrado!\n");
        system("pause");
        return;
    }

    // Conta quantos registros existem
    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    total = tamanho / sizeof(IngressoShow);
    rewind(arquivo);

    // Aloca dinamicamente memória para todos os shows
    shows = (IngressoShow*) malloc(total * sizeof(IngressoShow));
    if (shows == NULL) {
        printf("Erro de memoria!\n");
        fclose(arquivo);
        return;
    }

    fread(shows, sizeof(IngressoShow), total, arquivo);

    printf("\n===== SHOWS DISPONÍVEIS =====\n");
    for (i = 0; i < total; i++) {
        if (shows[i].ativo == 1) {
            printf("ID: %d | Nome: %s | Preco: R$%.2f\n",
                   shows[i].id, shows[i].nomeEvento, shows[i].preco);
        }
    }

    printf("\nDigite o ID do show que deseja atualizar: ");
    scanf("%d", &idBusca);

    // Procura o show e atualiza
    for (i = 0; i < total; i++) {
        if (shows[i].id == idBusca && shows[i].ativo == 1) {
            encontrado = 1;

            printf("\n--- Editando Show ID %d ---\n", shows[i].id);
            printf("Nome atual: %s\nNovo nome: ", shows[i].nomeEvento);
            fflush(stdin);
            scanf("%s", shows[i].nomeEvento);

            printf("Preço atual: R$%.2f\nNovo preco: ", shows[i].preco);
            scanf("%f", &shows[i].preco);

            printf("Status atual: %s\n", shows[i].ativo == 1 ? "Ativo" : "Inativo");
            printf("Novo status (1 - ativo / 0 - inativo): ");
            scanf("%d", &shows[i].ativo);

            // Atualiza o registro no arquivo
            fseek(arquivo, i * sizeof(IngressoShow), SEEK_SET);
            fwrite(&shows[i], sizeof(IngressoShow), 1, arquivo);
            printf("\nShow atualizado com sucesso!\n");
            break;
        }
    }

    fclose(arquivo);
    free(shows);

    if (!encontrado) {
        printf("\nShow com ID %d não encontrado!\n", idBusca);

        // Pergunta se deseja tentar novamente
        char opc;
        printf("Deseja tentar outro ID? (S/N): ");
        fflush(stdin);
        scanf(" %c", &opc);

        if (opc == 'S' || opc == 's') {
            atualizarShow(); 
            return;
        }
    }

    system("pause");
}


void listarShows() {
    IngressoShow show;
    FILE *arquivo;

    arquivo = fopen("showdeBola.bin", "rb");

    system("cls");
    if (arquivo == NULL) {
        printf("\nNenhum show cadastrado ainda!\n");
        return;
    }

    rewind(arquivo);
    printf("\n===== LISTA DE SHOWS =====\n");

    while (fread(&show, sizeof(IngressoShow), 1, arquivo) == 1) {
        if (show.ativo == 1) { // Exibe apenas shows ativos
            printf("ID: %d | Nome: %s | Preco: R$%.2f\n", show.id, show.nomeEvento, show.preco);
        }
    }

     system("pause");
    fclose(arquivo);
}

//CADASTRAR SHOW
void cadastrarShow(){
    IngressoShow show, aux;
    int idExiste;

    system("cls");
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
        printf("ERRO! Ja existe um show com o ID %d", show.id);
    }

    printf("\nNome: ");
    scanf(" %[^\n]", show.nomeEvento);
    printf("\nPreco: ");
    scanf("%f", &show.preco);
    printf("\nEsta ativo? 1 - SIM | 2 - NAO: ");
    scanf("%d", &show.ativo);

    //Se ID não estiver ocupado, grava normalmente.
    arquivo = fopen("showdeBola.bin", "ab");
    fwrite(&show, sizeof(IngressoShow), 1, arquivo);

    printf("\nSHOW CADASTRADO COM SUCESSO\n");

    system("pause");
    fclose(arquivo);
}

//EXCLUIR SHOW
void excluirShow(){
    IngressoShow show;
    int idProcurado; 
    int achou = 0;

    system("cls");
    printf("\n====EXCLUIR SHOW====\n");
    printf("Digite o ID do show que voce gostaria de exlcuir: ");
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
            printf("\nShow ID %d exlcuido com sucesso!!\n", idProcurado);
            break;
        }
    }

    if(!achou) //Caso não encontre o ID.
        printf("\nShow nao encontrado ou ja inativo!\n");

    system("pause");
    fclose(arquivo);
}
    void sairPrograma() {
        printf("\nEncerrando o programa...\n");
        exit(0);
    }