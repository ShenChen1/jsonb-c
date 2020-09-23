PROJECT := example
TMP := tmp
CC := gcc
PREFIX ?= /usr

all: test

build:
	mkdir -p $(TMP)
	python ./src/jsonb_c.py --source=./$(PROJECT)/$(PROJECT).jsonb --installdir=$(TMP)

test: build
	$(CC) -g -Wall -Werror $(PROJECT)/main.c cjson/cJSON.c $(TMP)/$(PROJECT).c -Icjson -Iinc -I$(TMP) -o $(PROJECT).exe

clean:
	rm -rf *.exe $(TMP)

install:
	mkdir -p $(PREFIX)/include
	cp -av inc/*.h $(PREFIX)/include
