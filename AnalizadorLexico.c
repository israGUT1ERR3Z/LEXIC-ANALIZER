#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>

struct Tokens {
    char *p_reserv[26],*operador[8],*s_puntuacion[4],*delimitador[6],*t_dato[4],*MAIN,*FMT,*PRINTLN,*SCANLN,*ASIGNACION,*COMPARACION[4];
}tk={
    .p_reserv = {"var","break", "default", "func", "interface", "select", "case", "defer", "go", "map", "struct", "chan", "else", "goto",
				"package","switch", "const", "fallthrough", "if", "range", "type", "continue", "for", "import", "return","default"},
	.operador = {"+", "&", "-", "<", ">", "%", "*", "/"},
	.s_puntuacion = {".", ",", ":", ";"},
	.delimitador = {"(", ")", "[", "]", "{", "}"},
    .t_dato={"int","float","string","complex"},
    .MAIN = "main",
    .FMT = "fmt",
    .PRINTLN = "Println",
    .SCANLN = "Scanln",
    .ASIGNACION=":=",
    .COMPARACION={"<=","==",">=","!="}
};

char delimitadores[] = " \n(){}[];,.:&+-*/%\t=><!";

FILE *abrirArchivo(char nombre[100]) {
    FILE *file = fopen(nombre, "r");
    if (file != NULL) {
        return file;
    } else {
        printf("ERROR AL ABRIR EL ARCHIVO\n");
        exit(1);
    }
}

FILE *crearFicheroSalida(char nombre[100]) {
    FILE *file = fopen(nombre, "w+");
    if (file != NULL) {
        return file;
    } else {
        printf("ERROR AL ABRIR EL LA SALIDA\n");
        exit(1);
    }
}

bool esDelimitador(char c) {
    return strchr(delimitadores, c) != NULL;
}

bool esNumero(char *palabra) {
    for (int i = 0; i < strlen(palabra); i++) {
        if (!isdigit(palabra[i]) && palabra[i] != '.') {
            return false;
        }
    }
    return true;
}

bool esVariable(char *palabra) {
    if (!isalpha(palabra[0]) && palabra[0] != '_') {
        return false;
    }
    for (int i = 1; i < strlen(palabra); i++) {
        if (!isalnum(palabra[i]) && palabra[i] != '_') {
            return false;
        }
    }
    return true;
}

void analisisPalabra(char *palabra, FILE *out) {
	for (int j = 0; j < 27; j++) {
        if (strcmp(palabra, tk.p_reserv[j]) == 0) {
			fprintf(out,"PALABRA RESERVADA -> %s\n", palabra);
            printf("PALABRA RESERVADA -> %s\n", palabra);
            return;
        }
    }
    for (int j = 0; j < 4; j++) {
        if (strcmp(palabra, tk.t_dato[j]) == 0) {
			fprintf(out,"TIPO DATO -> %s\n", palabra);
            printf("TIPO DATO -> %s\n", palabra);
            return;
        }
    }

     if (strcmp(palabra, tk.ASIGNACION) == 0) {
        fprintf(out,"ASIGNACION -> %s\n", palabra);
        printf("ASIGNACION -> %s\n", palabra);
        return;
    }

    for (int j = 0; j < 4; j++) {
        if (strcmp(palabra, tk.COMPARACION[j]) == 0) {
			fprintf(out,"OPERADOR COMPARACION -> %s\n", palabra);
            printf("OPERADOR COMPARACION -> %s\n", palabra);
            return;
        }
    }

    if (esNumero(palabra)) {
		fprintf(out,"NUMERO -> %s\n", palabra);
        printf("NUMERO -> %s\n", palabra);
    } else if (esVariable(palabra)&&strcmp(palabra, tk.MAIN) != 0&&strcmp(palabra, tk.FMT) != 0&&strcmp(palabra, tk.PRINTLN) != 0&&
			strcmp(palabra, tk.SCANLN) != 0&&strcmp(palabra, tk.ASIGNACION) != 0) {
        fprintf(out,"VARIABLE -> %s\n", palabra);
        printf("VARIABLE -> %s\n", palabra);
    } else if (strcmp(palabra, tk.MAIN) == 0) {
        fprintf(out,"MAIN -> %s\n", palabra);
        printf("MAIN -> %s\n", palabra);
    } else if (strcmp(palabra, tk.FMT) == 0) {
        fprintf(out,"FMT -> %s\n", palabra);
        printf("FMT -> %s\n", palabra);
    } else if (strcmp(palabra, tk.PRINTLN) == 0) {
        fprintf(out,"PRINTLN -> %s\n", palabra);
        printf("PRINTLN -> %s\n", palabra);
    } else if (strcmp(palabra, tk.SCANLN) == 0) {
        fprintf(out,"SCANLN -> %s\n", palabra);
        printf("SCANLN -> %s\n", palabra);
    } else if(strcmp(palabra, tk.ASIGNACION) == 0){
    	fprintf(out,"ASIGNACION -> %s\n", palabra);
    	printf("ASIGNACION -> %s\n", palabra);
    }else if (palabra[0] == '"' && palabra[strlen(palabra) - 1] == '"') {
        fprintf(out,"CADENA -> %s\n", palabra);
        printf("CADENA -> %s\n", palabra);
    } else if (strlen(palabra) == 1 && esDelimitador(palabra[0])) {
        fprintf(out,"DELIMITADOR -> %s\n", palabra);
        printf("DELIMITADOR -> %s\n", palabra);
    }
}

