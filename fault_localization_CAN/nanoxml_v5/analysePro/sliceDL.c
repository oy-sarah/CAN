#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define MAXLEN 100000
#define MINIMAL 0

struct Component
{
  int line;  //the sequence number
  float suspiciousness;  //the suspiciousness of the component
};
struct SliceComponent
{
  int sliceLine;  
  float sliceSuspiciousness;  //the suspiciousness of the slicecomponent
};

int SFLComponentCmp(const void *a, const void *b) 
{
  return ((struct Component*)a)->suspiciousness < ((struct Component*)b)->suspiciousness;
}

int main(argc,argv)
int argc;
char *argv[];
{
  FILE *fp1,*fp2,*fp3,*fp4;
  int componentNum; //the number of components
  char outFilename[100]; //the  result outputfile
  struct Component *components;  //the components
  struct SliceComponent *sliceComponents;  //the slice components
 //identify the outFilename
  strcpy(outFilename,"SliceDeepLearning.txt");
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
  if( (fp4=fopen(argv[3],"r"))==NULL )  //the slice result
  {
    printf("open sliceResult.txt failed.\n");
    fclose(fp1);
    fclose(fp2);
	fclose(fp3);
    exit(0);
  }

  //malloc space
  fscanf(fp1,"%d",&componentNum);
  components=(struct Component*)malloc(componentNum*sizeof(struct Component));
  sliceComponents=(struct SliceComponent*)malloc(componentNum*sizeof(struct SliceComponent));
  if(components==NULL )
  {
    free(components);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
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
        fscanf(fp2, "%f", &components[n].suspiciousness);
  }
  //record every slice statement's line number
  int sliceResult;
  int sliceNum = 0;
  int statementRecord = 0;
  i = 0;
  while( fscanf(fp4,"%d", &sliceResult) != EOF )
  {
     sliceComponents[sliceNum].sliceLine = sliceResult;
     sliceComponents[sliceNum].sliceSuspiciousness = 0;
	 for(i=statementRecord;i<componentNum;i++)//supposing slicing statements order is increasing,or use 0 replace statementRecord
	 {
               if(sliceComponents[sliceNum].sliceLine == components[i].line)
		{
		   sliceComponents[sliceNum].sliceSuspiciousness = components[i].suspiciousness;
		   statementRecord = i;
		}
	 }
	 sliceNum++;
  }
  //sort
  i = 0;
  int tmp = 0;
  int k = 0;
  double tmpLine = 0;
  double tmpSuspicious = 0;
  for(i = 0; i<sliceNum-1; i++)
  {
	  tmp = i;
	  for(k = i+1; k<sliceNum; k++)
	  {
		  if(sliceComponents[tmp].sliceSuspiciousness < sliceComponents[k].sliceSuspiciousness)
		  {
              tmp = k;
		  }
	  }
	  if(i!=tmp)
	  {
		  tmpSuspicious = sliceComponents[tmp].sliceSuspiciousness;
		  tmpLine = sliceComponents[tmp].sliceLine;

          sliceComponents[tmp].sliceSuspiciousness = sliceComponents[i].sliceSuspiciousness;
		  sliceComponents[tmp].sliceLine = sliceComponents[i].sliceLine;

		  sliceComponents[i].sliceSuspiciousness = tmpSuspicious;
		  sliceComponents[i].sliceLine = tmpLine;
	  }
  }

  //qsort(components,componentNum,sizeof(struct Component),SFLComponentCmp);
 // qsort(sliceComponents,sliceNum,sizeof(struct SliceComponent),SFLComponentCmp);
  i=0;
  for(i=0;i<sliceNum;i++)
  {
    fprintf(fp3,"%d:  %d   %f\n",i+1, sliceComponents[i].sliceLine,sliceComponents[i].sliceSuspiciousness);
  }

  //free space  
  fclose(fp1);
  fclose(fp2);
  fclose(fp3);
  fclose(fp4);
  free(components);
  exit(0);
}
