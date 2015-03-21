/*
 * queue
 *
 *  Created on: Jan 27, 2015
 *      Author: joel
 */

template <class T>
class Queue
{
private:
//Estrcutura del nodo
struct qNode
{
	T data;        //Datos del nodo
	qNode *next;   //Apuntador
};

qNode *front;   //Apuntador para el frente de la lista
qNode *back;    //Apuntador al nodo de átras de la lista

int queueNum;   //Llevar la cuenta de los nodos de la lista

public:
//Consctructor de la lista
Queue() {
	front = NULL;
	back = NULL;
	queueNum = 0;
}

//Destructor de la lista
~Queue() {
	while (!isEmpty()) {
		dequeue (front->data);
	}
}

//Agregar a la lista un valor
void enqueue(T val) {
	qNode *newNode;

	//Crear un nodo para agregar el valor
	newNode = new qNode;
	newNode->data = val;
	newNode->next = NULL;

	if (isEmpty()) {
		front = newNode;
		back = newNode;
	} else {
		back->next = newNode;
		back = newNode;
	}

	queueNum++;
}

//Regresa el valor al frente de la lista y remueve el valor
//Regresa true si removio el valor de la lista
bool dequeue(T &val) {
	qNode *temp;
	bool status;
	if (isEmpty()) {
		status=false;
	} else {
		//guardar el nodo que esta el frente en la variable por referencia
		val = front->data;
		//Remove front node, delete
		temp = front;
		front = front->next;
		delete temp;


		queueNum--;
		status = true;
	}
	return status;
}

//Regresa true si esta vacio
bool isEmpty() const {
	bool empty;
	if (queueNum > 0)
		empty = false;
	else
		empty = true;
	return empty;
}

};




