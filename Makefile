CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -fstack-protector-all

SRC = src/hashfile.c src/svg.c src/geo.c src/quadra.c src/pessoa.c src/pm.c src/qry.c
UNITY = Unity/unity.c
INCLUDE = -Iinclude -IUnity

all: ted

ted:
	$(CC) $(CFLAGS) src/main.c $(SRC) $(INCLUDE) -o src/ted

run:
	./src/ted -e tests -f cidade.geo -pm pessoas.pm -q comandos.qry -o saida

test:
	$(CC) $(CFLAGS) tests/test_hashfile.c src/hashfile.c $(UNITY) $(INCLUDE) -o test_hash
	./test_hash
	
	$(CC) $(CFLAGS) tests/test_svg.c src/svg.c $(UNITY) $(INCLUDE) -o test_svg
	./test_svg

	$(CC) $(CFLAGS) tests/test_quadra.c src/quadra.c $(UNITY) $(INCLUDE) -o test_quadra
	./test_quadra

	$(CC) $(CFLAGS) tests/test_geo.c src/geo.c src/svg.c src/quadra.c src/hashfile.c $(UNITY) $(INCLUDE) -o test_geo
	./test_geo

	$(CC) $(CFLAGS) tests/test_pessoa.c src/pessoa.c $(UNITY) $(INCLUDE) -o test_pessoa
	./test_pessoa

	$(CC) $(CFLAGS) tests/test_pm.c src/hashfile.c src/pessoa.c src/pm.c $(UNITY) $(INCLUDE) -o test_pm
	./test_pm

	$(CC) $(CFLAGS) tests/test_qry.c src/qry.c src/svg.c src/hashfile.c src/pessoa.c src/quadra.c $(UNITY) $(INCLUDE) -o test_qry
	./test_qry


clean:
	del /Q src\ted.exe prog.exe test_hash.exe test_svg.exe test_pessoa.exe test_pm.exe test_quadra.exe test_geo.exe test_qry.exe testeCENSO.txt saida.txt *.hf *.hfc *.svg *.hfd *.dat saida\*.svg saida\*.txt saida\*.hfd 2>nul
	-rmdir /S /Q saida 2>nul

clean-linux:
	rm -f src/ted prog test_hash test_svg test_pessoa test_pm test_quadra test_geo test_qry testeCENSO.txt saida.txt *.hf *.hfc *.svg *.hfd *.dat
	rm -rf saida

tudo: ted run test

