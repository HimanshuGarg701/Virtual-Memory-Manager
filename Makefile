vmm: vmm.c
	gcc -I -Wall vmm.c -o vmm

run: ./vmm
	./vmm addresses.txt BACKING_STORE.bin
	
clean:
	rm vmm
