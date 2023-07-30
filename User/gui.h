/**
 * @file gui.h
 * @author zsw (aaalvinnn@foxmail.com)
 * @brief gui.c的头文件
 * @version 0.1
 * @date 2023-07-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef _GUI_H
#define _GUI_H

#include "app.h"
#include "stdio.h"

#define REFRESH_CURSOR printf("ref s0\xff\xff\xff")    // 去除原有的光标
#define REFRESH_WINDOW printf("cle s0.id,0\xff\xff\xff")
// 曲线窗口的属性
#define CURVE_WINDOW_X  22
#define CURVE_WINDOW_Y  265

// 曲线控件显示模式
enum Curvedmode{
    AMP = 0,
    PHASE
};

// 曲线控件对象
typedef struct Curved{
    uint8_t array[NUMS];
    enum Curvedmode mode;   // 初始默认幅频曲线
}CURVED;

/*-----------DECLARTION-------------*/
extern CURVED curved;

void guiInit(void);
void printCurve(void);
void changeCurveMode(void);
void startCursorMode(void);

#endif