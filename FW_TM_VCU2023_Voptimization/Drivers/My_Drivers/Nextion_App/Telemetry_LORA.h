/*
 * Telemetri.h
 *
 *  Created on: Mar 31, 2023
 *      Author: yakup
 */

#ifndef MY_DRIVERS_TELEMETRY_APP_TELEMETRY_LORA_H_
#define MY_DRIVERS_TELEMETRY_APP_TELEMETRY_LORA_H_

#include "string.h"
#include "stdio.h"
#include "Canbus.h"






extern uint8_t	CAN_ID;
extern uint8_t CanRX[8];
extern UART_HandleTypeDef huart1;





void LORA_Send_Together_DATAS(void);
void LORA_Incomming_Packet_ID_From_CANBUS(uint8_t CAN_ID);
void Create_BMSMsgPacket_For_LORA(void);
void Create_BCM_MsgPacket_For_LORA(void);
void Create_SPEED_MsgPacket_For_LORA(void);
void Create_CURRENT_MsgPacket_For_LORA(void);




#endif /* MY_DRIVERS_TELEMETRY_APP_TELEMETRY_LORA_H_ */
