//Client 6
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<time.h>
#define PORT 7000
#define MAX 20

int main()
{
	int s,ns,n,a[MAX][MAX],choice;
		struct sockaddr_in addr;
		char result[40];
		
		s=socket(AF_INET,SOCK_STREAM,0);
		addr.sin_family=AF_INET;
		addr.sin_port=htons(PORT);
		addr.sin_addr.s_addr=inet_addr("127.0.0.1");
		
		connect(s,(struct sockaddr*)&addr,sizeof(addr));
		printf("Enter matrix order: ");
		scanf("%d",&n);
		
		printf("\n1.User input Matrix");
		printf("\n2.Random Matrix");
		printf("\nChoice: ");
		scanf("%d",&choice);
		
		if(choice==1)
		{ 
			printf("Enter the Matrix elements:\n");
			for(int i=0;i<n;i++){
				for(int j=0;j<n;j++){
					scanf("%d",&a[i][j]);
				}
			}
		}
		else
		{
			srand(time(NULL));
			for(int i=0;i<n;i++)
				for(int j=0;j<n;j++)
					a[i][j]=rand()%50+1;
		}
		
		printf("\nMatrix:\n");
		for(int i=0;i<n;i++)
		{
			for(int j=0;j<n;j++)
			{
				printf("%d ",a[i][j]);
			}
		printf("\n");
		}
		
		write(s,&n,sizeof(n));
		for(int i=0;i<n;i++)
			write(s,a[i],n*sizeof(int));
		
		read(s,result,sizeof(result));
		printf("\nMatrix Type: %s\n",result);
		
		close(s);
		return 0;
}
//Client 6 end
