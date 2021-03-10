#ifndef __JSONB_C_BASE_H__
#define __JSONB_C_BASE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "jsonb_c_type.h"

static inline void jsonb_opt_bool(jsonb_opt_e opt, cJSON *json, void *element, size_t size)
{
    if (opt == JSONB_OPT_J2S) {
        if (!cJSON_IsBool(json)) return;
        *(bool *)element = json->type == cJSON_True ? true : false;
    } else if (opt == JSONB_OPT_S2J) {
        json->type = *(bool *)element ? cJSON_True : cJSON_False;
    }
}

#define JSONB_OPT_NUMBER(TYPE) \
static inline void jsonb_opt_##TYPE(jsonb_opt_e opt, cJSON *json, void *element, size_t size) \
{ \
    if (opt == JSONB_OPT_J2S) { \
        if (!cJSON_IsNumber(json)) return; \
        *(TYPE *)element = (TYPE)cJSON_GetNumberValue(json); \
    } else if (opt == JSONB_OPT_S2J) { \
        json->type = cJSON_Number; \
        cJSON_SetNumberValue(json, *(TYPE *)element); \
    } \
}

JSONB_OPT_NUMBER(uint8_t)
JSONB_OPT_NUMBER(uint16_t)
JSONB_OPT_NUMBER(uint32_t)
JSONB_OPT_NUMBER(int8_t)
JSONB_OPT_NUMBER(int16_t)
JSONB_OPT_NUMBER(int32_t)
JSONB_OPT_NUMBER(char)
JSONB_OPT_NUMBER(short)
JSONB_OPT_NUMBER(int)
JSONB_OPT_NUMBER(float)
JSONB_OPT_NUMBER(double)

#define JSONB_OPT_NUMBER_2(TYPE, FUNC, FORMAT) \
static inline void jsonb_opt_##TYPE(jsonb_opt_e opt, cJSON *json, void *element, size_t size) \
{ \
    if (opt == JSONB_OPT_J2S) { \
        if (!cJSON_IsString(json)) return; \
        *(TYPE *)element = (TYPE)FUNC(cJSON_GetStringValue(json), NULL, 0); \
    } else if (opt == JSONB_OPT_S2J) { \
        char tmp[32]; \
        int len = snprintf(tmp, sizeof(tmp), FORMAT, *(TYPE *)element); \
        json->type = cJSON_String; \
        if (json->valuestring) { \
            cJSON_SetValuestring(json, tmp); \
        } else { \
            json->valuestring = cJSON_malloc(sizeof(tmp)); \
            memcpy(json->valuestring, tmp, len); \
            json->valuestring[len] = '\0'; \
        } \
    } \
}

#if __SIZEOF_POINTER__ == 4
JSONB_OPT_NUMBER(long)
JSONB_OPT_NUMBER_2(int64_t, strtoll, "%lld")
JSONB_OPT_NUMBER_2(uint64_t, strtoull, "%llu")
#elif __SIZEOF_POINTER__ == 8
JSONB_OPT_NUMBER_2(long, strtoll, "%zd")
JSONB_OPT_NUMBER_2(int64_t, strtoll, "%zd")
JSONB_OPT_NUMBER_2(uint64_t, strtoull, "%zu")
#endif

static inline void jsonb_opt_string(jsonb_opt_e opt, cJSON *json, void *element, size_t size)
{
    if (opt == JSONB_OPT_J2S) {
        if (!cJSON_IsString(json)) return;
        char *tmp = cJSON_GetStringValue(json);
        size_t len = strnlen(tmp, size - 1);
        memcpy(element, tmp, len);
        ((char *)element)[len] = '\0';
    } else if (opt == JSONB_OPT_S2J) {
        char *tmp = (char *)element;
        json->type = cJSON_String;
        if (json->valuestring) {
            cJSON_SetValuestring(json, tmp);
        } else {
            json->valuestring = cJSON_malloc(size);
            size_t len = strnlen(tmp, size - 1);
            memcpy(json->valuestring, tmp, len);
            json->valuestring[len] = '\0';
        }
    }
}

typedef struct {
    int value;
    const char *string;
} josnb_enum_t;

static inline void jsonb_opt_enum(jsonb_opt_e opt, cJSON *json, void *element, size_t size, const josnb_enum_t *enummap, size_t enumsize)
{
    int i = 0;
    assert(sizeof(int) == size);

    if (opt == JSONB_OPT_J2S) {
        if (!cJSON_IsString(json)) return;
        for (i = 0; i < enumsize; i++) {
            if (!strcmp(cJSON_GetStringValue(json), enummap[i].string)) {
                memcpy(element, (void *)&enummap[i].value, sizeof(int));
                break;
            }
        }
    } else if (opt == JSONB_OPT_S2J) {
        for (i = 0; i < enumsize; i++) {
            if (!memcmp(element, &enummap[i].value, sizeof(int))) {
                jsonb_opt_string(opt, json, (void *)enummap[i].string, strlen(enummap[i].string) + 1);
                break;
            }
        }
    }

    /* set default */
    if (i == enumsize) {
        const int tmp_v = INT_MAX;
        const char *tmp_s = "unknown";
        jsonb_opt_string(JSONB_OPT_S2J, json, (void *)tmp_s, strlen(tmp_s) + 1);
        memcpy(element, (void *)&tmp_v, sizeof(tmp_v));
    }
}

static inline void jsonb_opt_array(jsonb_opt_e opt, cJSON *json, void *e, size_t size, const size_t *size_list_data, const int size_list_len, int layer, jsonb_opt_func_t callback)
{
    size_t index = 0;
    cJSON *child = NULL;
    char *element = e;
    size_t subsize = 0;

    if (layer + 1 == size_list_len || !size_list_data[layer]) {
        return;
    } else {
        subsize = size / size_list_data[layer];
    }

    if (opt == JSONB_OPT_J2S) {
        if (!cJSON_IsArray(json)) return;
        if (cJSON_GetArraySize(json) != size_list_data[layer]) return;
        for (index = 0; index < size_list_data[layer]; index++) {
            child = cJSON_GetArrayItem(json, index);
            if (size_list_data[layer + 1]) {
                jsonb_opt_array(opt, child, element + index * subsize, subsize, size_list_data, size_list_len, layer + 1, callback);
            } else {
                callback(opt, child, element + index * subsize, subsize);
            }
        }
    } else if (opt == JSONB_OPT_S2J) {
        for (index = 0; index < size_list_data[layer]; index++) {
            if (size_list_data[layer + 1]) {
                child = cJSON_CreateArray();
                jsonb_opt_array(opt, child, element + index * subsize, subsize, size_list_data, size_list_len, layer + 1, callback);
            } else {
                child = cJSON_CreateObject();
                callback(opt, child, element + index * subsize, subsize);
            }
            cJSON_AddItemToArray(json, child);
        }
    }
}

#endif /* __JSONB_C_BASE_H__ */