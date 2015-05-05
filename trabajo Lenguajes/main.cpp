#include "maqVirtual.h"

int main()
{
    ifstream archivoEntrada;
    archivoEntrada.open("output.file");
    maqVirtual(archivoEntrada);
    return 0;
}
