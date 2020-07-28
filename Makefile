PROJECT := example
TMP := tmp
CROSS_COMPILE ?=
CC := $(CROSS_COMPILE)-gcc

all: test

build:
	mkdir -p $(TMP)
	python3 ./src/jsonb_c.py --source=./$(PROJECT)/$(PROJECT).jsonb --installdir=$(TMP)

test: build
	$(CC) -g -Wall -Werror $(PROJECT)/main.c cjson/cJSON.c $(TMP)/$(PROJECT).c -Icjson -Iinc -I$(TMP) -o $(PROJECT).exe

clean:
	rm -rf *.exe $(TMP)
