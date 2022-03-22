# STM32_FRAME
Core ：STM32GO31K8
GAUGE: BQ40Z50
DCDC : SC8886


PIN的定义如下：

SMBus:      PA4  PA5     	SCL   SDA
IIC master: PA11 PA12       SCL   SDA
IIC slave:  PB6  PB9        SCL   SDA

GPIO:  	    PB5  PC6	    EXIT  LED
ADC:		PA6  PA7
USART1:     PA9  PA10       TX    RX 

LED Display：PB0 PB1 PB2 PB3

/**硬件层特殊说明**/

IIC 和SMBus总线，在layout时，尽量和功率线远离，可采用GND线分层处理，并且在IIC总线上加上2个 100Ω电阻作为滤波避免干扰
总线设计时，需带上2个上拉电阻，阻值实际上是1-10K均可，目前设计实际上采用的是推挽输出，但按官方文档，理论上采用开漏输出


/**BQ40Z50特殊说明**/
在进行电量计交互时，模拟SMBus出现同样的硬件平台，不同板子的情况，有的电量计能交互，有的不行，待处理，感觉是时序问题。
可能后续还是需要改为硬件SMBus

BQ40Z50在上电时，如果接线异常的情况下，可能出现电池低电判断，导致永久保护，出现SOV故障,MOS关闭，需使用上位机将DXCG 和 DCCG mos手动打开，并且关闭错误