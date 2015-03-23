/*
 * Table.h
 *
 *  Created on: Mar 18, 2015
 *      Author: joel
 */

#ifndef TABLE_H_
#define TABLE_H_


#include <iostream>
#include <string>
#include <HashTable.h>
using namespace std;

class varEntry {

private:
      string nombreVar;
      int dirVirtual;
public:
      varEntry()
      {
    	  this->nombreVar = "";
    	  this->dirVirtual= 0;
      }
      varEntry(string nombreV, int dirVirtual) {
            this->nombreVar = nombreV;
            this->dirVirtual = dirVirtual;
      }

	string getNombreVar() const {
		return nombreVar;
	}

	void setNombreVar(string nombreVar) {
		this->nombreVar = nombreVar;
	}

	int getDirVirtual() {
		return dirVirtual;
	}

	void setDirVirtual(int dirVirtual) {
		this->dirVirtual = dirVirtual;

	}
};

class tableEntry {

private:
      string nombre;
      int dirVirtual;
      HashMap<varEntry> *varTable;

public:

      tableEntry() {
                        this->nombre = "";
                        this->dirVirtual = 0;
                        HashMap<varEntry>* map= new HashMap<varEntry>;
                        this->varTable = map;
                  }

       ~tableEntry() {
      	}


      tableEntry(string nombreV, int dirVirtual) {
                  this->nombre = nombreV;
                  this->dirVirtual = dirVirtual;
                  HashMap<varEntry>* map= new HashMap<varEntry>;
                  this->varTable = map;
            }

		string getNombre() {
			return nombre;
		}

		void setNombre(string nombre) {
			this->nombre = nombre;
		}

		int getDirVirtual() const {
			return dirVirtual;
		}

		void setDirVirtual(int dirVirtual) {
			this->dirVirtual = dirVirtual;
		}

		HashMap<varEntry>* getVarTable() const {
			return varTable;
		}

		void setVarTable(HashMap<varEntry>* varTable) {
			this->varTable = varTable;
		}

		bool addVarTable(varEntry entry) {
			// Para buscar la entrada por si ya estaba...
			varEntry entrySearch;

			string nombreVar= entry.getNombreVar();
			int k=0;  //La funcion de hash es la suma de los caracteres del string
			//HASH FUNCTION
			for (unsigned int i = 0; i < nombre.length(); i++)
			            k = k + int(nombreVar[k]);

			//Buscar la variable; si ya tiene esta ocupada esa llave hay que encadenarla a esa misma sumando el tamaño de la tabla
			while(varTable->search(k,entrySearch))
			{
				//el nombre de la variable ya estaba? entonces salir y regresar falso que no se pudo;

				if (entrySearch.getNombreVar()==entry.getNombreVar())
					return false;

				k+=TABLE_SIZE;
			}

			varTable->put(k,entry);

			//Si lo agrega a la tabla no hubo problema
			return true;

		}

		bool searchVarTable(string nombreVar, int &dirVirtual) {
			varEntry entrySearch;
				//Buscar la llave
					int k=0;
					//HASH FUNCTION
					for (unsigned int i = 0; i < nombreVar.length(); i++)
					            k = k + int(nombreVar[k]);

				//Ademas regresar la direccion virtual de la variable


					while(varTable->search(k,entrySearch))
					{
						//si el nombre de la Variable es igual al valor encontrado entonces si esta; sino esta hay que checar con la siguiente llave
						if (entrySearch.getNombreVar()==nombreVar)
							dirVirtual=entrySearch.getDirVirtual();
							return true;
						k+=64;
					}
					//Si la busqueda falla no esta la variable entonces
					//Declarar error variable no declarada
					return false;
	}
};

class table {
private:
    string nombre;
    HashMap<varEntry> *varTableGlobal;
	HashMap<tableEntry> *map;
public:

	table()
	{
		this->nombre = "";
		HashMap<varEntry>* map= new HashMap<varEntry>;
		this->varTableGlobal = map;
		this->map = new HashMap<tableEntry>;
	}

