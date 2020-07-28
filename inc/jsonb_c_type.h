#ifndef __JSONB_C_TYPE_H__
#define __JSONB_C_TYPE_H__

#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include "cJSON.h"

typedef enum {
    JSONB_OPT_GET,
    JSONB_OPT_SET,
} jsonb_opt_e;

typedef void (*jsonb_opt_func_t)(jsonb_opt_e opt, cJSON *json, void *element, size_t size);

#endif /* __JSONB_C_TYPE_H__ */