#include <stdio.h>
#include <windows.h>      
#include <stdio.h>

int saldo = 1000;
CRITICAL_SECTION mutex;   // mutex
HANDLE sem;               // semafóro

DWORD WINAPI saqueMutex(LPVOID arg) {
    EnterCriticalSection(&mutex);       // entra na seção crítica
    saldo -= 100;
    printf("Saldo mutex: %d\n", saldo);
    LeaveCriticalSection(&mutex);       // libera a seção crítica
    return 0;
}

DWORD WINAPI saqueSem(LPVOID arg) {
    WaitForSingleObject(sem, INFINITE);  // entra na seção crítica
    saldo -= 100;
    printf("Saldo semáforo: %d\n", saldo);
    ReleaseSemaphore(sem, 1, NULL);      // libera a seção crítica
    return 0;
}

int main() {
    HANDLE t1, t2, t3, t4;

    // inicializa mutex
    InitializeCriticalSection(&mutex);

    // inicializa semáforo binário
    sem = CreateSemaphore(NULL, 1, 1, NULL);

    // ===== TESTE COM MUTEX =====
    saldo = 1000;  // reinicia saldo
    t1 = CreateThread(NULL, 0, saqueMutex, NULL, 0, NULL);
    t2 = CreateThread(NULL, 0, saqueMutex, NULL, 0, NULL);

    // aguarda threads de mutex
    WaitForSingleObject(t1, INFINITE);
    WaitForSingleObject(t2, INFINITE);

    printf("Saldo final após mutex: %d\n\n", saldo);

    // ===== TESTE COM SEMÁFORO =====
    saldo = 1000;  // reinicia saldo
    t3 = CreateThread(NULL, 0, saqueSem, NULL, 0, NULL);
    t4 = CreateThread(NULL, 0, saqueSem, NULL, 0, NULL);

    // aguarda threads de semáforo
    WaitForSingleObject(t3, INFINITE);
    WaitForSingleObject(t4, INFINITE);

    printf("Saldo final após semáforo: %d\n", saldo);

    // limpa recursos
    DeleteCriticalSection(&mutex);
    CloseHandle(sem);
    CloseHandle(t1);
    CloseHandle(t2);
    CloseHandle(t3);
    CloseHandle(t4);

    return 0;
}


