#include <stdio.h>
#include "processador.h"

int main()
{
    const char *entrada_binario = "Entrada.txt";
    int PC = 0;

    lista_entradas list;
    inicializar(&list, contar_instrucoes(entrada_binario));
    ler_arquivo_salvar_na_lista(&list, entrada_binario);
    assembly_instruction_type_form(&list);
    exibir_lista(&list);
    finalizar(&list);

    return 0;
}
