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

void Controlador_Principal_Assembly();

extern uint32_t registradores[N_REGISTRADORES];
void inicializar_registradores();

void inicializar(lista_entradas *e, lista_saidas *s, int cap);
void finalizar(lista_entradas *l, lista_saidas *s);
void inserir_entrada(lista_entradas *l, instructions i);
void exibir_lista(lista_entradas *l);
void organizar_instrucoes(const char *nome_arquivo, lista_entradas *l);

void controle_principal(lista_entradas *l, int *PC);
bool definir_sinais_bin(lista_entradas *l);
void alteracao_pc(lista_entradas *l, int i, int *PC);
bool alteracao_registrador(lista_entradas *l, int i);
bool operacoes_registradores(lista_entradas *l, int i);

void assembly_instruction_type_form(lista_entradas *l);
void ler_arquivo_salvar_na_lista(lista_entradas *l, const char *nome_arquivo);
void formato_I_assembly(lista_entradas *l, int i);
void printar_codigo_assembly(lista_entradas *l, int i);
void printar_codigo_bin(lista_saidas *s, int i);
void definir_sinais_assembly(lista_entradas *l);
void alteracao_pc_assembly(lista_entradas *l, int *PC);
void printar_sinais_assembly(lista_entradas *l, int i);
void printar_pc_assembly(lista_entradas *l, int i);
void alteracao_registrador_assembly(lista_entradas *l);
void operacoes_registradores_assembly(lista_entradas *l);
void preencher_lista_out(lista_entradas *l, lista_saidas *s);
void preencher_tipo_R(lista_entradas *l, lista_saidas *s, int i);
void preencher_tipo_I(lista_entradas *l, lista_saidas *s, int i);
void intToBinaryString(int num, char *binaryString, int bits);
void escreverSaidasEmArquivo(lista_saidas *lista, const char *nomeArquivo);
void exibir_lista_assembly(lista_entradas *l, lista_saidas* s);


void printar_sinais(lista_entradas *l, int i);
void printar_tipo_instrucao(lista_entradas *l, int i);
void printar_codigo_binario(lista_entradas *l, int i);
void printar_pc(lista_entradas *l, int i);
void definir_formato(lista_entradas *l);
void formato_R(lista_entradas *l, int i);
void formato_I(lista_entradas *l, int i);
void printar_mudancas_memoria(lista_entradas *l, int i);
int contar_instrucoes(const char *nome_arquivo);

#endif