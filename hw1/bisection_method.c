/*             FERHAT ŞİRİN 161044080    
                            BİSECTİON METHOD  */
#include <stdio.h>
#include<ctype.h>
#include<math.h>
#define ITER 100
#define NO_ROOT -1
#define EXCESS 0
#define FOUND 1

double f(double x);
int numOfRoots(double a,double b);
void emptyBuffer();
double distance_to_root(double p1,double p2);
double absolute_error(double p1,double p2);
double relative_error(double p1,double p2);
int findTheoretical(double a,double b,double epsilon);
int bisection(double a,double b,double epsilon,double (*criterion)());


int main(void) {
    double x,y,ep; char choice ='Y';
    
    while(toupper(choice) == 'Y'){
        
        printf("Enter the start of root search interval \n");
        scanf("%lf",&x);
        printf("Enter the end of root search interval \n");
        scanf("%lf",&y);
        emptyBuffer();
        printf("Enter the type of stop criterion \n");
        printf("DISTANCE_TO_ROOT, ABSOLUTE_ERROR, RELATIVE_ERROR \n");
        scanf(" %c",&choice);
        emptyBuffer();
        printf("Enter the epsilon value \n");
        scanf("%lf",&ep);
        
        switch(toupper(choice)){
            case 'D':
                bisection(x,y,ep,&distance_to_root);
                break;
            case 'A':
                bisection(x,y,ep,&absolute_error);
                break;
            case 'R':
                bisection(x,y,ep,&relative_error);
                break;
            default :
                printf("WRONG CHOİCE!!! \n");
        }
        
        emptyBuffer();
        printf("Do you want to try it again ? Yes or No \n");
        scanf("%c",&choice);
        emptyBuffer();
    }

    return 0;
}

double f(double x){

    return 3*x-exp(x);

}

int bisection(double a,double b,double epsilon,double (*criterion)()){
    
    double p1, p2, xleft, xright; int count, found, numRoot;
    double fleft, fmid;
    
    numRoot =numOfRoots(a,b);
    if(numRoot == 0){
        printf("The function doesn't have any root in this interval (%.3lf,%.3lf) \n",a,b);
        return NO_ROOT;
    }
    if(1 < numRoot){
        printf("In the interval (%.3lf,%.3lf) The function has more than one root (%d) \n",a,b,numRoot);
        printf("You should narrow the interval \n");
        return EXCESS;
    }
    
    count =1;
    found =0;
    p1 =a;
    xleft =a;
    xright =b;

    printf("|iter|%6c| a |%6c| b |%6c| p |%8c| f(p) |%4c| ABS. ERR. |%c| REL. ERR. |\n",' ',' ',' ',' ',' ',' ');    
 
    while(count <= ITER && !found){
        p2 =(xleft+xright)/2.0;
        
        fleft =f(xleft);
        fmid =f(p2);
        
        printf(" %3d %12.6lf %12.6lf %12.6lf %12.6lf %12.6lf %12.6lf \n",count,xleft,xright,p2,fmid,absolute_error(p1,p2),relative_error(p1,p2));
        if(criterion(p1,p2) < epsilon){
            printf("Found at %d iteration f(%lf) = %lf \n",count,p2,fmid);
            printf("The theoritically required iteration is %d \n",findTheoretical(a,b,epsilon));
            found =1;
        }
        else if(fmid * fleft < 0){
            xright =p2;
        }
        else{
            xleft =p2;
        }
       
        p1 =p2;   
        ++count;
    }
    if(found == 1){
        return FOUND;
    }
    else
        printf("The function couldn't find the root!!! \n");
        return NO_ROOT;
}

double distance_to_root(double p1,double p2){
    return fabs(f(p1));
}

double absolute_error(double p1,double p2){
    return fabs(p2-p1);
}

double relative_error(double p1,double p2){

    return fabs((p2-p1)/p2);
}

int numOfRoots(double a,double b){
  
    int count =0;double sign;
    sign =f(a);
    
    while(a <= b){
        if(sign * f(a) < 0){
           ++count;
           sign *=-1;
        }
        a +=0.2;
    }
    
    return count;

}
int findTheoretical(double a,double b,double epsilon){
    int theoricIter; 
    
    theoricIter = ceil(-log(epsilon/(b-a))/log(2));

    return theoricIter;
}

void emptyBuffer(){
    while(getchar() != '\n');
}
