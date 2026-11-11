#include <iostream>
#include <thread>
//aa
using namespace std;

int contador = 0;
void incrementar() {
    for (int i = 0; i < 1000000; i++) {
        contador++;
    }
}

int main() {
    thread t1(incrementar);
    thread t2(incrementar);

    t1.join();
    t2.join();

    cout << "Valor esperado: 2000000" << endl;
    cout << "Valor obtido: " << contador << endl;

    return 0;