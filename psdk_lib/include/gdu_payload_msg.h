#ifndef GDU_PAYLOAD_MSG_H
#define GDU_PAYLOAD_MSG_H

#include "stdint.h"
#include "gdu_aircraft_info.h"

#define PSDK_SYSID      		0x05

typedef struct  __attribute__((packed)) 
{
	uint16_t msg_id;
	void (* fun)(uint8_t *data);
}payload_msg_fun_list_t;

typedef struct {
    uint8_t version[32]; /*!< The major version of firmware, the range is 0 ~ 99. */
} T_GduFirmwareVersion;

typedef struct {
    uint8_t index;
    uint8_t ip[4]; /*!< The major version of firmware, the range is 0 ~ 99. */
    uint8_t mask[4];
    uint8_t gateway[4];
} T_GduPayloadNetMsg;

/* Exported types ------------------------------------------------------------*/
typedef struct {
    T_GduReturnCode (*getPayloadVersion)(T_GduFirmwareVersion *version);
    T_GduReturnCode (*getPayloadIp)(int index, T_GduPayloadNetMsg *payloadIp);
    T_GduReturnCode (*setPayloadIp)(T_GduPayloadNetMsg payloadIp);
} T_GduPayloadMsgHandler;

T_GduReturnCode GduPayloadMsg_Init(void);
T_GduReturnCode GduPayloadMsg_RegPayloadMsgHandler(T_GduPayloadMsgHandler *payloadMsgHandler);
#endif

