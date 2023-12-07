#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h> // do not forget to compile : gcc main.c -lm
#define PI 3.141592654
#define MAX_LEN 61

typedef struct RobotState_struct{
	double x;
	double y;
	double theta;
}RobotState;

typedef struct RobotControl_struct{
   double v;
   double w;
}RobotControl;
/*
typedef struct RobotNode_struct{
   char name[50];
   RobotState f;
   RobotState i;
   struct RobotNode_struct *nextNodePtr;
}RobotNode;

void InsertAfter(RobotNode * thisNode, RobotNode * newNode){
   RobotNode* tmpNext = NULL;
   tmpNext = thisNode->nextNodePtr; // Remember next
   thisNode->nextNodePtr = newNode; // this -- new -- 
   newNode->nextNodePtr = tmpNext;
   return;
}
void FreeRobotList(RobotNode* head){
   RobotNode* tmp;
   while (head != NULL)
    {
       tmp = head;
       head = head->nextNodePtr;
       free(tmp);
    }
    return;
}*/

double X_k_1(double x_k, double T, double theta_k, double v_k, double w_k){
   if(fabs(w_k) < 0.000000001){
      return x_k + T*cos(theta_k)*v_k;
   }
   else{
      return x_k + (v_k/w_k)*(sin(theta_k + T*(w_k))-sin(theta_k));
   }
}

double Y_k_1(double y_k, double T, double theta_k, double v_k, double w_k){
   if(fabs(w_k) < 0.000000001){
      return y_k + T*sin(theta_k)*v_k;
   }
   else{
      return y_k - (v_k/w_k)*(cos(theta_k + T*(w_k))-cos(theta_k));
   }
}

double theta_k_1(double theta_k, double T, double v_k, double w_k){
   if(fabs(w_k) < 0.000000001){
      return theta_k;
   }
   else{ 
      return theta_k + T*w_k;
   }
   
}
RobotState robot(double T, RobotState s, RobotControl ctrl){
   RobotState next;
   next.x = X_k_1(s.x, T, s.theta, ctrl.v, ctrl.w);
   next.y = Y_k_1(s.y, T, s.theta, ctrl.v, ctrl.w);
   next.theta = theta_k_1(s.theta, T, ctrl.v, ctrl.w);
   return next;
}
double sq(double x);

double distance(RobotState s1, RobotState s2);

void PrintRobot(char *name, double d, double x0, double y0, double xf, double yf, double t){
   printf("Robot %s has traveled %.4lf meters, starting at (%.4lf, %.4lf)", name, d, y0, x0);
   printf(" and ending at (%.4lf, %.4lf) at time %.4lf seconds.\n", xf, yf, t);
   return;
}


void genOutFileName(const char *infile, char *outfile);

int main(int argv, char **argc){
   
   double T[5], x[5], y[5], theta[5], v[150], w[150], d[150], b1, b2, distances[5], K[5];
   int k=0, robotNum =0;
   char name[5][30], buf1[10], buf2[30], *end1, *end2;
   RobotControl u;
   RobotState p[5], s[5];
   
   FILE * infile, * outfile;
   char buf[MAX_LEN], inFileName[MAX_LEN][MAX_LEN], outFileName[MAX_LEN][MAX_LEN];
   char parsed_content[MAX_LEN] = "TODO: mod this with parsed content";
   int i, line =1;
   d[0]=0;
   
   if(argv < 2 ){
      printf("Error: Not enough input arguments.");
      return 0;
   }
   for(robotNum=0; robotNum<argv-1; robotNum++){
      for(i=0; i<strlen(argc[robotNum+1]); i++){ inFileName[robotNum][i] =argc[robotNum+1][i];//strcpy
      }inFileName[robotNum][i] = '\0';// ---end strcpy
      genOutFileName(inFileName[robotNum], outFileName[robotNum]);
     
      infile = fopen(inFileName[robotNum], "r"); // -----file operations 
      line = 1;
      while(fgets(buf, MAX_LEN, infile) != NULL){
         if(line==1){ strcpy(name[robotNum], buf); }
         if(line==2){ sscanf(buf, "%lf %lf %lf %lf", &T[robotNum], &x[robotNum], &y[robotNum], &theta[robotNum]);
            s[robotNum].x = x[robotNum];
            s[robotNum].y = y[robotNum];
            s[robotNum].theta = theta[robotNum];
            d[0] = 0;
            k=0;
         }
         if(line>2){ sscanf(buf, "%s %s", buf1, buf2); 
            b1 = strtod(buf1, &end1);
            b2 = strtod(buf2, &end2);
            
            v[k] = b1;
            w[k] = b2;
      
            u.v = v[k]; 
            u.w = w[k];
            p[robotNum] = s[robotNum];
            s[robotNum] = robot(T[robotNum], s[robotNum], u);
            d[k] = d[k-1] + distance(p[robotNum], s[robotNum]);
         }
         line++;
         k++;
      }K[robotNum] = k-1;
      distances[robotNum] = d[k-1];
     fclose(infile);
      
      
      /*
      outfile = fopen(outFileName[i-1], "w");
      
      strcat(parsed_content, "   awesome!");
      fprintf(outfile, "%s", buf);
      fclose(outfile);*/
   }
   //get distances and timestep for the last robot
   	//distances[robotNum] = d[k-1];
   	//K[robotNum]=k-1;
  
   //print all robots
   printf("Number of robots: %d\n", robotNum);
   for(i=0;i<robotNum; i++){
      if(K[i]>10){PrintRobot(name[i], distances[i], y[i], x[i], s[i].x, s[i].y, T[i]*K[i]);}
      else{
         PrintRobot(name[i], distances[i], x[i], y[i], s[i].x, s[i].y, T[i]*K[i]);
      }
   }
   return 0;
  
   
}


double sq(double x){
   return x*x;
}

double distance(RobotState s1, RobotState s2){
   double d, sx2 = sq(s1.x - s2.x), sy2 = sq(s1.y - s2.y);
   d = sqrt(sx2 + sy2);
   return d;
}

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
