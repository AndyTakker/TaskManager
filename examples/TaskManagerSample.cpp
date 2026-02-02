//==============================================================================
// Пример использования библиотеки TaskManager 
//------------------------------------------------------------------------------
#include <SysClock.h>
#include <TaskManager.hpp>
#include <debug.h>

TaskManager<3> tasks(Sysclock.Millis); // Создаем экземпляр TaskManage;

void display();  // Вывод информации на дисплей
void printPrm(); // Вывод информации в консоль


//==============================================================================
int main() {
  SystemCoreClockUpdate();
#ifdef LOG_ENABLE
  USART_Printf_Init(115200);
#endif
  printf("SystemClk: %lu\r\n", SystemCoreClock);        // Для посмотреть частоту процесора (48мГц)
  printf("   ChipID: 0x%08lX\r\n", DBGMCU_GetCHIPID()); // Для посмотреть ID чипа, от нефиг делать

  tasks.add(display, 1000);
  tasks.add(printPrm, 2000);

  while (1) {
    tasks.control(); // Исполняем периодические задачи

    // Делаем что-то содержательное
  }
}

void display() {
  printf("Message every seconds\r\n");
}

void printPrm() {
 printf("Message every two seconds\r\n");
}