#ifndef __JSONB_C_BASE_H__
#define __JSONB_C_BASE_H__

#include <string.h>
#include <assert.h>
#include "jsonb_c_type.h"

static inline void jsonb_opt_bool(jsonb_opt_e opt, cJSON *json, void *element, size_t size)
{
    if (opt == JSONB_OPT_GET) {
        if (!cJSON_IsBool(json)) assert(0);
        *(bool *)element = json->type == cJSON_True ? true : false;
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
        if (json->valuestring) {
            cJSON_SetValuestring(json, (char *)element);
         } else {
            json->valuestring = cJSON_malloc(strlen((char *)element));
            strcpy(json->valuestring, (char *)element);
         }
    }
}

static inline void jsonb_opt_array(jsonb_opt_e opt, cJSON *json, void *e, size_t size, size_t array_size, jsonb_opt_func_t callback)
{
    size_t index = 0;
    cJSON *child = NULL;
    char *element = e;

    if (opt == JSONB_OPT_GET) {
        if (!cJSON_IsArray(json)) assert(0);
        assert(cJSON_GetArraySize(json) == array_size);
        for (index = 0; index < array_size; index++) {
            child = cJSON_GetArrayItem(json, index);
            callback(opt, child, element + index * size, size);
        }
    } else if (opt == JSONB_OPT_SET) {
        for (index = 0; index < array_size; index++) {
            child = cJSON_CreateObject();
            callback(opt, child, element + index * size, size);
            cJSON_AddItemToArray(json, child);
        }
    }
}


#endif /* __JSONB_C_BASE_H__ */