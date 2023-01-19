/*
 * Tencent is pleased to support the open source community by making IoT Hub available.
 * Copyright (C) 2019 THL A29 Limited, a Tencent company. All rights reserved.

 * Licensed under the MIT License (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://opensource.org/licenses/MIT

 * Unless required by applicable law or agreed to in writing, software distributed under the License is
 * distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include "qcloud_iot_api_export.h"
#include "lite-utils.h"
#include "at_client.h"
#include "string.h"
#include "data_config.c"
#include "cmsis_os.h"
#include "main.h"

static bool sg_control_msg_arrived = false;
static char sg_data_report_buffer[AT_CMD_MAX_LEN];
static size_t sg_data_report_buffersize = sizeof(sg_data_report_buffer) / sizeof(sg_data_report_buffer[0]);

// ******************************Event Start*********************************
#ifdef EVENT_POST_ENABLED

#define EVENT_COUNTS     (1)

static TYPE_DEF_TEMPLATE_STRING sg_IcCartAuth_IcKey[30 + 1] = {0};
static DeviceProperty g_propertyEvent_IcCartAuth[] = {
        {.key = "IcKey", .data = &sg_IcCartAuth_IcKey, .type = TYPE_TEMPLATE_STRING},
};


static sEvent g_events[] = {
        {
                .event_name = "IcCartAuth",
                .type = "info",
                .timestamp = 0,
                .eventDataNum = sizeof(g_propertyEvent_IcCartAuth) / sizeof(g_propertyEvent_IcCartAuth[0]),
                .pEventData = g_propertyEvent_IcCartAuth,
        }
};

uint8_t *get_sg_IcCartAuth_IcKey() {
    return sg_IcCartAuth_IcKey;
}

#ifdef    EVENT_TIMESTAMP_USED
static void update_events_timestamp(sEvent *pEvents, int count)
{
    int i;

    for(i = 0; i < count; i++){
        if (NULL == (&pEvents[i])) {
            Log_e("null event pointer");
            return;
        }
        pEvents[i].timestamp = HAL_GetTimeSeconds();
    }
}
#endif

static void event_post_cb(char *msg, void *context) {

    Log_d("eventReply:%s", msg);
    IOT_Event_clearFlag(context, FLAG_EVENT0 | FLAG_EVENT1 | FLAG_EVENT2);
}

#endif
// ******************************Event End*********************************
// ******************************Action Start*********************************
#ifdef ACTION_ENABLED

#include "action_config.c"

// action : regist action and set the action handle callback, add your aciton logic here
static void OnActionCallback(void *pClient, const char *pClientToken, DeviceAction *pAction) {
    int i;
    sReplyPara replyPara;

    //do something base on input, just print as an sample
    DeviceProperty *pActionInput = pAction->pInput;
    for (i = 0; i < pAction->input_num; i++) {
        if (JSTRING == pActionInput[i].type) {
            Log_d("Input:[%s], data:[%s]", pActionInput[i].key, pActionInput[i].data);
            HAL_Free(pActionInput[i].data);
        } else {
            if (JINT32 == pActionInput[i].type) {
                Log_d("Input:[%s], data:[%d]", pActionInput[i].key, *((int *) pActionInput[i].data));
            } else if (JFLOAT == pActionInput[i].type) {
                Log_d("Input:[%s], data:[%f]", pActionInput[i].key, *((float *) pActionInput[i].data));
            } else if (JUINT32 == pActionInput[i].type) {
                Log_d("Input:[%s], data:[%u]", pActionInput[i].key, *((uint32_t *) pActionInput[i].data));
            }
        }
    }

    // construct output
    memset((char *) &replyPara, 0, sizeof(sReplyPara));
    replyPara.code = eDEAL_SUCCESS;
    replyPara.timeout_ms = QCLOUD_IOT_MQTT_COMMAND_TIMEOUT;
    strcpy(replyPara.status_msg, "action execute success!"); //add the message about the action resault


    DeviceProperty *pActionOutnput = pAction->pOutput;
    (void) pActionOutnput; //elimate warning
    //TO DO: add your aciont logic here and set output properties which will be reported by action_reply


    IOT_ACTION_REPLY(pClient, pClientToken, sg_data_report_buffer, sg_data_report_buffersize, pAction, &replyPara);
}

static int _register_data_template_action(void *pTemplate_client) {
    int i, rc;

    for (i = 0; i < TOTAL_ACTION_COUNTS; i++) {
        rc = IOT_Template_Register_Action(pTemplate_client, &g_actions[i], OnActionCallback);
        if (rc != QCLOUD_RET_SUCCESS) {
            rc = IOT_Template_Destroy(pTemplate_client);
            Log_e("register device data template action failed, err: %d", rc);
            return rc;
        } else {
            Log_i("data template action=%s registered.", g_actions[i].pActionId);
        }
    }

    return QCLOUD_RET_SUCCESS;
}

#endif

// ******************************Action End*********************************

// ******************************Msg Start*********************************
static void
OnControlMsgCallback(void *pClient, const char *pJsonValueBuffer, uint32_t valueLength, DeviceProperty *pProperty) {
    int i = 0;

    for (i = 0; i < TOTAL_PROPERTY_COUNT; i++) {
        if (strcmp(sg_DataTemplate[i].data_property.key, pProperty->key) == 0) {
            sg_DataTemplate[i].state = eCHANGED;
            Log_i("Property=%s changed", pProperty->key);
            sg_control_msg_arrived = true;
            return;
        }
    }

    Log_e("Property=%s changed no match", pProperty->key);
}


static int _register_data_template_property(void *ptemplate_client) {
    int i, rc;

    for (i = 0; i < TOTAL_PROPERTY_COUNT; i++) {
        rc = IOT_Template_Register_Property(ptemplate_client, &sg_DataTemplate[i].data_property, OnControlMsgCallback);
        if (rc != QCLOUD_RET_SUCCESS) {
            rc = IOT_Template_Destroy(ptemplate_client);
            Log_e("register device data template property failed, err: %d", rc);
            return rc;
        } else {
            Log_i("data template property=%s registered.", sg_DataTemplate[i].data_property.key);
        }
    }

    return QCLOUD_RET_SUCCESS;
}

static void
OnReportReplyCallback(void *pClient, Method method, ReplyAck replyAck, const char *pJsonDocument, void *pUserdata) {
    Log_i("recv report_reply(ack=%d): %s", replyAck, pJsonDocument);
}


static void deal_down_stream_user_logic(void *pClient, ProductDataDefine *pData) {
    // TODO : base the switch  to control the lock
    if (pData->m_switch) {
        HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
        Log_i("the light have been open");
    } else {
        HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
        Log_i("the light have been close");
    }
}


static int deal_up_stream_user_logic(void *pClient, DeviceProperty *pReportDataList[], int *pCount) {
    int i, j;

    //check local property state
    //_refresh_local_property. if property changed, set sg_DataTemplate[i].state = eCHANGED;

    for (i = 0, j = 0; i < TOTAL_PROPERTY_COUNT; i++) {
        if (eCHANGED == sg_DataTemplate[i].state) {
            pReportDataList[j++] = &(sg_DataTemplate[i].data_property);
            sg_DataTemplate[i].state = eNOCHANGE;
        }
    }
    *pCount = j;

    return (*pCount > 0) ? QCLOUD_RET_SUCCESS : QCLOUD_ERR_FAILURE;
}

static eAtResault net_prepare(void) {
    eAtResault Ret;
    DeviceInfo sDevInfo;
    at_client_t pclient = at_client_get();

    memset((char *) &sDevInfo, '\0', sizeof(DeviceInfo));
    Ret = (eAtResault) HAL_GetDevInfo(&sDevInfo);
    if (QCLOUD_RET_SUCCESS != Ret) {
        Log_e("Get device info err");
        return QCLOUD_ERR_FAILURE;
    }

    if (QCLOUD_RET_SUCCESS != module_init(eMODULE_ESP8266)) {
        Log_e("module init failed");
        goto exit;
    } else {
        Log_d("module init success");
    }

    //wait at parse thread run
    while (AT_STATUS_INITIALIZED != pclient->status) {
        HAL_SleepMs(1000);
    }

    Log_d("Start shakehands with module...");
    Ret = module_handshake(CMD_TIMEOUT_MS);
    if (QCLOUD_RET_SUCCESS != Ret) {
        Log_e("module connect fail,Ret:%d", Ret);
        goto exit;
    } else {
        Log_d("module connect success");
    }

    Ret = iot_device_info_init(sDevInfo.product_id, sDevInfo.device_name, sDevInfo.devSerc);
    if (QCLOUD_RET_SUCCESS != Ret) {
        Log_e("dev info init fail,Ret:%d", Ret);
        goto exit;
    }

    Ret = module_info_set(iot_device_info_get(), ePSK_TLS);
    if (QCLOUD_RET_SUCCESS != Ret) {
        Log_e("module info set fail,Ret:%d", Ret);
    }

    exit:

    return Ret;
}

static void eventPostCheck(void *client) {
#ifdef EVENT_POST_ENABLED
    int rc;
    int i;
    uint32_t eflag;
    sEvent *pEventList[EVENT_COUNTS];
    uint8_t event_count;

    eflag = IOT_Event_getFlag(client);
    if ((EVENT_COUNTS > 0) && (eflag > 0)) {
        event_count = 0;
        for (i = 0; i < EVENT_COUNTS; i++) {

            if ((eflag & (1 << i)) & ALL_EVENTS_MASK) {
                pEventList[event_count++] = &(g_events[i]);
                IOT_Event_clearFlag(client, 1 << i);
#ifdef    EVENT_TIMESTAMP_USED
                update_events_timestamp(&g_events[i], 1);
#endif
            }
        }

        rc = IOT_Post_Event(client, sg_data_report_buffer, sg_data_report_buffersize, \
                                            event_count, pEventList, event_post_cb);
        if (rc < 0) {
            Log_e("events post failed: %d", rc);
        }
    }
#endif

}


static int _get_sys_info(void *handle, char *pJsonDoc, size_t sizeOfBuffer) {

    DeviceProperty plat_info[] = {
            {.key = "module_hardinfo", .type = TYPE_TEMPLATE_STRING, .data = "ESP8266"},
            {.key = "module_softinfo", .type = TYPE_TEMPLATE_STRING, .data = "V1.0"},
            {.key = "fw_ver", .type = TYPE_TEMPLATE_STRING, .data = QCLOUD_IOT_AT_SDK_VERSION},
            {.key = "imei", .type = TYPE_TEMPLATE_STRING, .data = "11-22-33-44"},
            {.key = "lat", .type = TYPE_TEMPLATE_STRING, .data = "22.546015"},
            {.key = "lon", .type = TYPE_TEMPLATE_STRING, .data = "113.941125"},
            {NULL, NULL, JINT32}  //����
    };


    DeviceProperty self_info[] = {
            {.key = "append_info", .type = TYPE_TEMPLATE_STRING, .data = "your self define ifno"},
            {NULL, NULL, JINT32}
    };

    return IOT_Template_JSON_ConstructSysInfo(handle, pJsonDoc, sizeOfBuffer, plat_info, self_info);
}


void data_template_demo_task(void *arg) {
    eAtResault Ret;
    int rc;
    int ReportCont;
    void *client = NULL;
    at_client_t pclient = at_client_get();
    DeviceProperty *pReportDataList[TOTAL_PROPERTY_COUNT];

    Log_d("data_template_demo_task Entry...");
    do {
        Ret = net_prepare();
        if (QCLOUD_RET_SUCCESS != Ret) {
            Log_e("net prepare fail,Ret:%d", Ret);
            break;
        }

        /*
         * module_register_network
        */
        Ret = module_register_network(eMODULE_ESP8266);
        if (QCLOUD_RET_SUCCESS != Ret) {
            Log_e("network connect fail,Ret:%d", Ret);
            break;
        }


        MQTTInitParams init_params = DEFAULT_MQTTINIT_PARAMS;
        Ret = module_mqtt_conn(init_params);
        if (QCLOUD_RET_SUCCESS != Ret) {
            Log_e("module mqtt conn fail,Ret:%d", Ret);
            break;
        } else {
            Log_d("module mqtt conn success");
        }


        if (!IOT_MQTT_IsConnected()) {
            Log_e("mqtt connect fail");
            break;
        }

        Ret = (eAtResault) IOT_Template_Construct(&client);
        if (QCLOUD_RET_SUCCESS != Ret) {
            Log_e("data template construct fail,Ret:%d", Ret);
            break;
        } else {
            Log_d("data template construct success");
        }

        //init data template
        _init_data_template();


        //register data template property here
        rc = _register_data_template_property(client);
        if (rc == QCLOUD_RET_SUCCESS) {
            Log_i("Register data template property Success");
        } else {
            Log_e("Register data template property Failed: %d", rc);
            break;
        }

