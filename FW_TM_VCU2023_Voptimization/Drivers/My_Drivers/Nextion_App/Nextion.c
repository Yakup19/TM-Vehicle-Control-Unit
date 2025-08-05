/*
 * Nextion.c
 *
 *  Created on: Mar 30, 2023
 *      Author: yakup
 */

#include "Nextion.h"

/** Nextion hazır kalıplar..
 */
static char *dortlu = "D=";
static char *kisa_far = "K=";
static char *uzun_far = "U=";
static char *nex_sol_sinyal = "L=";
static char *nex_sag_sinyal = "R=";
static char *H_BMS = "HBMS=";
static char *H_BCM = "HBCM=";
static char *H_VCU = "H_V=";
static char *Nextion_Speed = "H=";
static char *Nextion_Current = "A=";
char *Nextion_Battery_Total_Voltage = "totalvolt=";
static char *Nextion_Battery_SOC = "SO=";

static char *Nextion_Battery_Max_Temperature = "TBAT=";
char *Nextion_Motor_Temperature = "TMOT=";
char *Nextion_Driver_Temperature = "TMOS=";
/********************* Değişkenler***************/
uint8_t far_stat = 0;
uint8_t dortlu_stat = 0;
uint8_t sinyal_stat = 0;
uint8_t Nextion_Buffer[NEXTION_BUFFER_SIZE];
uint8_t Nextion_BMS_Buffer[33];
uint8_t speed=0;
uint32_t current=0;
char Sending_Buffer_For_Nextion[50];
uint8_t Nextion_Ending[3] = { 0xFF, 0XFF, 0XFF };
uint8_t fault[6]={0};
extern uint32_t sd_card[5];

void Send_BMSMsgPacket_To_Nextion(void) {
	uint32_t written_bytes;

	switch (CanRX[0]) {
	case 1:
		written_bytes = sprintf(Sending_Buffer_For_Nextion,
				"%s%u%s%s%u%s%s%u%s", Nextion_Battery_Max_Temperature,
				Nextion_BMS_Buffer[0], (char*) Nextion_Ending,
				Nextion_Motor_Temperature, Nextion_BMS_Buffer[3],
				(char*) Nextion_Ending, Nextion_Driver_Temperature,
				Nextion_BMS_Buffer[4], (char*) Nextion_Ending);
		HAL_UART_Transmit(&huart2, (uint8_t*) Sending_Buffer_For_Nextion,
				written_bytes, 100);
		sd_card[2]=Nextion_BMS_Buffer[0];//MAKS BATARYA SICAKLIĞI
		break;
	case 2:
		written_bytes = sprintf(Sending_Buffer_For_Nextion,
				"%s%u%s%s%u%sV1=%u%sV2=%u%sV3=%u%sV4=%u%sV5=%u%s",
				Nextion_Battery_Total_Voltage, Nextion_BMS_Buffer[5],
				(char*) Nextion_Ending, Nextion_Battery_SOC,
				Nextion_BMS_Buffer[6], (char*) Nextion_Ending,
				Nextion_BMS_Buffer[7], (char*) Nextion_Ending,
				Nextion_BMS_Buffer[8], (char*) Nextion_Ending,
				Nextion_BMS_Buffer[9], (char*) Nextion_Ending,
				Nextion_BMS_Buffer[10], (char*) Nextion_Ending,
				Nextion_BMS_Buffer[11], (char*) Nextion_Ending);
		HAL_UART_Transmit(&huart2, (uint8_t*) Sending_Buffer_For_Nextion,
				written_bytes, 100);
		sd_card[3]=Nextion_BMS_Buffer[5];//toplam voltaj
		sd_card[4]=(Nextion_BMS_Buffer[5]*(1218));//batarya kalan enerji hesabı (24S 10P pil bankası)
		break;
	case 3:
		written_bytes = sprintf(Sending_Buffer_For_Nextion,
				"V6=%u%sV7=%u%sV8=%u%sV9=%u%sVA=%u%sVB=%u%sVC=%u%s",
				Nextion_BMS_Buffer[12], (char*) Nextion_Ending,
				Nextion_BMS_Buffer[13], (char*) Nextion_Ending,
				Nextion_BMS_Buffer[14], (char*) Nextion_Ending,
				Nextion_BMS_Buffer[15], (char*) Nextion_Ending,
				Nextion_BMS_Buffer[16], (char*) Nextion_Ending,
				Nextion_BMS_Buffer[17], (char*) Nextion_Ending,
				Nextion_BMS_Buffer[18], (char*) Nextion_Ending);
		HAL_UART_Transmit(&huart2, (uint8_t*) Sending_Buffer_For_Nextion,
				written_bytes, 100);
		break;
	case 4:
		written_bytes = sprintf(Sending_Buffer_For_Nextion,
				"VD=%u%sVE=%u%sVF=%u%sVG=%u%sVH=%u%sVI=%u%sVJ=%u%s",
				Nextion_BMS_Buffer[19], (char*) Nextion_Ending,
				Nextion_BMS_Buffer[20], (char*) Nextion_Ending,
				Nextion_BMS_Buffer[21], (char*) Nextion_Ending,
				Nextion_BMS_Buffer[22], (char*) Nextion_Ending,
				Nextion_BMS_Buffer[23], (char*) Nextion_Ending,
				Nextion_BMS_Buffer[24], (char*) Nextion_Ending,
				Nextion_BMS_Buffer[25], (char*) Nextion_Ending);
		HAL_UART_Transmit(&huart2, (uint8_t*) Sending_Buffer_For_Nextion,
				written_bytes, 100);
		break;
	case 5:
		written_bytes = sprintf(Sending_Buffer_For_Nextion,
				"VK=%u%sVL=%u%sVM=%u%sVN=%u%sVO=%u%s", Nextion_BMS_Buffer[26],
				(char*) Nextion_Ending, Nextion_BMS_Buffer[27],
				(char*) Nextion_Ending, Nextion_BMS_Buffer[28],
				(char*) Nextion_Ending, Nextion_BMS_Buffer[29],
				(char*) Nextion_Ending, Nextion_BMS_Buffer[30],
				(char*) Nextion_Ending);
		HAL_UART_Transmit(&huart2, (uint8_t*) Sending_Buffer_For_Nextion,
				written_bytes, 100);
		break;
	}

}

