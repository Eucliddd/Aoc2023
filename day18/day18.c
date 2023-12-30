#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#define inputN 634

int hexStringToInt(char* hexString) {
    int result = 0;
    int strlength = 5;

    for (int i = 0; i < strlength; i++) {
        char c = hexString[i];

        if (c >= '0' && c <= '9') {
            result = result * 16 + (c - '0');
        }
        else if (c >= 'A' && c <= 'F') {
            result = result * 16 + (c - 'A' + 10);
        }
        else if (c >= 'a' && c <= 'f') {
            result = result * 16 + (c - 'a' + 10);
        }
    }

    return result;
}

int main(){

    char array[4]={'R','D','L','U'};

    FILE *inputF,*resultF;
    inputF=fopen("input.txt","rt");
    resultF=fopen("result.txt","wt");
    char dir[inputN],color[inputN][12],longdir[inputN];
    int length[inputN],longlength[inputN];
    for(int i=0;i<inputN;i++){
        fscanf(inputF,"%c %d %s\n",dir+i,length+i,color+i);
/*      dir[i]=fgetc(inputF);
        fgetc(inputF);
        fscanf(inputF,"%d",&(length[i]));
        color[i]=fgetc(inputN);
        fgetc(inputF);
    */
        
        

        
        longdir[i]=array[color[i][7]-'0'];
        longlength[i]=hexStringToInt(color[i]+2);
        fprintf(resultF,"%c %d %s %c %d\n",*(dir+i),*(length+i),*(color+i),*(longdir+i),longlength[i]);

    }


    double area=0,areaThis=0,areaReal=0;
    int lengthSum=0;
    int x_now,y_now,x_last,y_last;
    
    x_now=0;
    y_now=0;
    for(int i=0;i<inputN;i++){
        lengthSum+=longlength[i];
        switch(longdir[i]){
            case 'U':
                x_last=x_now;
                y_last=y_now;
                x_now+=0;
                y_now+=longlength[i];
                break;
            case 'R':
                x_last=x_now;
                y_last=y_now;
                x_now+=longlength[i];
                y_now+=0;
                break;
            case 'D':
                x_last=x_now;
                y_last=y_now;
                x_now+=0;
                y_now-=longlength[i];
                break;
            case 'L':
                x_last=x_now;
                y_last=y_now;
                x_now-=longlength[i];
                y_now+=0;
                break;
        }
        areaThis=(double)((int64_t)x_last*(int64_t)y_now-(int64_t)x_now*(int64_t)y_last)/2;
        area+=areaThis;
    
    }

    areaReal=fabs(area)+((double)lengthSum)/2+1;
    printf("area = %f \n",areaReal);


    fclose(inputF);
    fclose(resultF);

    return 0;
}