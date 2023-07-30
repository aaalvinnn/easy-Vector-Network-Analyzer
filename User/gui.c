/**
 * @file gui.c
 * @author zsw (zsw@foxmail.com)
 * @brief 串口屏的ui界面接口
 * @version 0.1
 * @date 2023-07-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "gui.h"

CURVED curved;
CURSOR cursor;
/*---------------------- GUI ---------------------------*/
/**
 * @brief 串口屏初始化
 * 
 */
void guiInit(void)
{
    printf("\x00\xff\xff\xff");
    return ;
}

/**
 * @brief 将计算结果转换为串口屏曲线控件的大小和数据格式,极大值映射思想
 * @brief 不准确，如幅频响应和相频响应的纵坐标不同，各需要单独的转换接口，需改！！！！！！！！！！！！！！！！
 * @param _array 
 */
void changeArraySize(double _array[NUMS])
{
    // 更新中心频率点
    calculate_CenterFrequency();
    if(curved.mode == AMP)
    {
        // 查找序列中的极大值
        double max = _array[math_result.amp_max_index];
        // double -> uint8_t
        for(int i=0;i<NUMS;i++) curved.array[i] = (int)(_array[i] / max * 255.00);
    }
    else
    {

        for(int i=0;i<NUMS;i++) curved.array[i] = (int) ((_array[i] + 90.0) / 180.0 * 255.0);
    }
    return ;
}

/**
 * @brief 切换幅频/相频曲线
 * 
 */
void changeCurveMode(void)
{
    if (curved.mode == AMP) curved.mode = PHASE;
    else    curved.mode = AMP;
    return ;
}

/**
 * @brief 打印幅频、相频响应曲线
 * 
 */
void printCurve(void)
{
    switch (curved.mode)
    {
        case AMP:
            // 纵坐标轴
            printf("t1.txt=\"0\r\n\r\n-10\r\n\r\n-20\r\n\r\n-30\r\n\r\n-40\r\n\r\n-50\r\n\r\n-60\r\n\r\n-70\r\n\r\n-80\r\n\r\n-90\r\n\r\n-100\r\n\r\n\"\xff\xff\xff");
            // 幅频曲线
            changeArraySize(math_result.amp);   // 转换数据格式，储存于curved.array中
            for(int i=0;i<NUMS;i++)
            {
                printf("add s0.id,0,%d\xff\xff\xff",curved.array[i]);
            }
            break;
        case PHASE:
            // 纵坐标轴
            printf("t1.txt=\"90\r\n\r\n75\r\n\r\n60\r\n\r\n45\r\n\r\n30\r\n\r\n15\r\n\r\n0\r\n\r\n-15\r\n\r\n-30\r\n\r\n-45\r\n\r\n-60\r\n\r\n-75\r\n\r\n-90\"\xff\xff\xff");
            // 相频曲线
            changeArraySize(math_result.phase); // 转换数据格式，储存于curved.array中
            for(int i=0;i<NUMS;i++)
            {
                printf("add s0.id,0,%d\xff\xff\xff",curved.array[i]);
            }
            break;
        default:
            break;
    }
	return ;
}

/**
 * @brief 在指定位置画一个光标
 * 
 * @param x 光标的横坐标
 * @param y 光标的纵坐标
 */
void drawCursor(uint16_t x, uint16_t y)
{
    // 光标直径：4
    printf("cir %d,%d,4,BROWN\xff\xff\xff",x+CURVE_WINDOW_X,CURVE_WINDOW_Y-y);
    return ;
}

/**
 * @brief 打印当前光标处的数据
 * 
 * @param x 光标横坐标
 * @param y 光标纵坐标
 * @param n 光标序号
 */
void printCursorData(int x, int y, uint8_t n)
{
    if(curved.mode == AMP)
    {
        printf("t2.txt=\"%d: %.1fMHz  %.2fdB\"\xff\xff\xff",n,(float)adc1256.frequency_array[x] / 1000000.0,math_result.amp[x]);
    }

    else
    {
        printf("t2.txt=\"%d: %.1fMHz  %.2f\"\xff\xff\xff",n,(float)adc1256.frequency_array[x] / 1000000.0,math_result.phase[x]);
    }
    return ;
}

/**
 * @brief 在曲线控件中开启光标模式
 * 
 * @param array 正在显示的曲线数组, 应该传curved结构体的array成员
 */
void startCursorMode(void)
{
    // 光标的初始位置
    cursor.x = calculate_CenterFrequency();
    switch (curved.mode)
    {
        case AMP:
            cursor.y = curved.array[cursor.x];
			// printf("y=%d\r\n",y);
            break;
        case PHASE:
            cursor.y = curved.array[cursor.x];
            break;
        default:
            break;
    }
    drawCursor(cursor.x,cursor.y);
    // 打印当前光标位置处的数据
    printCursorData(cursor.x,cursor.y,1);
    return ;
}

/**
 * @brief 向右移动光标，并打印当前数据属性
 * 
 */
void moveCursor_Right(void)
{
    // 边界值处理
    if ((cursor.x + cursor.step) <= 1 || (cursor.x + cursor.step) >= ( NUMS - 1))  return ;
    // 刷新光标
    REFRESH_CURSOR;
    HAL_Delay(50);  // 等待刷新
    cursor.x += cursor.step;
    switch (curved.mode)
    {
        case AMP:
            cursor.y = curved.array[cursor.x];
            break;
        case PHASE:
            cursor.y = curved.array[cursor.x];
            break;
        default:
            break;
    }
		drawCursor(cursor.x,cursor.y);
    printCursorData(cursor.x,cursor.y,1);
    return ;
}

/**
 * @brief 向左移动光标，并打印当前数据属性
 * 
 */
void moveCursor_Left(void)
{
    // 边界值处理
    if ((cursor.x - cursor.step) <= 1 || (cursor.x - cursor.step) >= (NUMS - 1))  return ;
    // 刷新光标
    REFRESH_CURSOR;
    HAL_Delay(50);  // 等待刷新
    cursor.x -= cursor.step;
    switch (curved.mode)
    {
        case AMP:
            cursor.y = curved.array[cursor.x];
            break;
        case PHASE:
            cursor.y = curved.array[cursor.x];
            break;
        default:
            break;
    }
		drawCursor(cursor.x,cursor.y);
    printCursorData(cursor.x,cursor.y,1);
    return ;
}