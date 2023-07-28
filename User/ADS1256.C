#include "ADS1256.h"

//��ʱ����
void _delay_us(uint32_t time)
{
	while(time--);
}

void ADS1256_Delay(void)
{
	for(int i=0;i<15;i++);
}

//-----------------------------------------------------------------//
//	�� �� ��: ADS1256_Write_8Bits
//����˵��: ��SPI���߷���8��bit���ݡ� ����CS���ơ�
//��    ��: _Data : ����
//�� �� ֵ: ��
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
		SCK_L;    /* <----  ADS1256 ����SCK�½��ز���DIN����,*/
		ADS1256_Delay();				 
	}
} 

//-----------------------------------------------------------------//
//	�� �� ��: ADS1256_Read_8Bits
//	����˵��: ��SPI���߽���8��bit���ݡ� ����CS���ơ�
//	��    ��: ��
//�� �� ֵ: uint8_t����
//-----------------------------------------------------------------//

uint8_t ADS1256_Read_8Bits(void)
{
	uint8_t read = 0;
	ADS1256_Delay();
	for (int i = 0; i < 8; i++)
	{	
		SCK_H ;    /* <----  ADS1256 ����SCK�������������,*/
		ADS1256_Delay();
		read = read<<1;
		SCK_L ;
		
		if (DOUT)
		{
			read++;
		}
		ADS1256_Delay();
	}
	return read;//���ض���������
}

//-----------------------------------------------------------------//
//	�� �� ����ADS1256WREG
//	��    ��: regaddr--�Ĵ�����ַ  databyte--д�������
//	�� �� ֵ: ��
//	��    ��: ��ADS1256�е�ַΪregaddr�ļĴ���д��һ���ֽ�databyte
//-----------------------------------------------------------------//
void ADS1256WREG(uint8_t regaddr,uint8_t databyte)
{

	CS_L ;//Ƭѡ���Ϳ�ʼ
	while(DRDY);//��ADS1256_DRDYΪ��ʱ����д�Ĵ���
	ADS1256_Write_8Bits(ADS1256_CMD_WREG | (regaddr & 0x0F));//��Ĵ���д�����ݵ�ַ������λΪ�������λΪ��ַ
  	ADS1256_Write_8Bits(0x00);//д�����ݵĸ���n-1,����д��1�����ݣ�����Ϊ0x00
  	ADS1256_Write_8Bits(databyte);//��regaddr��ַָ��ļĴ���д������databyte
	CS_H ;//Ƭѡ���߽���
}

//-----------------------------------------------------------------//
//	�� �� ����ADS1256RREG
//	��    ��: regaddr--�Ĵ�����ַ  databyte--д�������
//	�� �� ֵ: ��
//	��    ��: ��ADS1256�е�ַΪregaddr�ļĴ�������һ���ֽ�databyte
//-----------------------------------------------------------------//
uint8_t ADS1256RREG(uint8_t regaddr)
{
	uint8_t databyte;
	
	CS_L ;//Ƭѡ���Ϳ�ʼ
	while(DRDY);//��ADS1256_DRDYΪ��ʱ����д�Ĵ���
	ADS1256_Write_8Bits(ADS1256_CMD_WREG | (regaddr & 0x0F));//��Ĵ���д�����ݵ�ַ������λΪ�������λΪ��ַ
  ADS1256_Write_8Bits(0x00);//д�����ݵĸ���n-1,����д��1�����ݣ�����Ϊ0x00
	ADS1256_Delay();
  databyte = ADS1256_Read_8Bits();//��regaddr��ַָ��ļĴ���д������databyte
	CS_H ;//Ƭѡ���߽���
	return databyte;
}

//-----------------------------------------------------------------//
//	�� �� ����ADS1256_Init
//	��    ��: GAIN--�ɱ�����汶��  RATE--�����ٶ�
//	�� �� ֵ: ��
//	��    ��: ��ʼ��ADS1256
//-----------------------------------------------------------------//
void ADS1256_Init(uint8_t GAIN,uint8_t RATE)//��ʼ��ADS1256
{
	while(DRDY);//�ж�æµ״̬
	CS_L;//Ƭѡ����
	ADS1256_Write_8Bits(ADS1256_CMD_SELFCAL);//������У׼����
	while(DRDY);//�ж�æµ״̬
	CS_H;//Ƭѡ����

	//ADS1256WREG(ADS1256_STATUS,0x06);               // ��λ��ǰ��У׼��ʹ�û���
	ADS1256WREG(ADS1256_STATUS,0x04);               // ��λ��ǰ���Զ�У׼����ʹ�û��壨�����Ƽ���

	ADS1256WREG(ADS1256_ADCON,GAIN); // PGA�Ŵ���
	ADS1256WREG(ADS1256_DRATE,RATE); // �������ʣ�����һ���˲����ã��ɲο������ֲ�
	ADS1256WREG(ADS1256_IO,0x00);    //����IO���ã����ù�û��
	
	
	while(DRDY);//�ж�æµ״̬
	CS_L;//Ƭѡ����
	ADS1256_Write_8Bits(ADS1256_CMD_SELFCAL);//������У׼����
	while(DRDY);//�ж�æµ״̬
	CS_H;//Ƭѡ����
}             


//-----------------------------------------------------------------//
//	�� �� ����ADS1256ReadData
//	��    ��: AINP--������  AINN������ //������ΪAINN-CIOM��Ϊ�������룬ANNXΪ���
//	�� �� ֵ: ������24λ����
//	��    ��: ���ζ�ȡ����
//-----------------------------------------------------------------//
uint32_t ADS1256ReadData(uint8_t AINP,uint8_t AINN) //��ȡ����
{

	uint32_t sum=0;
  	while(DRDY);//��ADS1256_DRDYΪ��ʱ����д�Ĵ��� 
	ADS1256WREG(ADS1256_MUX,(AINP|AINN));		//����ͨ��	
	CS_L;//Ƭѡ���Ϳ�ʼ����
 	ADS1256_Write_8Bits(ADS1256_CMD_SYNC);
 	ADS1256_Write_8Bits(ADS1256_CMD_WAKEUP);
  	while(DRDY);//��ADS1256_DRDYΪ��ʱ����д�Ĵ��� 	
	ADS1256_Write_8Bits(ADS1256_CMD_RDATA);//������ʼ��ȡ��������
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
//	�� �� ����ADS1256ReadData
//	��    ��: AINP--������  AINN������ //������ΪAINN-CIOM��Ϊ�������룬ANNXΪ���  GAINΪ���õ�����
//	�� �� ֵ: ��ѹ����
//	��    ��: ���ζ�ȡ��ѹ����
//-----------------------------------------------------------------//
double ADS1256GetVolt(uint8_t AINP_,uint8_t AINN_,int GAIN) //��ȡ����
{
	double volt=0;
  	volt=ADS1256ReadData(AINP_, AINN_)*0.000000598/GAIN;
  	return volt;
}

