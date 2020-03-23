#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int add(int a, int b)
{
	return a+b;
}

void bjr(int n)
{
	for(int i=0; i<n; i++)
	{
		printf("Bonjour\n");
	}
}
