#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[], char* env[]) 
{
	printf("���������� ���������� ��������� ������:%d\n" , argc);
	int i=0;
	printf("������ ���������� ���������� �� ���������:");
	for (i;i<argc;i++)
		printf("%s\n" , argv[i]);
	i = 0;
	printf("������ ���������� �� ������� �������� �� ���������� �����:");
	while( env[i] != NULL )
	{
		printf("%s\n" , env[ i ]);
		i++;
	}
	exit(0);
}
