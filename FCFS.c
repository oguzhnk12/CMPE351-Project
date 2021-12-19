#include<stdio.h>
#include <stdlib.h>
struct node
{
int pid;
int burstTime;
int arrivalTime;
int priority;
int waitingTime;
struct node *next;};

struct node * createNode(int, int, int);
struct node * insertBack(struct node *, int, int, int);
void setPid(struct node *);
void FirstComeFirstServe(struct node *);
void Display(struct node *);

int main(int argc, char* argv[]){

//taking data from file//
char* fileName = argv[1];
FILE* processes = fopen(fileName,"r");
if(processes == NULL){
    printf("ERROR: %s does not exist\n",argv[1]);
    return(0);}
struct node *allProcesses = NULL;
//struct node *copy = NULL;* 		//copy was conceived as a backup for real LinkedList operations.
					//And it was a preperation for next scheduling methods.
int b, a, p;
int numberOfProcess = 1;
while(!feof(processes)){
  fscanf(processes,"%d:%d:%d", &b, &a, &p);
  allProcesses = insertBack(allProcesses, b, a, p);
  //copy = insertBack(copy, b, a, p);*
  numberOfProcess++;}
  fclose(processes);
//taking data from file is finished//

int choice;
do{
  printf("\t\t********************************************************\n");
  printf("\t\t\t\tCPU Scheduler Simulator\n");
  printf("Choose the Scheduling Method\n");
  printf("1) First Come, First Served Scheduling\n");
  printf("2) Shortest-Job-First Scheduling\n");
  printf("3) Priority Scheduling\n");
  printf("4) Round-Robin Scheduling\n");
  printf("5)End Program\n");
  printf("Option>");
  scanf("%d",&choice);
  switch (choice) {
	case 1:
	  system("clear");
	  printf("Scheduling Method: First Come, First Served Scheduling\n");
	  FirstComeFirstServe(allProcesses);
	  Display(allProcesses);
	break;
	case 2:
	   printf("SJF\n");
	break;
	case 3:
	   printf("Priority Scheduling\n");
	break;
	case 4:
	   printf("Round Robin\n");
	break;
	case 5:
	   exit(0);
	break;
	default:
		printf("ERROR: You type wrong choice\n");
	break;
 }
}while(choice != 5);
	
return 0;
}
struct node * createNode(int b, int a, int p){
     struct node * temp;
     temp = (struct node *) malloc(sizeof(struct node));
     temp->burstTime = b;
		 temp->arrivalTime = a;
		 temp->priority = p;
     temp->next = NULL;
     return temp;}
struct node * insertBack(struct node *header, int b, int a, int p){
       struct node * temp = createNode(b, a, p);
       struct node * headertemp;
       if (header == NULL){
          header = temp;
          return header;}
       headertemp=header;
       while(headertemp->next != NULL)
            headertemp=headertemp->next;
       headertemp->next = temp;
       return header;}

//The function iterates all the processes and calculate sum of the burst times
//of all processes before each process and record it as a waiting time.
void FirstComeFirstServe(struct node *allProcesses){
	struct node *temp = allProcesses;
	int time = 0;
	while (temp->next != NULL){
	  temp->waitingTime = time;
	  time += temp->burstTime;
	  temp=temp->next;}
}
/*Display function calculates average waiting time and displays
it with the waiting times of each process.*/
void Display(struct node *allProcesses){
	struct node *temp = allProcesses;
	int i = 0;
	double sum = 0;
	printf("Process Waiting Times:\n");
	while (temp->next != NULL){
	  printf("P%d: %d ms\n", temp->pid, temp->waitingTime);
	  sum += temp->waitingTime;
	  i++;
	  temp=temp->next;}
	  printf("Average Waiting Time: %f ms\n", sum/i);}
//iterates linked list and assign process id for each process.
void setPid(struct node *allProcesses){
	struct node *temp = allProcesses;
	int i = 1;
	while (temp != NULL){
	  temp->pid = i;
	  temp=temp->next;
	  i++;}
}
