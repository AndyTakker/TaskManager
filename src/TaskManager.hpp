//============================================================= (c) A.Kolesov ==
// TaskManager.hpp
// Простая библиотека для выполнения задач с заданной периодичностью.
// Порядок использования:
// 1. Создать менеджер задач, указав ему количество задач и указатель на функцию
//    времени, отсчитывающую миллисекунды
// 2. Описать необходимые задачи (функции), которые будут выполняться.
// 3. Добавить задачи менеджеру.
// 4. Вызывать метод control() в loop().
// Смотри пример использования.
//------------------------------------------------------------------------------
#pragma once

#include <stdint.h>

// Тип для callback'а
using TaskCallback = void (*)();

template <uint8_t MAX_TASKS>
class TaskManager {
  public:
  struct Task {
    TaskCallback callback = nullptr;
    uint32_t period = 0;
    uint32_t lastRun = 0;
    bool enabled = false;
  };

  // Конструктор. Получает указатель на функцию времени, отсчитывающую миллисекунды
  explicit TaskManager(uint32_t (*getMillis)()) : m_getMillis(getMillis) {
    static_assert(MAX_TASKS > 0, "MAX_TASKS must be at least 1");
  }

  // Добавляет задачу, возвращает ID (0..MAX_TASKS-1) или -1, если число задач превышено
  // После добавления задача готова к запуску и будет выполняться при наступлении времени
  int8_t add(TaskCallback callback, uint32_t period_ms) {
    for (uint8_t i = 0; i < MAX_TASKS; ++i) {
      if (!tasks[i].enabled && tasks[i].callback == nullptr) {
        tasks[i].callback = callback;
        tasks[i].period = period_ms;
        tasks[i].lastRun = m_getMillis();
        tasks[i].enabled = true;
        return static_cast<int8_t>(i);
      }
    }
    return -1; // нет свободных слотов
  }

  // Останавливает задачу по ID
  void stop(uint8_t id) {
    if (id < MAX_TASKS) {
      tasks[id].enabled = false;
    }
  }
  // Перезапускает задачу. Интервал запуска начинается с начала
  void restart(uint8_t id) {
    if (id < MAX_TASKS && tasks[id].callback != nullptr) {
      tasks[id].lastRun = m_getMillis();
      tasks[id].enabled = true;
    }
  }

  // Главный метод — вызывать в loop() как можно чаще.
  void control() {
    const uint32_t now = m_getMillis();
    for (uint8_t i = 0; i < MAX_TASKS; ++i) {
      Task &t = tasks[i];
      if (t.enabled && t.callback && (now - t.lastRun >= t.period)) {
        t.lastRun = now; // важно обновить ДО вызова callback!
        t.callback();
      }
    }
  }

  private:
  Task tasks[MAX_TASKS] = {}; // инициализируем нулями
  uint32_t (*m_getMillis)();  // Функция получения текущего времени
};
