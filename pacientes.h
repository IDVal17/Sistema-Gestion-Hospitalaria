#ifndef PACIENTES_H
#define PACIENTES_H

#define MAX_PACIENTES 1000
#define ARCHIVO_PACIENTES "db/pacientes/pacientes.txt"

typedef struct {
	char nombre[50];
	char cedula[15];
	int edad;
	char telefono[15];
	char correo[50];
	int activo; 
} Paciente;

void cargarPacientes();
void guardarPacientes();
int existePaciente(const char* cedula);
void registrarPacienteSimple(char* cedulaPrevia);

#endif
