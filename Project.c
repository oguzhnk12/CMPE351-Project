#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include <getopt.h>
#define BUFFER_SIZE 1000000

	/*CMPE351-Operating System Project*/

	/*CPU Scheduling Simulator*/


	/*This program wrote in order to simulate service of processes by the CPU.
	This program takes all information about processes from an input file, then
	apply some CPU scheduling methods to get waiting time of each process and
	average waiting time of all processes. Finally, displays the results and
	write that results to the output file.*/

	/*The CPU scheduling methods that the program implements are;

	 -First Come First Serve Scheduling Method
	 -Shortest Job First Scheduling Method(Non-preemtive)
	 -Shortest Job First Scheduling Method(Preemtive)
	 -Priority Scheduling Method(Non-preemtive)
	 -Priority Scheduling Method(Preemtive)
	 -Round Robin Scheduling Method*/


	//Oguzhan Kaya
	//21806449


struct node{
		int burstTime;
		int arrivalTime;
		int priority;
		int waitingTime;				//Initially zero.
		int pid;								//process ID.
		int contextSwitchTime;	//Initially zero.
		int readyQueueState;  	//Initially equals to arrival time.
	 	struct node *next;};


//Enumerators for menus
enum methods {
	None = 0,
	FirstComeFirstServedScheduling = 1,
	ShortestJobFirstScheduling = 2,
	PrioritySchedulingMethod = 3,
	RoundRobinScheduling = 4
};

enum methods chosenMethodNumber;

enum modes {
	Off = 0,
	On = 1
};

enum modes chosenModeNumber;

//Global variables
char buffer[BUFFER_SIZE] = "THIS IS THE OUTPUT FILE OF CPU SCHEDULING SIMULATION PROGRAM\n";

char chosenMethod[50] = "None";
char chosenMode[50] = "Off";

char* outputFileName = NULL;
FILE* output;

int timeQuantum;

struct node *allProcesses = NULL;


/*Function Prototypes*/

//Menu function prototypes
void mainMenu();
void methodMenu();
void modeMenu();
void showResult();
void returnMenu();

//Other function prototypes
struct node * createNode(int, int, int);
struct node * insertBack(struct node *, int, int, int);
struct node* copyLinkedList(struct node* );
void setValues(struct node *);
void FirstComeFirstServe(struct node *);
void ShortestJobFirst(struct node *);
void PriorityScheduling(struct node *);
void RoundRobin(struct node *);
void SJFPreemtive(struct node *);
void PriorityPreemtive(struct node *);
void Display(struct node *);
void sortBurstTime(struct node *);
void sortPriority(struct node *);
void sortReadyQueueState(struct node *);
void sortPID(struct node *);
void swapNode(struct node *, struct node *);
void sortArrivalTime(struct node *);


int main(int argc, char* argv[])
{

	FILE* input;
	char* inputFileName = NULL;
	int option;
	int burst, arrival, priority;


	while ((option = getopt(argc, argv, "f:o:")) != -1)
	{
		switch (option)
		{
		case 'f':
			inputFileName = optarg;
			break;
		case 'o':
			outputFileName = optarg;
			break;
		default:
			printf("The valid command argument options are given below;\n");
			printf("./Project.c –f input.txt –o output.txt\n");
			return 0;
			break;
		}
	}


//Reading data from input file.
input = fopen(inputFileName,"r");
	if(input == NULL)
  {
    printf("ERROR: input file does not exist\n");
    return 0;
  }
  while(!feof(input))
  {
		fscanf(input,"%d:%d:%d\n", &burst, &arrival, &priority);
		if((burst < 0) || (arrival < 0) || (priority < 0)){
			printf("Data can not be a negative number. Please check input file.\n");
			exit(0);}
		allProcesses = insertBack(allProcesses, burst, arrival, priority);
  }
  fclose(input);
//Data read.

		mainMenu();

return 0;
}

