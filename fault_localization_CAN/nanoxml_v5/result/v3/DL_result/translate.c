#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAXLEN 2000 //the max length of charaters in each line
#define TOTALNUM 216 //change

int main(argc,argv)
int argc;
char *argv[];
{
  char line[210];
  char pass[15]=":  368 ";//change
  FILE *fp1,*fp2,*fp3;
  char tempstr[10];
  int m = 0;
  int n = 0;
  int totalNum = TOTALNUM;
  float p;
  if( (fp1=fopen(argv[1],"r"))==NULL ) 
  {
    printf("open %s failed.\n", "fp1");
    exit(0);
  }
  if( (fp2=fopen(argv[2],"r"))==NULL )
  {
    printf("open %s failed.\n", "fp2");
    exit(0);
  }
  if( (fp3=fopen("DLFinalresult.txt","a"))==NULL )
  {
    printf("open %s failed.\n", "fp2");
    exit(0);
  }
 // fprintf(fp3, "%s:",""); //change
  while( fgets(line,MAXLEN,fp1) != NULL )
  { 
	  if(strstr(line,pass))
	  {
		tempstr[0]=line[0];
		tempstr[1]=line[1];
		tempstr[2]=line[2];
		tempstr[3]=line[3];
		m = atoi(tempstr);
		fprintf(fp3, "%d",m);
		fprintf(fp3, " ");
	   }
  }
  while( fgets(line,MAXLEN,fp2) != NULL )
  { 
	  if(strstr(line,pass))
	  {
		tempstr[0]=line[0];
		tempstr[1]=line[1];
		tempstr[2]=line[2];
		tempstr[3]=line[3];
		n = atoi(tempstr);
		fprintf(fp3, "%d",n);
		fprintf(fp3, ";");
	   }
  }
  p = (float)m/(float)TOTALNUM;
//  fprintf(fp3, "%f",p);
//  fprintf(fp3, "\n");
  fclose(fp1);
  fclose(fp2);
  exit(0);
}

