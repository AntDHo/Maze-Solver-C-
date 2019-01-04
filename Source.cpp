//					Author:			 Anthony Ho
//
//					Project:		 CSC 412 Intro to AI, First Programming Assignment. 
//
//					Teacher:		Dr. Byrant Walley
//
//					Purpose:			Purpose of the project is to read in a text file called maze.txt into a 2d array. 
//									Then output the filled 2d array to the console screen in a nice visual way. 
//									Next was to find the starting node/entry node by traversing the left wall until it is found.
//									Once found, create a node based on the information that the starting node holds.
//									After creating the node, run DFS and BFS using the starting node as the initial node to run
//									run both searches. Both search will traverse the maze until X "Exit" is found.
//									While the searchs traverse, a node will be created for each step taken in any direction.
//									The path to each node will be recorded in a list, and each node visited will be marked visisted
//									so no repeated nodes will be an issue. Once X is found, the path taken by both searches
//									will be outputted to the screen in a nice visual manner. 
//
//					Date:			September 25, 2018
//
//					Due:			September 30, 2018 
//
//					My Logic:			Read in maze.txt into 2d array, output 2d array to screen. Make it look readable for user.
//									Using for loops and if statements to find/traverse left wall of array/maze to find entry 'E'.
//									Create node using information of Entry, push and pop the node to the queue/stack, check if its 
//									the exit 'X'. Loops and if statements to move UP DOWN LEFT RIGHT, dont worry about looking for
//									Letter E or W, just X or P, make sure location is in bounds, used boolean arrays to store visisted nodes
//									Used lists to store paths of each node to the exit.
//									Made sure user is alerted if searches failed, and also if file is not found or opened.

#include<iostream>// A lot of different methods were tried as to why there are many include statements.
#include<stdio.h> // I decided just to keep them.
#include<iomanip>
#include<string>
#include<cstring>
#include<map>
#include<queue>
#include<time.h>
#include<fstream>
#include<cstdlib>
#include<vector>
#include <list>
#include<stdlib.h>
#include <algorithm>
#include <stack>


using namespace std;


//static const int SIZE = 100;
//static const int WIDTH = 10;
//static const int HEIGHT = 10;

#define SIZE  100 // used as a check for staying in bounds of the array values when x and y values are combined
#define WIDTH  10 
#define HEIGHT  10 


int xCoord[] = { -1, 0, 0, 1 };// both xcoord and ycoord is needed to move UP DOWN LEFT RIGHT
int yCoord[] = { 0, -1, 1, 0 };// short cut method 
// You could type it long ways as for example:
// (Current.row -1, Current.col) Goes Left
// (Current.row +1, Current.col) Goes Right
// (Current.row, Current.col - 1) goes up
// (current.row, current.col + 1) goes down

//const char Wall = 'W';
//const char Path = 'P';
//const char Entry = 'E';
//const char Exit = 'X';
#define Wall 'W'
#define Path 'P'
#define Entry 'E'
#define Exit 'X'


struct Agent // as instructed, agent/node builder needs at least 3 characteristics
{
	int row; // row and col for x and y coords
	int col;
	int loc; // loc is x and y coords as a single value
	char Data;//store letter
	list<int>path;// store the traveled path to exit
};

void ReadWrite(char mArray[WIDTH][HEIGHT]);// read in maze.txt to 2d array, output maze to screen

bool checkValid(int xR, int yC);// check if values stay in bounds

bool checkLetter(char L);// check on letters that required to finish maze

void BFS(char mArr[WIDTH][HEIGHT]); // BREADTH FIRST SEARCH FUNCTION

void DFS(char mArr[WIDTH][HEIGHT]); // DEPTH FIRSTS SEARCH FUNCTION