void mainMenu()
{
	char menuChoice;
	do{
	printf("\n\t\t\t\t\tCPU Scheduler Simulator\n\n");
	printf("1) Scheduling Method(%s)\n",chosenMethod);
	printf("2) Preemtive Mode(%s)\n",chosenMode);
	printf("3) Show Result\n");
	printf("4) End Program\n");
	printf("Option>");
	scanf(" %c",&menuChoice);
	switch (menuChoice) {
		case '1':
			methodMenu();
		break;
		case '2':
			modeMenu();
		break;
		case '3':
			showResult();
		break;
		case '4':
		//Writing results to output file.
		output = fopen(outputFileName,"w");
		fputs(buffer, output);
		fclose(output);
		//Results written.
		exit(0);
		break;
		default:
			system("clear");
			printf("Invalid option.\nPlease try again.\n");
		break;
		}
	}while(menuChoice != '4');
}

void methodMenu(){

	char fcfs[] = "First Come, First Served Scheduling",
	sjf[] = "Shortest-Job-First Scheduling",
	priority[] = "Priority Scheduling",
	rr[] = "Round-Robin Scheduling",
	none[] = "None";
	char methodChoice;
	printf("\n");
	printf("1) %s\n", none);
	printf("2) %s\n",fcfs);
	printf("3) %s\n",sjf);
	printf("4) %s\n",priority);
	printf("5) %s\n", rr);
	printf("6) Go Back\n");
	printf("Option>");
	scanf(" %c",&methodChoice);
	switch (methodChoice) {
		case '1':
			strcpy(chosenMethod, none);
			chosenMethodNumber = None;
			system("clear");
		break;
		case '2':
			strcpy(chosenMethod, fcfs);
			chosenMethodNumber = FirstComeFirstServedScheduling;
			system("clear");
		break;
		case '3':
			strcpy(chosenMethod, sjf);
			chosenMethodNumber = ShortestJobFirstScheduling;
			system("clear");
		break;
		case '4':
			strcpy(chosenMethod, priority);
			chosenMethodNumber = PrioritySchedulingMethod;
			system("clear");
		break;
		case '5':
			strcpy(chosenMethod, rr);
			chosenMethodNumber = RoundRobinScheduling;
			system("clear");
		break;
		case '6':
			system("clear");
			return;
		break;
		default:
			system("clear");
			printf("Invalid option.\nPlease try again.\n");
		break;
	}
}

void modeMenu(){
	char off[] = "Off",
	on[] = "On";
	char modeChoice;
	printf("\n");
	printf("1) %s\n", off);
	printf("2) %s\n", on);
	printf("3) Go Back\n");
	printf("Option>");
	scanf(" %c",&modeChoice);
	switch (modeChoice) {
		case '1':
			strcpy(chosenMode, off);
			chosenModeNumber = Off;
			system("clear");
		break;
		case '2':
			strcpy(chosenMode, on);
			chosenModeNumber = On;
			system("clear");
		break;
		case '3':
			system("clear");
			return;
		break;
		default:
		system("clear");
		printf("Invalid option.\nPlease try again.\n");
		break;
	}
}

