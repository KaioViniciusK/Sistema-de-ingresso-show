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
            (*operacoesAdm[escolha - 1])(); // Chama funcao correspondente
        } else {
            printf("\nOpcao invalida!\n");
        }
         system("pause");
    } while (escolha != 6);
}

//funcao para login dos administradores
void loginAdministrador() {
    int senha;

    system("cls");
    printf("\n===== LOGIN ADMINISTRADOR =====\n");
    printf("Digite a senha: ");
    scanf("%d", &senha);

    // Verificando a senha 
    if (senha == 123) {
        printf("\nLogin bem-sucedido!\n");
        menuAdministrador();
    } else {
        printf("\nSenha incorreta!\n");
    }

     system("pause");
}

// Função responsável por permitir a edição dos dados de um show já cadastrado.
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

    // Aloca dinamicamente todos os shows
    shows = (IngressoShow*) malloc(total * sizeof(IngressoShow));
    if (shows == NULL) {
        printf("Erro de memória!\n");
        fclose(arquivo);
        return;
    }

    fread(shows, sizeof(IngressoShow), total, arquivo);

    printf("\n===== SHOWS DISPONÍVEIS =====\n");
    for (i = 0; i < total; i++) {
        if (shows[i].ativo == 1) {
            printf("ID: %d | Nome: %s | Preço: R$%.2f | Ingressos disponíveis: %d\n",
                   shows[i].id, shows[i].nomeEvento, shows[i].preco, shows[i].ingressoDisponivel);
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

            printf("Ingressos disponíveis atualmente: %d\nNova quantidade: ", shows[i].ingressoDisponivel);
            scanf("%d", &shows[i].ingressoDisponivel);

            printf("Status atual: %s\n", shows[i].ativo == 1 ? "Ativo" : "Inativo");
            printf("Novo status (1 - ativo / 0 - inativo): ");
            scanf("%d", &shows[i].ativo);

            // Atualiza o registro no arquivo
            fseek(arquivo, i * sizeof(IngressoShow), SEEK_SET);
            fwrite(&shows[i], sizeof(IngressoShow), 1, arquivo);
            printf("\n? Show atualizado com sucesso!\n");
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


// Funcao responsavel por exibir todos os shows ativos.
void listarShows() {
    IngressoShow show;
    FILE *arquivo = fopen("showdeBola.bin", "rb");
    system("cls");

    if (arquivo == NULL) {
        printf("\nNenhum show cadastrado ainda!\n");
        return;
    }

    printf("\n===== LISTA DE SHOWS =====\n");
    while (fread(&show, sizeof(IngressoShow), 1, arquivo) == 1) {
        if (show.ativo == 1) {
            printf("ID: %d | Nome: %s | Preço: R$%.2f | Ingressos disponíveis: %d\n",
                   show.id, show.nomeEvento, show.preco, show.ingressoDisponivel);
        }
    }

    fclose(arquivo);
    system("pause");
}


//CADASTRAR SHOW
void cadastrarShow() {
    IngressoShow show, aux;
    int idExiste = 0; 
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

    if (idExiste == 1) {
        printf("\nERRO! Já existe um show com o ID %d.\n", show.id);
        system("pause");
        return;
    }

        printf("\nNome: ");
        scanf(" %[^\n]", show.nomeEvento);
        printf("Preço: ");
        scanf("%f", &show.preco);
        printf("Quantidade de ingressos disponíveis: ");
        scanf("%d", &show.ingressoDisponivel);
        printf("Está ativo? (1 - Sim / 0 - Não): ");
        scanf("%d", &show.ativo);

    arquivo = fopen("showdeBola.bin", "ab");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para gravar.\n");
        return;
    }

    fwrite(&show, sizeof(IngressoShow), 1, arquivo);
    fclose(arquivo);

    printf("\nSHOW CADASTRADO COM SUCESSO!\n");
    system("pause");
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
    FILE *arquivo = fopen("showdeBola.bin", "rb");     // Verifica se o arquivo foi aberto corretamente
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de shows.\n");
        system("pause");
        return;
    }

    IngressoShow resultado;
    int opcao;                
    int idBusca;                
    char nomeBusca[50];
    int encontrado = 0;         // Flag para indicar se algum show foi encontrado

    system("cls");
    printf("\n=== PESQUISAR SHOW ===\n");
    printf("Digite (1) para pesquisar por NOME ou (2) por ID: ");
    scanf("%d", &opcao);

    if (opcao == 2) {
        printf("\nDigite o ID do show que deseja pesquisar: ");
        scanf("%d", &idBusca);
        encontrado = buscarShowPorID(arquivo, idBusca, &resultado);
    } 
    else if (opcao == 1) {
        printf("\nDigite o nome do show que deseja pesquisar: ");
        scanf(" %[^\n]", nomeBusca);
        encontrado = buscarShowPorNome(arquivo, nomeBusca, &resultado);
    } 
    else {
        printf("\nOpcao invalida.\n");
        fclose(arquivo);
        system("pause");
        return;
    }

    if (encontrado)
        mostrarShow(resultado);
    else
        printf("\nNenhum show encontrado com o criterio informado.\n");

    fclose(arquivo);
    system("pause");
}




