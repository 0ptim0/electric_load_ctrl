#include "stm32_base.h"
#include "gpio.h"
#include "bit.h"
#pragma once

// Кофингурация
#define LOAD_STEPS          10          // Количество ступеней
#define LOAD_ADDR           0x40        // Адрес устройства
#define LOAD_TIMEOUT_MS     100         // Таймаут на операцию обмена с контроллером индикации и измерений в мс

// Команды
#define LOAD_MANUAL_CONTROL     0x10    // Включение ручного управления
#define LOAD_REMOTE_CONTROL     0x20    // Включение дистанционного управления
#define LOAD_MIXED_CONTROL      0x30    // Включение смешанного управления

#define LOAD_DISARM             0x01    // Отключение всех ступеней, блокировка управления

#define LOAD_TM                 0x83    // Запрос телеметрии
#define LOAD_SET                0x60    // Включить ступени, принимает 2 байта, ничего не отвечает
 
// Структура кадра телеметрии
#pragma pack(push, 1)
struct load_tm_t {
    uint32_t V_mV;  // Значение напряжения в мВ
    uint32_t I_mA;  // Значение тока в мА
    uint16_t steps; // Битовая маска ступеней
    uint8_t state;  // Текущее состояние
    uint8_t error;  // Код ошибки
};
#pragma pack(pop)

// Список состояний
enum loadstate {
    MANUAL,     // Включение и выключение ступеней кнопками
    MIXED,      // Включение и выключение ступеней как кнопками, так и командами по CAN
    REMOTE,     // Включение и выключение командами по CAN
    DISARM,     // Ступени выключены, управление заблокировано
    ERR         // Есть ошибки, см. код ошибки
};

// Коды ошибок
#define LOAD_NO_ERR         0x00    // Нет ошибок
#define LOAD_ERR_TIMEOUT    0x01    // Таймаут операции обмена данными с контроллером индикации и измерений
#define LOAD_ERR_VOLTAGE    0x02    // Выход за допустимые пределы значений по напряжению
#define LOAD_ERR_CURRENT    0x03    // Выход за допустимые пределы значений по току

struct load_cfg_t {
    gpio_cfg_t step[LOAD_STEPS];
    gpio_cfg_t button[LOAD_STEPS];
    gpio_exti_cfg_t button_exti[LOAD_STEPS];
};

class load_class 
{
private:
    const uint8_t steps = LOAD_STEPS;
    load_cfg_t *cfg;
    gpio_class step[LOAD_STEPS];
    gpio_class button[LOAD_STEPS];
public:
    load_tm_t tm;
public:
    load_class(load_cfg_t *cfg);
    int Init(void);
    int Off(void);
    int On(void);
    int Set(uint16_t mask);
    int IT(uint32_t button);
    int Handle(void);
};