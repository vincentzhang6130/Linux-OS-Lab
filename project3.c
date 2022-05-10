#include<stdio.h>

#define NUMBER_OF_ITERATIONS 99999999


int main(){

	int i, t=2, u=3, v;
	unsigned int w;
	for(i=0; i<NUMBER_OF_ITERATIONS; i++){
		
		v = (++t)*(u++);
		if(get_number_of_context_switches(&w) != 0){

			printf("Error!\n");
		}
		else{
			printf("This process encounters %u times context switches.\n",w);
		
		}

	}


  	return 0;
}



