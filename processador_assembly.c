#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "./processador.h"

#define N_REGISTRADORES 32
#define MEMORIA 1024
#define MAX_INSTRUCOES 100
#define TAM_INSTRUCAO 32

uint32_t registradores[N_REGISTRADORES];

void inicializar_registradores()
{
    for (int i = 0; i < 32; i++)
    {
        registradores[i] = 0;
    }
}

void inicializar(lista_entradas *l, int cap)
{
    l->entradas = (instructions *)calloc(cap, sizeof(instructions));
    l->quantidade = 0;
    l->capacidade = cap;
    printf("lista inicializada\n");
}

void finalizar(lista_entradas *l)
{
    free(l->entradas);
    printf("Lista Finalizada com Sucesso!\n");
}

void inserir_entrada(lista_entradas *l, instructions i)
{
    if (l->quantidade < MAX_INSTRUCOES)
    {
        l->entradas[l->quantidade] = i;
        l->quantidade++;
    }
    else
    {
        printf("*** ERRO!!! O Limite de instrucoes e de %d instrucoes ***\n", MAX_INSTRUCOES);
    }
}

void ler_arquivo_salvar_na_lista(lista_entradas *l, const char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo)
    {
        fprintf(stderr, "Erro ao abrir o arquivo %s.\n", nome_arquivo);
        return;
    }

    char buffer[1024]; // Buffer para armazenar o conteúdo do arquivo
    while (fgets(buffer, sizeof(buffer), arquivo))
    {
        // Processar o buffer para encontrar instruções separadas por ';'
        char *instrucao = strtok(buffer, ";");
        while (instrucao)
        {
            // Ignorar espaços no início
            while (isspace((unsigned char)*instrucao))
            {
                instrucao++;
            }

            // Ignorar espaços no final
            char *end = instrucao + strlen(instrucao) - 1;
            while (end > instrucao && isspace((unsigned char)*end))
            {
                end--;
            }
            *(end + 1) = '\0';

            // Verificar se a instrução não está vazia
            if (strlen(instrucao) > 0)
            {
                // Criar uma nova instrução
                instructions nova_instrucao;
                memset(&nova_instrucao, 0, sizeof(instructions)); // Inicializa todos os campos com 0
                strncpy(nova_instrucao.cod_assembly, instrucao, sizeof(nova_instrucao.cod_assembly) - 1);

                // Inserir a instrução na lista
                inserir_entrada(l, nova_instrucao);
            }

            // Avançar para a próxima instrução
            instrucao = strtok(NULL, ";");
        }
    }

    fclose(arquivo);
}


void exibir_lista(lista_entradas *l)
{
    if (l->quantidade == 0)
    {
        printf("Lista de instrucoes vazia.\n");
        return;
    }

    printf("Exibindo as instrucoes armazenadas:\n");
    for (int i = 0; i < l->quantidade; i++)
    {
        printf("Instrucao MIPS %d:\n", i + 1);
        printar_codigo_assembly(l, i);
        printar_tipo_instrucao(l, i);
        // printar_mudancas_memoria(l, i);
        // printar_pc(l, i);
        printar_sinais(l, i);

        printf("\n");
    }
}

void printar_sinais(lista_entradas *l, int i)
{
    if (strcmp(l->entradas[i].instrucao_tipo, "add") == 0 ||
        strcmp(l->entradas[i].instrucao_tipo, "sub") == 0 ||
        strcmp(l->entradas[i].instrucao_tipo, "lw") == 0 ||
        strcmp(l->entradas[i].instrucao_tipo, "sw") == 0 ||
        strcmp(l->entradas[i].instrucao_tipo, "addi") == 0 ||
        strcmp(l->entradas[i].instrucao_tipo, "beq") == 0 ||
        strcmp(l->entradas[i].instrucao_tipo, "j") == 0)
    {
        printf("ALUOp: %s, RegDst: %c, ALUSrc: %c, MemtoReg: %c, RegWrite: %c, MemRead: %c, MemWrite: %c, Branch: %c\n",
               l->entradas[i].ALUOp, l->entradas[i].RegDst, l->entradas[i].ALUSrc,
               l->entradas[i].MemtoReg, l->entradas[i].RegWrite, l->entradas[i].MemRead,
               l->entradas[i].MemWrite, l->entradas[i].Branch);
    }
    else
    {
        printf("Erro!!!: Tipo de instrucao desconhecido");
    }
}

void printar_tipo_instrucao(lista_entradas *l, int i)
{
    printf("Tipo de instrucao: %s\n", l->entradas[i].instrucao_tipo);
}

