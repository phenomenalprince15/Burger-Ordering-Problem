SRC: burger.c cooks.c customers.c cashiers.c assure_state.c
INC: burger.h

target:
	gcc -pthread burger.c cooks.c customers.c cashiers.c assure_state.c -o all.out

run: target
	./all.out