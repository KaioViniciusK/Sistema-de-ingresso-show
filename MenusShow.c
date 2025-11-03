#include <stdio.h>
#include <string.h>
 
int loginAdministrador();
void menuAdministrador();

    // Função que exibe o menu principal do sistema
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
                // listarShows();
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

    // Menu exclusivo do administrador (aparece só após login)
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

    // Função que realiza o login do administrador
int loginAdministrador() {
    char usuario[30], senha[30];

    printf("\n-------- Login do Administrador --------\n");
    printf("Usuario: ");
    scanf("%s", usuario);
    printf("Senha: ");
    scanf("%s", senha);

    // Verifica usuário e senha para liberar acesso ao menu do administrador
    if (strcmp(usuario, "admin") == 0 && strcmp(senha, "123") == 0) {
        printf("\nLogin efetuado com sucesso!\n");
        return 1; // sucesso
    } else {
        printf("\nUsuario ou senha incorretos!\n");
        return 0; // falhou
    }
}
