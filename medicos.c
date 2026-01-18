#include <stdio.h>
#include <string.h>
#include "medicos.h"

Medico listaMedicos[MAX_MEDICOS];
int contadorMedicos = 0;

const char* ESPECIALIDADES[] = {
	"Medicina General",
		"Pediatria",
		"Cardiologia",
		"Dermatologia",
		"Ginecologia",
		"Traumatologia"
};
const int TOTAL_ESPECIALIDADES = 6;

void leerTexto(char* buffer, int max) {
	fgets(buffer, max, stdin);
	buffer[strcspn(buffer, "\n")] = 0;
}

void cargarMedicos() {
	FILE *archivo = fopen(ARCHIVO_MEDICOS, "r");
	if (archivo == NULL) return;
	contadorMedicos = 0;
	while (fscanf(archivo, "%[^;];%[^;];%[^;];%[^;];%[^;];%d\n",
				  listaMedicos[contadorMedicos].codigo,
				  listaMedicos[contadorMedicos].nombre,
				  listaMedicos[contadorMedicos].especialidad,
				  listaMedicos[contadorMedicos].clave,
				  listaMedicos[contadorMedicos].horario,
				  &listaMedicos[contadorMedicos].activo) == 6) {
		contadorMedicos++;
	}
	fclose(archivo);
}

void guardarMedicos() {
	FILE *archivo = fopen(ARCHIVO_MEDICOS, "w");
	if (archivo == NULL) return;
	for (int i = 0; i < contadorMedicos; i++) {
		if (listaMedicos[i].activo) {
			fprintf(archivo, "%s;%s;%s;%s;%s;%d\n",
					listaMedicos[i].codigo, listaMedicos[i].nombre,
					listaMedicos[i].especialidad, listaMedicos[i].clave,
					listaMedicos[i].horario, listaMedicos[i].activo);
		}
	}
	fclose(archivo);
}

void registrarMedico() {
	if (contadorMedicos >= MAX_MEDICOS) return;
	Medico m;
	m.activo = 1;
	strcpy(m.horario, "DISPONIBLE");
	
	printf("\n--- ALTA DE NUEVO MEDICO ---\n");
	
	printf("Codigo Medico (Usuario): "); 
	scanf("%s", m.codigo);
	
	printf("Nombre Completo: ");
	while(getchar()!='\n');
	leerTexto(m.nombre, 50);
	
	int opEsp;
	printf("\nSeleccione Especialidad:\n");
	for(int i=0; i<TOTAL_ESPECIALIDADES; i++) {
		printf("%d. %s\n", i+1, ESPECIALIDADES[i]);
	}
	do {
		printf("Opcion: ");
		scanf("%d", &opEsp);
	} while(opEsp < 1 || opEsp > TOTAL_ESPECIALIDADES);
	
	strcpy(m.especialidad, ESPECIALIDADES[opEsp-1]);
	
	printf("Cree su Contrasena: ");
	scanf("%s", m.clave);
	
	listaMedicos[contadorMedicos] = m;
	contadorMedicos++;
	guardarMedicos();
	printf("Medico registrado exitosamente.\n");
}

int loginMedico(char* codigo, char* clave) {
	for (int i=0; i<contadorMedicos; i++) {
		if (listaMedicos[i].activo && 
			strcmp(listaMedicos[i].codigo, codigo)==0 && 
			strcmp(listaMedicos[i].clave, clave)==0) return i;
	}
	return -1;
}

void listarMedicos() {
	printf("\n--- STAFF MEDICO ---\n");
	for(int i=0; i<contadorMedicos; i++){
		if(listaMedicos[i].activo) 
			printf("- Dr. %s [%s]\n", listaMedicos[i].nombre, listaMedicos[i].especialidad);
	}
}

int obtenerCantidadMedicos() { return contadorMedicos; }
