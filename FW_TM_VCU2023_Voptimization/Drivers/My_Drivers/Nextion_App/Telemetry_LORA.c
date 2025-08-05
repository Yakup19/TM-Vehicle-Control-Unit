 /*
 * Telemetri.c
 *
 *  Created on: Mar 31, 2023
 *      Author: yakup
 */
#include "Telemetry_LORA.h"

/**************	TELEMETRİ VERİ PARAMETRELERİ ***************/
//static char *uzun_far_t = "UF";
//static char *dortlu_t = "DO";
//static char *sol_sin = "LE";
//static char *sag_sin = "RI";
//static char *hiz_t = "HI";
//static char *akim_t = "CU";
//static char *max_temp_t = "TM";
//static char *max_motor = "MO";
//static char *max_m_surucu = "MS";
//static char *total_volt_t = "VT";
//static char *total_SOC_t = "SO";
extern uint32_t sd_card[5];

#define LORA_RECEIVER_HIGH	(char) 0x27
#define LORA_RECEIVER_LOW	(char) 0x27
/******** DİZİ PARAMMETRELERİ **********/

//char LORA_Sending_Buffer[2]=(char) LORA_RECEIVER_HIGH;
//char LORA_Sending_Buffer[3]=(char) LORA_RECEIVER_LOW;
uint8_t LORA_Instant_Buffer[200] = { 0 }; // GELEN VERİLER BU DİZİYE KAYIT EDİLİLYOR
char LORA_BMS_Buffer[100] = { '\0' };   // BMS BUFFER
char LORA_BCM_Buffer[25] = { '1',',','1',',','1',',','1',',','\0' };    // BCM BUFER
char LORA_SPEED_Buffer[10] = { '0',',','\0' };  // HIZ BUFFER
char LORA_CURRENT_Buffer[10] = { '0','\0' };  // AKIM BUFFERI
char xchar[2]={'x','\0'};
char dotchar[2]={',','\0'};

char LORA_Sending_Buffer[200];

void Create_BMS_MsgPacket_For_LORA(void) {
	switch (CanRX[0]) {
	case 1:
		//Max,min,ort,motor,sürücü temp
		for (int i = 0; i < 7; i++) {
			LORA_Instant_Buffer[i] = CanRX[i + 1];
		}
		break;
	case 2:
		//total volt, soc ,battery voltage 1-5
		for (int i = 0; i < 7; i++) {
			LORA_Instant_Buffer[i + 5] = CanRX[i + 1];
		}
		break;
	case 3:
		//battery voltage 6-12
		for (int i = 0; i < 7; i++) {
			LORA_Instant_Buffer[i + 12] = CanRX[i + 1];
		}
		break;
	case 4:
		//battery voltage 13-20
		for (int i = 0; i < 7; i++) {
			LORA_Instant_Buffer[i + 19] = CanRX[i + 1];
		}
		break;
	case 5:  // battery voltage 21-24
		for (int i = 0; i < 6; i++) {
			LORA_Instant_Buffer[i + 26] = CanRX[i + 1];
		}
		break;
	}
	sprintf(LORA_BMS_Buffer,
			"%u,0,0,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,",
			LORA_Instant_Buffer[0],LORA_Instant_Buffer[3],
			LORA_Instant_Buffer[4], LORA_Instant_Buffer[5],
			LORA_Instant_Buffer[6], LORA_Instant_Buffer[7],
			LORA_Instant_Buffer[8], LORA_Instant_Buffer[9],
			LORA_Instant_Buffer[10], LORA_Instant_Buffer[11],
			LORA_Instant_Buffer[12], LORA_Instant_Buffer[13],
			LORA_Instant_Buffer[14], LORA_Instant_Buffer[15],
			LORA_Instant_Buffer[16], LORA_Instant_Buffer[17],
			LORA_Instant_Buffer[18], LORA_Instant_Buffer[19],
			LORA_Instant_Buffer[20], LORA_Instant_Buffer[21],
			LORA_Instant_Buffer[22], LORA_Instant_Buffer[23],
			LORA_Instant_Buffer[24], LORA_Instant_Buffer[25],
			LORA_Instant_Buffer[26], LORA_Instant_Buffer[27],
			LORA_Instant_Buffer[28],LORA_Instant_Buffer[29],
			LORA_Instant_Buffer[30]/*LORA_Instant_Buffer[31]*/);

}
void Create_BCM_MsgPacket_For_LORA(void) {
//Sırası ile Sağ Sinyal, Sol sinyal, Dortlu, Uzunlar
	for (int i = 0; i < 5; i++) {
		LORA_Instant_Buffer[i] = CanRX[i];
	}
	sprintf(LORA_BCM_Buffer, "%u,%u,%u,%u,", LORA_Instant_Buffer[1],
			LORA_Instant_Buffer[2], LORA_Instant_Buffer[3],
			LORA_Instant_Buffer[4]);
}
void Create_SPEED_MsgPacket_For_LORA(void) {
	LORA_Instant_Buffer[0] = CanRX[0];
	sprintf(LORA_SPEED_Buffer, "%u,",  LORA_Instant_Buffer[0]);
}
void Create_CURRENT_MsgPacket_For_LORA(void) {
	uint32_t CURRENT = ((CanRX[0] * 256) + (CanRX[1] * 1000)
			+ (CanRX[2] * 256) + CanRX[3]);
	CURRENT=CURRENT/1000;
	sprintf(LORA_CURRENT_Buffer, "%lu",  CURRENT);
}
void LORA_Incomming_Packet_ID_From_CANBUS(uint8_t CAN_ID) {
	switch (CAN_ID) {
	case BMS_ID:
		Create_BMS_MsgPacket_For_LORA();

		break;
	case BCM_ID:
		Create_BCM_MsgPacket_For_LORA();

		break;
	case SPEED_ID:
		Create_SPEED_MsgPacket_For_LORA();

		break;
	case Current_ID:
		Create_CURRENT_MsgPacket_For_LORA();

		break;
	}

}

void LORA_Send_Together_DATAS(void) {
	uint32_t length = 0;
	char LORA_Adress[3]={'\0'};
	LORA_Adress[0] = (char) 0x05;
	LORA_Adress[1] = LORA_RECEIVER_HIGH;
	LORA_Adress[2] = LORA_RECEIVER_LOW;
	LORA_Adress[3] = '\0';

//	strcat(LORA_Sending_Buffer, LORA_BCM_Buffer);
//	strcat(LORA_Sending_Buffer, LORA_BMS_Buffer);
//	strcat(LORA_Sending_Buffer, LORA_CURRENT_Buffer);
//	strcat(LORA_Sending_Buffer, LORA_SPEED_Buffer);
//	int n = sizeof(LORA_Sending_Buffer)/ sizeof(LORA_Sending_Buffer[0]);
//	for(int i =3 ; i<=n;i++)
//	{
//		LORA_Sending_Buffer[i]= LORA_Sending_Buffer[i+1];
//	}

	sprintf(LORA_Sending_Buffer, "%s%s%s%s%s%s", LORA_Adress, LORA_BCM_Buffer,
			LORA_SPEED_Buffer, LORA_BMS_Buffer, LORA_CURRENT_Buffer, xchar);
	length = strlen(LORA_Sending_Buffer);

	LORA_Sending_Buffer[0] = (char) 0x00;
	HAL_UART_Transmit(&huart1, (uint8_t*) LORA_Sending_Buffer,
			(uint16_t) (length), 500);
	memset(LORA_Sending_Buffer, '\0', 200);
}
