//server 6
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#define PORT 7000
#define MAX 20

int main()
{
	int s,ns,n,a[MAX][MAX];
		struct sockaddr_in addr;
		socklen_t len=sizeof(addr);
		char result[40];
		
		s=socket(AF_INET,SOCK_STREAM,0);
		addr.sin_family=AF_INET;
		addr.sin_addr.s_addr=INADDR_ANY;
		addr.sin_port=htons(PORT);
		
		bind(s,(struct sockaddr*)&addr,sizeof(addr));
		listen(s,3);
		printf("Server is Listening...");
		ns=accept(s,(struct sockaddr*)&addr,&len);
		if(ns<=0){
			printf("Bind Failed");
			return 1;
			}
		printf("\nClient Connected");
		read(ns,&n,sizeof(n));
		
		for(int i=0;i<n;i++)
			read(ns,a[i],n*sizeof(int));
		int upper=1,lower=1,diagonal=1;
		
		printf("\n Matrix Recieved:\n");
		for(int i=0;i<n;i++)
		{
			for(int j=0;j<n;j++)
			{
				printf("%d ",a[i][j]);
			}
		printf("\n");
		}
		
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++)
			{
				if(i>j && a[i][j]!=0)upper=0;
				if(i<j && a[i][j]!=0)lower=0;
				if(i!=j && a[i][j]!=0)diagonal=0;
			}
		}
		
		if(diagonal)
			sprintf(result,"Diagonal Matrix");
		else if(upper)
			sprintf(result,"Upper Triangular");
		else if(lower)
			sprintf(result,"Lower Triangular");
		else
			sprintf(result,"Not a special Matrix");
		
		write(ns,result,sizeof(result));
		close(ns);
		close(s);
		return 0;
}
//server 6 end			
			
			
			
			
			
