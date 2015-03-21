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
      string tipoVar;
public:
      varEntry()
      {
    	  this->nombreVar = "";
    	  this->tipoVar = "";
      }
      varEntry(string nombreV, string tipoV) {
            this->nombreVar = nombreV;
            this->tipoVar = tipoV;
      }

	string getNombreVar() const {
		return nombreVar;
	}

	void setNombreVar(string nombreVar) {
		this->nombreVar = nombreVar;
	}

	string getTipoVar() const {
		return tipoVar;
	}

	void setTipoVar(string tipoVar) {
		this->tipoVar = tipoVar;
	}


};

class tableEntry {

private:
      string nombre;
      string tipo;
      HashMap<varEntry> *varTable;

public:

      tableEntry() {
                        this->nombre = "";
                        this->tipo = "";
                        HashMap<varEntry>* map= new HashMap<varEntry>;
                        this->varTable = map;
                  }

       ~tableEntry() {
      	}


      tableEntry(string nombreV, string tipoV) {
                  this->nombre = nombreV;
                  this->tipo = tipoV;
                  HashMap<varEntry>* map= new HashMap<varEntry>;
                  this->varTable = map;
            }

		string getNombre() {
			return nombre;
		}

		void setNombre(string nombre) {
			this->nombre = nombre;
		}

		string getTipo() {
			return tipo;
		}

		void setTipo(string tipo) {
			this->tipo = tipo;
		}

		HashMap<varEntry>* getVarTable() const {
			return varTable;
		}

		void setVarTable(HashMap<varEntry>* varTable) {
			this->varTable = varTable;
		}

		bool addVarTable(varEntry entry) {
			varEntry entrySearch;
			string nombreVar= entry.getNombreVar();
			int k=0;

			//HASH FUNCTION
			for (unsigned int i = 0; i < nombre.length(); i++)
			            k = k + int(nombreVar[k]);

			//Buscar la variable; si se encuentra cambiar la llave; probar con otra llave


			while(varTable->search(k,entrySearch))
			{
				//Esto quiere decir que si el que encontraste es el mismo esta repetida la variable
				if (entrySearch.getNombreVar()==entry.getNombreVar())
					return false;

				k+=64;
			}

			varTable->put(k,entry);

			//Si lo agrega a la tabla no hubo problema
			return true;

		}

		bool searchVarTable(string nombreVar, string &tipoVar) {
			varEntry entrySearch;
				//Buscar la llave
					int k=0;
					//HASH FUNCTION
					for (unsigned int i = 0; i < nombreVar.length(); i++)
					            k = k + int(nombreVar[k]);

				//Buscar la variable; si coincide con el tipo regresar bool
				//Ademas regresar el tipo de la variable


					while(varTable->search(k,entrySearch))
					{
						//si el nombre de la Variable es igual al valor encontrado entonces si esta; sino esta hay que checar con la siguiente llave
						if (entrySearch.getNombreVar()==nombreVar)
							tipoVar=entrySearch.getTipoVar();
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
	HashMap<tableEntry> *map;
public:

	table()
	{
	this->map = new HashMap<tableEntry>;
	}

	~table() {
		delete map;
	}


	HashMap<tableEntry>* getMap() const {
		return map;
	}

	void setMap(HashMap<tableEntry>* map) {
		this->map = map;
	}

	bool tableAddEntry(tableEntry entry) {
				tableEntry entrySearch;
				string nombre= entry.getNombre();
				int k=0;

				//HASH FUNCTION
				for (unsigned int i = 0; i < nombre.length(); i++)
				            k = k + int(nombre[k]);

				//Buscar la variable; si se encuentra cambiar la llave; probar con otra llave


				while(map->search(k,entrySearch))
				{
					//Esto quiere decir que si el que encontraste es el mismo entonces esta repetida la variable
					if (entrySearch.getNombre()==entry.getNombre())
						return false;

					k+=64;
				}

				map->put(k,entry);

				//Si lo agrega a la tabla no hubo problema
				return true;

			}

	bool tableSearchEntry(string nombre, string &tipoFunc)
	 {
			tableEntry entrySearch;
					//Buscar la llave
						int k=0;
						//HASH FUNCTION
						for (unsigned int i = 0; i < nombre.length(); i++)
						            k = k + int(nombre[k]);

					//Buscar la variable; si coincide con el tipo regresar bool
					//Ademas regresar el tipo de la variable


						while(map->search(k,entrySearch))
						{
							//si el nombre de la Variable es igual al valor encontrado entonces si esta; sino esta hay que checar con la siguiente llave
							if (entrySearch.getNombre()==nombre)
								tipoFunc=entrySearch.getTipo();
								return true;

							k+=64;
						}
						//Si la busqueda falla no esta la variable entonces
						//Declarar error variable no declarada
						return false;
		}


};

#endif /* TABLE_H_ */
