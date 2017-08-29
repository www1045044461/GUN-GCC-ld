#include<stdio.h>
#include<dlfcn.h>
typedef int(*fun)(int ,int);
int main()
{
	void *p=dlopen("./libA.so",RTLD_LAZY);
	//Only reference but not have path
	fun f=(fun)dlsym(p,"funcA");	
	printf("main:%d \n",f(3,4));
	return 0;
}
