#include <stdio.h>
#include <stdlib.h>

int main()
{
	int i,j;
	printf("input the value(for 0x12) to binary format!\n");
	while(1)
	{
		int v = 0;
		scanf("0x%x",&v);
		getchar();
		printf("input 0x%X,%d\n",v,v);
		const char* ft="%-5d";
		for (i = 3;i>-1;i--)
		{
			for (j=7;j>-1;j--)
			{
				printf(ft,i*8+j);
			}
			printf("\n");
			for(j=7;j>-1;j--)
			{
				printf(ft,(v>>i*8+j)&0x1);
			}
			printf("\n___________________________________________\n");
		}
	}
	return 0;
}
