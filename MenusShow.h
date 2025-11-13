#ifndef MENUS_H
#define MENUS_H

typedef struct {
    int id;
    int ativo;
    int ingressoDisponivel;
    float preco;
    char nomeEvento[50];
} IngressoShow;

// Protótipos - funções do usuário 
void listarShows();
void pesquisarShow();
void comprarIngresso();
void sairPrograma();

// Protótipos - funções do administrador 
void cadastrarShow();
void atualizarShow();
void excluirShow();

// Menus e login
void menuPrincipal();        
void menuAdministrador();    
void loginAdministrador();

// Funções auxiliares reutilizáveis
void mostrarShow(IngressoShow show);
int buscarShowPorID(FILE *arquivo, int idBusca, IngressoShow *resultado);
int buscarShowPorNome(FILE *arquivo, char *nomeBusca, IngressoShow *resultado);

#endif