void printar_codigo_assembly(lista_entradas *l, int i)
{
    printf("Codigo Assembly: %s\n", l->entradas[i].cod_assembly);
}

void definir_sinais_assembly(lista_entradas *l)
{
    for (int i = 0; i < l->quantidade; i++)
    {
        if (l->entradas[i].formato_tipo == 'R')
        {

            strcpy(l->entradas[i].ALUOp, "10");
            l->entradas[i].RegDst = '1';
            l->entradas[i].ALUSrc = '0';
            l->entradas[i].MemtoReg = '0';
            l->entradas[i].RegWrite = '1';
            l->entradas[i].MemRead = '0';
            l->entradas[i].MemWrite = '0';
            l->entradas[i].Branch = '0';
        }
        else if (l->entradas[i].formato_tipo == 'I')
        {
            formato_I_assembly(l, i);
        }
    }
}

void formato_I_assembly(lista_entradas *l, int i)
{
    // lw
    if (strcmp(l->entradas[i].instrucao_tipo, "lw") == 0 )
    {
        strcpy(l->entradas[i].ALUOp, "00");
        l->entradas[i].RegDst = '0';
        l->entradas[i].ALUSrc = '1';
        l->entradas[i].MemtoReg = '1';
        l->entradas[i].RegWrite = '1';
        l->entradas[i].MemRead = '1';
        l->entradas[i].MemWrite = '0';
        l->entradas[i].Branch = '0';
        strcpy(l->entradas[i].instrucao_tipo, "lw");
    }
    // sw
    else if (strcmp(l->entradas[i].instrucao_tipo, "sw") == 0 )
    {
        strcpy(l->entradas[i].ALUOp, "00");
        l->entradas[i].RegDst = 'X';
        l->entradas[i].ALUSrc = '1';
        l->entradas[i].MemtoReg = 'X';
        l->entradas[i].RegWrite = '0';
        l->entradas[i].MemRead = '0';
        l->entradas[i].MemWrite = '1';
        l->entradas[i].Branch = '0';
        strcpy(l->entradas[i].instrucao_tipo, "sw");
    }
    // beq
    else if (strcmp(l->entradas[i].instrucao_tipo, "beq") == 0 )
    {
        strcpy(l->entradas[i].ALUOp, "01");
        l->entradas[i].RegDst = 'X';
        l->entradas[i].ALUSrc = '0';
        l->entradas[i].MemtoReg = 'X';
        l->entradas[i].RegWrite = '0';
        l->entradas[i].MemRead = '0';
        l->entradas[i].MemWrite = '0';
        l->entradas[i].Branch = '1';
        strcpy(l->entradas[i].instrucao_tipo, "beq");
    }

    // addi
    else if (strcmp(l->entradas[i].instrucao_tipo, "addi") == 0 )
    {
        strcpy(l->entradas[i].ALUOp, "10");
        l->entradas[i].RegDst = '0';
        l->entradas[i].ALUSrc = '1';
        l->entradas[i].MemtoReg = '0';
        l->entradas[i].RegWrite = '1';
        l->entradas[i].MemRead = '0';
        l->entradas[i].MemWrite = '0';
        l->entradas[i].Branch = '0';
        strcpy(l->entradas[i].instrucao_tipo, "addi");
    }

    // jump
    else if (strcmp(l->entradas[i].instrucao_tipo, "j") == 0 )
    {
        strcpy(l->entradas[i].ALUOp, "00");
        l->entradas[i].RegDst = '0';
        l->entradas[i].ALUSrc = '0';
        l->entradas[i].MemtoReg = '0';
        l->entradas[i].RegWrite = '0';
        l->entradas[i].MemRead = '0';
        l->entradas[i].MemWrite = '0';
        l->entradas[i].Branch = '0';
        strcpy(l->entradas[i].instrucao_tipo, "j");
    }
    else
    {
        printf("Instrução desconhecida: %s\n", l->entradas[i].cod_assembly);
        strcpy(l->entradas[i].instrucao_tipo, "desconhecido");
    }
}

void formato_R(lista_entradas *l, int i)
{
    // add
    if (strncmp(l->entradas[i].cod_assembly + 26, "100000", 6) == 0)
    {
        strcpy(l->entradas[i].instrucao_tipo, "add");
    }
    // sub
    else if (strncmp(l->entradas[i].cod_assembly + 26, "100010", 6) == 0)
    {
        strcpy(l->entradas[i].instrucao_tipo, "sub");
    }
}

