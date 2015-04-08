/*
 * HashTable.h
 *
 *  Created on: Jan 29, 2015
 *      Author: joel
 */
#include <iostream>
const int TABLE_SIZE = 64;


template <class T>
class LinkedHashEntry {

private:
      int key;
      T value;
      LinkedHashEntry *next;
public:
      LinkedHashEntry(int key, T value) {
            this->key = key;
            this->value = value;
            this->next = NULL;
      }
      int getKey() {
            return key;
      }
      T getValue() {
            return value;
      }
      void setValue(T value) {
            this->value = value;
      }
     LinkedHashEntry *getNext() {
            return next;
      }
    void setNext(LinkedHashEntry *next) {
            this->next = next;
     }
};

template <class T>
class HashMap {

private:
      LinkedHashEntry<T> **table;

public:

      //Constructor: crea una tabla de hash como una tabla dinamica de listas enlazadas
      HashMap() {
    	  int i;
            table = new LinkedHashEntry<T>*[TABLE_SIZE];
            for (i = 0; i < TABLE_SIZE; i++)
                  table[i] = NULL;
      }

      //Regresa el valor buscado, recibe como entrada la llave del valor
      //Regresa true si encontro la llave
      bool search(int key, T &value) {
    	  	  	  bool status;
                  int hash = (key % TABLE_SIZE); //Función de hash: residuo del tamaño de tabla

                  if (table[hash] == NULL)
                        status=false;
                  else {
                        LinkedHashEntry<T> *entry = table[hash];
                        while (entry != NULL && entry->getKey() != key)
                              entry = entry->getNext();
                        if (entry == NULL)
                              status=false;
                        else
                              value=entry->getValue();
                        	status=true;
                  	  	 }
                  return status;
      }

      // Agrega un entrada a la tabla de hash
      void put(int key, T value) {
                  int hash = (key % TABLE_SIZE);
                  if (table[hash] == NULL)
                        table[hash] = new LinkedHashEntry<T>(key, value);
                  else {
                        LinkedHashEntry<T> *entry = table[hash];
                        while (entry->getNext() != NULL)
                              entry = entry->getNext();
                        if (entry->getKey() == key)
                              entry->setValue(value);
                       else
                              entry->setNext(new LinkedHashEntry<T>(key, value));
                  }
       }
      // Elimina una entrada de la tabla de hash, recibe como entrada la llave
      //Regresa true si la removio
      bool remove(int key) {
    	  	  	  bool status=false;
                  int hash = (key % TABLE_SIZE);
                  if (table[hash] != NULL) {
                        LinkedHashEntry<T> *prevEntry = NULL;
                        LinkedHashEntry<T> *entry = table[hash];
                        while (entry->getNext() != NULL && entry->getKey() != key) {
                              prevEntry = entry;
                              entry = entry->getNext();
                        }
                        if (entry->getKey() == key) {
                        	status=true; //Remover el nodo
                              if (prevEntry == NULL) {
                                   LinkedHashEntry<T> *nextEntry = entry->getNext();
                                   delete entry;
                                   table[hash] = nextEntry;

                              } else {
                                   LinkedHashEntry<T> *next = entry->getNext();
                                    delete entry;
                                   prevEntry->setNext(next);

                              }

                        }
                  }
                  return status;
            }

            ~HashMap() {
                  for (int i = 0; i < TABLE_SIZE; i++)
                        if (table[i] != NULL) {
                              LinkedHashEntry<T> *prevEntry = NULL;
                              LinkedHashEntry<T> *entry = table[i];
                              while (entry != NULL) {
                                   prevEntry = entry;
                                   entry = entry->getNext();
                                   delete prevEntry;
                              }
                        }
                  delete[] table;
            }


};

