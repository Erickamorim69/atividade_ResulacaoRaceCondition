#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define STACK_SIZE 2048
#define PRIORITY 7

/* Variável compartilhada (onde ocorre a Race Condition) */
int contador = 0;

/* Flags manuais para simular o Deadlock (sem mutex/semáforo) */
volatile int flag_t1 = 0;
volatile int flag_t2 = 0;

void thread_uma(void *p1, void *p2, void *p3) {
    while (1) {
        /* --- 1. CONDIÇÃO DE CORRIDA (RACE CONDITION) --- */
        /* Leitura e escrita desmembradas sem proteção */
        int temp = contador;
        k_msleep(50); // Força a troca de contexto durante a leitura
        contador = temp + 1;
        printk("THREAD 1: Incrementou contador para = %d\n", contador);

        /* --- 2. DEADLOCK (IMPASSE POR FLAGS MANUAIS) --- */
        flag_t1 = 1; // Thread 1 sinaliza que quer prioridade
        printk("THREAD 1: Aguardando Thread 2 baixar flag...\n");

        /* Se a Thread 2 também tiver marcado flag_t2 = 1, ambas travam aqui */
        while (flag_t2 == 1) {
            printk("====== DEADLOCK! THREAD 1 PRESA AGUARDANDO THREAD 2 ======\n");
            k_msleep(1000);
        }

        flag_t1 = 0;
        k_msleep(400);
    }
}

void thread_duas(void *p1, void *p2, void *p3) {
    while (1) {
        /* --- 1. CONDIÇÃO DE CORRIDA (RACE CONDITION) --- */
        /* Leitura e escrita desmembradas sem proteção */
        int temp = contador;
        k_msleep(50); // Força a troca de contexto durante a leitura
        contador = temp + 1;
        printk("THREAD 2: Incrementou contador para = %d\n", contador);

        /* --- 2. DEADLOCK (IMPASSE POR FLAGS MANUAIS) --- */
        flag_t2 = 1; // Thread 2 sinaliza que quer prioridade
        printk("THREAD 2: Aguardando Thread 1 baixar flag...\n");

        /* Se a Thread 1 também tiver marcado flag_t1 = 1, ambas travam aqui */
        while (flag_t1 == 1) {
            printk("====== DEADLOCK! THREAD 2 PRESA AGUARDANDO THREAD 1 ======\n");
            k_msleep(1000);
        }

        flag_t2 = 0;
        k_msleep(400);
    }
}

/* Definição das threads no Zephyr */
K_THREAD_DEFINE(t1, STACK_SIZE, thread_uma, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(t2, STACK_SIZE, thread_duas, NULL, NULL, NULL, PRIORITY, 0, 0);