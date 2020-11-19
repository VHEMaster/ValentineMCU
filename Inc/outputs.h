/*
 * outputs.h
 *
 *  Created on: 18 нояб. 2020 г.
 *      Author: VHEMaster
 */

#ifndef OUTPUTS_H_
#define OUTPUTS_H_

#include "main.h"


extern void out_main(void);
extern void out_batterylow(void);
extern void out_charging(void);

extern void out_updatecharginglevel(float level);

#endif /* OUTPUTS_H_ */
