/*Estrutura de dados que simula um processador*/
#ifndef PROCESSADOR_H
#define PROCESSADOR_H
#define N_REGISTRADORES 32
#include <stdint.h>
#include <stdbool.h>

typedef struct instruction
{
    char cod_assembly[33];
    char cod_binario[33];
    char formato_tipo;
    char instrucao_tipo[5];
    int rs, rt, rd, const_or_address;
    char ALUOp[3];
    char RegDst, ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, Branch;
} instructions;

typedef struct instruction_bin
{
    char cod_binario[33];
    char formato_tipo;
    char instrucao_tipo[5];
    int rs, rt, rd, const_or_address;

} instructions_bin;

typedef struct ent
{
    instructions *entradas;
    int quantidade, capacidade;
} lista_entradas;

typedef struct sai
{
    instructions_bin *saidas;
    int quantidade, capacidade;
} lista_saidas;

extern uint32_t registradores[N_REGISTRADORES];

/*
Função responsável por chamar todas as outras funções presentes no nosso simulador de processador monociclo MIPS
entrada: não tem
saída: não tem
*/
void Controlador_Principal_Assembly();

/*
Função responsável por inicializar os registradores, que nada mais é que um vetor de inteiros onde os valores salvos nos registradores são salvos em seus índices equivalentes
entrada: não tem
saída: não tem
*/
void inicializar_registradores();

/*
Função responsável por inicializar as listas onde são salvas as instruções no formato de Assembly e Bin 32 bits com todas as suas caracteristicas
entrada: referência para lista de entrada e, referência para lista de saída e, capacidad e das listas cap
saída: não tem
*/
void inicializar(lista_entradas *e, lista_saidas *s, int cap);

/*
Função responsável por finalizar as listas, ou seja, liberar o endereço de memória que era ocupado por cada lista durante a execução do programa para evitar vazamentos de memória indesejados
Entrada: referência para lista de entrada e, referência para lista de saída s
saída: não tem
*/
void finalizar(lista_entradas *l, lista_saidas *s);

/*
Função responsável por inserir as instruções presentes no arquivo Entrada.txt na minha lista de entradas l
entrada: referência para lista de entrada l, referência para lista de saída s
saída: não tem   
*/
void inserir_entrada(lista_entradas *l, instructions i);

/*
Função responsável por exibir a lista de entrada e todas suas particularidades, como por exemplo, o cod assembly, cod binário, tipo de instrução...
entrada: referência para lista de entrada l
saída: não tem
*/
void exibir_lista(lista_entradas *l);

/*
instrução responsável por organizar a lista de instruções OBS de ar
*/
void organizar_instrucoes(const char *nome_arquivo, lista_entradas *l);

/*

*/
void controle_principal(lista_entradas *l, int *PC);
/*

*/
bool definir_sinais_bin(lista_entradas *l);
/*

*/
void alteracao_pc(lista_entradas *l, int i, int *PC);
/*

*/
bool alteracao_registrador(lista_entradas *l, int i);
/*

*/
bool operacoes_registradores(lista_entradas *l, int i);
/*

*/

void assembly_instruction_type_form(lista_entradas *l);
/*

*/
void ler_arquivo_salvar_na_lista(lista_entradas *l, const char *nome_arquivo);

/*

*/
void formato_I_assembly(lista_entradas *l, int i);
/*

*/
void printar_codigo_assembly(lista_entradas *l, int i);
/*

*/
void printar_codigo_bin(lista_saidas *s, int i);
/*

*/
void definir_sinais_assembly(lista_entradas *l);
/*

*/
void alteracao_pc_assembly(lista_entradas *l, int *PC);
/*

*/
void printar_sinais_assembly(lista_entradas *l, int i);
/*

*/
void printar_pc_assembly(lista_entradas *l, int i,int PC);
/*

*/
void alteracao_registrador_assembly(lista_entradas *l);
/*

*/
void operacoes_registradores_assembly(lista_entradas *l);
/*

*/
void preencher_lista_out(lista_entradas *l, lista_saidas *s);
/*

*/
void preencher_tipo_R(lista_entradas *l, lista_saidas *s, int i);
/*

*/
void preencher_tipo_I(lista_entradas *l, lista_saidas *s, int i);
/*

*/
void intToBinaryString(int num, char *binaryString, int bits);
/*

*/
void escreverSaidasEmArquivo(lista_saidas *lista, const char *nomeArquivo);
/*

*/
void exibir_lista_assembly(lista_entradas *l, lista_saidas* s,int *PC);
/*

*/
void printar_sinais(lista_entradas *l, int i);
/*

*/
void printar_tipo_instrucao(lista_entradas *l, int i);
/*

*/
void printar_codigo_binario(lista_entradas *l, int i);
/*

*/
void printar_pc(lista_entradas *l, int i);
/*

*/
void definir_formato(lista_entradas *l);
/*

*/
void formato_R(lista_entradas *l, int i);
/*

*/
void formato_I(lista_entradas *l, int i);
/*

*/
void printar_mudancas_memoria(lista_entradas *l, int i);
/*

*/
int contar_instrucoes(const char *nome_arquivo);
/*

*/

#endif