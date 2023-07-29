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

#define REFRESH printf("ref s0\xff\xff\xff")    // 刷新曲线控件

// 曲线控件显示模式
enum Curvedmode{
    AMP,
    PHASE
};

// 曲线控件对象
typedef struct Curved{
    uint8_t array[NUMS];
    enum Curvedmode mode;
}CURVED;

/*-----------DECLARTION-------------*/
extern CURVED curved;

void guiInit(void);
void printCurve(void);
void startCursorMode(RESULT _result);

#endif