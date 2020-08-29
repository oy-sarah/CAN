#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAXLEN 2000 //the max length of charaters in each line
#define TOTALNUM 216 

int main(argc,argv)
int argc;
char *argv[];
{
  char line[5];
  FILE *fp1,*fp2;
  int totalRank = 0;
  int averageRank = 0;
  int rank[10];
  float p;
  if( (fp1=fopen(argv[1],"r"))==NULL ) 
  {
    printf("open %s failed.\n", "fp1");
    exit(0);
  }
  if( (fp2=fopen("faultStatementRank.txt","a"))==NULL )
  {
    printf("open %s failed.\n", "fp2");
    exit(0);
  }
  fprintf(fp2, "%s:","CAN Nanoxml_v5 fault version 3 rank:"); 
  
  int i = 0;
  for(i = 0; i < 10; i++) 
  {
      fscanf(fp1,"%d",&rank[i]);      
  }
  int temp = 0;
  for(i = 1; i < 10; i++)
  {
      for(int j = i - 1; j >= 0; j--)
      {
           if(rank[j + 1] < rank[j])
           {
               temp = rank[j+1];
               rank[j+1] = rank[j];
               rank[j] = temp;
           }
      }
  }
  for(i = 1; i < 9; i++)
  {
      totalRank = totalRank + rank[i];
  }
  averageRank = totalRank/8;
  fprintf(fp2, "%d",averageRank);
  fprintf(fp2, ",");
  p = (float)averageRank/(float)TOTALNUM;
  fprintf(fp2, "%s:","Exam:");
  fprintf(fp2, "%f",p);
  fprintf(fp2, "\n");
  fclose(fp1);
  fclose(fp2);
  exit(0);
}