void showResult(){

	char fcfsTitle[] = "\nScheduling Method: First Come, First Served Scheduling\nProcess Waiting Times:\n",
			 sjfTitle[] = "\nScheduling Method: Shortest Job First Scheduling(Non-preemtive)\nProcess Waiting Times:\n",
			 sjfpreemtiveTitle[] = "\nScheduling Method: Shortest Job First Scheduling(Preemtive)\nProcess Waiting Times:\n",
			 priorityTitle[] = "\nScheduling Method: Priority Scheduling(Non-preemtive)\nProcess Waiting Times:\n",
			 prioritypreemtiveTitle[] = "\nScheduling Method: Priority Scheduling(Preemtive)\nProcess Waiting Times:\n";

	char tempbuffer[99];

	if(chosenMethodNumber == 0){
		system("clear");
		printf("None of the methods are chosen. Please chose a method.\n");}
	else if(chosenMethodNumber == 1){
		system("clear");
		printf("%s",fcfsTitle);
		strcat(buffer,fcfsTitle);
		FirstComeFirstServe(allProcesses);
		returnMenu();}
	else if((chosenMethodNumber == 2) && (chosenModeNumber == 0)){
		system("clear");
		printf("%s",sjfTitle);
		strcat(buffer,sjfTitle);
		ShortestJobFirst(allProcesses);
		returnMenu();
	}
	else if((chosenMethodNumber == 2) && (chosenModeNumber == 1)){
		system("clear");
		printf("%s",sjfpreemtiveTitle);
		strcat(buffer,sjfpreemtiveTitle);
		SJFPreemtive(allProcesses);
		returnMenu();
	}
	else if((chosenMethodNumber == 3) && (chosenModeNumber == 0)){
		system("clear");
		printf("%s",priorityTitle);
		strcat(buffer,priorityTitle);
		PriorityScheduling(allProcesses);
		returnMenu();
	}
	else if((chosenMethodNumber == 3) && (chosenModeNumber == 1)){
		system("clear");
		printf("%s",prioritypreemtiveTitle);
		strcat(buffer,prioritypreemtiveTitle);
		PriorityPreemtive(allProcesses);
		returnMenu();
	}
	else if(chosenMethodNumber == 4){
		system("clear");
		printf("Enter the time_quantum\ntime_quantum = ");
		scanf("%d",&timeQuantum);
		system("clear");
		sprintf(tempbuffer,"\nScheduling Method: Round Robin Scheduling - time_quantum = %d\nProcess Waiting Times:\n", timeQuantum);
		printf("%s",tempbuffer);
		strcat(buffer,tempbuffer);
		RoundRobin(allProcesses);
		returnMenu();
	}
}

void returnMenu()
{
	char rtn;
	while ((rtn = getchar()) != EOF && rtn != '\n');
	printf("Enter a character to return menu.");
	getchar();
	system("clear");
}

struct node * createNode(int b, int a, int p){
	struct node * temp;
  temp = (struct node *) malloc(sizeof(struct node));
  temp->burstTime = b;
	temp->arrivalTime = a;
	temp->priority = p;
  temp->next = NULL;
  return temp;
}

struct node * insertBack(struct node *header, int b, int a, int p){
	struct node * temp = createNode(b, a, p);
  struct node * headertemp;
  if (header == NULL)
  	{
      header = temp;
      return header;
    }
  headertemp=header;

  while(headertemp->next != NULL)
    headertemp=headertemp->next;
  headertemp->next = temp;
  return header;
}


/*Iterates linked list and assign process id for each process and assigns
default waiting time, contextSwitchTime values. Also assigns ready queue
state which is initially equals to arrival time*/
void setValues(struct node *allProcesses){
	struct node *temp = allProcesses;
	int i = 1;
	while (temp != NULL){
	temp->pid = i;
	temp->waitingTime = 0;
	temp->contextSwitchTime = 0;
	temp->readyQueueState = temp->arrivalTime;
	i++;
	temp=temp->next;}
}


/*Display function, calculates average waiting time and displays
it with the waiting times of each process.*/
void Display(struct node *allProcesses){
	struct node *temp = allProcesses;
	int i = 0;
	double sum = 0;
	char tempbuffer[99];
	while (temp != NULL){
		sprintf(tempbuffer, "P%d: %d ms\n", temp->pid, temp->waitingTime);
		printf("%s",tempbuffer);
		strcat(buffer,tempbuffer);
		sum += temp->waitingTime;
		i++;
		temp=temp->next;
	}
	sprintf(tempbuffer,"Average Waiting Time: %f ms\n", sum/i);
	printf("%s",tempbuffer);
	strcat(buffer,tempbuffer);
	temp = allProcesses;
	setValues(temp);
}


/*Creates another node which carries the same data with the header of
the real linked list then iterates the real linked list and creates
new node with data of each processes and insert them to the coppied
header so we have copy of real linked list.*/
struct node* copyLinkedList(struct node* allProcesses){
	struct node* head = allProcesses;
	struct node* copy = NULL;
  while (head != NULL){
  	if (copy == NULL)
			copy = createNode(head->burstTime, head->arrivalTime, head->priority);
		else
			copy = insertBack(copy, head->burstTime, head->arrivalTime, head->priority);
		head = head->next;
	}
return copy;
}


