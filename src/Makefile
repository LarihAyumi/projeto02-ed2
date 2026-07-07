CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -fstack-protector-all

SRC = hashfile.c svg.c geo.c quadra.c pessoa.c pm.c qry.c grafo.c via.c
UNITY = ../Unity/unity.c
INCLUDE = -I../include -I../Unity

all: ted

ted:
	$(CC) $(CFLAGS) main.c $(SRC) $(INCLUDE) -o ted

run:
	./ted -e ../tests -f cidade.geo -pm pessoas.pm -q comandos.qry -o ../saida

tstall:
	cd .. && $(CC) $(CFLAGS) tests/test_hashfile.c src/hashfile.c Unity/unity.c -Iinclude -IUnity -o test_hash && ./test_hash
	cd .. && $(CC) $(CFLAGS) tests/test_svg.c src/svg.c Unity/unity.c -Iinclude -IUnity -o test_svg && ./test_svg
	cd .. && $(CC) $(CFLAGS) tests/test_quadra.c src/quadra.c Unity/unity.c -Iinclude -IUnity -o test_quadra && ./test_quadra
	cd .. && $(CC) $(CFLAGS) tests/test_geo.c src/geo.c src/svg.c src/quadra.c src/hashfile.c Unity/unity.c -Iinclude -IUnity -o test_geo && ./test_geo
	cd .. && $(CC) $(CFLAGS) tests/test_pessoa.c src/pessoa.c Unity/unity.c -Iinclude -IUnity -o test_pessoa && ./test_pessoa
	cd .. && $(CC) $(CFLAGS) tests/test_pm.c src/hashfile.c src/pessoa.c src/pm.c Unity/unity.c -Iinclude -IUnity -o test_pm && ./test_pm
	cd .. && $(CC) $(CFLAGS) tests/test_qry.c src/qry.c src/svg.c src/hashfile.c src/pessoa.c src/quadra.c Unity/unity.c -Iinclude -IUnity -o test_qry && ./test_qry

tstall-win:
	cd .. && $(CC) $(CFLAGS) tests/test_hashfile.c src/hashfile.c Unity/unity.c -Iinclude -IUnity -o test_hash && test_hash.exe
	cd .. && $(CC) $(CFLAGS) tests/test_svg.c src/svg.c Unity/unity.c -Iinclude -IUnity -o test_svg && test_svg.exe
	cd .. && $(CC) $(CFLAGS) tests/test_quadra.c src/quadra.c Unity/unity.c -Iinclude -IUnity -o test_quadra && test_quadra.exe
	cd .. && $(CC) $(CFLAGS) tests/test_geo.c src/geo.c src/svg.c src/quadra.c src/hashfile.c Unity/unity.c -Iinclude -IUnity -o test_geo && test_geo.exe
	cd .. && $(CC) $(CFLAGS) tests/test_pessoa.c src/pessoa.c Unity/unity.c -Iinclude -IUnity -o test_pessoa && test_pessoa.exe
	cd .. && $(CC) $(CFLAGS) tests/test_pm.c src/hashfile.c src/pessoa.c src/pm.c Unity/unity.c -Iinclude -IUnity -o test_pm && test_pm.exe
	cd .. && $(CC) $(CFLAGS) tests/test_qry.c src/qry.c src/svg.c src/hashfile.c src/pessoa.c src/quadra.c Unity/unity.c -Iinclude -IUnity -o test_qry && test_qry.exe

clean:
	rm -f ted ted.exe *.hf *.hfc *.svg *.hfd *.dat
	rm -f ../test_hash ../test_svg ../test_pessoa ../test_pm ../test_quadra ../test_geo ../test_qry
	rm -rf ../saida
	rm -rf ../o

clean-win:
	-del /Q ted.exe ted *.hf *.hfc *.svg *.hfd *.dat 2>nul
	-del /Q ..\*.hf ..\*.hfc ..\*.svg ..\*.hfd ..\*.dat 2>nul
	-del /Q ..\test_hash.exe ..\test_svg.exe ..\test_pessoa.exe ..\test_pm.exe ..\test_quadra.exe ..\test_geo.exe ..\test_qry.exe 2>nul
	-del /Q ..\o\*.svg ..\o\*.txt ..\o\*.hfd ..\o\*.hf ..\o\*.hfc ..\o\*.dot 2>nul
	-del /Q ..\saida\*.svg ..\saida\*.txt ..\saida\*.hfd ..\saida\*.hf ..\saida\*.hfc ..\saida\*.dot 2>nul

tudo: ted run tstall
