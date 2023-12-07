#include <stdio.h>
#include <string.h>
#define MAX_LEN 61
void genOutFileName(const char *infile, char *outfile){
   char * period_pointer, in_buf[MAX_LEN]="", out_buf[MAX_LEN]="", *i;
   int j=0;
   strcpy(in_buf, infile);
   if(strchr(infile, '.')!=NULL){
      //printf("  has ext!\n");
      period_pointer = strchr(in_buf, '.');
      for(i=in_buf; i!=period_pointer; i++){
         out_buf[j] = *i;
         j++;
      }out_buf[j] ='\0';
      strcat(out_buf, "_out");
      strcat(out_buf, period_pointer);
      strcpy(outfile, out_buf);
   }
   else{
      //printf("  has NO ext!\n");
      strcat(in_buf, "_out");
      strcpy(outfile, in_buf);
   }
   
   return;
}

int main(int argv, char **argc){
  FILE * infile, * outfile;
  char buf[MAX_LEN], inFileName[MAX_LEN][MAX_LEN], outFileName[MAX_LEN][MAX_LEN], parsed_content[MAX_LEN] = "TODO: mod this with parsed content";
  int i, line =1;
  if(argv < 2 ){
    printf("Error: Not enough input arguments.");
    return 0;
  }
  
  for(i=1; i<argv; i++){
     strcpy(inFileName[i-1], argc[i]);//use the last string in the command line argument at execution
     genOutFileName(inFileName[i-1], outFileName[i-1]);
     
     infile = fopen(inFileName[i-1], "r"); // -----file operations 
     line = 1;
     while(fgets(buf, MAX_LEN, infile) != NULL){
        if(line==1){ printf("robot name:%s", buf); }
        if(line==2){ printf("time x0 y0 theta0: %s", buf);}
        if(line>2){ printf("move: %s", buf); }
        line++;
     }
     
     fclose(infile);
     outfile = fopen(outFileName[i-1], "w");
     
     strcat(parsed_content, "   awesome!");
     fprintf(outfile, "%s", buf);
     fclose(outfile);
  }
  return 0;
}