void analizadorLexico(FILE *archivo) {
    char letra, token[200],c;
    char op[]="&+-*/%=",pu[]=".,;:",del[]="()[]{}",num[]="0123456789";
    int i = 0;
    bool dentroDeCadena = false;
    FILE *salida=crearFicheroSalida("salida.txt");
    fprintf(salida,"\n------------------------------------------------------\n\nTABLA DE TOKENS:\n\n");
    printf("\n------------------------------------------------------\n\nTABLA DE TOKENS:\n\n");
    while ((letra = getc(archivo)) != EOF) {
		if (letra == '"'){
			if(dentroDeCadena==false){
				dentroDeCadena = true;
                token[i] = letra;
				i++;
			}else{
				token[i] = letra;
				token[i+1]='\0';
				i = 0;
				dentroDeCadena=false;
				analisisPalabra(token,salida);
			}
		}else if(dentroDeCadena==true){
			token[i] = letra;
            i++;
		}else{
			if (!esDelimitador(letra)) {
				token[i] = letra;
				i++;
			} else {
				token[i] = '\0';
				if (i > 0) {
					analisisPalabra(token,salida);
				}
				if (!isspace(letra) && letra != '\n') {
						c=letra;
					for (int j = 0; j < 8; j++) {
						if (c == *tk.operador[j]) {
							if (c == '>'){
								char next=getc(archivo);
								if(next=='='){
									token[i++] = c;
									token[i++] = next;
									token[i] = '\0';
									analisisPalabra(token,salida);
									continue;
								}else{
									ungetc(next, archivo);
								}

							}else if (c == '<'){
								char next=getc(archivo);
								if(next=='='){
									token[i++] = c;
									token[i++] = next;
									token[i] = '\0';
									analisisPalabra(token,salida);
									continue;
								}else{
									ungetc(next, archivo);
								}

							}
							fprintf(salida,"OPERADOR -> %c\n", c);
							printf("OPERADOR -> %c\n", c);
							break;
						}
					}
					for (int j = 0; j < 4; j++) {
						if (c == *tk.s_puntuacion[j]) {
							if (c == ':'){
								char next=getc(archivo);
								if(next=='='){
									token[i++] = c;
									token[i++] = next;
									token[i] = '\0';
									analisisPalabra(token,salida);
									continue;
								}else{
									ungetc(next, archivo);
								}

							}
							fprintf(salida,"SIGNO PUNTUACION -> %c\n", c);
							printf("SIGNO PUNTUACION -> %c\n", c);
							break;

						}
					}
					for (int j = 0; j < 6; j++) {
						if (c == *tk.delimitador[j]) {
							fprintf(salida,"DELIMITADOR -> %c\n", c);
							printf("DELIMITADOR -> %c\n", c);
							break;
						}
					}
				}
				if (letra == '=' && (c = getc(archivo)) == '=') {
					token[i++] = letra;
					token[i++] = c;
					token[i] = '\0';
					analisisPalabra(token,salida);
				}else if (letra == '!' && (c = getc(archivo)) == '=') {
					token[i++] = letra;
					token[i++] = c;
					token[i] = '\0';
					analisisPalabra(token,salida);
				}
				i = 0;


			}
		}

    }
    if (fclose(archivo) != 0) {
        printf("ERROR AL CERRAR ARCHIVO\n");
    }
    if (fclose(salida) != 0) {
        printf("ERROR AL CERRAR ARCHIVO DE SALIDA\n");
    }
}


int main() {
    setlocale(LC_CTYPE, "spanish");
    FILE *archivo = abrirArchivo("codigo.txt");
    analizadorLexico(archivo);
    printf("\n\n");
    system("pause");
    return 0;
}
