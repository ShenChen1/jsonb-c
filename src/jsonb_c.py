#!/usr/bin/env python

import sys
import os
import argparse
import jsonb_c_api_h
import jsonb_c_api_c

def main():
    parser = argparse.ArgumentParser(prog='jsonb-c', usage=\
          "%(prog)s [-v] --source=jsonb --installdir=path \n")
    parser.add_argument('-v', '--verbose', dest='verbose', action='store_true', help='verbose mode')
    parser.add_argument('--source', dest='source', help='specify a jsonb file', default='./example.jsonb')
    parser.add_argument('--installdir', dest='installdir', help='specify the path of c code', default='./')
    args = parser.parse_args()

    h = jsonb_c_api_h.generator(args.verbose)
    h.source(args.source)
    h.installdir(args.installdir)
    h.run()
    c = jsonb_c_api_c.generator(args.verbose)
    c.source(args.source)
    c.installdir(args.installdir)
    c.run()

if __name__ == '__main__':
    main()