0、工程来自stm32f4_dsp_stdperiph_lib.zip，版本为V1.7.1
1、工程文件.uvopt和.uvproj属性为只读。所以在进行修改保存配置的时候会提示 工程为“只读”。
2、stm32是小端模式，在lib中选择arm_cortexM4lf_math.lib，M4后的l表示小端、b表示大端。f表示支持浮点运算单元。这里选择包含lf的文件。
3、打开stm32f4xx.h会发现：

#if !defined (STM32F40_41xxx) && !defined (STM32F427_437xx) && !defined (STM32F429_439xx) && !defined (STM32F401xx)
 #error "Please select first the target STM32F4xx device used in your application (in stm32f4xx.h file)"
#endif

所以需要stm32f4xx.h文件中（靠前的问题）将符合芯片的#define STM32F40_41xxx的注释去掉。
或者在Option的c/c++中添加STM32F40_41xxx，注意，在使用别的库的时候，同样是stm32f407芯片，可能是添加STM32F4XX，需要根据stm32f4xx.h来确定
4、在进行软件debug的时候会出现：error 65:access vilation at 0x0000000C:no 'read' permission
========================================================
目前在市场上最常用的ARM编程工具无非是Real View MDK和IAR Embedded Workbench。
我个人觉得，目前IAR是比较成熟、稳定、使用方便等优点，深受广大嵌入式开发者的喜爱。Real view MDK得到ARM公司的大力支持，功能很强，也逐步完善起来。

使用Real view（以下省略为RV）学习ARM指令的时候，初学者在调试过程中，经常会遇到类似于如下的错误消息。
Error 65:access violation at 0xFFFFFFFC: no “write” permission

这个错误一般在使用RV模拟器（使用’debug’的’Use simulator’模式）进行调试时出现。
在发生这个错误时，观察寄存器的话，会错误消息中的地址正是SP（R13）的值。从此可以推测，错误就在进行堆栈操作时发生。
那为什么进行堆栈操作时，发生错误呢？因为，现在使用的是模拟器，而不是真正的目标板，所以对这些内存区域没有任何定义。
所以我们必须在模拟器中要将这个内存地址段的属性设置为可读/可写。
下面，以S3C2410模拟器为例，说明解决上述问题的几种方法：
第一种方法：打开（project -> options..）工程的选项对话框，其中将’Debug’标签的’Dialog DLL’中的DLL文件名（默认为DARMSS9.DLL）改成DARMST9.DLL
第二种方法：在ini file（.ini文件）中写入以下部分，表示这个内存区域是可读/可写/可执行。
map 0xFFFFF000,0xFFFFFFFF read write exec
第三种方法：这是在调试过程中的修改方法，所以在每次运行的时候，都要设置。
先进入调试模式（crtl+F5），接下来在debug菜单下打开memory_map对话框，在这个对话框中添加新的映射区域（map range）和它的属性。

5、由于硬件使用的是8Mhz的晶振，但是STD标准库默认使用25Mhz的库，因此需要修改：
   STD/system_stm32f4xx.c中将   #define PLL_M  25  修改成#define PLL_M   8  ，因为硬件外部晶振是8M的;
   stm32f4xx.h中将 #define HSE_VALUE  ((uint32_t)25000000) 改为 #define HSE_VALUE ((uint32_t)8000000)

   程序为了库的兼容性，对STD/system_stm32f4xx.c中对PLL_M加了是否已经定义过的宏判断：
   #if !defined  (PLL_M)
   /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N */
   #define PLL_M      25
   #endif /* PLL_M */
   然后在options-->C/C++-->加上HSE_VALUE=8000000,PLL_M=8这两个系统宏
6、在使用Fatfs文件系统的时候，如果实验中出现不能保存很大的txt文件（其文件的最大只能与SD卡的文件系统的最小单元相同）。则需要修改Fatfs文件系统
   中ffconf.h中第164行的#define _MAX_SS 512 修改为 #define _MAX_SS 4096。这样就可以存储任意大小的txt文件。
7、硬件I2C需要最高的优先级，如果通讯过程中被中断，则会死掉。解决：单片机中断优先级的设定中，将底层传感器中的I2C DAM中断和滴答定时器中断的优先级定为最高优先级为NVIC_PriorityGroup_0，在Example/test_Allan_varian中的定时器7的优先级
   为NVIC_PriorityGroup_4。
8、如果通过重载fputc的方式构造printf函数，需要注意：1、添加USART_ClearFlag(USART1,USART_FLAG_TC);否则第一个字符会丢失，根本原因在于SR寄存器中TC标志模型为1
   2、在设置面板Target板块的Use MicroLIB要勾选
   在发送程序中添加USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET，否则有可能出现最后一个字符没有发送，因为缓冲区移位完毕，但串口发送器里还有一个字符未发送
9、在使用QBL_I2C的时候，除非只有一个芯片在使用I2C，否则如果多个芯片使用同一个I2C，最好不要使用USE_DMA_I2C1_INTERRUPT模型，因为QBL_I2C_LastReadFinished函数并不能准确判断是哪个I2C设备触发了中断
10、keil5的安装目录下，Keil5\UV4\global.prop.def是默认的编辑格式（字体颜色、大小、tab等等），可以直接替换
   