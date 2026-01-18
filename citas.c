#include <stdio.h>
#include <string.h>
#include "Citas.h"
#include "pacientes.h"
#include "medicos.h"

Cita listaCitas[MAX_CITAS];
int contadorCitas = 0;

const char* ESPECIALIDADES_CITAS[] = {
	"Medicina General", "Pediatria", "Cardiologia", "Dermatologia", "Ginecologia", "Traumatologia"
};
const int TOTAL_ESP_CITAS = 6;

extern Medico listaMedicos[]; 
extern int contadorMedicos;

void cargarCitasDesdeArchivo() {
	FILE *archivo = fopen(ARCHIVO_CITAS, "r");
	if (!archivo) return;
	contadorCitas = 0;
	while (fscanf(archivo, "%d;%[^;];%[^;];%[^;];%d;%[^;];%[^;]\n",
				  &listaCitas[contadorCitas].idCita,
				  listaCitas[contadorCitas].cedulaPaciente,
				  listaCitas[contadorCitas].codigoMedico,
				  listaCitas[contadorCitas].fechaHora,
				  (int*)&listaCitas[contadorCitas].estado,
				  listaCitas[contadorCitas].sinopsis,
				  listaCitas[contadorCitas].receta) == 7) {
		contadorCitas++;
	}
	fclose(archivo);
}

void guardarCitasEnArchivo() {
	FILE *archivo = fopen(ARCHIVO_CITAS, "w");
	if (!archivo) return;
	for(int i=0; i<contadorCitas; i++){
		fprintf(archivo, "%d;%s;%s;%s;%d;%s;%s\n",
				listaCitas[i].idCita, listaCitas[i].cedulaPaciente,
				listaCitas[i].codigoMedico, listaCitas[i].fechaHora,
				listaCitas[i].estado, listaCitas[i].sinopsis, listaCitas[i].receta);
	}
	fclose(archivo);
}

char* encontrarMedicoAuto(char* especialidad, char* fechaHora) {
	for (int i = 0; i < contadorMedicos; i++) {
		if (listaMedicos[i].activo && strcmp(listaMedicos[i].especialidad, especialidad) == 0) {
			
			int ocupado = 0;
			for (int j = 0; j < contadorCitas; j++) {
				if (listaCitas[j].estado == PROGRAMADA &&
					strcmp(listaCitas[j].codigoMedico, listaMedicos[i].codigo) == 0 &&
					strcmp(listaCitas[j].fechaHora, fechaHora) == 0) {
					ocupado = 1;
					break;
				}
			}
			
			if (!ocupado) {
				return listaMedicos[i].codigo;
			}
		}
	}
	return NULL;
}

