#include <stdio.h>
#include <string.h>
#include "admin.h"

Administrador listaAdmins[MAX_ADMINS];
int contadorAdmins = 0;

void leerTextoAdmin(char* buffer, int max) {
	fflush(stdin);
	fgets(buffer, max, stdin);
	buffer[strcspn(buffer, "\n")] = 0;
}

void cargarAdmins() {
	FILE *archivo = fopen(ARCHIVO_ADMINS, "r");
	if (archivo == NULL) return;
	contadorAdmins = 0;
	while (fscanf(archivo, "%[^;];%[^;];%[^;];%d\n",
				  listaAdmins[contadorAdmins].usuario,
				  listaAdmins[contadorAdmins].clave,
				  listaAdmins[contadorAdmins].nombre,
				  &listaAdmins[contadorAdmins].activo) == 4) {
		contadorAdmins++;
	}
	fclose(archivo);
}

void guardarAdmins() {
	FILE *archivo = fopen(ARCHIVO_ADMINS, "w");
	if (archivo == NULL) return;
	for (int i = 0; i < contadorAdmins; i++) {
		if (listaAdmins[i].activo) {
			fprintf(archivo, "%s;%s;%s;%d\n",
					listaAdmins[i].usuario, listaAdmins[i].clave,
					listaAdmins[i].nombre, listaAdmins[i].activo);
		}
	}
	fclose(archivo);
}

int existeAdmin(char* usuario) {
	for (int i=0; i<contadorAdmins; i++) 
		if(strcmp(listaAdmins[i].usuario, usuario)==0) return 1;
	return 0;
}

void registrarAdmin() {
	if (contadorAdmins >= MAX_ADMINS) { printf("Memoria llena.\n"); return; }
	
	Administrador a;
	printf("\n--- NUEVO ADMINISTRADOR ---\n");
	printf("Usuario: "); scanf("%s", a.usuario);
	if(existeAdmin(a.usuario)) { printf("Usuario ya existe.\n"); return; }
	
	printf("Contrasena: "); scanf("%s", a.clave);
	
	printf("Nombre Completo: ");
	while(getchar()!='\n');
	leerTextoAdmin(a.nombre, 50);
	
	a.activo = 1;
	
	listaAdmins[contadorAdmins] = a;
	contadorAdmins++;
	guardarAdmins();
	printf("Administrador creado con exito.\n");
}

int loginAdmin(char* usuario, char* clave) {
	for (int i=0; i<contadorAdmins; i++) {
		if (listaAdmins[i].activo && 
			strcmp(listaAdmins[i].usuario, usuario)==0 && 
			strcmp(listaAdmins[i].clave, clave)==0) return i;
	}
	return -1;
}
