#ifndef CITAS_H
#define CITAS_H

#define MAX_CITAS 1000
#define ARCHIVO_CITAS "db/registros/citas.txt"

typedef enum { PROGRAMADA, REALIZADA, CANCELADA, REPROGRAMADA } EstadoCita;

typedef struct {
	int idCita;
	char cedulaPaciente[15];
	char codigoMedico[15];
	char fechaHora[20];
	EstadoCita estado;
	char sinopsis[200]; 
	char receta[200];
} Cita;

void cargarCitasDesdeArchivo();
void guardarCitasEnArchivo();
void gestionCitaPaciente();
void gestionarCitasAdmin();
void finalizarCitaMedico(char* codigoMedicoLogueado);
void verHistorialGeneral();
void generarReporteGeneral();

#endif
