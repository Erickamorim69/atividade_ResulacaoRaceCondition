#include <iostream>
#include <thread>
#include <mutex>
#include <Windows.h>

using namespace std;


int tempo1 = 5;
int tempo2 = 10;

int contador = 0;
std::mutex a;

void incrementar(int n) {
    for (int i = 0; i < 1000; i++) {
        std::lock_guard<std::mutex> trava(a);

        contador++;
        printf("Thread %d : Contador: %d\n",n, contador);
        if(n == 1){
            Sleep(tempo1);
        }
        else if (n == 2){
            Sleep(tempo2);
        }
    }
}

int thread_t1(){
    while(1){
        incrementar(1);
        return 0;
    }
}
int thread_t2(){
    while(1){
        incrementar(2);
        return 0;
    }
}

int main() {
    std::thread t1(thread_t1);
    std::thread t2(thread_t2);

    t1.join();
    t2.join();

    std::cout << "Valor esperado: 2000" << endl;
    std::cout << "Valor obtido: " << contador << endl;

    return 0;
}