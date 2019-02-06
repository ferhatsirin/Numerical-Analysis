/*     FERHAT ŞİRİN   
                     161044080    SAYISAL ANALİZ HW2 PART 1                    */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define ITER 100 /* for jacobi method */

int takeArray(FILE *input,double ***array,double **sol,int *n); /* if failed return 0 otherwise 1 */
int scaled_pivot(double **array,double *sol,int n);  /* uniq -> 1  notuniq -> 0  */
void changeRow(double **array,int row1,int row2,int n);
void subtractEqu(double **array,int row1,int row2,double coef,int n);  /* row1 - coef*row2 ---> row1 */
void backwardSub(double **array,double *sol,int n);
int jacobi(double **array,double *sol,int n);     /* if found then return count, otherwise 0 */
double normOfDifference(double *vekt1,double *vekt2,int n);
double norm(double *vektor,int n);
void swap(double *num1,double *num2);
int controlInputs(int argc,char **argv,FILE **fin,int *method); /* if failed return 0 with output otherwise return 1, method 1 ->> GESP, method 2 ->> JCB */
void printArray(double **arr,int n);
void printSol(double *sol,int n);
void freeAll(double **array,double *sol,int n);


int main(int argc, char** argv) {
    
    double **array,*sol; int method, n, count;
    FILE *input; 
    
    if(controlInputs(argc,argv,&input,&method)){
        if(takeArray(input,&array,&sol,&n)){
            
            printArray(array,n);
            
            switch(method){
            case 1:
                 if(scaled_pivot(array,sol,n)){
                   printArray(array,n);
                   printSol(sol,n);   
                 }
                else{
                   printf("There is not a unique solution for this equation \n");
                }

                break;
                
            case 2:
                count =jacobi(array,sol,n);
                if(count){
                   printf("Found at %d. iteration \n",count);
                   printSol(sol,n);
                }
                else{
                   printf("The program could not find the solution \n");
                } 
                
                break;
            }
        }
        else{
            fprintf(stderr,"Memory Failed \n");
        }
        freeAll(array,sol,n);
    }
    else{
        printf("Check the inputs and try again \n");   
    }

    return (EXIT_SUCCESS);
    
}
int controlInputs(int argc,char **argv,FILE **fin,int *method){
    int i,iFlag,mFlag;
    if(argc != 5){    
        fprintf(stderr,"Wrong argument inputs \n");
        return 0;    
    }
    iFlag =0;
    for(i=1;i< argc-1; ++i){
        if(strcmp(argv[i],"-i")==0){
            iFlag =i+1;
            break;
        }
    }
    if(!iFlag){
        fprintf(stderr,"Missing input flag \n");
        return 0;
    }
    
    if(((*fin) =fopen(argv[iFlag],"r"))==NULL){
      fprintf(stderr,"The file can not be opened  \n");
      return 0;
    }
    
    mFlag =0;
    for(i=1;i<argc-1;++i){
        if(strcmp(argv[i],"-m")==0){
            mFlag =i+1;
            break;
        }
    }
    
    if(!mFlag){
        fprintf(stderr,"Missing method flag\n");
        return 0;
    }
    for(i=0;argv[mFlag][i] !='\0';++i){
        argv[mFlag][i] =toupper(argv[mFlag][i]);
    }
    if(strcmp(argv[mFlag],"GESP")==0){
      *method =1;
    
    }
    else if(strcmp(argv[mFlag],"JCB")==0){
        *method =2;
    }
    else{
        fprintf(stderr,"Unknown-Method \n");
        *method =0;
        return 0;
    }

    return 1;
}

int takeArray(FILE *input,double ***array,double **sol,int *n){
    int i,j; int ret;
    char line[256]; char *value; char selector[2] =",";
    (*array) =NULL;    /* for realloc */
    ret =1;
    for(i=0;ret != 0 && fgets(line,256,input) != NULL;++i){
        if(((*array) =(double**)realloc((*array),sizeof(double*)*(i+1))) ==NULL){
            ret =0;
            break;
        }
        value =strtok(line,selector); 
        for(j=0;ret != 0 && value != NULL;++j){
            
            if(((*array)[i] =(double*)realloc((*array)[i],sizeof(double)*(j+1))) ==NULL){
                ret =0;
                break;
            }
            sscanf(value,"%lf",&(*array)[i][j]);
            value =strtok(NULL,selector);
        }
    }
    if(((*sol) =(double*)malloc(sizeof(double)*i))==NULL){
        ret =0;
    }
    *n =i;
    return ret;
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

    int i;double temp;
    
    for(i=0; i<=n; ++i){
        if(array[row1][i] != array[row2][i]){
   
            temp =array[row1][i];
            array[row1][i] =array[row2][i];
            array[row2][i] =temp;
        
        }
    }
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


int jacobi(double **array,double *sol,int n){
    int i,j,count,found,ret; double sum;
    double *prevSol;
    prevSol =(double*)calloc(sizeof(double),n);
    
    count =0; 
    found =0;
    while(count < ITER && !found){
    
        for(i=0;i<n;++i){
            sum =0;
            for(j=0;j<n;++j){
                if(j !=i){
                   sum +=array[i][j]*prevSol[j];
                 }
            }
            sol[i] = (array[i][n]-sum)/array[i][i];
        }
   
        if((normOfDifference(sol,prevSol,n)/norm(sol,n)) < 1e-3){
            found =1;
        }
        else{
            printf("At %d. iteration \n",count);
            printSol(prevSol,n);
            for(i=0;i<n;++i){
                prevSol[i] =sol[i];
            }
            ++count;
        }
    } 
    
    if(found == 1){
        ret =count;
    }
    else{
        ret =0;
    }
    free(prevSol);
    return ret;
}

double norm(double *vektor,int n){
    int i;  double sum;
    sum =0.0;
    for(i=0;i<n;++i){
        sum +=pow(vektor[i],2.0);
    }
    
    return sqrt(sum);

}
double normOfDifference(double *vekt1,double *vekt2,int n){
    
    int i;  double sum;
    sum =0.0;
    for(i=0;i<n;++i){
        sum +=pow((vekt1[i]-vekt2[i]),2.0);
    }
    
    return sqrt(sum);
}

void printArray(double **arr,int n){

    int i,j;
    for(i=0;i<n;++i){
        for(j=0;j<n;++j){
            printf("%9.5lf ",arr[i][j]);
        }
        printf("| %9.5lf \n",arr[i][j]);
    }
    printf("\n");
}
void printSol(double *sol,int n){
    int i;
    for(i=0;i<n;++i){
        printf("x[%d] = %9.5lf \n",i+1,sol[i]);
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
