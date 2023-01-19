/*
 * Tencent is pleased to support the open source community by making IoT Hub available.
 * Copyright (C) 2016 THL A29 Limited, a Tencent company. All rights reserved.

 * Licensed under the MIT License (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://opensource.org/licenses/MIT

 * Unless required by applicable law or agreed to in writing, software distributed under the License is
 * distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef QCLOUD_IOT_EXPORT_DATA_TEMPLATE_H_
#define QCLOUD_IOT_EXPORT_DATA_TEMPLATE_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "config.h"
#include "qcloud_iot_export_mqtt.h"
#include "qcloud_iot_export_method.h"

#include "stdbool.h"
#include "stddef.h"
#define  MAX_CONTORL_REPLY_STATUS_LEN		64		   // control???????У?status?????????????????????????????

/**
 * @brief ?????????????????????
 */
#define TYPE_TEMPLATE_INT    	JINT32
#define TYPE_TEMPLATE_ENUM    	JINT32
#define TYPE_TEMPLATE_FLOAT  	JFLOAT
#define TYPE_TEMPLATE_BOOL   	JINT8
#define TYPE_TEMPLATE_STRING 	JSTRING
#define TYPE_TEMPLATE_TIME 		JUINT32
#define TYPE_TEMPLATE_JOBJECT 	JOBJECT

typedef int32_t   TYPE_DEF_TEMPLATE_INT;
typedef int32_t   TYPE_DEF_TEMPLATE_ENUM;
typedef float     TYPE_DEF_TEMPLATE_FLOAT;
typedef char      TYPE_DEF_TEMPLATE_BOOL;
typedef char      TYPE_DEF_TEMPLATE_STRING;
typedef uint32_t  TYPE_DEF_TEMPLATE_TIME;
typedef void *    TYPE_DEF_TEMPLATE_OBJECT;

#ifdef EVENT_POST_ENABLED					//?????????????????????

#define TYPE_STR_INFO			"info"
#define TYPE_STR_ALERT			"alert"
#define TYPE_STR_FAULT			"fault"

//??????????????????????????????UTC???ms????????ж??????
//#define  EVENT_TIMESTAMP_USED			 

#define  FLAG_EVENT0 			(1U<<0)
#define  FLAG_EVENT1			(1U<<1)
#define  FLAG_EVENT2			(1U<<2)
#define  FLAG_EVENT3			(1U<<3)
#define  FLAG_EVENT4 			(1U<<4)
#define  FLAG_EVENT5			(1U<<5)
#define  FLAG_EVENT6			(1U<<6)
#define  FLAG_EVENT7			(1U<<7)
#define  FLAG_EVENT8 			(1U<<8)
#define  FLAG_EVENT9			(1U<<9)

#define  ALL_EVENTS_MASK		(0xFFFFFFFF)


typedef enum {
	eEVENT_INFO,
	eEVENT_ALERT,    
    eEVENT_FAULT, 
}eEventType;

typedef struct  _sEvent_{
	char 	 *event_name;		 //???????	
	char 	 *type;			 	 //???????	
    uint32_t timestamp;			 //??????	
	uint8_t eventDataNum;		 //???????????
    DeviceProperty *pEventData;  //????????
} sEvent;

#endif

typedef enum _eControlReplyCode_{
		eDEAL_SUCCESS = 0,
		eDEAL_FAIL = -1,
}eReplyCode;

/**
 * @brief control msg reply ????
 */
typedef struct _sControlReplyPara {

    uint32_t  timeout_ms;         						      // ????????, ??λ:ms
    
    eReplyCode   code;    							  // ???code??0????? ??0?????

    char      status_msg[MAX_CONTORL_REPLY_STATUS_LEN];       // ???????

} sReplyPara;


/**
 * @brief ??????????????????
 */
typedef enum _eDataState_{
    eNOCHANGE = 0,
	eCHANGED = 1,	
} eDataState;