/*The function sorts processes according to arrival time and iterates all the
processes and calculate sum of the burst times of all processes before each
process then subtract arrival time from summation
to get the waiting time. Finally displays the waiting times of each processes
and average waiting time by calling Display function.*/
void FirstComeFirstServe(struct node *allProcesses){
	struct node *clone  = copyLinkedList(allProcesses);
	setValues(clone);
	struct node *temp = clone;
	int time;
	sortArrivalTime(temp);
	temp->waitingTime = 0;
	time = temp->burstTime + temp->arrivalTime;
	temp = temp->next;
	while (temp != NULL){
		if(temp->burstTime != 0){
			temp->waitingTime = time - temp->arrivalTime;
			if(temp->waitingTime < 0){
				time += temp->waitingTime*(-1);
				temp->waitingTime = 0;
			}
			time += temp->burstTime;
		}
		temp=temp->next;
	}
	sortPID(clone);
	Display(clone);
}


/*The function sorts processes according to burst time and iterates all the
processes and calculate sum of the burst times of all processes before each
process then subtract arrival time from summation
to get the waiting time.The function also count the time to understand
which of the processes in ready queue. Finally displays the waiting times of each processes
and average waiting time by calling Display function.*/
void ShortestJobFirst(struct node *allProcesses){
	struct node *temp;
	struct node *clone = copyLinkedList(allProcesses);
	setValues(clone);
	sortArrivalTime(clone);
	int time = clone->arrivalTime -1;
	sortBurstTime(clone);
	int check , control;
	do{
		time++;
		control = 0;
		do{
			temp = clone;
		  check = 0;
		 	while(temp != NULL){
				if(temp->burstTime != 0){
					if(temp->arrivalTime <= time){
						temp->waitingTime += time - temp->arrivalTime;
						time += temp->burstTime;
						temp->burstTime = 0;
						check = 1;
						break;
					}
				}
			 temp = temp->next;
			}
	}while(check == 1);

	temp = clone;
	while(temp != NULL){
		if(temp->burstTime != 0){
			control = 1;
			break;
		}
		temp = temp->next;
	}
	}while(control == 1);
	sortPID(clone);
	Display(clone);
}


/*It implements the same algorithm as sjf. However, the priority function sorts
the linked list by priority rather than burst time.*/
void PriorityScheduling(struct node *allProcesses){
	struct node *temp;
	struct node *clone = copyLinkedList(allProcesses);
	setValues(clone);
	sortArrivalTime(clone);
	int time = clone->arrivalTime -1;
	sortPriority(clone);
	int check , control;
	do{
		time++;
		control = 0;
		do{
			temp = clone;
		  check = 0;
		 	while(temp != NULL){
				if(temp->burstTime != 0){
					if(temp->arrivalTime <= time){
						temp->waitingTime += time - temp->arrivalTime;
						time += temp->burstTime;
						temp->burstTime = 0;
						check = 1;
						break;
					}
				}
			 temp = temp->next;
		  }
	}while(check == 1);

	temp = clone;
	while(temp != NULL){
		if(temp->burstTime != 0){
			control = 1;
			break;
		}
		temp = temp->next;
	}
	}while(control == 1);
	sortPID(clone);
	Display(clone);
}


/*SJF Preemptive function has a similar approach with non-preemptive sjf.
But it has one more loop for control, it sorts updated list again and again
and also increments timer one by one.*/
void SJFPreemtive(struct node *allProcesses){
	struct node *temp;
	struct node *clone = copyLinkedList(allProcesses);
	setValues(clone);
	sortArrivalTime(clone);
	int time = clone->arrivalTime - 1;
	int check, control;
	do{
		time++;
		control = 0;
		do{
			sortBurstTime(clone);
			temp = clone;
			check = 0;
			while(temp != NULL){
				if(temp->burstTime != 0){
					if(temp->arrivalTime <= time){
							temp->waitingTime += time - temp->arrivalTime - temp->contextSwitchTime;
							time ++;
							temp->burstTime--;
							temp->contextSwitchTime = time;
							temp->arrivalTime = 0;
							check = 1;
							break;
					}
				}
				temp = temp->next;
			}
		}while(check == 1);

		temp = clone;
		while(temp != NULL){
			if(temp->burstTime != 0){
				control = 1;
				break;
				}
			temp = temp->next;
		}
		}while(control == 1);
		sortPID(clone);
		Display(clone);
}

