
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>

using namespace std;
void maqVirtual(ifstream &archivoCuadruplos);
void istabmode();  //Solo para denotar lineas de indentacion
void checktemp(string &word);
void InsertHeader(ofstream &archivoSalida);
void ChecarComillas(string &word, ifstream &archivoCuadruplos);

bool funcmode = false;
ofstream archivoSalida;
string word1,word2,word3,word4;
string auxword;

vector<string> t;  //Temporales t[1],t2,t3,...

vector<int> oper; //Para definir jerarquia de operaciones :   *,/ ~ 1  ;   +,- ~ 2   ;  >,<,>=,<=,== ~ 3
int numVector;

int cont_tab = 0;
int caseMode = false; // POR IMPLEMENTAR (EXTRAOPTION): Sirve para implementar el extraoption con la identacion en instrucciones.

void maqVirtual(ifstream &archivoCuadruplos)
{
    t.push_back("0");  // Es solo para que el vector empieze desde 1 (t1,t2,...)
    oper.push_back(0);

    bool param1 = true;
    //bool error = false;
    int caseState = 0; //0=No esta en case, 1=PrimerOpcion, 2=OtraOpcion... (solo para ayudar a insertar estructura)
    bool firstPrint = true;

    archivoSalida.open("BBCode.js");

    //Solo un comando de inicio
    archivoSalida << "var b = require('bonescript');" << endl << endl;

    InsertHeader(archivoSalida);

    while (archivoCuadruplos >> word1)
    {
        //cout<< word1 << '\n';
        //error = false;

        if (word1 == "VAR")
        {
            archivoCuadruplos >> word2 >> word3 >> word4;

            istabmode();
            archivoSalida << "var " + word2 + ";" << endl;
        }
        if (word1 == "FUNCTION" || funcmode == true)
        {
            if (funcmode == false)
            {
                param1 = true;
                funcmode = true;

                archivoCuadruplos >> word2 >> word3;

                archivoSalida << endl << "function " + word2 + "(";
            }
            else if (word1 == "PASS")
            {
                archivoCuadruplos >> word2 >> word3;
                //cout << word2 <<"   "<< word3 << endl;

                if(param1 == false)  //Solo para poner coma entre parametros de funcion
                {
                    archivoSalida << ", " + word2;
                }
                else
                {
                    archivoSalida << word2;
                    param1 = false;
                }
            }
        }
        if (word1 == "ENDFUNCTION")
        {
            archivoSalida << ") {" << endl;
            cont_tab += 1;
        }
        if (word1 == "+")
        {
            archivoCuadruplos >> word2;
            ChecarComillas(word2, archivoCuadruplos);
            checktemp(word2);  //Verifica si existen temporales para sacar dato de vector

            archivoCuadruplos >> word3;
            ChecarComillas(word3, archivoCuadruplos);
            checktemp(word3);
            //checktemp(word4);

            archivoCuadruplos >> word4;

            t.push_back(word2 + " + " + word3);  // Mete operacion en vector
            oper.push_back(2);  // Mete prioridad de operacion en su vector (posicion igual al de temporales)
            //cout<< "Dato: " + t[t.size()-1] << endl;

            //isfuncmode();
            //archivoSalida << word4 + " = " + t[t.size()-1] + ";"<< endl;
        }
        if (word1 == "-")
        {
            archivoCuadruplos >> word2;
            ChecarComillas(word2, archivoCuadruplos);
            checktemp(word2);

            archivoCuadruplos >> word3;
            ChecarComillas(word3, archivoCuadruplos);
            checktemp(word3);

            archivoCuadruplos >> word4;

            //checktemp(word4);

            t.push_back(word2 + " - " + word3);
            oper.push_back(2);
            //cout<< "Dato: " + t[t.size()-1] << endl;

            //isfuncmode();
            //archivoSalida << word4 + " = " + t[t.size()-1] + ";" << endl;
        }
        if (word1 == "*") //Priority 1
        {
            archivoCuadruplos >> word2;
            ChecarComillas(word2, archivoCuadruplos);
            checktemp(word2);

            if( oper[numVector]> 1 ) //Verifica si word2 tiene un temporal con operacion de menor prioridad, si es asi le pone parentesis
            {
                word2 = "(" + word2 + ")";
            }

            archivoCuadruplos >> word3;
            ChecarComillas(word3, archivoCuadruplos);
            checktemp(word3);

            if( oper[numVector]> 1 )
            {
                word3 = "(" + word3 + ")";
            }

            archivoCuadruplos >> word4;

            t.push_back(word2 + " * " + word3);
            oper.push_back(1);
            //cout<< "Dato: " + t[t.size()-1] << endl;

            //isfuncmode();
            //archivoSalida << word4 + " = " + t[t.size()-1] + ";" << endl;
        }
        if (word1 == "/")
        {
            archivoCuadruplos >> word2;
            ChecarComillas(word2, archivoCuadruplos);
            checktemp(word2);

            if( oper[numVector]> 1 ) //Verifica si word2 tiene un temporal con operacion de menor prioridad, si es asi le pone parentesis
            {
                word2 = "(" + word2 + ")";
            }

            archivoCuadruplos >> word3;
            ChecarComillas(word3, archivoCuadruplos);
            checktemp(word3);

            if( oper[numVector]> 1 )
            {
                word3 = "(" + word3 + ")";
            }

            archivoCuadruplos >> word4;
            //checktemp(word4);

            t.push_back(word2 + " / " + word3);
            oper.push_back(1);
            //cout<< "Dato: " + t[t.size()-1] << endl;

            //isfuncmode();
            //archivoSalida << word4 + " = " + t[t.size()-1] + ";" << endl;
        }
        if (word1 == "=")
        {
            archivoCuadruplos >> word2;
            ChecarComillas(word2, archivoCuadruplos);
            checktemp(word2);

            archivoCuadruplos >> word3;
            ChecarComillas(word3, archivoCuadruplos);
            checktemp(word3);

            istabmode();
            archivoSalida << word3 + " = " + word2 + ";"<< endl;  //Cambiado de lugar
        }
        if (word1 == "==")
        {
            archivoCuadruplos >> word2;
            ChecarComillas(word2, archivoCuadruplos);
            checktemp(word2);

            archivoCuadruplos >> word3;
            ChecarComillas(word3, archivoCuadruplos);
            checktemp(word3);

            t.push_back(word2 + " == " + word3);
            oper.push_back(3);

            //istabmode();
            //archivoSalida << word2 + " == " + word3 << endl;

        }
        if (word1 == "!=")
        {
            archivoCuadruplos >> word2;
            ChecarComillas(word2, archivoCuadruplos);
            checktemp(word2);

            archivoCuadruplos >> word3;
            ChecarComillas(word3, archivoCuadruplos);
            checktemp(word3);

            t.push_back(word2 + " != " + word3);
            oper.push_back(3);

            //istabmode();
            //archivoSalida << word2 + " == " + word3 << endl;
        }
        if (word1 == "<")
        {
            archivoCuadruplos >> word2 >> word3 >> word4;
            checktemp(word2);
            checktemp(word3);

            t.push_back(word2 + " < " + word3);
            oper.push_back(3);

            //istabmode();
            //archivoSalida << word2 + " == " + word3 << endl;
        }
        if (word1 == ">")
        {
            archivoCuadruplos >> word2 >> word3 >> word4;
            checktemp(word2);
            checktemp(word3);

            t.push_back(word2 + " > " + word3);
            oper.push_back(3);

            //istabmode();
            //archivoSalida << word2 + " == " + word3 << endl;
        }
        if (word1 == ">=")
        {
            archivoCuadruplos >> word2 >> word3 >> word4;
            checktemp(word2);
            checktemp(word3);

            t.push_back(word2 + " >= " + word3);
            oper.push_back(3);

            //istabmode();
            //archivoSalida << word2 + " == " + word3 << endl;
        }
        if (word1 == "<=")
        {
            archivoCuadruplos >> word2 >> word3 >> word4;
            checktemp(word2);
            checktemp(word3);

            t.push_back(word2 + " <= " + word3);
            oper.push_back(3);

            //istabmode();
            //archivoSalida << word2 + " == " + word3 << endl;
        }
        if (word1 == "IF")
        {
            archivoCuadruplos >> word2;
            checktemp(word2);

            istabmode();
            archivoSalida << "if (" + word2 + ")" << endl;

            istabmode();
            archivoSalida << "{" << endl;

            cont_tab += 1;

        }
        if (word1 == "ELSE")
        {
            istabmode();
            archivoSalida << "else" << endl;
            istabmode();
            archivoSalida << "{" << endl;
            cont_tab += 1;
        }
        if (word1 == "WHILE")
        {
            archivoCuadruplos >> word2;
            checktemp(word2);

            istabmode();
            archivoSalida << "while (" + word2 + ")" << endl;

            istabmode();
            archivoSalida << "{" << endl;

            cont_tab += 1;
        }
        if (word1 == "CASE")
        {
            archivoCuadruplos >> word2;
            checktemp(word2);

            istabmode();
            archivoSalida << "switch (" + word2 + ") {" << endl;

            cont_tab += 1;
            caseState = 1; //Definir estructura primera opcion
        }
        if (word1 == "OPTION")
        {
            archivoCuadruplos >> word2;
            checktemp(word2);

            if(caseState == 1)
            {
                istabmode();
                archivoSalida << "case " + word2 + ":" << endl;

                cont_tab += 1;
                caseState = 2;
            }
            else  //Si no es el primer option, se ocupa poner el break; del caso anterior
            {
                istabmode();
                archivoSalida << "break;" << endl;

                cont_tab -= 1;
                istabmode();
                archivoSalida << "case " + word2 + ":" << endl;

                cont_tab += 1;
            }
        }
        if (word1 == "EXTRAOPTION")
        {
            archivoCuadruplos >> word2;
            checktemp(word2);

            cont_tab -= 1;
            istabmode();
            archivoSalida << "case " + word2 + ":" << endl;

            cont_tab += 1;

        }
        if(word1 == "ENDCASE")
        {
            istabmode();
            archivoSalida << "break;" << endl;

            caseState = 0;
            cont_tab -= 2;

            istabmode();
            archivoSalida << "}" << endl;
        }

        if (word1 == "ENDIF" || word1 == "ENDELSE" || word1 == "ENDWHILE")
        {
            cont_tab -= 1;
            istabmode();
            archivoSalida << "}" << endl;
        }
        if (word1 == "RETURN")
        {
            archivoCuadruplos >> word2;

            istabmode();
            archivoSalida << "return " + word2 + ";" << endl;

            cont_tab -= 1;
            istabmode();
            archivoSalida << "}" << endl;

            funcmode = false;
        }
        if (word1 == "RETVOID")
        {
            cont_tab -= 1;
            istabmode();
            archivoSalida << "}" << endl;

            funcmode = false;
        }
        if (word1 == "PRINT")
        {
            archivoCuadruplos >> word2;

            ChecarComillas(word2, archivoCuadruplos);

//            if(word2.substr(0,1) == "\"")  //Verifica si es un string con las 1eras "
//            {
//                //cout << word2.substr(1,word2.length()-1) << endl;
//
//                if (word2.substr(1,word2.length()-1).find("\"") == string::npos) //Checa si hay o no las 2das " en la palabra
//                {
////                    cout << "Falta agregar \" " << endl;
////
////                    word2 = word2 + " \"" ; //Se agregan las dobles comillas que faltaban (por un espacio)
//
//                    do      // Si faltan las " finales, ocupa tomar palabras hasta que la encuentre.
//                    {
//                        archivoCuadruplos >> word3;
//                        checktemp(word3);
//
//                        word2 = word2 + " " + word3;
//
//                    }while(word3.find("\"") == string::npos);
//                }
//            }

            checktemp(word2);

            if( oper[numVector] > 1 )
            {
                word3 = "(" + word2 + ")";
            }
//            istabmode();
//            archivoSalida << "console.log(" + word2 + ");" << endl;

            if(firstPrint) // Necesario para que no ponga un + al inicio.
            {
                auxword += word2;
                firstPrint = false;
            }
            else
            {
                auxword += " + " + word2;
            }
        }
        if (word1 == "ENDL")  //Sirve para mandar varios print en una misma linea.
        {
            istabmode();
            archivoSalida << "console.log(" + auxword + ");" << endl ;

            auxword = "";
            firstPrint = true;
        }
        if (word1 == "BEGIN")
        {
            if(funcmode == true)
            {
                archivoSalida << endl ;
                funcmode = false;
            }
        }
        if (word1 == "END")
        {
            //No necesita hacer nada??
        }
        if (word1 == "RWAIST")
        {
            archivoCuadruplos >> word2;
            checktemp(word2);

            istabmode();
            archivoSalida << "rotateWaist(" + word2 + ");" << endl;
        }
        if (word1 == "RSHOULDER")
        {
            archivoCuadruplos >> word2;
            checktemp(word2);

            istabmode();
            archivoSalida << "rotateShoulder(" + word2 + ");" << endl;
        }
        if (word1 == "RELBOW")
        {
            archivoCuadruplos >> word2;
            checktemp(word2);

            istabmode();
            archivoSalida << "rotateElbow(" + word2 + ");" << endl;
        }
        if (word1 == "RWRISTM")
        {
            archivoCuadruplos >> word2;
            checktemp(word2);

            istabmode();
            archivoSalida << "rotateWristM(" + word2 + ");" << endl;
        }
        if (word1 == "RWRISTR")
        {
            archivoCuadruplos >> word2;
            checktemp(word2);

            istabmode();
            archivoSalida << "rotateWristR(" + word2 + ");" << endl;
        }
        if (word1 == "RTOOL")
        {
            archivoCuadruplos >> word2;
            checktemp(word2);

            istabmode();
            archivoSalida << "rotateTool(" + word2 + ");" << endl;
        }
        if (word1 == "SLEEP")
        {
            archivoCuadruplos >> word2;
            checktemp(word2);

            istabmode();
            archivoSalida << "sleep(" + word2 + ");" << endl;
        }
        if (word1 == "READ")    //Requiere implementacion de headers
        {
            archivoCuadruplos >> word2;
            checktemp(word2);

            istabmode();
            //archivoSalida << "read(" + word2 + ");" << endl;

            if(word2 == "b1")
            {
                archivoSalida << "b1 = b.digitalRead(PB_1);" << endl;
            }
            else if(word2 == "b2")
            {
                archivoSalida << "b2 = b.digitalRead(PB_2);" << endl;
            }
            else if(word2 == "b3")
            {
                archivoSalida << "b3 = b.digitalRead(PB_3);" << endl;
            }
            else
            {
                archivoSalida << word2 + "= b.digitalRead(" + word2 + ");" << endl;
                cout << "Error: Invalid Variable {"  + word2 + "}" << endl;
            }
//            if(word2 != "b1" && word2 != "b2" && word2 != "b3")
//            {
//                cout << "Error: Invalid Variable {"  + word2 + "}" << endl;
//            }
        }
        if (word1 == "CALL")
        {
            archivoCuadruplos >> word2;
            checktemp(word2);

            istabmode();
            archivoSalida << word2 + ";" << endl;
        }

    }

    archivoCuadruplos.close();
    archivoSalida.close();

//    for (int i=1; i < t.size() ; i++)
//    {
//        cout<< t[i] << endl;
//    }

}
void istabmode()
{
    int cont = cont_tab;
    if(caseMode)
    {
        cont+=1;
    }

    while(cont > 0)   //Solo para denotar lineas de funcion
    {
        archivoSalida << "  ";
        cont-=1;
    }

//    if (funcmode == true)   //Solo para denotar lineas de funcion
//    {
//        archivoSalida << "  ";
//    }
}
void checktemp(string &word)
{
    numVector = 0;   //Se utiliza para obtener la maxima prioridad (cuando no se compara un temporal con operador).

    while (word.find("t[") != string::npos)  //Mientras encuentre temporales en la palabra del cuadruplo
    {
        //cout<< "Encontre un temporal en word" << endl;

        //Obtiene el numero dentro de la palabra del cuadruplo
        unsigned inicio = word.find("[");
        unsigned fin = word.find("]");

        string VectorIndex = word.substr (inicio+1,fin-inicio-1);
        numVector = atoi(VectorIndex.c_str());

        //numero = atoi(num.c_str());
        //cout << atoi(strNew.c_str()) << endl;

        //Ahora crea la nueva palabra haciendo referencia al vector

        string parte1 = word.substr (0,inicio-1);
        string parte2 = word.substr (fin+1, word.length()-1);
        //cout << parte1 + t[numVector] + parte2 << endl;

        word = parte1 + t[numVector] + parte2;
    }

}
void InsertHeader(ofstream &archivoSalida)
{
    ifstream headerInfo;
    headerInfo.open("header.file");

    archivoSalida << headerInfo.rdbuf();

    archivoSalida << endl ;
    archivoSalida << "///////////// MAIN CODE ///////////////" << endl << endl;

    headerInfo.close();
}
void ChecarComillas(string &word,ifstream &archivoCuadruplos)
{
    string tempword;

    if(word.substr(0,1) == "\"")  //Verifica si es un string con las 1eras "
    {
        //cout << word.substr(1,word.length()-1) << endl;

        if (word.substr(1,word.length()-1).find("\"") == string::npos) //Checa si hay o no las 2das " en la palabra
        {
//          cout << "Falta agregar \" " << endl;
//
//          word = word + " \"" ; //Se agregan las dobles comillas que faltaban (por un espacio)

            do      // Si faltan las " finales, ocupa tomar palabras hasta que la encuentre.
            {
                archivoCuadruplos >> tempword;
                checktemp(tempword);

                word = word + " " + tempword;

            }while(tempword.find("\"") == string::npos);
        }
    }
}
