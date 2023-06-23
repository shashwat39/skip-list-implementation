#include <iostream>
#include <stdlib.h>
#include <time.h> // required for tossing a coin stimulation which decides whether to upgrade a level or not
using namespace std;

//the maximum height of the levels in skip-list to be 100 so as to ...
#define max_height 100

typedef struct skipList
{
	long long value; // stores the value a node contains
	struct skipList *next, *prev, *up, *down; // stores address of/ pointers to next, prev, up, down
	int height; // randomly choosen (similar to geometric distribution HHHHHHT <- this T's first occurence will determine the height)
	int curlevel; // this is not part of Skip List implementation, primarily there to check ..

} node;

node *neginf, *posinf;

// Search function implementation, tells if there is an element present in the skip list in O(log n) with high probability
bool search(int x)
{
	// A traversal variable to store the address of the Leftmost pointer of the skip-list
	node *temp = neginf;

	// A variable to store the current level of pointer neginf
	int lvl = neginf->curlevel;

	//until we reach the bottom-most level
	while(lvl--)
	{
		// for each level we continue to iterate(in the level concerned) until we don't reach an element which is greater than x 
		while (temp->next->value <= x)
        {
            // Moving onto the next element in linked list on a particular level
            temp = temp->next;
        }

        // If the element at breaking point of above while loop is equal to x that means we've found x
        if (temp->value == x)
            return true;

        // decrementing the level by 1
        temp = temp->down;
	}
	// since while loop is over, meaning I didn't find the element
	// I will return false
	return false;
}

// function to insert an element in the skip list DS
int insert(int x, int level)
{
	// insertionPoints is an array of pointers to node objects. OR, The insertionPoints variable is a pointer to an array of pointers to nodes.
	// allocates memory on the heap for the array. The size of the array is neginf->curlevel * sizeof(node *), where neginf->curlevel is the current level of the skip list.
	node** insertionPoints = new node*[neginf->curlevel];

	// next, we create a temporary traversal node
	node *temp = neginf;

	int lvl = neginf->curlevel; // current level

	//until we reach the bottom-most level
	while(lvl--)
	{
		//Move forward until you find the element greater than or equal to the element x
        while (temp->next->value < x)
        {
            temp = temp->next;
        }

        //As we traverse down, we need to keep track of the nodes where we need to make the connections between the new node and the existing nodes in the skip list.
        // This allows us to later access these nodes and correctly link the new node between them at each level.
        insertionPoints[lvl] = temp;

        //If you are not at the lowermost level, go down and search in next level starting from this temp
        if (temp->down != NULL)
            temp = temp->down;
	}

	//In this case, the skip list needs to be extended to accommodate the new level
	if(level > neginf->height) //checks if the desired level (level) is greater than the current height of the skip list (neginf->height).
	{
		insertionPoints = new node*[level]; // //Allocate more memory to store the new infinity values and the new node required

		// Allocate additional infinity nodes as needed
		for(int i=neginf->height; i < level; i++)
		{
			// Allocate memory for the new infinity node
			node* newinf = new node();

			// Duplicate the contents of the existing infinity node to the new infinity node
			newinf->value = neginf->value;
            newinf->height = level; // clearly tells the difference b/w height and curlevel
            newinf->curlevel = i + 1;
            newinf->next = newinf->prev = newinf->up = newinf->down = NULL;
            newinf->down = neginf;
            newinf->next = posinf;
            neginf = newinf;

            //Set the previous of the new level to the new infinity
            insertionPoints[i] = neginf;

		}
	}

	 //new temporary node(will update in loop)
    node *tmp = NULL;

    //Go from the lowermost to the highest level
    for (int i = 0; i < level; i++)
    {   
        //Allocate memory for the new node
        node* newnode = new node();

        // make required changes to connect the new node properly
        newnode->value = x;
        newnode->prev = insertionPoints[i];
        newnode->next = insertionPoints[i]->next;

        insertionPoints[i]->next = newnode;
        newnode->down = tmp;
        newnode->next->prev = newnode;

        tmp = newnode;
        newnode->curlevel = i + 1;
        newnode->height = level;
    }
}

// Function to delete any element from skip list
int del(int x)
{
	node** deletePointers = new node*[neginf->curlevel]; // Allocating memory for the deletePointers array
	
	// Setting temp to point to the neginf node
	node *temp = neginf;

	// Storing the current level of the neginf node in the variable lvl
	int lvl = neginf->curlevel;

	// While iterating through the levels from top to bottom
	while (lvl--)
	{
	    // While the value in the next node of temp is less than or equal to x, move to the next node
	    while (temp->next->value <= x)
	    {
	        // Move to the next node
	        temp = temp->next;
	    }

	    // Store the current node (temp) in deletePointers[lvl]
	    deletePointers[lvl] = temp; //This is done to keep track of the previous node at each level for potential deletions.

	    // If temp->value equals x and temp is not a corner element or bottommost element,
	    // delete the element from the linked list by updating the previous and next pointers
	    if (temp->value == x && (temp->next != posinf || temp->prev->value != neginf->value || temp->prev->down == NULL))
	    {
	        temp->prev->next = temp->next;
	        temp->next->prev = temp->prev;
	    }

	    // If temp->value equals x and it is a corner element, free the pointer neginf after moving one level down
	    else if (temp->value == x && temp->next == posinf && temp->prev->value == neginf->value)
	    {
	        node *ptr = neginf;
	        neginf = neginf->down;
	        delete ptr;
	    }

	    // If temp->down is not NULL, move temp to temp->down
	    if (temp->down != NULL)
	    {
	        // Store the current node (temp) in ptr
	        node *ptr = temp;

	        // Move one level down
	        temp = temp->down;

	        // If the value in temp is equal to x, free the pointer ptr
	        if (temp->value == x)
	            delete ptr;
	    }
	}
}