/*It implements the same algorithm as sjf(preemtive). However, the preemtive
priority function sorts the linked list by priority rather than burst time.*/
void PriorityPreemtive(struct node *allProcesses){
	struct node *temp;
	struct node *clone = copyLinkedList(allProcesses);
	setValues(clone);
	sortArrivalTime(clone);
	int time = clone->arrivalTime - 1;
	int check, control;
	do{
		time++;
		control = 0;
		do{
			sortPriority(clone);
			temp = clone;
			check = 0;
			while(temp != NULL){
				if(temp->burstTime != 0){
					if(temp->arrivalTime <= time){
						temp->waitingTime += time - temp->arrivalTime - temp->contextSwitchTime;
						time ++;
						temp->burstTime--;
						temp->contextSwitchTime = time;
						temp->arrivalTime = 0;
						check = 1;
						break;
					}
				}
				temp = temp->next;
			}
		}while(check == 1);

		temp = clone;
		while(temp != NULL){
			if(temp->burstTime != 0){
				control = 1;
				break;
			}
			temp = temp->next;
		}
		}while(control == 1);
		sortPID(clone);
		Display(clone);
}


/*Uses sortReadyQueueState  to indentify the correct order(initially it sorts
by arrival time). After assigning waiting time for a process, compares process
burst time with time slice(quantum). if burst time  of the process is not lower
than quantum time  repeats same algorithm untill its satisfy that condition.
if it is lower or equal then makes burst time zero.
Repeats that till all processes burst time will be zero.*/
void RoundRobin(struct node *allProcesses){
	struct node *temp;
	struct node *clone = copyLinkedList(allProcesses);
	setValues(clone);
	sortArrivalTime(clone);
	int time = clone->arrivalTime - 1;
	int check, control;
		do{
			time++;
			control = 0;
			do{
				sortReadyQueueState(clone);
				temp = clone;
				check = 0;
				while(temp != NULL){
					if(temp->burstTime != 0){
						if(temp->readyQueueState <= time){
							temp->waitingTime += (time - temp->arrivalTime - temp->contextSwitchTime);
							if(temp->burstTime <= timeQuantum){
              	time += temp->burstTime;
								temp->burstTime = 0;
							}
							else{
								check = 1;
	              time += timeQuantum;
								temp->burstTime -= timeQuantum;
								temp->contextSwitchTime = time;
								temp->readyQueueState = time;
								temp->arrivalTime = 0;
						  	break;
							}
					}
				}
				temp = temp->next;
			}
		}while(check == 1);

		temp = clone;
		while(temp != NULL){
			if(temp->burstTime != 0){
				control = 1;
				break;
			}
			temp = temp->next;
		}
	}while(control == 1);
	sortPID(clone);
	Display(clone);
}


//Swaps the data of the two nodes.
//Uses temporary node for prevent data loss.
void swapNode(struct node *a, struct node *b){

	struct node * temp = createNode(0,0,0);

	temp->burstTime = a->burstTime;
	a->burstTime = b->burstTime;
	b->burstTime = temp->burstTime;

	temp->priority = a->priority;
	a->priority = b->priority;
	b->priority = temp->priority;

	temp->waitingTime = a->waitingTime;
	a->waitingTime = b->waitingTime;
	b->waitingTime = temp->waitingTime;

	temp->arrivalTime = a->arrivalTime;
	a->arrivalTime = b->arrivalTime;
	b->arrivalTime = temp->arrivalTime;

	temp->contextSwitchTime = a->contextSwitchTime;
	a->contextSwitchTime = b->contextSwitchTime;
	b->contextSwitchTime = temp->contextSwitchTime;

	temp->readyQueueState = a->readyQueueState;
	a->readyQueueState = b->readyQueueState;
	b->readyQueueState = temp->readyQueueState;

	temp->pid = a->pid;
	a->pid = b->pid;
	b->pid = temp->pid;

}


