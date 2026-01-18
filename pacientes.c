#include <stdio.h>
#include <string.h>
#include "pacientes.h"

Paciente listaPacientes[MAX_PACIENTES];
int contadorPacientes = 0;

void leerTextoPaciente(char* buffer, int max) {
	fflush(stdin);
	fgets(buffer, max, stdin);
	buffer[strcspn(buffer, "\n")] = 0;
}

void cargarPacientes() {
	FILE *archivo = fopen(ARCHIVO_PACIENTES, "r");
	if (!archivo) return;
	contadorPacientes = 0;
	while (fscanf(archivo, "%[^;];%[^;];%d;%[^;];%[^;];%d\n",
				  listaPacientes[contadorPacientes].nombre,
				  listaPacientes[contadorPacientes].cedula,
				  &listaPacientes[contadorPacientes].edad,
				  listaPacientes[contadorPacientes].telefono,
				  listaPacientes[contadorPacientes].correo,
				  &listaPacientes[contadorPacientes].activo) == 6) {
		contadorPacientes++;
	}
	fclose(archivo);
}

void guardarPacientes() {
	FILE *archivo = fopen(ARCHIVO_PACIENTES, "w");
	if (!archivo) return;
	for (int i = 0; i < contadorPacientes; i++) {
		if (listaPacientes[i].activo) {
			fprintf(archivo, "%s;%s;%d;%s;%s;%d\n",
					listaPacientes[i].nombre, listaPacientes[i].cedula,
					listaPacientes[i].edad, listaPacientes[i].telefono,
					listaPacientes[i].correo, listaPacientes[i].activo);
		}
	}
	fclose(archivo);
}

int existePaciente(const char* cedula) {
	for (int i = 0; i < contadorPacientes; i++) 
		if (listaPacientes[i].activo && strcmp(listaPacientes[i].cedula, cedula) == 0) return 1;
	return 0;
}

void registrarPacienteSimple(char* cedulaPrevia) {
	Paciente p;
	p.activo = 1;
	strcpy(p.cedula, cedulaPrevia);
	
	printf("\n>> REGISTRO DE PACIENTE NUEVO\n");
	
	printf("Nombre Completo: ");
	while(getchar()!='\n');
	leerTextoPaciente(p.nombre, 50);
	
	printf("Edad: "); scanf("%d", &p.edad);
	printf("Telefono: "); scanf("%s", p.telefono);
	printf("Correo: "); scanf("%s", p.correo);
	
	listaPacientes[contadorPacientes] = p;
	contadorPacientes++;
	guardarPacientes();
	printf("Datos guardados.\n");
}
