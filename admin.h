#ifndef ADMIN_H
#define ADMIN_H

#define MAX_ADMINS 10
#define ARCHIVO_ADMINS "db/admins/admins.txt"

typedef struct {
	char usuario[20];
	char clave[20];
	char nombre[50];
	int activo;
} Administrador;

void cargarAdmins();
void guardarAdmins();
int loginAdmin(char* usuario, char* clave);
void registrarAdmin();
int existeAdmin(char* usuario);

#endif
