#include "Canbus.h"
extern CAN_HandleTypeDef hcan;
CAN_TxHeaderTypeDef TxHeader; //CAN Bus Transmit Header
CAN_RxHeaderTypeDef RxHeader; //CAN Bus Receive Header
CAN_FilterTypeDef Canfil; //CAN Bus Filter

uint8_t CAN_ID;
uint8_t CanRX[8];  //CAN Bus Receive Buffer
uint32_t CanMailbox; //CAN Bus Mail box variable

/**
 * @fn void CANBUS_ConfigFilter_Init(void)
 * @brief
 *
 * @pre
 * @post
 */
void CANBUS_ConfigFilter_Init(void) {
	//Gelen Mesaj İçin Filtre Ayarları Yapılıyor...
	TxHeader.StdId = VCU_CANBUS_ID;
	TxHeader.DLC = 8;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;
	Canfil.FilterBank = 0;
	Canfil.FilterActivation = ENABLE;
	Canfil.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	Canfil.FilterIdHigh = 0;
	Canfil.FilterIdLow = 0;
	Canfil.FilterMaskIdHigh = 0x0000;
	Canfil.FilterMaskIdLow = 0x0000;
	Canfil.FilterMode = CAN_FILTERMODE_IDMASK;
	Canfil.FilterScale = CAN_FILTERSCALE_32BIT;
	Canfil.SlaveStartFilterBank = 0;

	//Filtreleme Aktif Ediliyor..
	HAL_CAN_ConfigFilter(&hcan, &Canfil);
}

/**
 * @fn void CANBUS_Init(void)
 * @brief
 *
 * @pre
 * @post
 */
void CANBUS_Init(void) {
	HAL_CAN_Start(&hcan);
	HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
	CANBUS_ConfigFilter_Init();

}

/**
 * @fn void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef*)
 * @brief
 *
 * @pre
 * @post
 * @param hcan
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, CanRX);
	CAN_ID = RxHeader.StdId;
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	for (uint8_t i = 0; i < 8; i++) {
		Nextion_Buffer[i] = CanRX[i];
	}
	Nextion_Incomming_Packet_ID_From_CANBUS(CAN_ID);
	LORA_Incomming_Packet_ID_From_CANBUS(CAN_ID);


}
