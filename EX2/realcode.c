#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ENGYEAR 1 // Type of year: English
#define HEBYEAR 0 // Type of year: Hebrew

// Worker struct
typedef struct Worker
{
	unsigned long id;
	char* name;
	unsigned long salary;
	union StartingYear
	{
		char HebYear[7];
		unsigned long EngYear;
	};
} Worker;

// Linked list struct
typedef struct WorkerList
{
	Worker* data;
	struct WorkerList* next;
} WorkerList;

char* initString();
Worker* CreateWorker(int Heb_or_Eng);
void PrintWorker(Worker* worker, int Heb_or_Eng);
WorkerList* addWorker(WorkerList* head, Worker* w);
int indexLoop(WorkerList* head, unsigned long id);
int indexRecursion(WorkerList* head, unsigned long id);
WorkerList* deleteWorstWorker(WorkerList* head);
void updateWorker(WorkerList* head, float percent);
WorkerList* reverse(WorkerList* head);
void freeWorkers(WorkerList* head);
void printList(WorkerList* head, int Heb_or_Eng);

void main()
{
	Worker* worker1, * worker2, * worker3;
	worker1 = CreateWorker(ENGYEAR);
	worker2 = CreateWorker(ENGYEAR);
	worker3 = CreateWorker(ENGYEAR);
	PrintWorker(worker1, ENGYEAR);
	WorkerList* List = NULL;
	List = addWorker(List, worker1);
	List = addWorker(List, worker2);
	List = addWorker(List, worker3);
	printList(List, ENGYEAR);
	printf("\nIndex by loop for ID %d: %d\n", 2, indexLoop(List, 2)); // '2' - random number to check the function
	printf("\nIndex by recursion for ID %d: %d\n", 17, indexRecursion(List, 17)); // '17' - random number to check the function
	List = deleteWorstWorker(List);
	updateWorker(List, 10); // '10' - Random percent to check the function
	List = reverse(List);
	printList(List, ENGYEAR);
	freeWorkers(List);
	return;
}

// Dynamic string input
char* initString()
{
	char temp_str[50];
	fseek(stdin, 0, SEEK_END);
	gets(temp_str);
	int size = strlen(temp_str);
	char* new_str = (char*)malloc(size * sizeof(char) + 1);
	if (!new_str)
	{
		printf("Memory allocation failed!\n");
		exit(0);
	}
	strcpy(new_str, temp_str);
	return new_str;
}

// Creats new worker
Worker* CreateWorker(int Heb_or_Eng)
{
	Worker* NewWorker = (Worker*)malloc(sizeof(Worker));
	if (!NewWorker)
	{
		printf("Memory allocation failed!\n");
		exit(0);
	}
	printf("\nPlease enter worker information:\n\n");
	printf("Enter worker ID:\n");
	scanf("%ld", &NewWorker->id);
	printf("Enter worker name:\n");
	NewWorker->name = initString();
	printf("Enter worker salary:\n");
	scanf("%ld", &NewWorker->salary);
	printf("Enter worker's starting year:\n");
	switch (Heb_or_Eng)
	{
	case 0:
	{
		fseek(stdin, 0, SEEK_END);
		gets(NewWorker->HebYear);
		break;
	}
	case 1:
	{
		scanf("%ld", &NewWorker->EngYear);
		break;
	}
	default:
	{
		printf("Wrong choice!\n");
		free(NewWorker);
		return NULL;
	}
	}
	return NewWorker;
}

// Prints worker information
void PrintWorker(Worker* worker, int Heb_or_Eng)
{
	printf("\nWorker information:\n\n");
	printf("Worker's ID: %ld\n", worker->id);
	printf("Worker's name: %s\n", worker->name);
	printf("Worker's salary: %ld\n", worker->salary);
	switch (Heb_or_Eng)
	{
	case 0:
	{
		printf("Worker's starting year: %s\n", worker->HebYear);
		break;
	}
	case 1:
	{
		printf("Worker's starting year: %ld\n", worker->EngYear);
		break;
	}
	default:
	{
		printf("Wrong year type!\n");
		return;
	}
	}
}

