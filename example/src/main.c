#include <stdio.h>
#include <string.h>
#include "example.h"

static student_t orignal_student = {
    .person = {
        .name = "peter",
        .married = false,
        .height = 180.0,
        .weight = 150.0,
        .age = 15,
        .birthday = UINT16_MAX,
        .hometown = "hz",
        .id = UINT64_MAX,
    },
    .class = {
        .name = "class",
        .row = CHAR_MIN,
        .col = SHRT_MIN,
        .total = LONG_MAX,
        .id = LONG_MIN,
    },
    .union_test = {
        .condition = "b",
        .a = 'c',
    },
    .subjects[0] = "math",
    .subjects[1] = "english",
    .scores[0] = 60,
    .scores[1] = 60,
    .teachers[0] = {
        .name = "a",
        .married = true,
        .age = 35,
        .birthday = UINT32_MAX,
        .height = 170.0,
        .weight = 120.0,
        .hometown = "hz",
        .id = INT64_MAX,
    },
    .teachers[1] = {
        .name = "b",
        .married = false,
        .age = 30,
        .birthday = UINT64_MAX,
        .height = 180.0,
        .weight = 160.0,
        .hometown = "hz",
        .id = INT64_MIN,
    },
    .multi_array_test = {
        {{11,12}, {13,14}, {15,16}},
        {{21,22}, {23,24}, {25,26}},
        {{31,32}, {33,34}, {35,36}},
        {{41,42}, {43,44}, {45,46}},
    },
};

int main(void) {

    cJSON *json = NULL;
    json = cJSON_CreateObject();
    jsonb_opt_student_t(JSONB_OPT_S2J, json, &orignal_student, sizeof(student_t));
    printf("%s\n", cJSON_Print(json));

    static student_t student = {};
    jsonb_opt_student_t(JSONB_OPT_J2S, json, &student, sizeof(student_t));
    cJSON_Delete(json);

    /* compare */
    if (memcmp(&orignal_student, &student, sizeof(student_t))) {
        printf("Converted failed!\n");
    } else {
        printf("Converted OK!\n");
    }

    return 0;
}