void Create_BMSMsgPacket_For_Nextion(void) {
	switch (CanRX[0]) {
	case 1:
		//Max,min,ort,motor,sürücü temp
		for (uint8_t i = 0; i < 5; i++) {
			Nextion_BMS_Buffer[i] = CanRX[i + 1];

		}
		Send_BMSMsgPacket_To_Nextion();
		sd_card[2]=Nextion_BMS_Buffer[0];
		break;
	case 2:
		//total volt, soc ,battery voltage 1-5
		for (uint8_t i = 0; i < 7; i++) {
			Nextion_BMS_Buffer[i + 5] = CanRX[i + 1];
		}
		Send_BMSMsgPacket_To_Nextion();
		sd_card[3]=Nextion_BMS_Buffer[5];

		break;
	case 3:
		//battery voltage 6-12
		for (uint8_t i = 0; i < 7; i++) {
			Nextion_BMS_Buffer[i + 12] = CanRX[i + 1];
		}
		Send_BMSMsgPacket_To_Nextion();
		break;
	case 4:
		//battery voltage 13-20
		for (uint8_t i = 0; i < 7; i++) {
			Nextion_BMS_Buffer[i + 19] = CanRX[i + 1];
		}
		Send_BMSMsgPacket_To_Nextion();
		break;
	case 5:
		for (uint8_t i = 0; i < 7; i++) {
			Nextion_BMS_Buffer[i + 26] = CanRX[i + 1];
		}
		Send_BMSMsgPacket_To_Nextion();
		break;
	}

}
void nextion_kisalar(void) {
	//kısa far durumu gönderiliyor..
	sprintf(Sending_Buffer_For_Nextion, "%s%c%s", kisa_far, far_stat, (char*)Nextion_Ending);
	HAL_UART_Transmit(&huart2, (uint8_t*) Sending_Buffer_For_Nextion, strlen(Sending_Buffer_For_Nextion), 100);
}

void nextion_uzunlar(void) {
	//uzun far durumu gönderiliyor..
	sprintf(Sending_Buffer_For_Nextion, "%s%c%s", uzun_far, far_stat, (char*)Nextion_Ending);
	//HAL_UART_Transmit(&huart2, (uint8_t*) Sending_Buffer_For_Nextion, strlen(Sending_Buffer_For_Nextion), 100);
}

void nextion_dortlu(void) {
	//Dörtlülerin durumu gönderiliyor..
	sprintf(Sending_Buffer_For_Nextion, "%s%c%s", dortlu, dortlu_stat,(char*)Nextion_Ending);
	HAL_UART_Transmit(&huart2, (uint8_t*) Sending_Buffer_For_Nextion, strlen(Sending_Buffer_For_Nextion), 100);
}

void nextion_sol_sinyal(void) {
	//Sol sinyal gönderiliyor..
	sprintf(Sending_Buffer_For_Nextion, "%s%c%s",nex_sag_sinyal , sinyal_stat, (char*)Nextion_Ending);
	HAL_UART_Transmit(&huart2, (uint8_t*) Sending_Buffer_For_Nextion, strlen(Sending_Buffer_For_Nextion), 100);
}

void nextion_sag_sinyal(void) {
	//Sağ sinyal gönderiliyor..
	sprintf(Sending_Buffer_For_Nextion, "%s%c%s", nex_sol_sinyal, sinyal_stat, (char*)Nextion_Ending);
	HAL_UART_Transmit(&huart2, (uint8_t*) Sending_Buffer_For_Nextion, strlen(Sending_Buffer_For_Nextion), 100);
}

