# jsonb-c

[jsonb-c](https://github.com/ShenChen1/jsonb-c) is an automatic generated binding layer for converting C structure to/from JSON messages.

## HOWTO

### Declare structure

|                        keyword                         |                    comment
|--------------------------------------------------------|------------------------------------------------
| JSONB_STRUCT_START(type)                               | Use it to start declaring a structure named `type`
| JSONB_STRUCT_END(type)                                 | Use it at the end of structure declaration
| JSONB_STRING(element, string_size)                     | Use it for `char element[string_size]`
| JSONB_FIELD(element, type)                             | Use it for `type element`
| JSONB_STRING_ARRAY(elements, array_size, string_size)  | Use it for `char elements[array_size][string_size]`
| JSONB_FIELD_ARRAY(elements, array_size, type)          | Use it for `type elements[array_size]`
| JSONB_FIELD_ARRAY(elements, one, two, ..., num, type)  | Use it for `type elements[one][two]...[num]`
| JSONB_INCLUDE_HEADER(file)                             | Use it for `#include "file"`
| JSONB_MACRO_DEFINE(key, value)                         | Use it for `#define key (value)`
| JSONB_UNION_START(type)                                | Use it to start declaring a union named `type`
| JSONB_UNION_END(type)                                  | Use it at the end of union declaration
| JSONB_UNION_FIELD(element, type, condition)            | Use it for `type element` in union depending on `condition`

[example](https://github.com/ShenChen1/jsonb-c/blob/master/example/example.jsonb)

### Generate binding layer

Use py script to build .h file and .c file

```
python ./src/jsonb_c.py --source=./example/example.jsonb --installdir=tmp
```

### Use binding layer in your code

#### Serialize data to JSON object

```
jsonb_opt_student_t(JSONB_OPT_S2J, json, &orignal_student, sizeof(student_t));
```

#### Deserialize data to structure object

```
jsonb_opt_student_t(JSONB_OPT_J2S, json, &student, sizeof(student_t));
```

[example](https://github.com/ShenChen1/jsonb-c/blob/master/example/main.c)