// Adds worker to linked list
WorkerList* addWorker(WorkerList* head, Worker* w)
{
	// Creates new list element for w
	WorkerList* NewElement = (WorkerList*)malloc(sizeof(WorkerList));
	if (!NewElement)
	{
		printf("Memory allocation failed!\n");
		exit(0);
	}
	NewElement->data = w;
	NewElement->next = NULL;

	// In case list is empty
	if (head == NULL)
	{
		head = NewElement;
		NewElement->next = NULL;
		return head;
	}

	// In case NewElement should be first element in list
	if (NewElement->data->salary > head->data->salary)
	{
		NewElement->next = head;
		head = NewElement;
		return head;
	}

	// In case NewElement should be place in the middle or in the end of the list
	WorkerList* temp_curr = head;
	WorkerList* temp_prev = NULL;
	while ((temp_curr != NULL) && (NewElement->data->salary < temp_curr->data->salary))
	{
		temp_prev = temp_curr; // temp_prev keeps temp_curr's last position
		temp_curr = temp_curr->next;
	} // temp_curr now points to list element with lower salary than NewElement OR points to NULL
	temp_prev->next = NewElement;
	NewElement->next = temp_curr;
	return head;
}

// Search worker's place in list according to ID (By loop)
int indexLoop(WorkerList* head, unsigned long id)
{
	// In case the list is empty
	if (head == NULL)
	{
		return -1;
	}

	int counter = 1;
	WorkerList* temp_ptr = head;
	while ((temp_ptr != NULL) && (id != temp_ptr->data->id))
	{
		counter++;
		temp_ptr = temp_ptr->next;
	}
	if (temp_ptr == NULL) // In case the ID was not found in list
	{
		return -1;
	}
	return counter;
}

// Search worker's place in list according to ID (By recursion)
int indexRecursion(WorkerList* head, unsigned long id)
{
	// In case the list is empty
	if (head == NULL)
	{
		return -1;
	}

	static int counter = 1;
	if (head->data->id == id) // Stoping condition
	{
		return counter;
	}
	if (head == NULL) // In case ID was not found in list
	{
		return -1;
	}
	counter++;
	indexRecursion(head->next, id); // Recursion step
}

// Deletes the worker with the lowest salary
WorkerList* deleteWorstWorker(WorkerList* head)
{
	// In case the list is empty
	if (head == NULL)
	{
		return NULL;
	}

	// The worker with the lowest salary is the last worker in the list
	WorkerList* temp_curr = head;
	WorkerList* temp_prev = NULL;
	while (temp_curr->next != NULL)
	{
		temp_prev = temp_curr; // temp_prev keeps temp_curr's last position
		temp_curr = temp_curr->next;
	} // temp_curr now points to the last element in the list
	temp_prev->next = NULL;
	free(temp_curr);
	return head;
}

// Raises the salary of all employees in the list by a percent
void updateWorker(WorkerList* head, float percent)
{
	// In case the list is empty
	if (head == NULL)
	{
		return;
	}

	WorkerList* temp_ptr = head;
	while (temp_ptr != NULL)
	{
		temp_ptr->data->salary = temp_ptr->data->salary + ((temp_ptr->data->salary * percent) / 100);
		temp_ptr = temp_ptr->next;
	}
}

// Reverse linked list order
WorkerList* reverse(WorkerList* head)
{
	// In case the list is empty or only have one element
	if ((head == NULL) || (head->next == NULL))
	{
		return head;
	}

	WorkerList* temp_curr = head;
	WorkerList* temp_next = NULL;
	WorkerList* temp_prev = NULL;
	while (temp_curr != NULL)
	{
		temp_next = temp_curr->next;
		temp_curr->next = temp_prev;
		temp_prev = temp_curr;
		temp_curr = temp_next;
	}
	head = temp_prev;
	return head;
}

// Delete the linked list and free memory
void freeWorkers(WorkerList* head)
{
	WorkerList* temp_ptr = head;
	while (head != NULL)
	{
		temp_ptr = head;
		head = head->next;
		free(temp_ptr);
	}
}

// Prints the linked list
void printList(WorkerList* head, int Heb_or_Eng)
{
	WorkerList* temp_ptr = head;
	printf("\nPrinting list: \n");
	while (temp_ptr != NULL)
	{
		PrintWorker(temp_ptr->data, Heb_or_Eng);
		temp_ptr = temp_ptr->next;
	}
}

// התוכנית עוברת על כל איברי הרשימה המקושרת(קוארדינטות) ומוחקת ממנה את כל הכפיליות.