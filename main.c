#include <stdio.h>
#include <stdlib.h>
#include "citas.h"
#include "pacientes.h"
#include "medicos.h"
#include "admin.h"

void limpiar() { 
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

void pausar() { printf("\nPresione Enter..."); while(getchar()!='\n'); getchar(); }

void menuSesionAdmin() {
	int op;
	do {
		limpiar();
		printf(">> PANEL ADMINISTRADOR <<\n");
		printf("1. Gestionar Citas\n");
		printf("2. Ver Historial\n");
		printf("3. Listar Medicos\n");
		printf("4. Salir\n");
		printf("Op: "); scanf("%d", &op);
		
		switch(op) {
		case 1: gestionarCitasAdmin(); pausar(); break;
		case 2: verHistorialGeneral(); pausar(); break;
		case 3: listarMedicos(); pausar(); break;
		}
	} while(op != 4);
}

void menuSesionMedico(char* codigoMedico) {
	int op;
	do {
		limpiar();
		printf(">> PANEL MEDICO (Dr. %s) <<\n", codigoMedico);
		printf("1. Ver Historial\n");
		printf("2. Finalizar Cita\n");
		printf("3. Salir\n");
		printf("Op: "); scanf("%d", &op);
		
		switch(op) {
		case 1: verHistorialGeneral(); pausar(); break; 
		case 2: finalizarCitaMedico(codigoMedico); pausar(); break;
		}
	} while(op != 3);
}
// Modificacion realizada por la rama isaac
void zonaStaff() {
	int tipo, accion;
	limpiar();
	printf("=== ZONA STAFF ===\n");
	printf("1. ADMINISTRADOR\n");
	printf("2. MEDICO\n");
	printf("Op: "); scanf("%d", &tipo);
	
	printf("\n1. Iniciar Sesion\n");
	printf("2. Crear Usuario (Registro Abierto)\n");
	printf("Op: "); scanf("%d", &accion);
	
	char user[20], pass[20];
	
	if (accion == 2) {
		if (tipo == 1) registrarAdmin();
		else registrarMedico();
		pausar();
		return;
	}
	
	if (accion == 1) {
		printf("Usuario: "); scanf("%s", user);
		printf("Contrasena: "); scanf("%s", pass);
		
		if (tipo == 1) { 
			if (loginAdmin(user, pass) != -1) menuSesionAdmin();
			else { printf("Credenciales incorrectas.\n"); pausar(); }
		} else { 
			if (loginMedico(user, pass) != -1) menuSesionMedico(user);
			else { printf("Credenciales incorrectas.\n"); pausar(); }
		}
	}
}

int main() {
	cargarAdmins();
	cargarPacientes();
	cargarMedicos();
	cargarCitasDesdeArchivo();
	
	int opcion;
	do {
		limpiar();
		printf("======================================\n");
		printf("      SISTEMA HOSPITALARIO V4.0\n");
		printf("======================================\n");
		printf("1. INICIAR SESION (Staff)\n");
		printf("2. ENTRAR COMO PACIENTE\n");
		printf("3. Salir\n");
		printf("======================================\n");
		printf("Seleccione: ");
		scanf("%d", &opcion);
		
		switch(opcion) {
		case 1: zonaStaff(); break;
		case 2: gestionCitaPaciente(); pausar(); break;
		case 3:
			guardarAdmins();
			guardarPacientes();
			guardarMedicos();
			guardarCitasEnArchivo();
			generarReporteGeneral();
			break;
		}
	} while(opcion != 3);
	
	return 0;
}
