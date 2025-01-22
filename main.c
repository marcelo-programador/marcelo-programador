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
    definir_sinais_assembly(&list);
    alteracao_registrador_assembly(&list);
    operacoes_registradores_assembly(&list);
    alteracao_pc_assembly(&list, &PC);
    exibir_lista(&list);
    finalizar(&list);

    return 0;
}
