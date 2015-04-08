/*
 *
 *
 */

template <class T>
class Stack
{
private:
   // Estructura de cada nodo del stack
   struct StackNode
   {
      T value;
      StackNode *next;  // Apuntador del siguiente nodo en el stack
   };

   StackNode *top;      // Apuntador al ultimo nodo que se ha insertado

public:



   //Constructor: Crea un nodo apuntando a null
   Stack() {
	   top = NULL;
   }
   //Destructor
   ~Stack()
   {
      StackNode *nodePtr, *nextNode;
      //Apunta al tope del stack
      nodePtr = top;

      // Navega por el stack eliminando cada nodo
      while (nodePtr != 0)
      {
         nextNode = nodePtr->next;
         delete nodePtr;
         nodePtr = nextNode;
      }
   }


   // Agrega un item al stack
   void push(T item)
   {
      StackNode *newNode; // Apunta a un nuevo nodo

      // Guarda en un nodo el item
      newNode = new StackNode;
      newNode->value = item;

      // Si no hay nodo en el stack crear uno
      if (isEmpty())
      {
         top = newNode;
         newNode->next = 0;
      }
      else  // Si no entonces insertar el nodo en el tope
      {
         newNode->next = top;
         top = newNode;
      }
   }

   //Regresa el valor del tope y lo elimina del stack.
   //Regresa true si lo removio
   bool pop( T &item )
   {
	   bool status;
      StackNode *temp; // Apuntador temporal

      // Asegurarse que no hay nodo vacios
      if (isEmpty())
      {
         status=false;
      }
      else  // Sacar el valor de la pila
      {
         item = top->value;
         temp = top->next;
         delete top;
         top = temp;
         status = true;
      }
      return status;
   }
   //Regresa un valor Bool: true si esta vacio el stack
   bool isEmpty()
   {
      bool status;

      if (!top)
         status = true;
      else
         status = false;

      return status;
   }


};

