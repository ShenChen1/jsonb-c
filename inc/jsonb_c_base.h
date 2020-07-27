#ifndef __JSONB_C_BASE_H__
#define __JSONB_C_BASE_H__

#include <string.h>
#include <assert.h>
#include "jsonb_c_type.h"

static inline void jsonb_opt_bool(jsonb_opt_e opt, cJSON *json, void *element, size_t size)
{
    if (opt == JSONB_OPT_GET) {
        if (!cJSON_IsBool(json)) assert(0);
        *(bool *)element = json->type;
    } else if (opt == JSONB_OPT_SET) {
        json->type = *(bool *)element ? cJSON_True : cJSON_False;
    }
}

static inline void jsonb_opt_int(jsonb_opt_e opt, cJSON *json, void *element, size_t size)
{
    if (opt == JSONB_OPT_GET) {
        if (!cJSON_IsNumber(json)) assert(0);
        *(int *)element = (int)cJSON_GetNumberValue(json);
    } else if (opt == JSONB_OPT_SET) {
        json->type = cJSON_Number;
        cJSON_SetNumberValue(json, *(int *)element);
    }
}

static inline void jsonb_opt_double(jsonb_opt_e opt, cJSON *json, void *element, size_t size)
{
    if (opt == JSONB_OPT_GET) {
        if (!cJSON_IsNumber(json)) assert(0);
        *(double *)element = (double)cJSON_GetNumberValue(json);
    } else if (opt == JSONB_OPT_SET) {
        json->type = cJSON_Number;
        cJSON_SetNumberValue(json, *(double *)element);
    }
}

static inline void jsonb_opt_string(jsonb_opt_e opt, cJSON *json, void *element, size_t size)
{
    if (opt == JSONB_OPT_GET) {
        if (!cJSON_IsString(json)) assert(0);
        strcpy((char *)element, cJSON_GetStringValue(json));
    } else if (opt == JSONB_OPT_SET) {
        json->type = cJSON_String;
        cJSON_SetValuestring(json, (char *)element);
    }
}

static inline void jsonb_opt_array(jsonb_opt_e opt, cJSON *json, void *element, size_t size, jsonb_opt_func_t callback)
{
    size_t index = 0;

    if (opt == JSONB_OPT_GET) {
        if (!cJSON_IsArray(json)) assert(0);
        for (index = 0; index < size; index++) {
            cJSON *child = cJSON_GetArrayItem(json, index);
            callback(opt, child, element, size);
        }
    }
}


#endif /* __JSONB_C_BASE_H__ */