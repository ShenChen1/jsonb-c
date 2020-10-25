#!/usr/bin/env python

import sys
import os
import argparse
import jsonb_c_api

def main():
    parser = argparse.ArgumentParser(prog='jsonb-c', usage=\
          "%(prog)s [-v] --source=jsonb --installdir=path \n")
    parser.add_argument('-v', '--verbose', dest='verbose', action='store_true', help='verbose mode')
    parser.add_argument('--source', dest='source', help='specify a jsonb file', default='./example.jsonb')
    parser.add_argument('--installdir', dest='installdir', help='specify the path of c code', default='./')
    args = parser.parse_args()

    jsonb = jsonb_c_api.generator(args.verbose)
    jsonb.source(args.source)
    jsonb.installdir(args.installdir)
    jsonb.run("source")
    jsonb.run("header")

if __name__ == '__main__':
    main()