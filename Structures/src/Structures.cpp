//============================================================================
// Name        : Structures.cpp
// Author      : Joel
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stack.h>
#include <queue.h>
#include <string>
#include <Table.h>

using namespace std;

int main() {
	int x=20;
	int y=15;
	string t;
	char z=0;
	bool temp;
	Stack<int> intStack;
	Queue<int> intQueue;
	HashMap<char> map;

	string tipo1="";
	string tipo2="";

	varEntry variables=varEntry("xa","int");
	varEntry variables2=varEntry("ax","int");

	tableEntry entry=tableEntry("prog","void");

	table process=table();

	if (entry.addVarTable(variables))
		cout<<"Se añadio una entrada a una tabla de variables!"<<endl;

	if (entry.addVarTable(variables2))
			cout<<"Se añadio otra entrada a una tabla de variables!"<<endl;

	if (entry.searchVarTable("ax",tipo1))
			cout<<"Se encontro la variable!"<<endl;


	temp=process.tableAddEntry(entry);
	if (temp)
		cout<<"Se añadio una entrada a la tabla"<<endl;

	if (process.tableSearchEntry("prog",tipo2))
			cout<<"Se encontro la funcion en la tabla"<<endl;



	intStack.push(x);
	intStack.push(y);
	intStack.pop(x);

	intQueue.enqueue(20);
	intQueue.dequeue(x);

	map.put(1,'a');
	map.put(64,'b');
	if (map.search(64,z)==true)
	{
		cout<<"Lo encontro"<<endl;
	}



	cout << "Stack:  " <<x<< "  List:  "<<y <<" hash:"<<z<<endl; // prints !!!Hello World!!!


	return 0;



}