void formato_I(lista_entradas *l, int i)
{
    // lw
    if (strncmp(l->entradas[i].cod_assembly, "100011", 6) == 0)
    {
        strcpy(l->entradas[i].ALUOp, "00");
        l->entradas[i].RegDst = '0';
        l->entradas[i].ALUSrc = '1';
        l->entradas[i].MemtoReg = '1';
        l->entradas[i].RegWrite = '1';
        l->entradas[i].MemRead = '1';
        l->entradas[i].MemWrite = '0';
        l->entradas[i].Branch = '0';
        strcpy(l->entradas[i].instrucao_tipo, "lw");
    }
    // sw
    else if (strncmp(l->entradas[i].cod_assembly, "101011", 6) == 0)
    {
        strcpy(l->entradas[i].ALUOp, "00");
        l->entradas[i].RegDst = 'X';
        l->entradas[i].ALUSrc = '1';
        l->entradas[i].MemtoReg = 'X';
        l->entradas[i].RegWrite = '0';
        l->entradas[i].MemRead = '0';
        l->entradas[i].MemWrite = '1';
        l->entradas[i].Branch = '0';
        strcpy(l->entradas[i].instrucao_tipo, "sw");
    }
    // beq
    else if (strncmp(l->entradas[i].cod_assembly, "000100", 6) == 0)
    {
        strcpy(l->entradas[i].ALUOp, "01");
        l->entradas[i].RegDst = 'X';
        l->entradas[i].ALUSrc = '0';
        l->entradas[i].MemtoReg = 'X';
        l->entradas[i].RegWrite = '0';
        l->entradas[i].MemRead = '0';
        l->entradas[i].MemWrite = '0';
        l->entradas[i].Branch = '1';
        strcpy(l->entradas[i].instrucao_tipo, "beq");
    }

    // addi
    else if (strncmp(l->entradas[i].cod_assembly, "001000", 6) == 0)
    {
        strcpy(l->entradas[i].ALUOp, "10");
        l->entradas[i].RegDst = '0';
        l->entradas[i].ALUSrc = '1';
        l->entradas[i].MemtoReg = '0';
        l->entradas[i].RegWrite = '1';
        l->entradas[i].MemRead = '0';
        l->entradas[i].MemWrite = '0';
        l->entradas[i].Branch = '0';
        strcpy(l->entradas[i].instrucao_tipo, "addi");
    }

    // jump
    else if (strncmp(l->entradas[i].cod_assembly, "000010", 6) == 0)
    {
        strcpy(l->entradas[i].ALUOp, "00");
        l->entradas[i].RegDst = '0';
        l->entradas[i].ALUSrc = '0';
        l->entradas[i].MemtoReg = '0';
        l->entradas[i].RegWrite = '0';
        l->entradas[i].MemRead = '0';
        l->entradas[i].MemWrite = '0';
        l->entradas[i].Branch = '0';
        strcpy(l->entradas[i].instrucao_tipo, "j");
    }
    else
    {
        printf("Instrução desconhecida: %s\n", l->entradas[i].cod_assembly);
        strcpy(l->entradas[i].instrucao_tipo, "desconhecido");
    }
}

void alteracao_pc_assembly(lista_entradas *l, int i, int *PC)
{
    if (strcmp(l->entradas[i].instrucao_tipo, "beq") == 0)
    {
        printf("BEQ: $rs = %d, $rt = %d, offset = %d\n",
               registradores[l->entradas[i].rs],
               registradores[l->entradas[i].rt],
               l->entradas[i].const_or_address);

        // Verifica se os registradores são iguais
        if (l->entradas[i].rs == l->entradas[i].rt)
        {
            // Calcula o novo PC para o salto, considerando que o deslocamento deve ser multiplicado por 4
            int novo_pc = *PC + (l->entradas[i].const_or_address * 4);

            printf("Novo PC: %d\n", novo_pc); // Verifique o cálculo do novo PC

            *PC = novo_pc;
            printf("PC alterado para: %d\n", *PC);
        }
        else
        {
            printf("*** ERRO!!: Acesso invalido a memoria (Endereco fora dos limites de memoria) ***\n");
        }
    }
    else
    {
        // Se a condição não for atendida, o PC segue normalmente (PC + 4)
        if (*PC <= MEMORIA - 4)
        {
            *PC = *PC + 4;
            printf("PC alterado para: %d\n", *PC);
        }
        else
        {
            printf("*** ERRO!!: Acesso invalido a memoria (Endereco fora dos limites de memoria) ***\n");
        }
    }

    // Para instruções aritméticas e de acesso à memória
    if (strcmp(l->entradas[i].instrucao_tipo, "add") == 0 ||
        strcmp(l->entradas[i].instrucao_tipo, "sub") == 0 ||
        strcmp(l->entradas[i].instrucao_tipo, "lw") == 0 ||
        strcmp(l->entradas[i].instrucao_tipo, "sw") == 0 ||
        strcmp(l->entradas[i].instrucao_tipo, "addi") == 0)
    {
        if (*PC <= MEMORIA - 4)
        {
            *PC = *PC + 4;
        }
        else
        {
            printf("*** ERRO!!: Acesso invalido a memoria (Endereco fora dos limites de memoria) ***\n");
        }
    }
}

