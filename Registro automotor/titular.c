#include <stdio.h>
#include <string.h>
#include "titular.h"
#include "automotor.h"
#include "domicilio.h"

void altaTitular() {
    FILE *archivo = fopen("titulares.txt", "a");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de titulares.\n");
        return;
    }

    Titular nuevo;
    getchar(); // limpia el '\n' que queda en el buffer
    printf("Ingrese nombre: ");
    fgets(nuevo.nombre, sizeof(nuevo.nombre), stdin);
    nuevo.nombre[strcspn(nuevo.nombre, "\n")] = '\0';

    printf("Ingrese CUIT: ");
    scanf("%d", &nuevo.cuit);
    getchar(); // limpia el '\n' que queda en el buffer

    printf("Ingrese tipo de documento (DNI, LC, LE, etc): ");
    fgets(nuevo.tipoDocumento, sizeof(nuevo.tipoDocumento), stdin);
    nuevo.tipoDocumento[strcspn(nuevo.tipoDocumento, "\n")] = '\0';

    printf("Ingrese numero de documento: ");
    scanf("%d", &nuevo.nroDocumento);
    getchar(); // limpia el '\n' que queda en el buffer

    printf("Ingrese fecha de nacimiento (dd/mm/aaaa): ");
    fgets(nuevo.fechaNacimiento, sizeof(nuevo.fechaNacimiento), stdin);
    nuevo.fechaNacimiento[strcspn(nuevo.fechaNacimiento, "\n")] = '\0';

    nuevo.idDomicilio = crearDomicilio(); 

    fprintf(archivo, "%s;%d;%s;%d;%s;%d\n",
        nuevo.nombre,
        nuevo.cuit,
        nuevo.tipoDocumento,
        nuevo.nroDocumento,
        nuevo.fechaNacimiento,
        nuevo.idDomicilio
    );

    fclose(archivo);
    printf("Titular guardado exitosamente.\n");
}

void listarTitularesConVehiculos() {
    FILE *ft = fopen("titulares.txt", "r");
    FILE *fa = fopen("registro.txt", "r");
    FILE *fd = fopen("domicilios.txt", "r");

    if (!ft || !fa || !fd) {
        printf("Error al abrir archivos.\n");
        return;
    }

    Titular t;
    Automotor a;
    Domicilio d;

    while (fscanf(ft, "%[^;];%d;%[^;];%d;%[^;];%d\n",
                  t.nombre, &t.cuit, t.tipoDocumento,
                  &t.nroDocumento, t.fechaNacimiento,
                  &t.idDomicilio) == 6) {

        printf("\n--- Titular ---\n");
        printf("Nombre: %s\n", t.nombre);
        printf("CUIT: %d\n", t.cuit);
        printf("Tipo de Documento: %s\n", t.tipoDocumento);
        printf("Numero de Documento: %d\n", t.nroDocumento);
        printf("Fecha de Nacimiento: %s\n", t.fechaNacimiento);

        // Buscar domicilio
        rewind(fd);
        int encontrado = 0;
        while (fscanf(fd, "%d;%[^;];%[^;];%d;%[^;];%d\n",
                      &d.idDomicilio, d.ciudad, d.provincia,
                      &d.codigoPostal, d.calle, &d.numero) == 6) {
            if (d.idDomicilio == t.idDomicilio) {
                printf("Domicilio: %s %d, %s (%d), %s\n",
                       d.calle, d.numero, d.ciudad, d.codigoPostal, d.provincia);
                encontrado = 1;
                break;
            }
        }
        if (!encontrado) {
            printf("Domicilio: [No encontrado]\n");
        }

        // Buscar autos
        rewind(fa);
        int tieneAutos = 0;

        while (fscanf(fa, "%d;%[^;];%[^;];%[^;];%[^;];%[^;];%d;%[^;];%[^;];%d;%d\n",
                      &a.idVehiculo, a.dominio, a.marca, a.modelo, a.chasis, a.motor,
                      &a.anioFabricacion, a.paisOrigen, a.tipoUso, &a.peso,
                      &a.nroDocTitular) == 11) {

            if (a.nroDocTitular == t.nroDocumento) {
                if (!tieneAutos) {
                    printf("Vehiculos:\n");
                    tieneAutos = 1;
                }
                printf("  | Dominio: %s | Marca: %s | Modelo: %s | Anio: %d\n",
                       a.dominio, a.marca, a.modelo, a.anioFabricacion);
            }
        }

        if (!tieneAutos) {
            printf("No tiene vehiculos registrados.\n");
        }

        printf("---------------------------\n");
    }

    fclose(ft);
    fclose(fa);
    fclose(fd);
}