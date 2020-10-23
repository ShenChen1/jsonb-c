#!/usr/bin/env python

import sys
import os
import re
import logging

class generator:
    def __init__(self, verbose):
        level = logging.ERROR if verbose == False else logging.DEBUG
        format = "[%(levelname)s]: %(message)s"
        logging.basicConfig(level=level, format=format)

        self.__verbose = verbose
        self.__source = None
        self.__dir = "./"
        self.__tmp = None

    def source(self, file):
        if not os.access(file, os.R_OK):
            raise RuntimeError("file err: {0} nonexistent".format(file))
        self.__source = file
        logging.debug('source={0}'.format(self.__source))

    def installdir(self, path):
        os.system("mkdir -p {0}".format(path))
        self.__dir = path
        logging.debug('installdir={0}'.format(self.__dir))

    def run(self):
        # read all lines from jsonb file
        lines = None
        with open(self.__source, 'r') as f:
            lines = f.readlines()

        filename = os.path.basename(self.__source).split('.')[0]
        self.__tmp = open(self.__dir + '/' + filename + '.c', 'w')
        self.__deal_with_file_start(filename)
        for line in lines:
            self.__deal_with_file_line(line)
        self.__deal_with_file_end(filename)
        self.__tmp.close()

    def __writeline(self, str):
        self.__tmp.write(str + os.linesep)

    def __deal_with_file_line(self, line):
        d = {}
        d['JSONB_MACRO_DEFINE'] = self.__deal_with_macro_define
        d['JSONB_INCLUDE_HEADER'] = self.__deal_with_include_header
        d['JSONB_STRUCT_START'] = self.__deal_with_struct_start
        d['JSONB_STRUCT_END'] = self.__deal_with_struct_end
        d['JSONB_FIELD'] = self.__deal_with_field
        d['JSONB_STRING'] = self.__deal_with_string
        d['JSONB_FIELD_ARRAY'] = self.__deal_with_field_array
        d['JSONB_STRING_ARRAY'] = self.__deal_with_string_array

        pattern = re.compile(r"^(\w+)\((.*)\)$")
        match = pattern.match(line)
        if match is None:
            return

        (func, line) = match.groups()
        line = line.replace(" ", "")
        parameter = line.split(',')
        logging.debug('func={0} parameter={1}'.format(func, parameter))
        if func in d.keys():
            d[func](parameter)

    def __deal_with_file_start(self, filename):
        self.__writeline('#include "jsonb_c_type.h"')
        self.__writeline('#include "jsonb_c_base.h"')
        self.__writeline('#include "{0}.h"'.format(filename))

    def __deal_with_file_end(self, filename):
        pass

    def __deal_with_include_header(self, parameter):
        pass

    def __deal_with_macro_define(self, parameter):
        pass

    def __deal_with_struct_start(self, parameter):
        [type] = parameter
        self.__writeline('void jsonb_opt_{0}(jsonb_opt_e opt, cJSON *json, void *e, size_t size)'.format(type))
        self.__writeline('{')
        self.__writeline('    {0} *element= e;'.format(type))

    def __deal_with_struct_end(self, parameter):
        self.__writeline('}')

    def __deal_with_field(self, parameter):
        [element, type] = parameter
        self.__writeline('{')
        self.__writeline('    cJSON *json_child = NULL;')
        self.__writeline('    if (opt == JSONB_OPT_J2S) {')
        self.__writeline('        if (cJSON_IsNull(json)) assert(0);')
        self.__writeline('        json_child = cJSON_GetObjectItem(json, "{0}");'.format(element))
        self.__writeline('    } else if (opt == JSONB_OPT_S2J) {')
        self.__writeline('        json_child = cJSON_CreateObject();')
        self.__writeline('        cJSON_AddItemToObject(json, "{0}", json_child);'.format(element))
        self.__writeline('    }')
        self.__writeline('    jsonb_opt_{0}(opt, json_child, &element->{1}, sizeof({0}));'.format(type, element))
        self.__writeline('}')

    def __deal_with_string(self, parameter):
        [element, length] = parameter
        self.__writeline('{')
        self.__writeline('    cJSON *json_child = NULL;')
        self.__writeline('    if (opt == JSONB_OPT_J2S) {')
        self.__writeline('        if (cJSON_IsNull(json)) assert(0);')
        self.__writeline('        json_child = cJSON_GetObjectItem(json, "{0}");'.format(element))
        self.__writeline('    } else if (opt == JSONB_OPT_S2J) {')
        self.__writeline('        json_child = cJSON_CreateObject();')
        self.__writeline('        cJSON_AddItemToObject(json, "{0}", json_child);'.format(element))
        self.__writeline('    }')
        self.__writeline('    jsonb_opt_string(opt, json_child, element->{0}, {1});'.format(element, length))
        self.__writeline('}')

    def __deal_with_string_array(self, parameter):
        [element, size, length] = parameter
        self.__writeline('{')
        self.__writeline('    cJSON *json_child = NULL;')
        self.__writeline('    if (opt == JSONB_OPT_J2S) {')
        self.__writeline('        if (cJSON_IsNull(json)) assert(0);')
        self.__writeline('        json_child = cJSON_GetObjectItem(json, "{0}");'.format(element))
        self.__writeline('    } else if (opt == JSONB_OPT_S2J) {')
        self.__writeline('        json_child = cJSON_CreateArray();')
        self.__writeline('        cJSON_AddItemToObject(json, "{0}", json_child);'.format(element))
        self.__writeline('    }')
        self.__writeline('    const size_t array_size_list[] = {' + '{0},0'.format(size) + '};')
        self.__writeline('    jsonb_opt_array(opt, json_child, element->{0}, {1} * array_size_list[0], array_size_list, jsonb_opt_string);'.format(element, length))
        self.__writeline('}')

    def __deal_with_field_array(self, parameter):
        element = parameter[0]
        type = parameter[-1]
        num = "1"
        tmp = '{'
        for p in parameter[1:-1]:
            tmp += p + ','
            num += ' * ' + p
        tmp += '0}'
        self.__writeline('{')
        self.__writeline('    cJSON *json_child = NULL;')
        self.__writeline('    if (opt == JSONB_OPT_J2S) {')
        self.__writeline('        if (cJSON_IsNull(json)) assert(0);')
        self.__writeline('        json_child = cJSON_GetObjectItem(json, "{0}");'.format(element))
        self.__writeline('    } else if (opt == JSONB_OPT_S2J) {')
        self.__writeline('        json_child = cJSON_CreateArray();')
        self.__writeline('        cJSON_AddItemToObject(json, "{0}", json_child);'.format(element))
        self.__writeline('    }')
        self.__writeline('    const size_t array_size_list[] = {0};'.format(tmp))
        self.__writeline('    jsonb_opt_array(opt, json_child, element->{0}, sizeof({1}) * {2}, array_size_list, jsonb_opt_{1});'.format(element, type, num))
        self.__writeline('}')