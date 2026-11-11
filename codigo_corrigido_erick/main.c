#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define STACK_SIZE 2048
#define PRIORITY 7

/* Variável compartilhada entre as threads */
int contador = 0;

void thread_uma(void *p1, void *p2, void *p3) {
    while (1) {
                /* --- CONDIÇÃO DE CORRIDA (RACE CONDITION) --- */
        /* 1. Leitura do valor atual */
        int temp = contador;

        /* 2. Atraso para forçar a troca de contexto enquanto a leitura está retida */
        k_msleep(50);

         /* 3. Escrita não atômica */
        contador = temp + 1;

        printk("THREAD 1: Incrementou contador para = %d\n", contador);

        k_msleep(500);
    }
}

void thread_duas(void *p1, void *p2, void *p3) {
    while (1) {
                /* --- CONDIÇÃO DE CORRIDA (RACE CONDITION) --- */
        /* 1. Leitura do valor atual */
        int temp = contador;

                /* 2. Atraso para forçar a troca de contexto enquanto a leitura está retida */
        k_msleep(50);

                /* 3. Escrita não atômica */
        contador = temp + 1;

        printk("THREAD 2: Incrementou contador para = %d\n", contador);

        k_msleep(500);
    }
}

/* Criando e iniciando as threads automaticamente no Zephyr */
K_THREAD_DEFINE(t1, STACK_SIZE, thread_uma, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(t2, STACK_SIZE, thread_duas, NULL, NULL, NULL, PRIORITY, 0, 0);