#include "ADS1256.h"

//延时函数
void _delay_us(uint32_t time)
{
	while(time--);
}

void ADS1256_Delay(void)
{
	for(int i=0;i<15;i++);
	// uint32_t delay = (HAL_RCC_GetHCLKFreq() / 4000000 * 1);		// 这里us = 1
  //   while (delay--)
	// {
	// 	;
	// }
}

//-----------------------------------------------------------------//
//	函 数 名: ADS1256_Write_8Bits
//功能说明: 向SPI总线发送8个bit数据。 不带CS控制。
//形    参: _Data : 数据
//返 回 值: 无
//-----------------------------------------------------------------//

void ADS1256_Write_8Bits(uint8_t Data)
{
	
	ADS1256_Delay();	
	for(int i = 0; i < 8; i++)
	{
		if (Data & 0x80)
			DIN_H ;
		else
			DIN_L ;
		SCK_H;	
		ADS1256_Delay();
		Data <<= 1;
		SCK_L;    /* <----  ADS1256 是在SCK下降沿采样DIN数据,*/
		ADS1256_Delay();				 
	}
} 

//-----------------------------------------------------------------//
//	函 数 名: ADS1256_Read_8Bits
//	功能说明: 从SPI总线接收8个bit数据。 不带CS控制。
//	形    参: 无
//返 回 值: uint8_t数据
//-----------------------------------------------------------------//

uint8_t ADS1256_Read_8Bits(void)
{
	uint8_t read = 0;
	ADS1256_Delay();
	for (int i = 0; i < 8; i++)
	{	
		SCK_H ;    /* <----  ADS1256 是在SCK上升沿输出数据,*/
		ADS1256_Delay();
		read = read<<1;
		SCK_L ;
		
		if (DOUT)
		{
			read++;
		}
		ADS1256_Delay();
	}
	return read;//返回读到的数据
}

//-----------------------------------------------------------------//
//	函 数 名：ADS1256WREG
//	形    参: regaddr--寄存器地址  databyte--写入的数据
//	返 回 值: 无
//	描    述: 向ADS1256中地址为regaddr的寄存器写入一个字节databyte
//-----------------------------------------------------------------//
void ADS1256WREG(uint8_t regaddr,uint8_t databyte)
{

	CS_L ;//片选拉低开始
	while(DRDY);//当ADS1256_DRDY为低时才能写寄存器
	ADS1256_Write_8Bits(ADS1256_CMD_WREG | (regaddr & 0x0F));//向寄存器写入数据地址，高四位为命令，低四位为地址
  	ADS1256_Write_8Bits(0x00);//写入数据的个数n-1,我们写入1个数据，所以为0x00
  	ADS1256_Write_8Bits(databyte);//向regaddr地址指向的寄存器写入数据databyte
	CS_H ;//片选拉高结束
}

//-----------------------------------------------------------------//
//	函 数 名：ADS1256RREG
//	形    参: regaddr--寄存器地址  databyte--写入的数据
//	返 回 值: 无
//	描    述: 向ADS1256中地址为regaddr的寄存器读入一个字节databyte
//-----------------------------------------------------------------//
uint8_t ADS1256RREG(uint8_t regaddr)
{
	uint8_t databyte;
	
	CS_L ;//片选拉低开始
	while(DRDY);//当ADS1256_DRDY为低时才能写寄存器
	ADS1256_Write_8Bits(ADS1256_CMD_WREG | (regaddr & 0x0F));//向寄存器写入数据地址，高四位为命令，低四位为地址
  ADS1256_Write_8Bits(0x00);//写入数据的个数n-1,我们写入1个数据，所以为0x00
	ADS1256_Delay();
  databyte = ADS1256_Read_8Bits();//向regaddr地址指向的寄存器写入数据databyte
	CS_H ;//片选拉高结束
	return databyte;
}

//-----------------------------------------------------------------//
//	函 数 名：ADS1256_Init
//	形    参: GAIN--可编程增益倍数  RATE--采样速度
//	返 回 值: 无
//	描    述: 初始化ADS1256
//-----------------------------------------------------------------//
void ADS1256_Init(uint8_t GAIN,uint8_t RATE)//初始化ADS1256
{
	while(DRDY);//判断忙碌状态
	CS_L;//片选拉低
	ADS1256_Write_8Bits(ADS1256_CMD_SELFCAL);//发送自校准命令
	while(DRDY);//判断忙碌状态
	CS_H;//片选拉高

	//ADS1256WREG(ADS1256_STATUS,0x06);               // 高位在前、校准、使用缓冲
	ADS1256WREG(ADS1256_STATUS,0x04);               // 高位在前、自动校准、不使用缓冲（个人推荐）

	ADS1256WREG(ADS1256_ADCON,GAIN); // PGA放大倍数
	ADS1256WREG(ADS1256_DRATE,RATE); // 数据速率，具有一定滤波作用，可参考数据手册
	ADS1256WREG(ADS1256_IO,0x00);    //数字IO设置，不用管没用
	
	
	while(DRDY);//判断忙碌状态
	CS_L;//片选拉低
	ADS1256_Write_8Bits(ADS1256_CMD_SELFCAL);//发送自校准命令
	while(DRDY);//判断忙碌状态
	CS_H;//片选拉高
}             


//-----------------------------------------------------------------//
//	函 数 名：ADS1256ReadData
//	形    参: AINP--正输入  AINN负输入 //负输入为AINN-CIOM则为单端输入，ANNX为差分
//	返 回 值: 读出的24位数据
//	描    述: 单次读取数据
//-----------------------------------------------------------------//
uint32_t ADS1256ReadData(uint8_t AINP,uint8_t AINN) //读取数据
{

	uint32_t sum=0;
  	while(DRDY);//当ADS1256_DRDY为低时才能写寄存器 
	ADS1256WREG(ADS1256_MUX,(AINP|AINN));		//设置通道	
	CS_L;//片选拉低开始传输
 	ADS1256_Write_8Bits(ADS1256_CMD_SYNC);
 	ADS1256_Write_8Bits(ADS1256_CMD_WAKEUP);
  	while(DRDY);//当ADS1256_DRDY为低时才能写寄存器 	
	ADS1256_Write_8Bits(ADS1256_CMD_RDATA);//发出开始读取数据命令
	ADS1256_Delay();
  	sum |= (ADS1256_Read_8Bits() << 16);
	sum |= (ADS1256_Read_8Bits() << 8);
	sum |= ADS1256_Read_8Bits();
	CS_L;
	if (sum>0x7FFFFF)           // if MSB=1, 
	{
		sum -= 0x1000000;       // do 2's complement
	}
    return sum;
}


//-----------------------------------------------------------------//
//	函 数 名：ADS1256ReadData
//	形    参: AINP--正输入  AINN负输入 //负输入为AINN-CIOM则为单端输入，ANNX为差分  GAIN为设置的增益
//	返 回 值: 电压数据
//	描    述: 单次读取电压数据
//-----------------------------------------------------------------//
double ADS1256GetVolt(uint8_t AINP_,uint8_t AINN_,int GAIN) //读取数据
{
	double volt=0;
  	volt=ADS1256ReadData(AINP_, AINN_)*0.000000598/GAIN;
  	return volt;
}


