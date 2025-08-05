#include "main.h"
#include "Nextion.h"
#include "Telemetry_LORA.h"

#define CANBUS
#define VCU_CANBUS_ID	0x36

void CANBUS_ConfigFilter_Init(void);
void CANBUS_Init(void);



extern uint8_t Nextion_Buffer[];