int main()
{


	char mazeArray[HEIGHT][WIDTH]; //declaring 2d array of required size for program

	ReadWrite(mazeArray);// read file, output to console

	BFS(mazeArray);// insert maze into BFS algorithm

	DFS(mazeArray); // insert maze into DFS algorithm

	system("PAUSE");
	return 0;

}





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ReadWrite(char mArray[WIDTH][HEIGHT])// declared maze from main as parameter
{

	fstream inFile("maze.txt"); // read file 

	if (!inFile.is_open())// if file is not found/open
	{
		cout << "File was not found or is not opened. " << endl;
		system("PAUSE");
		exit(-1);

		
	}

	if (inFile.is_open()) // open the file/make sure its open
	{
		for (int i = 0; i < HEIGHT && !inFile.eof(); i++)
		{
			for (int j = 0; j < WIDTH && !inFile.eof(); j++)// for loops to read letters from text file
			{
				inFile >> mArray[i][j];// letters are read into array through for loops
			}
		}
		inFile.close();// close the file
	}


	cout << "  0	1	2	3	4	5	6	7	8	9" << endl; // labeling maze

	for (int i = 0; i < HEIGHT; i++)
	{
		cout << i << " ";// labeling maze

		for (int j = 0; j < WIDTH; j++)// for loops to output maze to console
		{
			cout << mArray[i][j] << '\t';// cout statement to output to screen
		}

		cout << '\n';// new line, needed for better visability 
	}
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool checkLetter(char L)// looking only for the needed letters
{
	if ((L == Path) || (L == Exit)) // if letter equals Path 'P' or Exit 'X'
	{
		return true;// return true if it is a letter we need
	}
	else
		return false;// not a letter we need

}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool checkValid(int xR, int yC)// check for staying in bound
{
	if ((xR >= 0) && (xR < WIDTH) && (yC >= 0) && (yC < HEIGHT))// if x and y values are not less than 0 and more than instructed size of array
	{
		return true;// if location is in bounds
	}
	else
	{
		return false;// not in bounds
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void BFS(char mArr[WIDTH][HEIGHT])// BFS function
{
	Agent next;// node to store next node after the current
	Agent state;// initial node , stores Entry 'E' node
	int XY = 0;// used to combine x and y values into a single variable
	queue<Agent> q; // queue created for BFS use
	list<int>::iterator it;// iterate through list function to return path taken to find Exit 'X' node
	int check = 0;// variable to let user know if exit path exists

	bool visited[WIDTH][HEIGHT]; // bool array, instead of using map function, keeps track of visited nodes

	for (int m = 0; m < WIDTH; m++)
	{
		for (int n = 0; n < HEIGHT; n++)
			visited[m][n] = false; // setting bool array elements to unvisited
	}


	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)// for loops i,j used to find entry node/ traversing left wall to find entry
		{
			// Finding entry/starting node
			if (mArr[i][j] == Entry)
			{
				int XY = (10 * i + j);
				//state = { i, j,'E',{ XY } }; this is one way i could store info, but keep them separated was better for my eyes
				state.row = i; // storing row in node
				state.col = j;// storing col in node
				state.loc = XY;// storing location/x and y coords combined in node
				state.Data = Entry;//storing the letter 'E'/ Entry
				state.path.push_back(XY);// storing x and y coords into list
				visited[state.row][state.col] = true;// marking starting node as visited
			
			}
		}
	}

	q.push(state); // adding starting node to queue

	while (!q.empty())// queue is not empty
	{


		Agent current = q.front();// current takes on value of node stored in queue
		
		//char data = current.Data;
	
		if (mArr[current.row][current.col] == Exit)// Exit found
		{
			check = 1;// variable to let user know if exit path exists
			XY = (10 * current.row + current.col); // combining x and y values into one variable
			cout << "Congrats you found the exit at: " << XY << endl;

			cout << "The pathway to freedown using BFS is as follows: " << endl;
			for (it = current.path.begin(); it != current.path.end(); it++)//output the path taken to find exit node
			{
				cout << ' ' << *it;
			}
			cout << endl << endl;
			
		}

		

		q.pop(); // pop node from queue

		for (int i = 0; i < 4; i++)// for loop to traverse maze moving UP DOWN LEFT RIGHT
		{
			int row = current.row + xCoord[i];
			int col = current.col + yCoord[i];
			char data = mArr[row][col];// variable storing letter 

			if (checkValid(row, col) && checkLetter(data) && visited[row][col] == false)
			{
				visited[row][col] = true;// mark location visited
				next.row = row;// store next node's row num
				next.col = col;// store next node's col num
				next.Data = data;// store next node's letter 
				next.path = current.path;// storing next node's path location/ copying path
				XY = (10 * row + col);// making next node's path into a single variable
				next.path.push_back(XY);// add next node's path
				q.push(next);// pushing next node onto queue
			}
		}

	

	}
	

	if (check == 0)// if value stayed 0, output no path was found
	{
		cout << "BREADTH FIRST SEARCH could not find a path to the exit. Try a different maze. " << endl;
	}
	
	
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void DFS(char mArr[WIDTH][HEIGHT]) // depth first search function
{
	Agent next;// node to store next node after the current
	Agent state;// initial node , stores Entry 'E' node
	int XY = 0;// used to combine x and y values into a single variable
	stack<Agent> q; // queue created for DFS use
	list<int>::iterator it;// iterate through list function to return path taken to find Exit 'X' node
	int check = 0;// variable to let user know if exit path exists

	bool visited[WIDTH][HEIGHT]; // bool array, instead of using map function, keeps track of visited nodes

	for (int m = 0; m < WIDTH; m++)
	{
		for (int n = 0; n < HEIGHT; n++)
			visited[m][n] = false; // setting bool array elements to unvisited
	}


	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)// for loops i,j used to find entry node/ traversing left wall to find entry
		{
			// Finding entry/starting node
			if (mArr[i][j] == Entry)
			{
				int XY = (10 * i + j);
				//state = { i, j,'E',{ XY } }; this is one way i could store info, but keep them separated was better for my eyes
				state.row = i; // storing row in node
				state.col = j;// storing col in node
				state.loc = XY;// storing location/x and y coords combined in node
				state.Data = Entry;//storing the letter 'E'/ Entry
				state.path.push_back(XY);// storing x and y coords into list
				visited[state.row][state.col] = true;// marking starting node as visited

			}
		}
	}

	q.push(state); // adding starting node to stack

	while (!q.empty())// stack is not empty
	{


		//Agent current = q.front();// current takes on value of node stored in queue
		Agent current = q.top(); // as this is for stacks, changed q.front to q.top
								

		if (mArr[current.row][current.col] == Exit)// Exit found
		{
			check = 1; // variable to let user know if exit path exists
			XY = (10 * current.row + current.col);// combining x and y variable
			cout << "Congrats you found the exit at: " << XY << endl;

			cout << "The pathway to freedown using DFS is as follows: " << endl;
			for (it = current.path.begin(); it != current.path.end(); it++)// output path taken to get to exit node
			{
				cout << ' ' << *it;
			}
			cout << endl;
		}



		q.pop();// pop node from stack

		for (int i = 0; i < 4; i++)// for loop to traverse maze moving UP DOWN LEFT RIGHT
		{
			int row = current.row + xCoord[i];// coords do move left and right
			int col = current.col + yCoord[i];// coords to move up and down
			char data = mArr[row][col];// store letter value of current location

			if (checkValid(row, col) && checkLetter(data) && visited[row][col] == false)// check if in bounds, check if its a letter needed, and check if its not visisted
			{
				visited[row][col] = true;// marking node as visisted
				next.row = row;// storing values of the next node *
				next.col = col;// *
				next.Data = data;// *
				next.path = current.path;// * copying path of old node
				XY = (10 * row + col);// single variable format for x and y
				next.path.push_back(XY);// adding location to list
				q.push(next);// push next node onto stack
			}
		}




	}

	if (check == 0)// if value stayed 0, output no path was found
	{
		cout << "DEPTH FIRST SEARCH could not find a path to the exit. Try a different maze. " << endl;
	}

}
