
 
![GPL license](https://img.shields.io/badge/license-GPL-blue.svg)
![Generic badge](https://img.shields.io/badge/RTOS-miosix-orange.svg)
![made-with-c++](https://img.shields.io/badge/Made%20with-C/C++-1f425f.svg)

# Digital_Audio_Effect


## Index

* ### [Overview](#intro)
* ### [Videos](#videos)
* ### [Requirements](#req)
  * #### [Hardware](#hw)
  * #### [Software](#sw)
* ### [Getting Started](#get)
* ### [DSP Effects](#dsp)
  * #### [Delay](#delay)
  * #### [Filter](#filter) 
* ### [Future Improvements](#future)

## <a name="intro"></a>Overview
This project is a platform to experiment with **real-time DSP effects** on
audio, with an STM32 microprocessor, using a multi-threaded, real-time environment.
The application has been developed on top of the **Miosix** embedded operative system which provided real-time operating system (RTOS) features, as well as a native multi-threaded enviroment.
A full detailed document explaining the project in details can be found at:
https://github.com/GenniImparato/Digital_Audio_Effect/blob/main/docs/report/Digital_Audio_Effects.pdf


## <a name="videos"></a>Videos
https://user-images.githubusercontent.com/37338634/153053574-94ea6515-ac46-4c21-b3ac-aa7877b238f6.mp4

https://user-images.githubusercontent.com/37338634/153054718-db798998-3f35-4ed6-b5b4-32b3baa28a30.mp4

https://user-images.githubusercontent.com/37338634/153054537-bed419cf-c9f3-40c2-ad0e-0b04481bedc5.mp4


## <a name="req"></a>Requirements

### <a name="hw"></a>Hardware Requirements
- STM32F407 Discovery Board
- Custom PCB (see `hardware` folder for more details)

### <a name="sw"></a>Software Requirements
- [Miosix](https://miosix.org/) OS
- [QSTLink2](https://github.com/fpoussin/QStlink2) for board configuration

## <a name="get"></a>Getting started

1. Connect the microcontroller to the PC with the USB cable or with the external power supply of the PCB.

2. Open `QSTLink2` to connect the board through the `connect` command.

3. Open a terminal, go to the `firmware` folder and type `make` to compile the project.

   ```
   cd path/to/project/firmware   
   make
   ```

4. From `QSTLink2`, send the code to the board through the `send` command.



## <a name="dsp"></a>DSP Effects
The following are the implemented DSP effects, which values can be dinamically modified. 

### <a name="delay"></a>Delay
As the name implies, this effect delays the flow of the audio source by a given amount.

### <a name="filter"></a>Filter
This effect can be configured as:
- Low-pass filter
- High-pass filter
- Band-pass filter

Each of those modes can be dinamically changed, as well as their values.

## <a name="future"></a>Future Improvements
**[+]** Implementation of more audio effects

**[+]** Anti-aliasing filtering

**[+]** Sine curves visualization



