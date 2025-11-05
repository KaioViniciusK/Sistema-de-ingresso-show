#ifndef MENUS_H
#define MENUS_H

typedef struct {
    char artista[100];
    char local[150];
    char data[12];     
    char horario[7];    
    float valor;
    int ingressosDisponiveis;
    char categoria[50];
} Show;


void menuPrincipal();
void menuAdministrador();
int loginAdministrador();
void listarShows();

#endif
