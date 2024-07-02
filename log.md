#### SUGGESTION

- 请直接抛弃Keil作为你的代码编辑器的想法（**不要使用Keil来写代码，而是只把Keil作为编译和调试的工具**）
- 初次看代码前可考虑使用Source Insight这样的工具进行阅读（**SI读代码实在是太方便了**）。
- 推荐使用STM32CubeMX来进行工程的生成（**如果使用的是ST的芯片的话**）。
1. 推荐使用vscode安装C/C++拓展插件和Keil uVision Assistant插件来进行程序的编辑。
>即使Vscode官方的C/C++插件的Intellisense的速度和准确率并不如clangd，但好处是不需要compile_commands.json文件。处理单个兵种的电控代码还是足够的。在初期使用C/C++插件的Intellisense个人认为是个好的选择，在熟悉且拥有一些经验后使用Cmake生成compile_commands.json改用clangd也是可以的。
2. 然后使用Keil5的AC6（armclang）编译器对工程进行编译（**armclang会比armcc更好，除非你在维护使用armcc的旧代码**）。
3. 再然后在Keil5上完成对程序的调试（**Keil5在调试方面有GDB无法比拟的优势。当然现在官方也推出了ARM-GNU工具链，有队伍已经在使用，可以进行围观**）。
> 上述建议写于2024/07/01，请带着辩证唯物史观思想来看待上述建议，没有什么是一成不变的，也许未来会有更合适的方案。

###### 本工程代码风格基于Google C++ Style（**全局变量除外，全局变量依旧使用小写下划线命名**）
> <https://google.github.io/styleguide/cppguide.html>

#### NOTICES:

- 工程中可能会有一些在编写时留下的“注意”或“警告”，可以进行全局搜索“//@notice”和“//@warning”
- 工程中可能会标注一些需要进行修改的配置项如电机ID等，可以进行全局搜索“//@config”
- 工程中可能会标注一些在开发时打下的待解决的提示，可以进行全局搜索“//@develop”

定时器的用户定义周期回调函数*TimerPeriodCallback*在main.c里面的定时器回调函数*HAL_TIM_PeriodElapsedCallback*内被调用。

本工程除GimbalTask任务以外的部分（代码）均运行在非FreeRTOS管理的中断下（**中断优先级皆为0**），因此请不要在中断会调用到的函数内添加任何FreeRTOS的函数（整个工程基本都运行在中断内），不然将会导致程序无法运行。中断详情可见本工程STM32CubeMX的NVIC选单内。

@develop 重力补偿的theta角直接根据ch110的pitch来算即可。
@develop VOFA的串口和视觉的是同一个。