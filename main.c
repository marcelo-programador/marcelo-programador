#include <stdio.h>
#include "processador.h"

int main()
{
    const char *entrada_binario = "Entrada.txt";
    const char *saida_binario = "Saida.txt";
    int PC = 0;

    lista_entradas lista;
    lista_saidas lista_output;

    inicializar(&lista, &lista_output, contar_instrucoes(entrada_binario));
    ler_arquivo_salvar_na_lista(&lista, entrada_binario);
    assembly_instruction_type_form(&lista);
    definir_sinais_assembly(&lista);
    alteracao_registrador_assembly(&lista);
    operacoes_registradores_assembly(&lista);
    alteracao_pc_assembly(&lista, &PC);
    exibir_lista(&lista);
    preencher_lista_out(&lista, &lista_output);
    escreverSaidasEmArquivo(&lista_output, saida_binario);
    finalizar(&lista, &lista_output);

    return 0;
}
