
 
![GPL license](https://img.shields.io/badge/license-GPL-blue.svg)
![Generic badge](https://img.shields.io/badge/RTOS-miosix-orange.svg)
![made-with-c++](https://img.shields.io/badge/Made%20with-C/C++-1f425f.svg)

# Digital_Audio_Effect


## Index

* ### [Overview](#intro)
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



