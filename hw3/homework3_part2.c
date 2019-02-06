#include <stdio.h>                            /* HOMEWORK 3 PART 2 FERHAT ŞİRİN 161044080   */
#include <stdlib.h>
#include <string.h>

void takeCoordinates(double ***coord,int *n);
void printCoord(double **coord,int n);
void calculatePolynomial(double **coord,int n);

int main(void) {
    
    int i,n;
    double **coord;

  
    takeCoordinates(&coord,&n);
    printCoord(coord,n);
    
    calculatePolynomial(coord,n);
    
    for(i=0;i<n;++i){
        free(coord[i]);
    }
    free(coord);

    return (EXIT_SUCCESS);
}

void printCoord(double **coord,int n){
    int i;
    printf("       Coordinates\n");
    for(i=0;i<n;++i){
        printf("x[%d] -> %f  y[%d] -> %f \n",i,coord[i][0],i,coord[i][1]);
    }
}

void takeCoordinates(double ***coord,int *n){
    int i,j;  FILE *input; char *value;
    char name[50],line[256]; char selector[2] =",";
    printf("Enter the name of file \n");
    scanf("%s",name);
    input =fopen(name,"r");
    while(input ==NULL){
        printf("File can not be opened try it again\n");
        scanf("%s",name);
        input =fopen(name,"r");
    }
    *coord =NULL;
    for(i=0;fgets(line,256,input)!=NULL;++i){
        *coord =realloc((*coord),sizeof(double*)*(i+1));
        value =strtok(line,selector);
        for(j=0;value !=NULL;++j){
            (*coord)[i] =realloc((*coord)[i],sizeof(double)*(j+1));
            sscanf(value,"%lf",&(*coord)[i][j]);
            value =strtok(NULL,selector);
        }
    }
    *n =i;
    
    fclose(input);
}

void calculatePolynomial(double **coord,int n){

    int i,j;
    double **p;
    
    p =malloc(sizeof(double*)*n);
    
    for(i=0;i<n;++i){
        p[i] =malloc(sizeof(double)*n);
    }
    
    for(i=0;i<n;++i){
        p[i][0] =coord[i][1];
    }
    
    for(i=1;i<n;++i){
        for(j=1;j<=i;++j){
            p[i][j] =(p[i][j-1]-p[i-1][j-1])/(coord[i][0]-coord[i-j][0]);
        }
    }
    printf("\n         Polynomial \n");
    printf("P(x) = %f",p[0][0]);
    for(i=1;i < n;++i){
        printf("+ %f ",p[i][i]);
        for(j=0;j<i;++j){
            printf("*(x-%f) ",coord[j][0]);
        }
    }
    printf("\n");
    
    for(i=0;i<n;++i){
        free(p[i]);
    }
    free(p);
}
