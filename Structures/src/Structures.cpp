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

	Stack<int> intStack;
	Queue<int> intQueue;
	HashMap<char> map;

	int dirV1=0;
	int dirV2=0;
	int dirV3=0;

	varEntry variables=varEntry("xa",5);
	varEntry variables2=varEntry("ax",12);

	tableEntry entry=tableEntry("sumar",23);
	table pragma= table("programa");

	//Pruebas de funcion Hash
	string texto="A";
	int k=0;
	//HASH FUNCTION
	for (unsigned int i = 0; i < texto.length(); i++)
				            k = k + int(texto[k]);
	cout<<"Hash de texto:"<<k<<endl;

	//Termina Prueba de Hash

	if (entry.addVarTable(variables))
		cout<<"variable Local! "<<endl;

	if (entry.searchVarTable("xa",dirV1))
			cout<<"Variable Local: "<<dirV1<<endl;

	if (pragma.addGlobalVarTable(variables2))
			cout<<"Variable Global!"<<endl;

	if (pragma.searchVarGlobalTable("ax",dirV2))
				cout<<"Variable Global: "<<dirV2<<endl;

	if (pragma.tableAddEntry(entry))
		cout<<"Funcion!"<<endl;

	if (pragma.tableSearchEntry("sumar",dirV3))
			cout<<"Funcion sumar:"<<dirV3<<endl;




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