/**
 * @brief ?????????????????
 */
typedef struct {
    DeviceProperty data_property;
    eDataState state;
} sDataPoint;

uint8_t *get_sg_IcCartAuth_IcKey();
/**
 * @brief ???data template client
 *
 * @return ShadowClient
 */
void *get_template_client(void);


/**
 * @brief ????TemplateClient
 *
 * @param client ??????????????data template client
 *
 * @return ????NULL: ???????
 */
int IOT_Template_Construct(void **client);

/**
 * @brief ????????????????
 *
 * @param pClient Template Client????
 * @return ????true, ??????????????
 */
bool IOT_Template_IsConnected(void *handle);

/**
 * @brief ????TemplateClient ???MQTT????
 *
 * @param pClient TemplateClient????
 *
 * @return ????QCLOUD_ERR_SUCCESS, ??????
 */
int IOT_Template_Destroy(void *handle);

/**
 * @brief ???????, ?????????????, ?????????
 *
 * @param handle     data template client
 * @param timeout_ms ??????, ??λ:ms
 */
void IOT_Template_Yield(void *handle, uint32_t timeout_ms);


/**
 * @brief ?????豸???豸????
 *
 * @param pClient    Client????
 * @param pProperty  ?豸????
 * @param callback   ?豸???????????????
 * @return           ????QCLOUD_ERR_SUCCESS, ?????????
 */
int IOT_Template_Register_Property(void *handle, DeviceProperty *pProperty, OnPropRegCallback callback);

/**
 * @brief ?????????????豸????
 *
 * @param pClient    Client????
 * @param pProperty  ?豸????
 * @return           ????QCLOUD_ERR_SUCCESS, ?????????
 */
int IOT_Template_UnRegister_Property(void *handle, DeviceProperty *pProperty);

/**
 * @brief ??JSON????????reported??Σ??????????
 *
 *
 * @param jsonBuffer    ??洢JSON???????????????????
 * @param sizeOfBuffer  ????????С
 * @param count         ???????????, ??????????豸????????
 * @return              ????QCLOUD_ERR_SUCCESS, ??????
 */
int IOT_Template_JSON_ConstructReportArray(void *handle, char *jsonBuffer, size_t sizeOfBuffer, uint8_t count, DeviceProperty *pDeviceProperties[]); 

/**
 * @brief  ???????????????????
 *
 * @param pClient             data template client
 * @param pJsonDoc          ??????????????????????   
 * @param sizeOfBuffer      ???????
 * @param callback            ???????? ?????????????
 * @param userContext       ???????, ??????????????????????????
 * @param timeout_ms        ????????, ??λ:ms
 * @return                  ????QCLOUD_ERR_SUCCESS, ?????????
 */
int IOT_Template_Report(void *handle, char *pJsonDoc, size_t sizeOfBuffer, OnReplyCallback callback, void *userContext, uint32_t timeout_ms);

/**
 * @brief  ????????????????????
 *
 * @param pClient             data template client
 * @param pJsonDoc          ??????????????????????   
 * @param sizeOfBuffer      ???????
 * @param timeout_ms       ????????, ??λ:ms
 * @return                  ????QCLOUD_ERR_SUCCESS, ?????????
 */
int IOT_Template_Report_Sync(void *handle, char *pJsonDoc, size_t sizeOfBuffer, uint32_t timeout_ms);


 /**
  * @brief	????????????????????????
  *                 ???????????豸????????????????????
  *
  * @param pClient		   data template client
  * @param callback 		   ??????????? ?????????????
  * @param userContext	   ???????, ??????????????????????????
  * @param timeout_ms	   ????????, ??λ:ms
  * @return 				  ????QCLOUD_ERR_SUCCESS, ?????????
  */
int IOT_Template_GetStatus(void *handle, OnReplyCallback callback, void *userContext, uint32_t timeout_ms);

 /**
  * @brief	?????????????????????????
  * @param pClient		   data template client
  * @param timeout_ms	   ????????, ??λ:ms
  * @return 				  ????QCLOUD_ERR_SUCCESS, ?????????
  */