// Function to find and return the minimum value among the elements in the skip-list
long long min()
{
	// Storing the address of the highest level node in the variable temp
	node *temp = neginf;
	// Traverse down the skip-list until reaching the lowest level
	while (temp->down != NULL)
	{
	    temp = temp->down;
	}

	// Return the value stored in the next node pointer at the bottommost level of the skip-list
	return temp->next->value;
}

// Function to print one level of the skip-list
void print(node* ptr)
{
    // Print the current level number
    cout << "Level " << ptr->curlevel << " - ";

    // Move to the next node in this level
    ptr = ptr->next;

    // Traverse through the nodes in this level until reaching the last node
    while (ptr->next != NULL)
    {
        // If the next node of ptr doesn't point to positive infinity, print the value followed by an arrow (->)
        if (ptr->next != posinf)
            cout << ptr->value << " -> ";
        // If it is the last node, don't print the arrow (->)
        else
            cout << ptr->value;

        // Move to the next node in this level
        ptr = ptr->next;
    }

    // Print a new line to end the level
    cout << endl;
}

// Function to print the entire skip-list
void traverse()
{
    // Store the address of neginf in ptr
    node* ptr = neginf;

    // Traverse through each level of the skip-list from top to bottom
    while (ptr != NULL)
    {
        // Call the print function to print the current level of the skip-list
        print(ptr);

        // Move down by one level
        ptr = ptr->down;
    }
}

