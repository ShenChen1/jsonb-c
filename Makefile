PROJECT := example
TMP := tmp

all:
	mkdir -p $(TMP)
	python3 ./src/jsonb_c.py --source=./$(PROJECT)/$(PROJECT).jsonb --installdir=$(TMP)
	gcc -g $(PROJECT)/main.c cjson/cJSON.c $(TMP)/$(PROJECT).c -Icjson -Iinc -I$(TMP) -o $(PROJECT).exe
	./$(PROJECT).exe

clean:
	rm -rf $(PROJECT).exe $(TMP)