void comprarIngresso() {
    FILE *arquivo = fopen("showdeBola.bin", "r+b");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de shows.\n");
        return;
    }

    IngressoShow show;
    int opcao, idBusca, quantidade;
    char nomeBusca[50];
    int encontrado = 0;

    printf("\nDigite (1) para buscar por NOME ou (2) por ID: ");
    scanf("%d", &opcao);

    if (opcao == 2) {
        printf("Digite o ID do show: ");
        scanf("%d", &idBusca);
        encontrado = buscarShowPorID(arquivo, idBusca, &show);
    } else if (opcao == 1) {
        printf("Digite o nome do show: ");
        scanf(" %[^\n]", nomeBusca);
        encontrado = buscarShowPorNome(arquivo, nomeBusca, &show);
    } else {
        printf("\nOpção inválida.\n");
        fclose(arquivo);
        return;
    }

    if (!encontrado) {
        printf("\nNenhum show encontrado.\n");
        fclose(arquivo);
        system("pause");
        return;
    }

    mostrarShow(show);

    // Verifica disponibilidade
    if (show.ingressoDisponivel <= 0) {
        printf("\nIngressos esgotados para este show!\n");
        fclose(arquivo);
        return;
    }

    printf("Quantos ingressos deseja comprar? ");
    scanf("%d", &quantidade);

    // Valida quantidade
    if (quantidade > show.ingressoDisponivel) {
        printf("\nNão há ingressos suficientes! Restam apenas %d.\n", show.ingressoDisponivel);
        fclose(arquivo);
        return;
    }

    float total = show.preco * quantidade;
    printf("Total: R$%.2f\nConfirmar compra? (S/N): ", total);

    char confirma;
    scanf(" %c", &confirma);

    if (confirma == 'S' || confirma == 's') {
        // Atualiza o show no arquivo
        show.ingressoDisponivel -= quantidade;

        // Volta uma posição no arquivo antes de gravar
        fseek(arquivo, -sizeof(IngressoShow), SEEK_CUR);
        fwrite(&show, sizeof(IngressoShow), 1, arquivo);

        printf("\nCompra concluída!\n");
        printf("Você comprou %d ingresso(s). Restam %d disponíveis.\n",
               quantidade, show.ingressoDisponivel);

        if (show.ingressoDisponivel == 0)
            printf("Os ingressos para este show acabaram!\n");
    } else {
        printf("\nCompra cancelada.\n");
    }

    fclose(arquivo);
    system("pause");
}


// Função que exibe um show formatado
void mostrarShow(IngressoShow show) {
    printf("\n--- SHOW ENCONTRADO ---\n");
    printf("ID: %d\n", show.id);
    printf("Nome: %s\n", show.nomeEvento);
    printf("Preco: R$%.2f\n", show.preco);
    printf("Ingressos disponíveis: %d\n", show.ingressoDisponivel);
}

int buscarShowPorID(FILE *arquivo, int idBusca, IngressoShow *resultado) {
    IngressoShow show;
    rewind(arquivo); // Garante leitura do início
    while (fread(&show, sizeof(IngressoShow), 1, arquivo) == 1) {
        if (show.id == idBusca && show.ativo == 1) {
            *resultado = show;
            return 1;
        }
    }
    return 0;
}

int buscarShowPorNome(FILE *arquivo, char *nomeBusca, IngressoShow *resultado) {
    IngressoShow show;
    rewind(arquivo);
    while (fread(&show, sizeof(IngressoShow), 1, arquivo) == 1) {
        if (strcmp(show.nomeEvento, nomeBusca) == 0 && show.ativo == 1) {
            *resultado = show;
            return 1;
        }
    }
    return 0;
}

