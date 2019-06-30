/*
 * CAN_cfg.h
 *
 *  Created on: Feb 28, 2019
 *      Authors: Yara Amrallah, Ayman Eskander
 */

#ifndef CAN_CFG_H_
#define CAN_CFG_H_

#include "CAN.h"

/* Available CAN group number */
#define CAN_GROUPS_NUMBER 1u

/* System Frequency in MegaHertz */
#define SYSTEM_FREQUENCY 16u

/* Minimum Baud Rate in Kbps */
#define MINIMUM_BAUD_RATE 1u

/* Maximum Baud Rate in Kbps */
#define MAXIMUM_BAUD_RATE 1000u

/*Auto-retransmission of disturbed messages */
#define AUTO_RETRANSMISSION  ENABLE

/* Define message objects number */
#define TOTAL_MESSAGE_OBJECT_NUMBER 7u

/* Define maximum message object number */
/* N.B. The total number of message objects should not exceed 32. */
#define MAXIMUM_MESSAGE_OBJECT_NUMBER 32u

/* Use 2u to use the additional message objects functions of CAN0
 *     4u to use the additional message objects functions of CAN1 (with CAN0 additional functions all enabled)
 *     0u to use neither of them
 */
#define RESERVED_OBJECTS_NUMBER 2u

#endif
/* CAN_CFG_H_ */