int IOT_Template_GetStatus_sync(void *handle, uint32_t timeout_ms);

/**
 * @brief  ???????????control data
 * @param pClient		  data template client
 * @param code	  		  0?????     ??0?????
 * @param pClientToken	  ?????get_status_reply	?е?clientToken
 * @return					 ????QCLOUD_ERR_SUCCESS, ?????????
 */
int IOT_Template_ClearControl(void *handle, char *pClientToken, OnRequestCallback callback, uint32_t timeout_ms); 


/**
 * @brief  ???????control???
 * @param pClient		   data template client
 * @param jsonBuffer		   ?????????????????buffer
 * @param sizeOfBuffer	  ???????
 *	@param replyPara		  ????????????/???????????????  
 * @return					 ????QCLOUD_ERR_SUCCESS, ?????????
 */ 
int IOT_Template_ControlReply(void *handle, char *pJsonDoc, size_t sizeOfBuffer, sReplyPara *replyPara);

/**
 * @brief  ????????????json?????
 * @param pClient		   data template client
 * @param jsonBuffer	   ???????????buffer
 * @param sizeOfBuffer	   ???????
 * @param pPlatInfo		   ????????????? 
 * @param pSelfInfo		   ?豸??????????????????
 * @return				   ????QCLOUD_ERR_SUCCESS, ?????????

 *?????????????
* {
*  "method": "report_info",
*  "clientToken": "client-token1618",
*  "params": {
*  "module_hardinfo": "????????????  N10,,??????豸????",
*  "module_softinfo":  "???????汾,??????豸????",
*  "fw_ver":       "mcu????汾,????",
*  "imei":       "?豸imei?????????",
*  "lat":        "γ????軻???10????,??????,???λ?÷????豸????,????豸????????"
*  "lon":        "??????軻???10????,??????,???λ?÷????豸????,????豸????????",
*  "device_label": {
*    "append_info": "?豸?????????????????"
* ...
*   }
* }
*/
int IOT_Template_JSON_ConstructSysInfo(void *handle, char *jsonBuffer, size_t sizeOfBuffer, DeviceProperty *pPlatInfo, DeviceProperty *pSelfInfo); 

/**
 * @brief ?????????????
 * @param pClient		   data template client
 * @param jsonBuffer	   ?????Json??
 * @param sizeOfBuffer	   ?????Json??????
 * @param callback		   ??????
 * @param userContext	   ???????, ??????????????????????
 * @param timeout_ms	   ?????????, ??λ:ms
 * @return				   ????QCLOUD_ERR_SUCCESS, ?????????
*/
int IOT_Template_Report_SysInfo(void *handle, char *pJsonDoc, size_t sizeOfBuffer, OnReplyCallback callback, void *userContext, uint32_t timeout_ms);

/**
 * @brief ??????????????
 * @param pClient		   data template client
 * @param jsonBuffer	   ?????Json??
 * @param sizeOfBuffer	   ?????Json??????
 * @param timeout_ms	   ???????????????, ??λ:ms
 * @return				   ????QCLOUD_ERR_SUCCESS, ?????????
*/
int IOT_Template_Report_SysInfo_Sync(void *handle, char *pJsonDoc, size_t sizeOfBuffer, uint32_t timeout_ms);


#ifdef EVENT_POST_ENABLED
/**
 * @brief ??????????????
 *
 * @param msg    	 ??????????????
 * @param context    data template client
 *
 */
typedef void (*OnEventReplyCallback)(char *msg, void *context);


/**
 * @brief ??????????
 *
 * @param  flag  ???÷??????????
 */
void IOT_Event_setFlag(void *client, uint32_t flag);

/**
 * @brief ?????????
 *
 * @param  flag  ????????????
 */
void IOT_Event_clearFlag(void *client, uint32_t flag);