void printar_pc_assembly(lista_entradas *l, int i)
{
    if (strcmp(l->entradas[i].instrucao_tipo, "add") == 0 ||
        strcmp(l->entradas[i].instrucao_tipo, "sub") == 0 ||
        strcmp(l->entradas[i].instrucao_tipo, "lw") == 0 ||
        strcmp(l->entradas[i].instrucao_tipo, "sw") == 0 ||
        strcmp(l->entradas[i].instrucao_tipo, "addi") == 0)
    {
        printf("PC = PC + 4\n");
    }

    else if (strcmp(l->entradas[i].instrucao_tipo, "beq") == 0)
    {
        if (l->entradas[i].rs == l->entradas[i].rt)
        {
            printf("PC = PC + %d\n", l->entradas[i].const_or_address * 4);
        }
        else
        {
            printf("PC = PC + 4\n");
        }
    }
    else if (strcmp(l->entradas[i].instrucao_tipo, "j") == 0)
    {
        printf("PC = endereco do alvo do salto (endereco calculado com base no opcode)\n");
    }
}

void alteracao_registrador_assembly(lista_entradas *l, int i)
{
    const char *registradores_nome[N_REGISTRADORES] = {
        "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
        "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
        "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
        "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};
    int offset = l->entradas[i].const_or_address / 4;

    char rs_ass[6];
    char rt_ass[6];
    char rd_ass[6];

    char const_or_adress_bin[17];
    if (strcmp(l->entradas[i].instrucao_tipo, "add") == 0 || strcmp(l->entradas[i].instrucao_tipo, "sub") == 0)
    {     
        sscanf(l->entradas[i].cod_assembly, "%*s %s %s", rd_ass, rs_ass, rt_ass);
        
            for(int j = 0; j<=15; j++){
                if(rd_ass == registradores_nome[j+8]){
                    l->entradas[i].rd = j+8;
                }

            }
        

    }

    else if (strcmp(l->entradas[i].instrucao_tipo, "lw") == 0 || strcmp(l->entradas[i].instrucao_tipo, "sw") == 0 || strcmp(l->entradas[i].instrucao_tipo, "beq") == 0 || strcmp(l->entradas[i].instrucao_tipo, "j") == 0 || strcmp(l->entradas[i].instrucao_tipo, "addi") == 0)
    {
        
       
    }

    
}

bool operacoes_registradores(lista_entradas *l, int i)
{
    if (strcmp(l->entradas[i].instrucao_tipo, "add") == 0)
    {
        registradores[l->entradas[i].rd] = registradores[l->entradas[i].rs] + registradores[l->entradas[i].rt];
        return true;
    }
    else if (strcmp(l->entradas[i].instrucao_tipo, "sub") == 0)
    {
        registradores[l->entradas[i].rd] = registradores[l->entradas[i].rs] - registradores[l->entradas[i].rt];
        return true;
    }
    else if (strcmp(l->entradas[i].instrucao_tipo, "addi") == 0)
    {
        registradores[l->entradas[i].rt] = registradores[l->entradas[i].rs] + l->entradas[i].const_or_address;
        return true;
    }

    return false;
}

void printar_mudancas_memoria(lista_entradas *l, int i)
{
    const char *registradores_nome[N_REGISTRADORES] = {
        "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
        "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
        "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
        "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};
    int offset = l->entradas[i].const_or_address / 4;
    // Para a instrução "add"
    if (strcmp(l->entradas[i].instrucao_tipo, "add") == 0)
    {
        printf("Executando %s: %s = %s + %s\nResultado: %s = %d\n",
               l->entradas[i].instrucao_tipo,
               registradores_nome[l->entradas[i].rd],
               registradores_nome[l->entradas[i].rs],
               registradores_nome[l->entradas[i].rt],
               registradores_nome[l->entradas[i].rd],
               registradores[l->entradas[i].rd]);
    }
    // Para a instrução "sub"
    else if (strcmp(l->entradas[i].instrucao_tipo, "sub") == 0)
    {
        printf("Executando %s: %s = %s - %s\nResultado: %s = %d\n",
               l->entradas[i].instrucao_tipo,
               registradores_nome[l->entradas[i].rd],
               registradores_nome[l->entradas[i].rs],
               registradores_nome[l->entradas[i].rt],
               registradores_nome[l->entradas[i].rd],
               registradores[l->entradas[i].rd]);
    }
    // Para a instrução "addi"
    else if (strcmp(l->entradas[i].instrucao_tipo, "addi") == 0)
    {
        printf("Executando %s: %s = %s + %d\nResultado: %s = %d\n",
               l->entradas[i].instrucao_tipo,
               registradores_nome[l->entradas[i].rt],
               registradores_nome[l->entradas[i].rs],
               l->entradas[i].const_or_address,
               registradores_nome[l->entradas[i].rt],
               registradores[l->entradas[i].rt]);
    }
    // Para a instrução "lw"
    else if (strcmp(l->entradas[i].instrucao_tipo, "lw") == 0)
    {
        printf("Executando %s: %s = Mem[%s + %d]\nResultado: %s = %d\n",
               l->entradas[i].instrucao_tipo,
               registradores_nome[l->entradas[i].rt],
               registradores_nome[l->entradas[i].rs],
               l->entradas[i].const_or_address,
               registradores_nome[l->entradas[i].rt],
               registradores[l->entradas[i].rt]);
    }
    // Para a instrução "sw"
    else if (strcmp(l->entradas[i].instrucao_tipo, "sw") == 0)
    {
        if (l->entradas[i].rs > 7 && l->entradas[i].rs < 24 && l->entradas[i].rt > 7 && l->entradas[i].rt < 24)
        {
            printf("Executando %s: %s[%d] = %s\n",
                   l->entradas[i].instrucao_tipo,
                   registradores_nome[l->entradas[i].rt],
                   offset,
                   registradores_nome[l->entradas[i].rs]);
        }
        else
        {
            printf("ERRO!!! : A instrucao utiliza um registrador invalido\n");
        }
    }
    // Para a instrução "beq"
    else if (strcmp(l->entradas[i].instrucao_tipo, "beq") == 0)
    {
        printf("Executando %s: if %s == %s, PC = PC + 4 + (%d * 4)\n",
               l->entradas[i].instrucao_tipo,
               registradores_nome[l->entradas[i].rs],
               registradores_nome[l->entradas[i].rt],
               l->entradas[i].const_or_address);
    }
    // Para a instrução "j"
    else if (strcmp(l->entradas[i].instrucao_tipo, "j") == 0)
    {
        printf("Executando %s: PC = (PC & 0xF0000000) | (address << 2)\n",
               l->entradas[i].instrucao_tipo);
    }
}

int contar_instrucoes(const char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo)
    {
        perror("Erro ao abrir o arquivo");
        return -1;
    }

    char buffer[1024];
    int contador_instrucoes = 0;

    while (fgets(buffer, sizeof(buffer), arquivo))
    {
        // Processar o buffer para contar instruções separadas por ';'
        char *instrucao = strtok(buffer, ";");
        while (instrucao)
        {
            // Remover espaços em branco no início e no fim da instrução
            while (isspace(*instrucao))
                instrucao++;
            char *end = instrucao + strlen(instrucao) - 1;
            while (end > instrucao && isspace(*end))
                end--;
            *(end + 1) = '\0';

            // Contar apenas se não for uma instrução vazia
            if (strlen(instrucao) > 0)
            {
                contador_instrucoes++;
            }
            instrucao = strtok(NULL, ";");
        }
    }

    fclose(arquivo);
    return contador_instrucoes;
}

void assembly_instruction_type_form(lista_entradas * l){
    if(l != NULL){
        for(int i = 0; i <l->quantidade; i++){
            sscanf(l->entradas[i].cod_assembly,"%s", l->entradas[i].instrucao_tipo);
            if(strcmp(l->entradas[i].instrucao_tipo, "add") == 0 || strcmp(l->entradas[i].instrucao_tipo, "sub") == 0){
                l->entradas[i].formato_tipo = 'R';
            }
            else{
                l->entradas[i].formato_tipo = 'I';
            }
        }
    }
}