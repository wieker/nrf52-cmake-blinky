/*
 * soilMoistureSensor.h
 *
 *  Created on: Apr 25, 2020
 *      Author: seregia
 */

#ifndef SOILMOISTURESENSOR_H_
#define SOILMOISTURESENSOR_H_

#define BOOL uint32_t
#define TRUE 1
#define FALSE 0
#define HIGH 1
#define LOW 0

void soilMoistureSensor_setup();
BOOL soilMoistureSensor_hasWater();

#endif /* SOILMOISTURESENSOR_H_ */
