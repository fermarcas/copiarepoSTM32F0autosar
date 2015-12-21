/**
 ******************************************************************************
 * @file    StdDef.h
 * @author  Bernhard Berger
 * @version V1.0.0
 * @date    17.11.2014
 * @brief   Standard definitions for all modules.
 *
 *
 * (c) Research Group Embedded Systems, FH OÖ Forschungs und EntwicklungsgmbH
 * You may not use or copy this file except with written permission of FH OÖ
 ******************************************************************************
 */

#ifndef STD_DEF_H
#define STD_DEF_H

#include <stdint.h>

typedef uint8_t BOOL;

#define FALSE               0
#define TRUE                1

#define NULL_PTR        	  ((void*)0)

#define UNUSED_PARAM(x)     x = x; if(x == 0) {}

#endif
