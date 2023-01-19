/*-----------------data config start  -------------------*/ 
#include "qcloud_iot_export_data_template.h"

#define TOTAL_PROPERTY_COUNT 1

static sDataPoint    sg_DataTemplate[TOTAL_PROPERTY_COUNT];

typedef struct _ProductDataDefine {
    TYPE_DEF_TEMPLATE_BOOL m_switch;
} ProductDataDefine;

static   ProductDataDefine     sg_ProductData;

static void _init_data_template(void)
{
    sg_ProductData.m_switch = 0;
    sg_DataTemplate[0].data_property.data = &sg_ProductData.m_switch;
    sg_DataTemplate[0].data_property.key  = "switch";
    sg_DataTemplate[0].data_property.type = TYPE_TEMPLATE_BOOL;

};
