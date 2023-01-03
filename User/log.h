//
// Created by 93462 on 2023/1/1.
//

#ifndef STM32LEARN_LOG_H
#define STM32LEARN_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * 日志输出等级
 */
typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
} LOG_LEVEL;

/**
 * 全局日志级别输出标志, 只输出小于或等于该等级的日志信息
 */
extern LOG_LEVEL g_log_level;


/**
 * @brief 设置当前日志等级
 *
 * @param 日志等级
 */
void Log_Set_Level(LOG_LEVEL level);

/**
 * @brief 获取当前日志等级
 *
 * @return 日志等级
 */
LOG_LEVEL Log_Get_Level(void);


/**
 * @brief 日志打印函数，默认打印到标准输出
 *
 * @param file 源文件名
 * @param func 函数名
 * @param line 行号
 * @param level 日志等级
 */
void Log_writter(const char *file, const char *func, const int line, const int level, const char *fmt, ...);

/**
 * @brief buff数据dump
 *
 * @param pData 待dump的buff指针
 * @param len   待dump的buff数据长度
 */
void HexDump(const uint8_t *pData, unsigned int len);

#define Log_d(args...) Log_writter(__FILE__, __FUNCTION__, __LINE__, LOG_DEBUG, args)
#define Log_info(args...) Log_writter(__FILE__, __FUNCTION__, __LINE__, LOG_INFO, args)
#define Log_warn(args...) Log_writter(__FILE__, __FUNCTION__, __LINE__, LOG_WARN, args)
#define Log_error(args...) Log_writter(__FILE__, __FUNCTION__, __LINE__, LOG_ERROR, args)

//#define DEBUG_EABLE
#ifdef DEBUG_EABLE
#define IOT_FUNC_ENTRY    \
        {\
        printf("FUNC_ENTRY:   %s L#%d \n", __FUNCTION__, __LINE__);  \
        }
#define IOT_FUNC_EXIT    \
        {\
        printf("FUNC_EXIT:   %s L#%d \n", __FUNCTION__, __LINE__);  \
        return;\
        }
#define IOT_FUNC_EXIT_RC(x)    \
        {\
        printf("FUNC_EXIT:   %s L#%d Return Code : %ld \n", __FUNCTION__, __LINE__, (long)(x));  \
        return x; \
        }
#else
#define IOT_FUNC_ENTRY
#define IOT_FUNC_EXIT            \
        {\
            return;\
        }
#define IOT_FUNC_EXIT_RC(x)     \
        {\
            return x; \
        }
#endif

#ifdef __cplusplus
}
#endif

#endif //STM32LEARN_LOG_H
