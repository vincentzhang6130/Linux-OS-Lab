#include<syscall.h>
#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include<time.h>
#include<pthread.h>
#include<stdlib.h>
#include<dlfcn.h>

#define __NR_get_physical_addresses 442


unsigned long get_shr_mem_addr(){
	
	void *fHandle;
	fHandle = dlopen("/lib/x86_64-linux-gnu/libc-2.27.so",RTLD_LAZY);

	void (*func)(); //This a function pointer //point to a function which will return void
	unsigned long addr;
	if(!fHandle){
		fprintf(stderr,"%s\n", dlerror());
		return 0;
	
	}
	func = (void(*)())dlsym(fHandle,"printf"); // (void(*)()) 用來轉型成會回傳void的函式的function pointer
	addr = func;
	return addr;
}


//Data Segment//TLS
__thread int var = 1;

//Data Segment//Gloal
int MyDataInData = 100;

unsigned long *MyPhysicalAddress[7];


void* worker(void* address_main){

	printf("This is my thread\n");	
	//Stack
	int MyDataInStack = 1000;
	//BSS
	int MyDataInBss;   
	//Heap
	int *MyDataInHeap = malloc(sizeof(int));	
	*MyDataInHeap = 100;
	//libraries
	void* shr_address = get_shr_mem_addr();


	unsigned long *MyVirtualAddress[7];
	MyVirtualAddress[0] = &var;
	MyVirtualAddress[1] = &MyDataInStack;
	MyVirtualAddress[2] = &MyDataInBss;
	MyVirtualAddress[3] = MyDataInHeap;
	MyVirtualAddress[4] = shr_address;
	MyVirtualAddress[5] = &MyDataInData;
	MyVirtualAddress[6] = address_main; 

	printf("Convert virtual addresses to physical addresses ...\n");
	syscall(__NR_get_physical_addresses,  MyVirtualAddress, 7, MyPhysicalAddress, 7);
	int i;
	for(i=0;i<7;i++){
		printf("Virtual address : %p ===> ",MyVirtualAddress[i]);
		printf("Physical address : %p\n",MyPhysicalAddress[i]);
	}

}


int main(){

  	pthread_t pid1,pid2,pid3;
	pthread_create(&pid1,NULL,worker,main);
	sleep(1);
	pthread_create(&pid2,NULL,worker,main);
	sleep(1);
	pthread_create(&pid3,NULL,worker,main);
	sleep(1);
	pthread_join(pid1,NULL);
	pthread_join(pid2,NULL);
	pthread_join(pid3,NULL);

   	return 0;

}
