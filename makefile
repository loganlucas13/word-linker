build:
	rm -f out.exe
	gcc main.c -o out.exe

run:
	./out.exe

build_test:
	rm -f test.exe
	gcc test.c -o test.exe

run_test:
	./test.exe

mac:
	export MallocStackLogging=1

leaks:
	leaks --atExit -- ./out.exe