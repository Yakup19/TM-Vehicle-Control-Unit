/*
 * Nextion.h
 *
 *  Created on: Mar 30, 2023
 *      Author: yakup
 */

/**
 * @def MY_DRIVERS_NEXTION_APP_NEXTION_H_
 * @brief
 *
 */
#ifndef MY_DRIVERS_NEXTION_APP_NEXTION_H_
#define MY_DRIVERS_NEXTION_APP_NEXTION_H_

#include "main.h"
#include "string.h"
#include "stdio.h"

#define NEXTION

#define NEXTION_BUFFER_SIZE	8
extern uint8_t	CAN_ID;
extern uint8_t CanRX[8];
extern UART_HandleTypeDef huart2;
typedef enum{
	BMS_ID = 0x27,
	VCU_ID=0x36,
	SPEED_ID=0x50,
	BCM_ID=0x23,
	Current_ID=0x30
}MsgID_Typedef;

void Send_BMSMsgPacket_To_Nextion(void);
void Create_BMSMsgPacket_For_Nextion(void);
void Nextion_Incomming_Packet_ID_From_CANBUS(uint8_t CAN_ID);
#endif /* MY_DRIVERS_NEXTION_APP_NEXTION_H_ */
