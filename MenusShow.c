#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <locale.h>
#include "MenusShow.h"
 
void menuAdministrador();
void listarShows();

    // Funcao que exibe o menu principal do sistema
void menuPrincipal() {
    int escolha;

    // Vetor de ponteiros para funcao do usuario
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
            (*operacoesUsuario[escolha - 1])(); // Chama funcao correspondente
        } else {
            printf("\nOpcao Invalida!\n");
        }
         system("pause");
    } while (escolha != 5);
}

    // Menu exclusivo do administrador (aparece apos login)
    
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

        if (escolha >= 1 && escolha <= 4) { 
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

// Funcao Responsavel por permitir a edicoes dos dados de um show ja cadastrado.
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

    // Aloca dinamico para todos os shows
    shows = (IngressoShow*) malloc(total * sizeof(IngressoShow));
    if (shows == NULL) {
        printf("Erro de memoria!\n");
        fclose(arquivo);
        return;
    }

    fread(shows, sizeof(IngressoShow), total, arquivo);

    printf("\n===== SHOWS DISPONIVEIS =====\n");
    for (i = 0; i < total; i++) {
        if (shows[i].ativo == 1) {
            printf("ID: %d | Nome: %s | Preco: R$%.2f\n",
                   shows[i].id, shows[i].nomeEvento, shows[i].preco);
        }
    }

    printf("\nDigite o ID do show que deseja atualizar: ");
    scanf("%d", &idBusca);
    getchar();

    // Procura o show e atualiza
    for (i = 0; i < total; i++) {
        if (shows[i].id == idBusca && shows[i].ativo == 1) {
            encontrado = 1;

            printf("\n--- Editando Show ID %d ---\n", shows[i].id);

            printf("Nome atual: %s\nNovo nome: ", shows[i].nomeEvento);
            fgets(shows[i].nomeEvento, sizeof(shows[i].nomeEvento), stdin);
            shows[i].nomeEvento[strcspn(shows[i].nomeEvento, "\n")] = '\0';

            printf("Preço atual: R$%.2f\nNovo preço: ", shows[i].preco);
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
        printf("\nShow com ID %d nÃ£o encontrado!\n", idBusca);

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

// Funcao responsavel por exibir todos os shows ativos.
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

    // Le e exibe apenas os shows marcados como ativos
    while (fread(&show, sizeof(IngressoShow), 1, arquivo) == 1) {
        if (show.ativo == 1) { // Exibe apenas shows ativos
            printf("ID: %d | Nome: %s | Preco: R$%.2f\n", show.id, show.nomeEvento, show.preco);
        }
    }

     system("pause");
    fclose(arquivo);
}

//CADASTRAR SHOW
void cadastrarShow() {
    IngressoShow show, aux;
    int idExiste = 0; // Inicializa a variável
    FILE *arquivo;

    printf("\n==== CADASTRAR SHOW ====\n");
    printf("ID: ");
    scanf("%d", &show.id);

    // Verifica se o ID já existe
    arquivo = fopen("showdeBola.bin", "rb");
    if (arquivo != NULL) {
        while (fread(&aux, sizeof(IngressoShow), 1, arquivo) == 1) {
            if (aux.id == show.id) {
                idExiste = 1;
                break;
            }
        }
        fclose(arquivo);
    }

    // Se o ID já existe, exibe erro e volta ao menu
    if (idExiste == 1) {
        printf("\nERRO! Já existe um show com o ID %d.\n", show.id);
        printf("Retornando ao menu do administrador...\n");
        system("pause"); 
        system("cls");   
        return;          
    }

    // Continua normalmente o cadastro
    printf("\nNome: ");
    scanf(" %[^\n]", show.nomeEvento);
    printf("\nPreco: ");
    scanf("%f", &show.preco);
    printf("\nEstá ativo? 1 - SIM | 2 - NÃO: ");
    scanf("%d", &show.ativo);

    // Grava o show no arquivo
    arquivo = fopen("showdeBola.bin", "ab");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para gravar.\n");
        return;
    }

    fwrite(&show, sizeof(IngressoShow), 1, arquivo);
    fclose(arquivo);

    printf("\nSHOW CADASTRADO COM SUCESSO!\n");
    system("pause");
    system("cls");
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
    arquivo = fopen("showdeBola.bin", "r+b"); //Abre o arquivo para leitura e escrita binÃ¡ria.
    
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

    if(!achou) //Caso nÃ£o encontre o ID.
        printf("\nShow nao encontrado ou ja inativo!\n");

    system("pause");
    fclose(arquivo);
}
    void sairPrograma() {
        printf("\nEncerrando o programa...\n");
        exit(0);
    }

void pesquisarShow() {
    FILE *arquivo = fopen("showdeBola.bin", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de shows.\n");
        system("pause");
        return;
    }

    IngressoShow show;
    int opcao;
    int idBusca;
    char nomeBusca[50];
    int encontrado = 0;

    system("cls");
    printf("\n=== PESQUISAR SHOW ===\n");
    printf("Digite (1) para pesquisar por NOME ou (2) por ID: ");
    scanf("%d", &opcao);

    if (opcao == 2) {
        printf("\nDigite o ID do show que deseja pesquisar: ");
        scanf("%d", &idBusca);

        while (fread(&show, sizeof(IngressoShow), 1, arquivo) == 1) {
            if (show.id == idBusca && show.ativo == 1) {
                encontrado = 1;
                printf("\n--- SHOW ENCONTRADO ---\n");
                printf("ID: %d\n", show.id);
                printf("Nome: %s\n", show.nomeEvento);
                printf("Preco: R$%.2f\n", show.preco);
                break;
            }
        }
    } 
    else if (opcao == 1) {
        printf("\nDigite o nome do show que deseja pesquisar: ");
        scanf(" %[^\n]", nomeBusca);

        while (fread(&show, sizeof(IngressoShow), 1, arquivo) == 1) {
            if (strcmp(show.nomeEvento, nomeBusca) == 0 && show.ativo == 1) {
                encontrado = 1;
                printf("\n--- SHOW ENCONTRADO ---\n");
                printf("ID: %d\n", show.id);
                printf("Nome: %s\n", show.nomeEvento);
                printf("Preco: R$%.2f\n", show.preco);
                break;
            }
        }
    } 
    else {
        printf("\nOpcao invalida.\n");
    }

    fclose(arquivo);

    if (!encontrado) {
        printf("\nNenhum show encontrado com o criterio informado.\n");
    }

    system("pause");
}


void comprarIngresso()
{
    FILE *arquivo = fopen("showdeBola.bin", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de shows.\n");
        return;
    }

    IngressoShow show;
    int opcao;
    int idBusca;
    char nomeBusca[50];
    int encontrado = 0;
    int quantidadeIngressos;

    printf("\nDigite (1) para pesquisar show pelo nome e (2) pelo ID:\n");
    scanf("%d", &opcao);

    if (opcao == 2) {
        printf("Digite o ID do show que deseja pesquisar: ");
        scanf("%d", &idBusca);

        while (fread(&show, sizeof(IngressoShow), 1, arquivo) == 1) {
            if (show.id == idBusca && show.ativo == 1) {
                encontrado = 1;
                printf("\n--- Show Encontrado ---\n");
                printf("ID: %d\n", show.id);
                printf("Nome: %s\n", show.nomeEvento);
                printf("Preco: R$%.2f\n", show.preco);

                printf("Quantos ingressos deseja comprar? ");
                scanf("%d", &quantidadeIngressos);

                float total = show.preco * quantidadeIngressos;
                printf("\nO total vai ser de R$%.2f.\n", total);

                char confirmar;
                printf("Deseja finalizar a compra? (S/N): ");
                scanf(" %c", &confirmar);

                if (confirmar == 'S' || confirmar == 's') {
                    printf("\nCompra finalizada com sucesso!\n");
                    printf("Você comprou %d ingresso(s) para o show '%s'.\n", quantidadeIngressos, show.nomeEvento);
                } else {
                    printf("\nCompra cancelada.\n");
                }
                break;
            }
        }
    } 
    else if (opcao == 1) {
        printf("Digite o nome do show que deseja pesquisar: ");
        scanf(" %[^\n]", nomeBusca);

        while (fread(&show, sizeof(IngressoShow), 1, arquivo) == 1) {
            if (strcmp(show.nomeEvento, nomeBusca) == 0 && show.ativo == 1) {
                encontrado = 1;
                printf("\n--- Show Encontrado ---\n");
                printf("ID: %d\n", show.id);
                printf("Nome: %s\n", show.nomeEvento);
                printf("Preco: R$%.2f\n", show.preco);

                printf("Quantos ingressos deseja comprar? ");
                scanf("%d", &quantidadeIngressos);

                float total = show.preco * quantidadeIngressos;
                printf("\nO total vai ser de R$%.2f.\n", total);

                char confirmar;
                printf("Deseja finalizar a compra? (S/N): ");
                scanf(" %c", &confirmar);

                if (confirmar == 'S' || confirmar == 's') {
                    printf("\nCompra finalizada com sucesso!\n");
                    printf("Você comprou %d ingresso(s) para o show '%s'.\n", quantidadeIngressos, show.nomeEvento);
                } else {
                    printf("\nCompra cancelada.\n");
                }
                break;
            }
        }
    } 
    else {
        printf("Opcao invalida.\n");
    }

    fclose(arquivo);

    if (!encontrado) {
        printf("\nNenhum show encontrado com o criterio informado.\n");
    }
}

