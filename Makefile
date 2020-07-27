PROJECT := example

all:
	mkdir -p tmp
	python3 ./src/jsonb_c.py --source=./$(PROJECT)/$(PROJECT).jsonb --installdir=tmp
	gcc $(PROJECT)/main.c cjson/cJSON.c tmp/$(PROJECT).c -Icjson -Iinc -Itmp