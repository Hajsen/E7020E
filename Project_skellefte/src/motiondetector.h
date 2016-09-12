/*
 * motiondetector.h
 *
 *  Created on: Mar 7, 2016
 *      Author: davbom-2
 */

#ifndef MOTIONDETECTOR_H_
#define MOTIONDETECTOR_H_

#define ON 1
#define OFF 0

//motiondetector at PA5 collides with led on nucleo board.
// using PA4 for testing purpose
//void initGPIOmotionDetector();
void initTimerMotionDetector();
void Configure_PA5(void);
int getMotionLogStatus();
void setMotionLogStatus(int status);

#endif /* MOTIONDETECTOR_H_ */