int main()
{
	// Asking the user if they want to see the time taken for every function
	cout << "Should the times for each operation be displayed? (Y/N): ";

	// Variable to store the input for Y/N
	char displayTimes;

	// Taking the input for Y/N
	cin >> displayTimes;

	// Allocating memory to the pointer neginf
	neginf = new node();

	// Setting the value of value to -1
	neginf->value = -1;

	// Making the value of neginf->value equal to -2^63
	for (int i = 0; i < 63; i++)
	    neginf->value *= 2;

	// Setting the initial height of neginf as 1
	neginf->height = 1;

	// Setting the current level of neginf as 1
	neginf->curlevel = 1;

	// Making next, prev, up, down of neginf as NULL
	neginf->next = neginf->prev = neginf->up = neginf->down = NULL;

	// Allocating memory to posinf
	posinf = new node();

	// Setting the value of posinf to 2^63 - 1
	posinf->value = 9223372036854775807;

	// Setting the height of posinf to 1
	posinf->height = 1;

	// Setting the current level of posinf to 1
	posinf->curlevel = 1;

	// Giving neginf to the prev of posinf
	posinf->prev = neginf;

	// Giving posinf to the next of neginf
	neginf->next = posinf;

	// Setting next, up, down of posinf to NULL
	posinf->down = NULL;
	posinf->next = NULL;
	posinf->up = NULL;

	// Printing the menu of operations
	cout << "1. Insert\n2. Search\n3. Delete\n4. Minimum\n5. Traverse\n-1 To EXIT\n";


	srand(static_cast<unsigned>(time(0))); /* Used to seed the random number generator
    Otherwise, the sequence of random numbers generated will be the same every time the program is executed */

	// Asking the user for a command
	cout << "Command: ";

	// startTime is the start time of the while loop
	// finishTime would be the finish time of the while loop
	// The difference between finishTime and startTime would be the total time taken for execution
	time_t startTime, finishTime;

	// startTime takes the current time value
	startTime = clock();

	// Repeat the loop as many times as the user wants, and break when the user enters -1.
	while(true)
	{
		// Variable to store the command number
		int command;

		// Taking input of the command number
		cin >> command;

		if (command == 1)
		{
		    // If the command is 1, we need to insert a value in the skip-list

		    // Start is the start time of the insert function
		    // Finish is the finish time of the insert function
		    time_t start, finish;

		    // Asking the user for the number to be inserted
		    cout << "Enter the number to be inserted in the skip list: ";

		    // Variable to store the input number
		    int number;

		    // Taking input of the number to be inserted
		    cin >> number;

		    // Setting the current level to 1
		    int level = 1;

		    // Choosing a random number and storing it in p
		    int p = rand();

		    // Starting the clock for calculating insertion time
		    start = clock();

		    while (p % 2)
		    {
		        // If p is odd, increase the level by 1
		        level++;

		        // Toss the coin again at this level (select a random number)
		        p = rand();

		        // If the level reaches the maximum supported level, break out of the loop
		        if (level == max_height)
		            break;
		    }

		    // Calling the insert function with the number and its level
		    insert(number, level);

		    // Finishing time of the clock
		    finish = clock();

		    if (displayTimes == 'Y')
		    {
		        // If we're asked to display the total insertion time, print the following line
		        cout << "The time taken to perform the insert operation is " << (double)(finish - start) / CLOCKS_PER_SEC << "s" << endl;
		    }

		}
		else if(command == 2)
		{
			// If the command is 2, we need to search for a value in the skip-list

			// Start time of the search function
			// Finish time of the search function
			time_t start, finish;

			// Asking the user to enter the number to be searched for
			cout << "Enter the number to be searched in the skip list: ";

			// Variable to store the number to be searched
			int number;

			// Taking input of the value to be searched for
			cin >> number;

			// Starting the clock to measure searching time
			start = clock();

			// If search(number) returns true, the element is present in the skip-list
			if (search(number))
			    cout << "Present in the skip list" << endl;
			else
			    cout << "Not present in the skip list" << endl;

			// Finishing time of the clock
			finish = clock();

			if (displayTimes == 'Y')
			{
			    // If the user wants to display the total searching time, print the following line
			    cout << "The time taken to perform the search operation is " << (double)(finish - start) / CLOCKS_PER_SEC << "s" << endl;
			}

		}

		else if(command == 3)
		{
			// If the command is 3, we need to delete a value from the skip-list

			// Start time of the delete function
			// Finish time of the delete function
			time_t start, finish;

			// Asking the user to input the value to be deleted from the skip-list
			cout << "Enter the number to be deleted from the skip list: ";

			// Variable to store the number to be deleted
			int number;

			// Taking input of the value to be deleted from the skip-list
			cin >> number;

			// Starting the clock to measure deletion time
			start = clock();

			// Calling the delete function with the value to be deleted
			del(number);

			// Uncomment the below 4 lines if duplicates are to be deleted when command 2 is selected
			// while (search(number))
			// {
			//     deleteValue(number);
			// }

			// Finishing time of the clock
			finish = clock();

			if (displayTimes == 'Y')
			{
			    // If the user wants to display the total deletion time, print the following line
			    cout << "The time taken to perform the delete operation is " << (double)(finish - start) / CLOCKS_PER_SEC << "s" << endl;
			}

		}

		else if(command == 4)
		{
			// If the command is 4, we need to find the minimum element in the skip-list

			// Start time of the min function
			// Finish time of the min function
			time_t start, finish;

			// Starting the clock to measure the time for finding the minimum element
			start = clock();

			// Calling the min function and storing the minimum value in 'minValue'
			long long minValue = min();

			// Finishing time of the clock
			finish = clock();

			// If 'minValue' is not equal to the minimum 'value' value, there is an element in the skip-list and the minimum value is 'minValue'
			if (minValue != posinf->value)
			    cout << "The minimum element is: " << minValue << endl;

			// If 'minValue' is equal to the minimum 'value' value, there are no elements in the skip-list
			else
			    cout << "No element has been inserted in the skip list" << endl;

			if (displayTimes == 'Y')
			{
			    // If the user wants to display the total time taken for finding the minimum element, print the following line
			    cout << "The time taken to perform the min operation is " << (double)(finish - start) / CLOCKS_PER_SEC << "s" << endl;
			}

		}
		else if(command == 5)
		{
			// If the command is 5, we need to traverse the entire skip-list and print it

			// Start time of the traverse function
			// Finish time of the traverse function
			time_t start, finish;

			// Starting the clock to measure the time for traversal
			start = clock();

			// Calling the traverse function to print the entire skip-list level-wise
			traverse();

			// Finishing time of the clock
			finish = clock();

			if (displayTimes == 'Y')
			{
			    // If the user wants to display the total time taken for traversing the skip-list, print the following line
			    cout << "The time taken to perform the traversal operation is " << (double)(finish - start) / CLOCKS_PER_SEC << "s" << endl;
			}
		}
		else if(command == -1)
		{
			cout << "Exiting..." << endl;
			break;
		}
		else
		{
		    cout << "The command you entered is not recognized. Please try again.." << endl;
		    cout << "1. Insert\n2. Search\n3. Delete\n4. Minimum\n5. Traverse\n-1 To EXIT\n";
		}
		cout << endl;
		cout << "Command: ";
	}

	// Stop the clock after all the functions have finished executing.
	finishTime = clock();

	if(displayTimes == 'Y') // Printing the total execution time by taking difference of finish and start time
	{
		double total_time;
		// Calculate the total execution time.
		total_time = (double)(finishTime - startTime) / CLOCKS_PER_SEC;

		// Print the total execution time.
		cout << "Total time taken for the execution of the program is " << total_time << " seconds." << endl;
	}

	delete neginf;
	delete posinf;

	return 0;
}