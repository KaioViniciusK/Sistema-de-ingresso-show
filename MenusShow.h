#ifndef MENUS_H
#define MENUS_H

typedef struct{
    int id;
    int ativo;
    float preco;
    char nomeEvento[50];
} IngressoShow;

void cadastrarAluno();
void menuPrincipal();
void menuAdministrador();
int loginAdministrador();
void listarShows();

#endif;
