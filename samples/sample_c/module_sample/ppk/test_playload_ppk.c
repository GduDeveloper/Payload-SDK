#include "test_playload_ppk.h"
#include "gdu_logger.h"
#include "gdu_ppk.h"
#include "time_sync.h"

T_GduPPKHandlerList GduPPKHandlerList;
static T_GduOsalHandler *osalHandler = NULL;
static T_GduTaskHandle s_PPKTestThread;
static void *GduTest_PPKTask(void *arg);

T_GduReturnCode GduTest_PPKEventPinInit(void)
{
	return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode GduTest_PPKEventTrigger(E_GduPPKEventPinState pinState)
{

	return GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_GduReturnCode GduTest_PPKStartService(void)
{
	T_GduReturnCode returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR; 
    osalHandler = GduPlatform_GetOsalHandler();

    GduPPKHandlerList.EventPinInit = GduTest_PPKEventPinInit;
    GduPPKHandlerList.EventTrigger = GduTest_PPKEventTrigger;
	returnCode = GduPPK_RegHandlerList(&GduPPKHandlerList);
	if(GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS != returnCode) {
		goto out;	
	}

	returnCode = GduPPK_Init();
	if(GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS != returnCode) {
		goto out;	
	}

#if 1
    //Step 4 : Run widget api sample task
    if (osalHandler->TaskCreate("user_ppk_task", GduTest_PPKTask, 4096, NULL,
                                &s_PPKTestThread) != GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Gdu PPK test task create error.");
        return GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }
#endif

out:
	return returnCode;
}

/* Private functions definition-----------------------------------------------*/
static void *GduTest_PPKTask(void *arg)
{
    uint32_t sysTimeMs = 0;
    T_GduReturnCode gduStat;
    T_GduOsalHandler *osalHandler = GduPlatform_GetOsalHandler();

	uint64_t localTimeUs = 0;

	uint8_t buf[1024];
	uint16_t readReturnLength = 0;
	uint16_t cameraLens_id = 0;
	uint16_t exposure_id = 0;
	char out_event_str[50]; 
	T_GduTimeSyncAircraftTime aircraftTime = {0};
	T_GduReturnCode returnCode = GDU_ERROR_SYSTEM_MODULE_CODE_UNKNOWN; 

	uint16_t timeCount = 0;

	while (1) 
	{
		if(GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS == GduPPK_GetRawData(buf, &readReturnLength)) {
			USER_LOG_INFO("read raw data length %d \n");
		}

		if(0 == timeCount % 100)
		{
			int i = 0;
			for(i = 0;i < 5;i ++)
			{
				if(GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS != GduTimeSync_TransferToAircraftTime(localTimeUs, &aircraftTime)) {
					continue;
				}
				if(GDU_ERROR_SYSTEM_MODULE_CODE_SUCCESS == GduPPK_GetQianXunEventData(cameraLens_id,exposure_id, &aircraftTime,out_event_str)) {
					USER_LOG_INFO("qianxun event: %s",out_event_str);	
				}
			}
		}

		timeCount ++; 
		osalHandler->TaskSleepMs(10);
	}
}