void Create_BCM_MsgPacket_For_Nextion() {
	for (int i = 0; i < 8; i++) {
		switch (i) {
		case 0:
			if (CanRX[0] == 1) {
				//Sağ Sinyal aktif
				sinyal_stat = 49;
				nextion_sol_sinyal();

			} else {
				sinyal_stat = 48;
				nextion_sol_sinyal();
			}
			break;
		case 1:
			if (CanRX[1] == 1) {
				//Sol Sinyal aktif
				sinyal_stat = 49;
				nextion_sag_sinyal();

			} else {
				sinyal_stat = 48;
				nextion_sag_sinyal();

			}
			break;
		case 2:
			if (CanRX[2] == 1) {
				//Dörtlü Aktif
				dortlu_stat = 49;
				nextion_dortlu();
			} else {
				dortlu_stat = 48;
				nextion_dortlu();
			}
			break;
		case 3:
			if (CanRX[3] == 1) {
				//Far aktif
				far_stat = 49;
				nextion_uzunlar();
			} else {
				far_stat = 48;
				nextion_uzunlar();
			}
		case 4:
			if (CanRX[4] == 1) {

			}
		default:
			break;
		}

	}
}
void Create_SPEED_MsgPacket_For_Nextion() {
	speed = CanRX[0];
	sd_card[1]=speed;
		sprintf(Sending_Buffer_For_Nextion, "%s%u%s", Nextion_Speed, speed, Nextion_Ending);
		HAL_UART_Transmit(&huart2, (uint8_t*) Sending_Buffer_For_Nextion, strlen(Sending_Buffer_For_Nextion), 100);


}
void Create_CURRENT_MsgPacket_For_Nextion() {
	current = ((CanRX[0] * 256) + (CanRX[1] * 1000) + (CanRX[2] * 256) + CanRX[3]);
	current = current/1000;
		/*Nextion Ekrana veri basılıyor..*/

		sprintf(Sending_Buffer_For_Nextion, "%s%lu%s", Nextion_Current, current, Nextion_Ending);
		HAL_UART_Transmit(&huart2, (uint8_t*) Sending_Buffer_For_Nextion, strlen(Sending_Buffer_For_Nextion), 100);

}
void Nextion_Fault() {
	//VCU İletişim Hatası Yok
	sprintf(Sending_Buffer_For_Nextion, "%s%d%s", H_VCU, 1, Nextion_Ending);
	HAL_UART_Transmit(&huart2, (uint8_t*) Sending_Buffer_For_Nextion,
			strlen(Sending_Buffer_For_Nextion), 100);

	//Hata sinyal gönderiliyor..
	if (fault[2] == 1) {
		//BMS İletişim Hatası
		sprintf(Sending_Buffer_For_Nextion, "%s%d%s", H_BMS, 1, Nextion_Ending);
		HAL_UART_Transmit(&huart2, (uint8_t*) Sending_Buffer_For_Nextion,
				strlen(Sending_Buffer_For_Nextion), 100);
	} else {
		sprintf(Sending_Buffer_For_Nextion, "%s%d%s", H_BMS, 0, Nextion_Ending);
		HAL_UART_Transmit(&huart2, (uint8_t*) Sending_Buffer_For_Nextion,
				strlen(Sending_Buffer_For_Nextion), 100);
		//Hata sinyal gönderiliyor..
		if (fault[0] == 1) {
			//BCM İletişim Hatası
			sprintf(Sending_Buffer_For_Nextion, "%s%d%s", H_BCM, 1,
					Nextion_Ending);
			HAL_UART_Transmit(&huart2, (uint8_t*) Sending_Buffer_For_Nextion,
					strlen(Sending_Buffer_For_Nextion), 100);
		} else {
			sprintf(Sending_Buffer_For_Nextion, "%s%d%s", H_BCM, 0,
					Nextion_Ending);
			HAL_UART_Transmit(&huart2, (uint8_t*) Sending_Buffer_For_Nextion,
					strlen(Sending_Buffer_For_Nextion), 100);
		}

	}
}


void Nextion_Incomming_Packet_ID_From_CANBUS(uint8_t CAN_ID) {
	switch (CAN_ID) {
	case BMS_ID:
		Create_BMSMsgPacket_For_Nextion();/*TÜM BMS KODLARI TAMAMLANDI*/
		Nextion_Fault();
		break;
	case VCU_ID:
		Nextion_Fault();
		break;
	case BCM_ID:
		Create_BCM_MsgPacket_For_Nextion();/*KODLARI TAMAMLANDI TEST EDİLECEK*/
		break;
	case SPEED_ID:
		Create_SPEED_MsgPacket_For_Nextion();/*KODLARI TAMAMLANDI TEEST EDİLECEK*/
		break;
	case Current_ID:
		Create_CURRENT_MsgPacket_For_Nextion();/*KODLARI TAMAMLANDI TEST EDİLECEK*/
		break;
	}

}
