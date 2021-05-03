#ifndef HWMAPPING_H
#define	HWMAPPING_H

#include "interfaces/gpio.h"
#include "board_settings.h"

typedef miosix::Gpio<GPIOD_BASE,11> ROW1;
typedef miosix::Gpio<GPIOD_BASE,10> ROW2;
typedef miosix::Gpio<GPIOD_BASE,9> ROW3;
typedef miosix::Gpio<GPIOD_BASE,8> ROW4;
typedef miosix::Gpio<GPIOB_BASE,13> ROW5;
typedef miosix::Gpio<GPIOB_BASE,14> ROW6;
typedef miosix::Gpio<GPIOB_BASE,11> ROW7;
typedef miosix::Gpio<GPIOB_BASE,12> ROW8;
typedef miosix::Gpio<GPIOE_BASE,15> ROW9;
typedef miosix::Gpio<GPIOE_BASE,13> ROW10;

typedef miosix::Gpio<GPIOC_BASE,1> COL1;
typedef miosix::Gpio<GPIOC_BASE,2> COL2;
typedef miosix::Gpio<GPIOC_BASE,5> COL3;
typedef miosix::Gpio<GPIOB_BASE,1> COL4;
typedef miosix::Gpio<GPIOD_BASE,2> COL5;
typedef miosix::Gpio<GPIOD_BASE,1> COL6;
typedef miosix::Gpio<GPIOD_BASE,3> COL7;
typedef miosix::Gpio<GPIOD_BASE,6> COL8;
typedef miosix::Gpio<GPIOB_BASE,3> COL9;
typedef miosix::Gpio<GPIOD_BASE,7> COL10;
typedef miosix::Gpio<GPIOB_BASE,4> COL11;
typedef miosix::Gpio<GPIOE_BASE,2> COL12;
typedef miosix::Gpio<GPIOE_BASE,4> COL13;
typedef miosix::Gpio<GPIOE_BASE,5> COL14;
typedef miosix::Gpio<GPIOE_BASE,6> COL15;


#endif //HWMAPPING_H