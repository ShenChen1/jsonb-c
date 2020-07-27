#include <stdio.h>
#include "example.h"

static student_t orignal_student = {
    .person = {
        .name = "peter",
        .married = false,
        .age = 15,
        .height = 180.0,
        .weight = 150.0,
        .hometown = "hz",
    },
    .class = "class",
    .seat = 123,
    .subjects[0] = "math",
    .subjects[1] = "english",
    .scores[0] = 60,
    .scores[1] = 60,
    .teachers[0] = {
        .name = "a",
        .married = true,
        .age = 35,
        .height = 170.0,
        .weight = 120.0,
        .hometown = "hz",
    },
    .teachers[1] = {
        .name = "b",
        .married = false,
        .age = 30,
        .height = 180.0,
        .weight = 160.0,
        .hometown = "hz",
    },
};

int main(void) {

    cJSON *json = NULL;
    json = cJSON_CreateObject();
    jsonb_opt_student_t(JSONB_OPT_SET, json, &orignal_student, sizeof(student_t));
    printf("%s\n", cJSON_Print(json));

    static student_t student = {};
    jsonb_opt_student_t(JSONB_OPT_GET, json, &student, sizeof(student_t));
    cJSON_Delete(json);

    /* compare */
    if(memcmp(&orignal_student, &student, sizeof(student_t))) {
        printf("Converted failed!\n");
    } else {
        printf("Converted OK!\n");
    }

    return 0;
}