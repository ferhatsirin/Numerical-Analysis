#include <stdio.h>                   /* HOMEWORK 3 PART 3 FERHAT ŞİRİN 161044080   */
#include <stdlib.h>
#include <string.h>
#include <math.h>

void takeCoordinates(double ***coord,int *n);
void printCoord(double **coord,int n);
void calculatePolynomial(double **coord,int numOfCoord,int degree);
int scaled_pivot(double **array,double *sol,int n);  /* uniq -> 1  notuniq -> 0  */
void changeRow(double **array,int row1,int row2,int n);
void subtractEqu(double **array,int row1,int row2,double coef,int n);  /* row1 - coef*row2 ---> row1 */
void backwardSub(double **array,double *sol,int n);
void swap(double *num1,double *num2);
void printSol(double *sol,int n);
void freeAll(double **array,double *sol,int n);

int main(void) {
    
    int i,n,degree;
    double **coord;

  
    takeCoordinates(&coord,&n);
    printCoord(coord,n);
    printf("Enter the degree of polynomial you want \n");
    scanf("%d",&degree);
    while( n-1 <= degree){
        printf("The degree of polynomial can not be higher than or equal to number of coordiantes \n");
        printf("Enter smaller number \n");
        scanf("%d",&degree);
    }
    
    calculatePolynomial(coord,n,degree);
    
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

void calculatePolynomial(double **coord,int numOfCoord,int degree){
    int i,j,k; double sum;
    double **p,*sol;
    
    sol =malloc(sizeof(double)*(degree+1));
    p =malloc(sizeof(double*)*(degree+1));
    for(i=0;i<degree+1;++i){
        p[i] =malloc(sizeof(double)*(degree+2));
    }
    
    for(i=0;i<=degree;++i){
        for(j=0;j<=degree;++j){
            sum =0;
            for(k=0;k<numOfCoord;++k){
               sum +=pow(coord[k][0],j+i);
            }
            p[i][j] =sum;
        }
        sum =0;
        for(k=0;k<numOfCoord;++k){
            sum +=pow(coord[k][0],i)*coord[k][1];
        }
        p[i][j] =sum;
    }

    scaled_pivot(p,sol,degree+1);
    printSol(sol,degree+1);
    
    printf("P(x) = %f ",sol[0]);
    for(i=1;i<=degree;++i){
        printf("+ %f*x^%d ",sol[i],i);
    }
    printf("\n");
    
    freeAll(p,sol,degree+1);
}

int scaled_pivot(double **array,double *sol,int n){

    int i,j,pivot; double coef,max; int notUniq,ret;
    double *rowMax=(double*)malloc(sizeof(double)*n);
 
    notUniq =0;
    for(i=0;i<n && !notUniq;++i){
       rowMax[i] =fabs(array[i][0]);
       for(j=0;j<n;++j){
           if(rowMax[i] < fabs(array[i][j])){
               rowMax[i] =fabs(array[i][j]);
           }
        }
        if(rowMax[i] == 0){
            notUniq =1;
        }
            
    }
     
    for(i=0; i<n-1 && !notUniq;++i){

       max =fabs(array[i][i])/rowMax[i];
       pivot =i;
       for(j=i+1;j<n;++j){
           if(max < fabs(array[j][i])/rowMax[j]){
               max =fabs(array[j][i]);
               pivot =j;
            }
        }
        
        if(max == 0){
           notUniq =1;
         }
        else{
               
             if(i != pivot){
                changeRow(array,i,pivot,n);
                swap(&rowMax[pivot],&rowMax[i]);
             }
             for(j = i+1;j<n;++j){
                 coef =array[j][i] / array[i][i];
                 subtractEqu(array,j,i,coef,n);   /* j - coef*i */
            }
        }
         
 
    } 
    if(array[n-1][n] == 0 ){
       notUniq =1;
    }
    
    if(notUniq){
        ret =0;
    }
    else{
        ret =1;
        backwardSub(array,sol,n);
    }
    
    free(rowMax);
    
    return ret;
}

void changeRow(double **array,int row1,int row2,int n){

    double *temp;
    
    temp =array[row1];
    array[row1] =array[row2];
    array[row2] =temp;
 
}

void subtractEqu(double **array,int row1,int row2,double coef,int n){  /* row1 - coef*row2 ---> row1 */
    int i;                                                          
    for(i=0; i<=n; ++i){
        array[row1][i] =array[row1][i] - coef*array[row2][i];
    
    }
}
void backwardSub(double **array,double *sol,int n){
    int i,j; double sum;
    --n;
    sol[n] = array[n][n+1] / array[n][n];
    for(i= n-1; 0 <= i; --i){
        sum =0;
        for(j=n; i < j ; --j ){
            sum += sol[j]*array[i][j];
        }
        sol[i] = (array[i][n+1]-sum) / array[i][i];
    }
}

void swap(double *num1,double *num2){
    double temp;
    temp =*num1;
    *num1 =*num2;
    *num2 =temp;
}

void printSol(double *sol,int n){
    int i;
    for(i=0;i<n;++i){
        printf("a[%d] = %9.5f \n",i,sol[i]);
    }
    printf("\n");
}

void freeAll(double **array,double *sol,int n){
    int i;
    
    for(i=0;i<n;++i){
        free(array[i]);
    }
    free(array);
    free(sol);

}
