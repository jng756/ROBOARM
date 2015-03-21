//============================================================================
// Name        : cuboSemantico.cpp
// Author      : Joel
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

int DecOp(string op);
int DecS(string signo);
string cubo(string op1, string op2, string symb);
// Op1, Op2, Simbolo
string cuboSemantico[5][5][10] = //verify
					//char:0		 + 0	>  6
					//string:1		 - 1	<= 7
					//int:2			 * 2	>= 8
					//float:3		 / 3	== 8
					//bool:4		 < 4	<> 9
{{ { "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "bool", "bool" },
					{ "ERROR","ERROR", "ERROR", "ERROR", "ERROR", "ERROR",	"ERROR", "ERROR", "ERROR", "ERROR" }, { "ERROR",
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
							"ERROR", "ERROR", "ERROR", "ERROR" }, { "ERROR",
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
							"ERROR", "ERROR", "ERROR", "ERROR" }, { "ERROR",
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
							"ERROR", "ERROR", "ERROR", "ERROR" } }, { {
							"string", "ERROR", "ERROR", "ERROR", "ERROR",
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR" }, {
							"string", "ERROR", "ERROR", "ERROR", "ERROR",
							"ERROR", "ERROR", "ERROR", "bool", "bool" }, {
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR" }, {
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR" }, {
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR" } }, { {
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR" }, {
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR" }, {
							"int", "int", "int", "int", "bool", "bool", "bool",
							"bool", "bool", "bool" }, { "float", "float",
							"float", "float", "bool", "bool", "bool", "bool",
							"bool", "bool" }, { "ERROR", "ERROR", "ERROR",
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
							"ERROR", "ERROR" } }, { { "ERROR", "ERROR", "ERROR",
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
							"ERROR", "ERROR" }, { "ERROR", "ERROR", "ERROR",
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
							"ERROR", "ERROR" }, { "float", "float", "float",
							"float", "bool", "bool", "bool", "bool", "bool",
							"bool" }, { "float", "float", "float", "float",
							"bool", "bool", "bool", "bool", "bool", "bool" }, {
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR" } }, { {
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR" }, {
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR" }, {
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR" }, {
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR" }, {
							"ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
							"ERROR", "ERROR", "ERROR", "bool", "bool" } } };



int main() {

	string op1, op2, simbolo;
	string repetir;

	do {
		cout << "Operando 1? : ";
		cin >> op1;
		cout << endl;

		cout << "Operando 2? : ";
		cin >> op2;
		cout << endl;

		cout << "Operador? : ";
		cin >> simbolo;
		cout << endl;
//Cubo(op1,op2,simbolo)

		cout << "El resultado es: " << cubo(op1,op2,simbolo) << endl;

		cout << "Otra vez? (S/N): ";
		cin >> repetir;
	} while (repetir == "S");


}

int DecOp(string op) {
	int num;

	if (op == "char")
		num = 0;
	else if (op == "string")
		num = 1;
	else if (op == "int")
		num = 2;
	else if (op == "float")
		num = 3;
	else if (op == "bool")
		num = 4;
	else
		num = 5;
	return num;
}
int DecS(string signo) {
	int num;

	if (signo == "+")
		num = 0;
	else if (signo == "-")
		num = 1;
	else if (signo == "*")
		num = 2;
	else if (signo == "/")
		num = 3;
	else if (signo == "<")
		num = 4;
	else if (signo == ">")
		num = 5;
	else if (signo == "<=")
		num = 6;
	else if (signo == ">=")
		num = 7;
	else if (signo == "==")
		num = 8;
	else if (signo == "<>")
		num = 9;
	else
		num = 10;
	return num;
}

string cubo(string op1, string op2, string symb)
{
	return cuboSemantico[DecOp(op1)][DecOp(op2)][DecS(symb)];
}


