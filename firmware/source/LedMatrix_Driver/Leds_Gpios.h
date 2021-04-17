#ifndef HWMAPPING_H
#define	HWMAPPING_H

#include "interfaces/gpio.h"
#include "board_settings.h"

namespace miosix {
    namespace ledsGpio {
    typedef Gpio<GPIOD_BASE,11> ROW1;
    typedef Gpio<GPIOD_BASE,10> ROW2;
    typedef Gpio<GPIOD_BASE,9> ROW3;
    typedef Gpio<GPIOD_BASE,8> ROW4;
    typedef Gpio<GPIOB_BASE,13> ROW5;
    typedef Gpio<GPIOB_BASE,14> ROW6;
    typedef Gpio<GPIOB_BASE,11> ROW7;
    typedef Gpio<GPIOB_BASE,12> ROW8;
    typedef Gpio<GPIOE_BASE,15> ROW9;
    typedef Gpio<GPIOE_BASE,13> ROW10;

    typedef Gpio<GPIOC_BASE,1> COL1;
    typedef Gpio<GPIOC_BASE,2> COL2;
    typedef Gpio<GPIOC_BASE,5> COL3;
    typedef Gpio<GPIOB_BASE,1> COL4;
    typedef Gpio<GPIOD_BASE,2> COL5;
    typedef Gpio<GPIOD_BASE,1> COL6;
    typedef Gpio<GPIOD_BASE,3> COL7;
    typedef Gpio<GPIOD_BASE,6> COL8;
    typedef Gpio<GPIOB_BASE,3> COL9;
    typedef Gpio<GPIOD_BASE,7> COL10;
    typedef Gpio<GPIOB_BASE,4> COL11;
    typedef Gpio<GPIOE_BASE,2> COL12;
    typedef Gpio<GPIOE_BASE,4> COL13;
    typedef Gpio<GPIOE_BASE,5> COL14;
    typedef Gpio<GPIOE_BASE,6> COL15;
    }
} //namespace miosix

#endif //HWMAPPING_H