/**
 * @brief ???????λ???????
 *
 * @return ????λ???????
 */
uint32_t IOT_Event_getFlag(void *client);

/**
 * @brief ???client?????????????????????????
 *
 * @param c    shadow ??????
 */
int IOT_Event_Init(void *c);

/**
 * @brief ?????????????????????????
 * 
 * @param client   data template client
 */

void handle_template_expired_event(void *client);


/**
 * @brief ??????????????????飬SDK????????json??????
 * @param pClient shadow ??????
 * @param pJsonDoc    ???????json???????????buffer
 * @param sizeOfBuffer    ???????json???????????buffer??С
 * @param event_count     ??????????????
 * @param pEventArry	  ???????????????
 * @param replyCb	  ????????????? 
 * @return @see IoT_Error_Code	  
 */
int IOT_Post_Event(void *pClient, char *pJsonDoc, size_t sizeOfBuffer, uint8_t event_count, sEvent *pEventArry[], OnEventReplyCallback replyCb);

/**
 * @brief ????????????????????????????json?????SDK???????????????
 * @param pClient shadow ??????
 * @param pJsonDoc    ???????json???????????buffer
 * @param sizeOfBuffer    ???????json???????????buffer??С
 * @param pEventMsg     ??????????json??? 
 *  json????????
 *  ?????????
 *	 {"method": "event_post",
 *		"clientToken": "123",
 *		"version": "1.0",
 *		"eventId": "PowerAlarm",
 *		"type": "fatal",
 *		"timestamp": 1212121221,
 *		"params": {
 *			"Voltage": 2.8,
 *			"Percent": 20
 *		}
 *	}
 *
 *  ????????
 *	 {
 *		 "eventId": "PowerAlarm",
 *		 "type": "fatal",
 *		 "timestamp": 1212121221,
 *		 "params": {
 *			 "Voltage": 2.8,
 *			 "Percent": 20
 *		 }
 *	 },
 *	 {
 *		 "name": "PowerAlarm",
 *		 "type": "fatal",
 *		 "timestamp": 1212121223,
 *		 "params": {
 *			 "Voltage": 2.1,
 *			 "Percent": 10
 *		 }
 *	 },
 *   ....
 *
 * @param replyCb	  ????????????? 
 * @return @see IoT_Error_Code	  
 */
int IOT_Post_Event_Raw(void *pClient, char *pJsonDoc, size_t sizeOfBuffer, char *pEventMsg, OnEventReplyCallback replyCb);                                            


#endif

#ifdef ACTION_ENABLED
/**
 * @brief ?????豸?????
 *
 * @param pClient    Client????
 * @param pProperty  ?豸????
 * @param callback   ?豸???????????????
 * @return           ????QCLOUD_ERR_SUCCESS, ?????????
 */

int IOT_Template_Register_Action(void *handle, DeviceAction *pAction, OnActionHandleCallback callback);

/**
 * @brief ?????????????豸???
 *
 * @param pClient    Client????
 * @param pProperty  ?豸????
 * @return           ????QCLOUD_ERR_SUCCESS, ?????????
 */

int IOT_Template_UnRegister_Action(void *handle, DeviceAction *pAction); 

/**
* @brief ?豸?????? 
* @param pClient		  handle to data_template client
* @param pClientToken	  correspond to the clientToken of action msg 
* @param pJsonDoc	  	  data buffer for reply
* @param sizeOfBuffer     length of data buffer
* @param pAction 		  pointer of action 	
* @param replyPara        action reply info
* @return				  QCLOUD_RET_SUCCESS when success, or err code for failure
*/ 

int IOT_ACTION_REPLY(void *pClient, const char *pClientToken, char *pJsonDoc, size_t sizeOfBuffer, DeviceAction *pAction, sReplyPara *replyPara);

#endif



#ifdef __cplusplus
}
#endif

#endif /* QCLOUD_IOT_EXPORT_TEMPLATE_H_ */
