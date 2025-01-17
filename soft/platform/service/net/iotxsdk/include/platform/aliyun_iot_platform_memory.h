#ifndef ALIYUN_IOT_PLATFORM_MEMORY_H
#define ALIYUN_IOT_PLATFORM_MEMORY_H

#include <stdlib.h>
#include "platform/aliyun_iot_platform_datatype.h"
#include "utility/aliyun_iot_common_error.h"


void* aliyun_iot_memory_malloc(uint32_t size);


void aliyun_iot_memory_free(void* ptr);

#endif
