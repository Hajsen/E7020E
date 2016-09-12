

/*
 * remoteSwitchController.h
 *
 *  Created on: Mar 22, 2016
 *      Author: davbom-2
 */

#ifndef REMOTESWITCHCONTROLLER_H_
#define REMOTESWITCHCONTROLLER_H_

int getRemoteSwitchState();
void setRemoteSwitchOFFState();
void setRemoteSwitchONState();
void OrderTransceiver(char * ID, int Status);
void setMotionRemoteSwitch(char * ID, int Status);

#define ON 1
#define OFF 0
#endif /* REMOTESWITCHCONTROLLER_H_ */