void gestionCitaPaciente() {
	char cedula[15];
	printf("\n--- ZONA DE PACIENTES ---\n");
	printf("Ingrese su Cedula: ");
	scanf("%s", cedula);
	
	if (!existePaciente(cedula)) {
		printf(">> Registrando nuevo paciente...\n");
		registrarPacienteSimple(cedula);
	}
	
	int indiceCitaActiva = -1;
	for(int i=0; i<contadorCitas; i++) {
		if(strcmp(listaCitas[i].cedulaPaciente, cedula) == 0 && 
		   (listaCitas[i].estado == PROGRAMADA)) {
			indiceCitaActiva = i;
			break;
		}
	}
	
	if (indiceCitaActiva != -1) {
		printf("\n[AVISO] Ya tiene una cita programada.\n");
		printf("Detalles: ID %d | Fecha: %s\n", listaCitas[indiceCitaActiva].idCita, listaCitas[indiceCitaActiva].fechaHora);
		char resp;
		printf(">> Desea ANULAR esta cita? (S/N): ");
		while(getchar()!='\n');
		scanf("%c", &resp);
		
		if (resp == 'S' || resp == 's') {
			printf("Motivo de anulacion: ");
			char motivo[100];
			scanf("%s", motivo);
			char buffer[200];
			sprintf(buffer, "CANCELADA POR PACIENTE: %s", motivo);
			strcpy(listaCitas[indiceCitaActiva].sinopsis, buffer);
			listaCitas[indiceCitaActiva].estado = CANCELADA;
			guardarCitasEnArchivo();
			printf("Cita anulada.\n");
		}
	} else {
		printf("\n>> AGENDAMIENTO DE CITA\n");
		
		int opEsp;
		printf("Seleccione la especialidad requerida:\n");
		for(int i=0; i<TOTAL_ESP_CITAS; i++) {
			printf("%d. %s\n", i+1, ESPECIALIDADES_CITAS[i]);
		}
		do {
			printf("Opcion: ");
			scanf("%d", &opEsp);
		} while(opEsp < 1 || opEsp > TOTAL_ESP_CITAS);
		
		char especialidadElegida[50];
		strcpy(especialidadElegida, ESPECIALIDADES_CITAS[opEsp-1]);
		
		char fechaHora[20];
		printf("Fecha y Hora deseada (DD-MM-AAAA_HH:MM): ");
		scanf("%s", fechaHora);
		
		char* codigoDoctorEncontrado = encontrarMedicoAuto(especialidadElegida, fechaHora);
		
		if (codigoDoctorEncontrado != NULL) {
			Cita nueva;
			nueva.idCita = contadorCitas + 1;
			strcpy(nueva.cedulaPaciente, cedula);
			strcpy(nueva.codigoMedico, codigoDoctorEncontrado);
			strcpy(nueva.fechaHora, fechaHora);
			nueva.estado = PROGRAMADA;
			strcpy(nueva.sinopsis, "PENDIENTE");
			strcpy(nueva.receta, "N/A");
			
			listaCitas[contadorCitas] = nueva;
			contadorCitas++;
			guardarCitasEnArchivo();
			
			printf("\n============================================\n");
			printf(" EXITO! Cita Agendada.\n");
			printf(" Especialidad: %s\n", especialidadElegida);
			printf(" Medico Asignado: %s\n", codigoDoctorEncontrado);
			printf(" Fecha: %s\n", fechaHora);
			printf("============================================\n");
			
		} else {
			printf("\n[LO SENTIMOS] No hay medicos disponibles de %s para esa fecha/hora.\n", especialidadElegida);
			printf("Por favor intente otro horario.\n");
		}
	}
}

void finalizarCitaMedico(char* codigoMedicoLogueado) {
	int id;
	printf("Ingrese ID de la cita a finalizar: ");
	scanf("%d", &id);
	
	for(int i=0; i<contadorCitas; i++) {
		if(listaCitas[i].idCita == id) {
			if(strcmp(listaCitas[i].codigoMedico, codigoMedicoLogueado) != 0) {
				printf("Error: Esta cita no le pertenece.\n");
				return;
			}
			printf("Sinopsis clinica (Use_guiones): ");
			scanf("%s", listaCitas[i].sinopsis);
			printf("Receta (Use_guiones): ");
			scanf("%s", listaCitas[i].receta);
			
			listaCitas[i].estado = REALIZADA;
			guardarCitasEnArchivo();
			printf("Cita finalizada.\n");
			return;
		}
	}
	printf("Cita no encontrada.\n");
}

void gestionarCitasAdmin() {
	int id;
	printf("\n--- GESTION ADMIN ---\n");
	printf("Ingrese ID de Cita: ");
	scanf("%d", &id);
	
	for(int i=0; i<contadorCitas; i++) {
		if(listaCitas[i].idCita == id) {
			printf("Estado actual: %d (0=Prog, 2=Cancel)\n", listaCitas[i].estado);
			printf("1. Cancelar Cita\nOp: ");
			int op; scanf("%d", &op);
			if(op==1) {
				listaCitas[i].estado = CANCELADA;
				strcpy(listaCitas[i].sinopsis, "CANCELADA POR ADMIN");
			}
			guardarCitasEnArchivo();
			printf("Actualizado.\n");
			return;
		}
	}
	printf("ID no encontrado.\n");
}

void verHistorialGeneral() {
	printf("\n--- HISTORIAL COMPLETO ---\n");
	for(int i=0; i<contadorCitas; i++) {
		printf("ID: %d | Pcte: %s | Doc: %s | Fecha: %s | Est: %d\n", 
			   listaCitas[i].idCita, listaCitas[i].cedulaPaciente, 
			   listaCitas[i].codigoMedico, listaCitas[i].fechaHora, listaCitas[i].estado);
	}
}

void generarReporteGeneral() {
	FILE *f = fopen("reportes/resumen.txt", "w");
	if(!f) return;
	fprintf(f, "REPORTE GENERAL\nTotal Citas: %d\n", contadorCitas);
	fclose(f);
}