	table (string nombre)
	{
		this->nombre = nombre;
		HashMap<varEntry>* map= new HashMap<varEntry>;
		this->varTableGlobal = map;
		this->map=new HashMap<tableEntry>;
	}
	~table() {
		delete map;
		delete varTableGlobal;
	}

	string getNombre() const {
		return nombre;
	}

	void setNombre(string nombre) {
		this->nombre = nombre;
	}

	HashMap<varEntry>* getVarTableGlobal() const {
		return varTableGlobal;
	}

	void setVarTableGlobal(HashMap<varEntry>* varTableGlobal) {
		this->varTableGlobal = varTableGlobal;
	}

	HashMap<tableEntry>* getMap() const {
		return map;
	}

	void setMap(HashMap<tableEntry>* map) {
		this->map = map;
	}

	bool addGlobalVarTable(varEntry entry) {
				// Para buscar la entrada por si ya estaba...
				varEntry entrySearch;

				string nombreVar= entry.getNombreVar();
				int k=0;  //La funcion de hash es la suma de los caracteres del string
				//HASH FUNCTION
				for (unsigned int i = 0; i < nombre.length(); i++)
				            k = k + int(nombreVar[k]);

				//Buscar la variable; si ya tiene esta ocupada esa llave hay que encadenarla a esa misma sumando el tamaño de la tabla
				while(varTableGlobal->search(k,entrySearch))
				{
					//el nombre de la variable ya estaba? entonces salir y regresar falso que no se pudo;
					if (entrySearch.getNombreVar()==entry.getNombreVar())
						return false;

					k+=TABLE_SIZE;
				}

				varTableGlobal->put(k,entry);

				//Si lo agrega a la tabla no hubo problema
				return true;

			}

	bool searchVarGlobalTable(string nombreVar, int &dirVirtual) {
				varEntry entrySearch;
					//Buscar la llave
						int k=0;
						//HASH FUNCTION
						for (unsigned int i = 0; i < nombreVar.length(); i++)
						            k = k + int(nombreVar[k]);

					//Ademas regresar la direccion virtual de la variable


						while(varTableGlobal->search(k,entrySearch))
						{
							//si el nombre de la Variable es igual al valor encontrado entonces si esta; sino esta hay que checar con la siguiente llave
							if (entrySearch.getNombreVar()==nombreVar)
								dirVirtual=entrySearch.getDirVirtual();
								return true;
							k+=64;
						}
						//Si la busqueda falla no esta la variable entonces
						//Declarar error variable no declarada
						return false;
		}

	bool tableAddEntry(tableEntry entry) {
				tableEntry entrySearch;
				string nombre= entry.getNombre();
				int k=0;

				//HASH FUNCTION
				for (unsigned int i = 0; i < nombre.length(); i++)
				{
					k = k + int(nombre[k]);
				}

				//Buscar la variable; si se encuentra cambiar la llave; probar con otra llave
				while(map->search(k,entrySearch))
				{
					//Esto quiere decir que si el que encontraste es el mismo entonces esta repetida la variable
					if (entrySearch.getNombre()==entry.getNombre())
						return false;

					k+=TABLE_SIZE;
				}

				map->put(k,entry);

				//Si lo agrega a la tabla no hubo problema
				return true;

			}

	bool tableSearchEntry(string nombre, int &dirVirtual)
	 {
			tableEntry entrySearch;
					//Buscar la llave
						int k=0;
						//HASH FUNCTION
						for (unsigned int i = 0; i < nombre.length(); i++)
						            k = k + int(nombre[k]);

					//Regresar el direccion virtual de la variable


						while(map->search(k,entrySearch))
						{
							//si el nombre de la Variable es igual al valor encontrado entonces si esta; sino esta hay que checar con la siguiente llave
							if (entrySearch.getNombre()==nombre)
								dirVirtual=entrySearch.getDirVirtual();
								return true;

							k+=64;
						}
						//Si la busqueda falla no esta la variable entonces
						//Declarar error variable no declarada
						return false;
		}


};

#endif /* TABLE_H_ */
