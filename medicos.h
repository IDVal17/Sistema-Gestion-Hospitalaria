#ifndef MEDICOS_H
#define MEDICOS_H

#define MAX_MEDICOS 100
#define ARCHIVO_MEDICOS "db/medicos/medicos.txt"

typedef struct {
	char codigo[15];
	char nombre[50];
	char especialidad[50];
	char clave[20];
	char horario[50]; 
	int activo;
} Medico;

void cargarMedicos();
void guardarMedicos();
void listarMedicos();
int obtenerCantidadMedicos();
void registrarMedico();
int loginMedico(char* codigo, char* clave);

char* buscarMedicoPorEspecialidad(char* especialidadBuscada, char* fechaHora);

#endif
