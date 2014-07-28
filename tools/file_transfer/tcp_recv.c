#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>


int main(int argc,char* argv[])
{
	if (argc != 3){
		printf("argc err!\n");
		printf("port,saveFileName\n");
		return -1;
	}
	
	const int					port = atoi(argv[1]);
	const char*				file_name = argv[2];

	int								sd = 0;
	struct						sockaddr_in addr_local;
	
	if( (sd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  
		perror("socket");
		return -1;
	}

	int re_use = 1;
	if (setsockopt(sd,SOL_SOCKET ,SO_REUSEADDR,\
			&re_use,sizeof(re_use))){
	}

	bzero(&addr_local,sizeof(addr_local));
	addr_local.sin_family = AF_INET;           // Protocol Family
	addr_local.sin_port = htons(port);         // Port number
	addr_local.sin_addr.s_addr  = INADDR_ANY; 
	if(bind(sd, (struct sockaddr*)&addr_local, \
				sizeof(struct sockaddr)) == -1 ){ 
		perror("bind");
		return  -1;
	}
	
	if(listen(sd,4)) { 
		perror("listen");
		return -1;
	}
	
	while(1)
	{
		struct sockaddr_in addr_remote;
		int sin_size = sizeof(struct sockaddr_in);
		bzero(&addr_remote,sizeof(addr_remote));
		
//		printf("sd:%d,sin_size:%d\n",sd,sin_size);
		int new_sd = accept(sd,(struct sockaddr*)&addr_remote,&sin_size);
		if (new_sd < 0){
			perror("accept");
			sleep(1);
			continue;
		}
//		printf("new_sd:%d\n",new_sd);
		FILE* fd = fopen(file_name,"wb");
		if (fd == NULL){
			perror("fopen");
			close(new_sd);
			continue;
		}
		while(1)
		{
			char buf[1024];
			const int len = recv(new_sd,buf,sizeof(buf),0);
			if (len < 0){
				perror("recv");
				break;
			}else if (len == 0){
				break;
			}
			const int write_size = fwrite(buf,1,len,fd);
			if (write_size != len){
				perror("fwrite");
			}
		}
		fclose(fd);
	}
	return 0;
}
