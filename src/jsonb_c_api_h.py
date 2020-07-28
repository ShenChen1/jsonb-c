#!/usr/bin/env python

import sys
import os
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
        self.__tmp = open(self.__dir + '/' + filename + '.h', 'w')
        self.__deal_with_file_start(filename)
        for line in lines:
            self.__deal_with_file_line(line)
        self.__deal_with_file_end(filename)
        self.__tmp.close()

    def __writeline(self, str):
        self.__tmp.write(str + os.linesep)

    def __deal_with_file_line(self, line):
        d = {}
        d['JSONB_STRUCT_START'] = self.__deal_with_struct_start
        d['JSONB_STRUCT_END'] = self.__deal_with_struct_end
        d['JSONB_FIELD'] = self.__deal_with_field
        d['JSONB_STRING'] = self.__deal_with_string
        d['JSONB_FIELD_ARRAY'] = self.__deal_with_field_array
        d['JSONB_STRING_ARRAY'] = self.__deal_with_string_array

        line = line.strip()
        line = line.replace(" ", "")
        line = line.replace("(", ",")
        line = line.replace(")", "")
        func = line.split(',')[0]
        parameter = line.split(',')[1:]
        logging.debug('func={0} parameter={1}'.format(func, parameter))

        if func in d.keys():
            d[func](parameter)

    def __deal_with_file_start(self, filename):
        self.__writeline('#ifndef __{0}_JSONB_C_API_H__'.format(filename.upper()))
        self.__writeline('#define __{0}_JSONB_C_API_H__'.format(filename.upper()))
        self.__writeline('#include "jsonb_c_type.h"')

    def __deal_with_file_end(self, filename):
        self.__writeline('#endif /* __{0}_JSONB_C_API_H__ */'.format(filename.upper()))

    def __deal_with_struct_start(self, parameter):
        [type] = parameter
        self.__writeline('extern void jsonb_opt_{0}(jsonb_opt_e opt, cJSON *json, void *element, size_t size);'.format(type))
        self.__writeline('typedef struct {')

    def __deal_with_struct_end(self, parameter):
        [type] = parameter
        self.__writeline('} ' + '{0};'.format(type))

    def __deal_with_field(self, parameter):
        [element, type] = parameter
        self.__writeline('    {0} {1};'.format(type, element))

    def __deal_with_string(self, parameter):
        [element, length] = parameter
        self.__writeline('    char {0}[{1}];'.format(element, length))

    def __deal_with_field_array(self, parameter):
        [element, size, type] = parameter
        self.__writeline('    {0} {1}[{2}];'.format(type, element, size))

    def __deal_with_string_array(self, parameter):
        [element, size, length] = parameter
        self.__writeline('    char {0}[{1}][{2}];'.format(element, size, length))