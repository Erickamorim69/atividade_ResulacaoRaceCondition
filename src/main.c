#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>

#define PRIO_THREAD_A 5   // Prioridade do padeiro
#define PRIO_THREAD_B 5   // Prioridade dos clientes

volatile int saldo_vitrine = 0; // quantidadea de paos na vitrine

K_MUTEX_DEFINE(fila);

void thread_A(void *p1, void *p2, void *p3) //padeiro
{
    int pao = 0;
    int saldo_pao = 0;
    while (1) {
        //if (saldo_vitrine < 10)
        //{
        k_mutex_lock(&fila, K_FOREVER);
        pao++;
        saldo_pao = saldo_vitrine;
        saldo_pao++;
        //k_msleep(50);
        saldo_vitrine = saldo_pao;
        printk("PADEIRO: Pão #%d pronto (Saldo vitrine = %d)\n", pao, saldo_vitrine);
        k_mutex_unlock(&fila);
        k_msleep(1000);
        //}
        /* else
        {
            printk("PADEIRO: Vitrine cheia ================================\n");
            k_msleep(1000);
        } */
    }
}

void thread_B(void *p1, void *p2, void *p3) //clientes
{
    int pao = 0;
    int saldo_pao = 0;
    while (1) {
        //if (saldo_vitrine > 0)
        //{
        k_mutex_lock(&fila, K_FOREVER);
        pao++;
        saldo_pao = saldo_vitrine;
        saldo_pao--;
        //k_msleep(50);
        saldo_vitrine = saldo_pao;
        k_mutex_unlock(&fila);
        printk("CLIENTE: Peguei pão #%d (Saldo vitrine = %d)\n", pao, saldo_vitrine);    
        k_msleep(1500);
        //}
        /* els
        {
            printk("CLIENTE: Vitrine vazia ================================\n");
            k_msleep(1500);
        } */
    }
}

K_THREAD_DEFINE(a_tid, 512, thread_A, NULL, NULL, NULL, PRIO_THREAD_A, 0, 0);
K_THREAD_DEFINE(b_tid, 512, thread_B, NULL, NULL, NULL, PRIO_THREAD_B, 0, 0);

void main(void){

    printk("Padaria aberta...\n");
    while (1) {
    k_sleep(K_FOREVER); // Main dorme para liberar CPU.
    }
}
