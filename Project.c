#include<stdio.h>
#include <stdlib.h>
//linked list for processes
struct node
{
	int pid;
	int waitingTime;
	int burstTime;
	int arrivalTime;
	int priority;
 	struct node *next;
};
struct node * createNode(int, int, int);
struct node * insertBack(struct node *, int, int, int);
void setPid(struct node *allProcesses);

int main(int argc, char* argv[])
{

	//reading data from file
	char* fileName = argv[1];
  FILE* processes = fopen(fileName,"r");
	if(processes == NULL)
  {
    printf("ERROR: %s does not exist\n",argv[1]);
    return(0);
  }

  struct node *allProcesses = NULL;
  int b, a, p;
	int numberOfProcess = 1;
  while(!feof(processes))
  {
  fscanf(processes,"%d:%d:%d", &b, &a, &p);
  allProcesses = insertBack(allProcesses, b, a, p);
	setPid(allProcesses);
	numberOfProcess++;
  }
  fclose(processes);    
  return 0;
}

struct node * createNode(int b, int a, int p)
{
     struct node * temp;
     temp = (struct node *) malloc(sizeof(struct node));
     temp->burstTime = b;
		 temp->arrivalTime = a;
		 temp->priority = p;
     temp->next = NULL;
     return temp;
}

struct node * insertBack(struct node *header, int b, int a, int p)
{
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

	void setPid(struct node *allProcesses)
	{
		struct node *temp = allProcesses;
		int i = 1;
			while (temp != NULL)
			{
					temp->pid = i;
					temp=temp->next;
					i++;
		}
	}

