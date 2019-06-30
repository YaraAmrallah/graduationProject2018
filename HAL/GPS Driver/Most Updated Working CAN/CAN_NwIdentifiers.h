/*
 * CAN_NwIdentifiers.h
 *
 *  Created on: Apr 22, 2019
 *  Authors: Yara Amrallah, Ayman Eskander
 */

#ifndef CAN_NWIDENTIFIERS_H_
#define CAN_NWIDENTIFIERS_H_



#define  START                 0x02
#define  MIRROR1_TILT_R        0x06  // Left Mirror Receiver.
#define  MIRROR2_TILT_R        0x07  // Right Mirror Receiver.
#define  MIRROR1_TILT_T        0x06  // Left Mirror Transmitter.
#define  MIRROR2_TILT_T        0x07  // Right Mirror Transmitter.
#define  TEMP_DEG              0x0B
#define  GPS_FRAME             0x01
#define  IMU_DATA              0x04
#define  HIBERNATE             0x0A
#define  END                   0x03



#endif /* CAN_NWIDENTIFIERS_H_ */
