#ifndef __DEBUG_H
#define __DEBUG_H

#include <Wire.h>

#define USE_DEBUG 1
#if USE_DEBUG
	#define Debug(__info) Serial.println(__info)
#else
	#define Debug(__info)  
#endif

#endif

