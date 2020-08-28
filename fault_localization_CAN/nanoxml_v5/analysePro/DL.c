#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define MAXLEN 100000
#define MINIMAL 0

struct Component
{
  int line;  //the sequence number
  double suspiciousness;  //the suspiciousness of the component
};

int SFLComponentCmp(const void *a, const void *b) 
{
  return ((struct Component*)a)->suspiciousness < ((struct Component*)b)->suspiciousness;
}

int main(argc,argv)
int argc;
char *argv[];
{
  FILE *fp1,*fp2,*fp3;
  int componentNum; //the number of components
  char outFilename[100]; //the  result outputfile
  struct Component *components;  //the components
 //identify the outFilename
  strcpy(outFilename,"DeepLearning.txt");
  //open file
  if( (fp1=fopen(argv[1],"r"))==NULL )  //the information of components
  {
    printf("open componentinfo.txt failed.\n");
    exit(0);
  }
  if( (fp2=fopen(argv[2],"r"))==NULL )  //the test results of DL
  {
    printf("open DL_result.txt failed.\n");
    fclose(fp1);
    exit(0);
  }
  if( (fp3=fopen(outFilename,"w"))==NULL )  //the DL result
  {
    printf("open %s failed.\n",outFilename);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    exit(0);
  }

  //malloc space
  fscanf(fp1,"%d",&componentNum);
  components=(struct Component*)malloc(componentNum*sizeof(struct Component));
  if(components==NULL )
  {
    free(components);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    printf("malloc error.\n");
    exit(0);
  }

  //get the line number of each component
  int i;
  for(i=0; i<componentNum; i++)
  {
     int temp;
     fscanf(fp1,"%d",&temp);
     components[i].line=temp;
  }
  //get the suspiciousness of each component
  char tempstr[100];
  char line[MAXLEN];
  int j = 0;
  int n = 0;
  long fp2longBytes = 100000;
  fseek(fp2,0,SEEK_END);
  fp2longBytes=ftell(fp2);
  fseek(fp2,0,SEEK_SET);
  float a = 0;
//  fscanf(fp2, "%f", &a);
//  fprintf(fp3,"%f",a);
  for(n = 0; n < componentNum; n++ )
  {
        fscanf(fp2, "%lf", &components[n].suspiciousness);
  }
/*  while( fgets(line,fp2longBytes,fp2) != NULL )
  {
      for(n = 0; n <= fp2longBytes; n++)
	  {
         fprintf(fp3,"%c",line[n]);
		if((line[n]=='[')&&(line[n+3]=='.'))
		{
            for(i = 0; i< 8; i++)
            {
		    	tempstr[i]=line[i+n+4];
            }
            tempstr[8]='\0';
			if( atoi(tempstr) != 0 )
			{
			   components[j].suspiciousness=atoi(tempstr);  
			   j++;
			   n = n+12;
			}
		}
     }
  }
  */
  // qsort(components,componentNum,sizeof(struct Component),SFLComponentCmp);
  //sort
  i = 0;
  int tmp = 0;
  int k = 0;
  double tmpLine = 0;
  double tmpSuspicious = 0;
  for(i = 0; i<componentNum-1; i++)
  {
	  tmp = i;
	  for(k = i+1; k<componentNum; k++)
	  {
		  if(components[tmp].suspiciousness < components[k].suspiciousness)
		  {
              tmp = k;
		  }
	  }
	  if(i!=tmp)
	  {
		  tmpSuspicious = components[tmp].suspiciousness;
		  tmpLine = components[tmp].line;

          components[tmp].suspiciousness = components[i].suspiciousness;
		  components[tmp].line = components[i].line;

		  components[i].suspiciousness = tmpSuspicious;
		  components[i].line = tmpLine;
	  }
  }
  i=0;
  for(i=0;i<componentNum;i++)
  {
    fprintf(fp3,"%d:  %d   %e\n",i+1, components[i].line,components[i].suspiciousness);
  }

  //free space  
  fclose(fp1);
  fclose(fp2);
  fclose(fp3);
  free(components);
  exit(0);
}
