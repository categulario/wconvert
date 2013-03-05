#include<stdio.h>
#include<stdlib.h>
#include<string.h>

const short int PNORTE=1,PESTE=2,PSUR=4,POESTE=8;

struct TTokenMundo{
    short int p1,p2,p3;
};
struct TTokenMundo2{
   unsigned short int p1,p2,p3;
};
struct casilla{
 int pared,zumbadores;
};

FILE *entrada;
TTokenMundo token;
TTokenMundo2 token2;
int alto,ancho,mochila;
int columna,fila,orientacion;
char s_orientacion[100], s_paredes[100],s_salida[200];
unsigned short int numParedes,numZumb;
casilla mundo[101][101];
int i,j,contador,c;
float k;


int main(int argc, char *argv[]) {
    if(argc<=1) {
        printf("Falta el nombre del archivo .mdo\n");
        exit(0);
    }
    if(argc<=2)  {
        printf("Falta el nombre del archivo .world\n");
        exit(0);
    }

    entrada=fopen(argv[1],"rb");
    sprintf(s_salida,"%s",argv[2]);
    if(!entrada) {
        printf("No se encontro el archivo %s\n",argv[1]);
        exit(0);
    }
    if(!fread(&token,sizeof(TTokenMundo),1,entrada)) {
        printf("no se pudo leer el archivo %s\n",argv[1]);
        fclose(entrada);
        exit(0);
    }
    if(token.p1!=16715 or token.p2!=17746 || token.p3!=8268) {
        printf("El formato del archivo de entrada (%s) es incorrecto\n",argv[1]);
        fclose(entrada);
        exit(0);
    }
    if(!fread(&token,sizeof(TTokenMundo),1,entrada)) {
        printf("no se pudo leer el archivo %s\n",argv[1]);
        fclose(entrada);
        exit(0);
    }
    if(token.p1!=19791 or token.p2!=11849) {
        printf("El formato del archivo de entrada (%s) es incorrecto\n",argv[1]);
        fclose(entrada);
        exit(0);
    }
    freopen(s_salida,"w",stdout);
    fread(&token,sizeof(TTokenMundo),1,entrada);

    ancho=token.p1;
    alto=token.p2;
    mochila=token.p3;

    fread(&token,sizeof(TTokenMundo),1,entrada);
    columna=token.p1;
    fila=token.p2;
    orientacion=token.p3;
    if(orientacion==1)
        sprintf(s_orientacion,"norte");
    if(orientacion==2)
        sprintf(s_orientacion,"este");
    if(orientacion==3)
        sprintf(s_orientacion,"sur");
    if(orientacion==4)
        sprintf(s_orientacion,"oeste");
    printf("{\n");
    printf("\t\"karel\": {\n");
    printf("\t\t\"posicion\": [\n");
    printf("\t\t\t%hd,\n",fila);
    printf("\t\t\t%hd\n",columna);
    printf("\t\t],\n");
    printf("\t\t\"orientacion\": \"%s\",\n",s_orientacion);
    printf("\t\t\"mochila\": %hd\n",mochila);
    printf("\t},\n");
    printf("\t\"casillas\": [\n");
    fread(&token,sizeof(TTokenMundo),1,entrada);

    numParedes=token.p1;
    numZumb=token.p2;

    for(i=1;i<=100;i++){
        for(j=1;j<=100;j++) {
            mundo[i][j].pared=0;
            mundo[i][j].zumbadores=0;
        }
    }
    for(i=1;i<=numParedes;i++) {
        fread(&token,sizeof(TTokenMundo),1,entrada);
        if(token.p2==100)
            token.p3=token.p3-1;
        if(token.p2==1)
            token.p3=token.p3-4;
        if(token.p1==100)
            token.p3=token.p3-2;
        if(token.p1==1)
            token.p3=token.p3-8;
        if(token.p3>0)
            mundo[token.p2][token.p1].pared=token.p3;
    }

    for(i=1;i<=numZumb;i++) {
        fread(&token,sizeof(TTokenMundo),1,entrada);
        mundo[token.p2][token.p1].zumbadores=token.p3;
    }

    contador=0;
    for(i=1;i<=100;i++){
        for(j=1;j<=100;j++){
            if(mundo[i][j].pared>0 || mundo[i][j].zumbadores!=0) {
                if(contador>0) {
                    printf("\t\t,{\n");
                    contador=0;
                } else {
                    printf("\t\t{\n");
                }
                printf("\t\t\t\"zumbadores\": %hd,\n",mundo[i][j].zumbadores);
                sprintf(s_paredes,"");
                c=0;
                k=8;
                while(k>=1) {
                    if(mundo[i][j].pared>=k) {
                        if(c>0) {
                            sprintf(s_paredes,"%s,",s_paredes);
                            c=0;
                        }
                        if(k==8) {
                            sprintf(s_paredes,"%s\"oeste\"",s_paredes);
                            mundo[i][j].pared=mundo[i][j].pared-8;
                        }
                        if(k==4) {
                            sprintf(s_paredes,"%s\"sur\"",s_paredes);
                            mundo[i][j].pared=mundo[i][j].pared-4;
                        }
                        if(k==2) {
                            sprintf(s_paredes,"%s\"este\"",s_paredes);
                            mundo[i][j].pared=mundo[i][j].pared-2;
                        }
                        if(k==1) {
                            sprintf(s_paredes,"%s\"norte\"",s_paredes);
                            mundo[i][j].pared=mundo[i][j].pared-1;
                        }
                        c=1;
                    }
                    k=k/2;
                }
                printf("\t\t\t\"paredes\": [%s],\n",s_paredes);
                printf("\t\t\t\"fila\": %d,\n",i);
                printf("\t\t\t\"columna\": %d",j);
                printf("\t\t}\n");
                contador=1;
            }
        }
    }

    printf("\t],\n");
    printf("\t\"dimensiones\": {\n");
    printf("\t\t\"columnas\": %hd,\n",ancho);
    printf("\t\t\"filas\": %hd\n",alto);
    printf("\t}\n");
    printf("}\n");
    fclose(entrada);
    return 0;
}