/*Sorts linked list according to the burst time in ascending order.
If burst times are equal then function sorts according to pid.*/
void sortBurstTime(struct node *allProcesses){
	struct node * temp;
	struct node *temp2 = NULL;
	int isSwapped;
	do{
		temp = allProcesses;
		isSwapped = 0;
		while(temp->next != temp2){
			if(temp->burstTime > temp->next->burstTime){
				swapNode(temp,temp->next);
				isSwapped = 1;
			}
			else if(temp->burstTime == temp->next->burstTime){
				if(temp->pid > temp->next->pid){
				swapNode(temp,temp->next);
				isSwapped = 1;
				}
			}
			temp = temp->next;
		}
		temp2 = temp;
	}
while(isSwapped == 1);
}


/*Sorts linked list according to the priority in ascending order.
If priority are equal then function sorts according to pid.*/
void sortPriority(struct node *allProcesses){
	struct node * temp;
	struct node *temp2 = NULL;
	int isSwapped;
	do{
		temp = allProcesses;
		isSwapped = 0;
		while(temp->next != temp2){
			if(temp->priority > temp->next->priority){
				swapNode(temp,temp->next);
				isSwapped = 1;
			}
			else if(temp->priority == temp->next->priority){
				if(temp->pid > temp->next->pid){
				swapNode(temp,temp->next);
				isSwapped = 1;
				}
			}
			temp = temp->next;
		}
		temp2 = temp;
	}
while(isSwapped == 1);
}


//Sorts linked list according to the pid in ascending order.
void sortPID(struct node *allProcesses){
	struct node * temp;
	struct node *temp2 = NULL;
	int isSwapped;
	do{
		temp = allProcesses;
		isSwapped = 0;
		while(temp->next != temp2){
			if(temp->pid > temp->next->pid){
					swapNode(temp,temp->next);
					isSwapped = 1;
			}
			temp = temp->next;
		}
		temp2 = temp;
	}while(isSwapped == 1);
}


/*Sorts linked list according to the arrival time in ascending order.
If arrival times are equal then function sorts according to pid.*/
void sortArrivalTime(struct node *allProcesses){
	struct node * temp;
	struct node *temp2 = NULL;
	int isSwapped;
	do{
		temp = allProcesses;
		isSwapped = 0;
		while(temp->next != temp2){
			if(temp->arrivalTime > temp->next->arrivalTime){
				swapNode(temp,temp->next);
				isSwapped = 1;
			}
			else if(temp->arrivalTime == temp->next->arrivalTime){
				if(temp->pid > temp->next->pid){
				swapNode(temp,temp->next);
				isSwapped = 1;
				}
			}
			temp = temp->next;
		}
		temp2 = temp;
	}
while(isSwapped == 1);
}


/*Sorts linked list according to the ready queue state in ascending order.
If ready queue states are equal then  the function sorts according
to context switch time. If context switch times are also equal then
function sorts according to pid. This function is design for identify when
a interrupted process turn back to the cpu in round robin method.*/
void sortReadyQueueState(struct node *allProcesses){
	struct node * temp;
	struct node *temp2 = NULL;
	int isSwapped;
	do{
		temp = allProcesses;
		isSwapped = 0;
		while(temp->next != temp2){
			if(temp->readyQueueState > temp->next->readyQueueState){
					swapNode(temp,temp->next);
					isSwapped = 1;
			}

			else if(temp->readyQueueState == temp->next->readyQueueState){

				if(temp->contextSwitchTime > temp->next->contextSwitchTime){
					swapNode(temp,temp->next);
					isSwapped = 1;
				}

				else if(temp->contextSwitchTime == temp->next->contextSwitchTime){

					if(temp->pid > temp->next->pid){
						swapNode(temp,temp->next);
						isSwapped = 1;
					}
				}
			}
			temp = temp->next;
		}
		temp2 = temp;
}while(isSwapped == 1);
}