#ifdef ACTION_ENABLED
        //register data template actions
        rc = _register_data_template_action(client);
        if (rc == QCLOUD_RET_SUCCESS) {
            Log_i("Register data template actions Success");
        } else {
            Log_e("Register data template actions Failed: %d", rc);
            break;
        }
#endif

        // report sys base Info
        rc = _get_sys_info(client, sg_data_report_buffer, sg_data_report_buffersize);
        if (QCLOUD_RET_SUCCESS == rc) {
            rc = IOT_Template_Report_SysInfo_Sync(client, sg_data_report_buffer, sg_data_report_buffersize,
                                                  QCLOUD_IOT_MQTT_COMMAND_TIMEOUT);
            if (rc != QCLOUD_RET_SUCCESS) {
                Log_e("Report system info fail, err: %d", rc);
                break;
            }
        } else {
            Log_e("Get system info fail, err: %d", rc);
            break;
        }

        // get outline info
        rc = IOT_Template_GetStatus_sync(client, QCLOUD_IOT_MQTT_COMMAND_TIMEOUT);
        if (rc != QCLOUD_RET_SUCCESS) {
            Log_e("Get data status fail, err: %d", rc);
            break;
        } else {
            Log_d("Get data status success");
        }


        while (1) {
            HAL_SleepMs(1000);
            IOT_Template_Yield(client, 2000);

            if (sg_control_msg_arrived) {

                deal_down_stream_user_logic(client, &sg_ProductData);

                sReplyPara replyPara;
                memset((char *) &replyPara, 0, sizeof(sReplyPara));
                replyPara.code = eDEAL_SUCCESS;
                replyPara.timeout_ms = QCLOUD_IOT_MQTT_COMMAND_TIMEOUT;
                replyPara.status_msg[0] = '\0';

                rc = IOT_Template_ControlReply(client, sg_data_report_buffer, sg_data_report_buffersize, &replyPara);
                if (rc == QCLOUD_RET_SUCCESS) {
                    Log_d("Control msg reply success");
                    sg_control_msg_arrived = false;
                } else {
                    Log_e("Control msg reply failed, err: %d", rc);
                    break;
                }
            }


            eventPostCheck(client);
        }
    } while (0);

    hal_thread_destroy(NULL);
    Log_e("Task terminated,Something goes wrong!!!");
}

void data_template_sample(void) {

#ifdef OS_USED

    osThreadDef(clientMain, (os_pthread) data_template_demo_task, (osPriority) osPriorityNormal, 0, 1024);
    osThreadCreate(osThread(clientMain), NULL);
#else
#error os should be used just now
#endif
}

