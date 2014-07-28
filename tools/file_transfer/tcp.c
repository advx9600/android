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
	if (argc != 4){
		printf("argc err!\n");
		printf("ip,port,fileName\n");
		return -1;
	}

	const char* 				ip = argv[1];
	const int						port = atoi(argv[2]);
	const char*					file_name = argv[3];

	int									sd = 0;
	FILE*								fd = NULL;
	
	if( (sd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
    perror("socket");
    return -1;
  }

	struct sockaddr_in addr_remote;
	bzero(&addr_remote,sizeof(addr_remote));
	addr_remote.sin_family = AF_INET;
	addr_remote.sin_port = htons(port);
	addr_remote.sin_addr.s_addr = inet_addr(ip);
	if (connect(sd,(struct sockaddr*)&addr_remote,sizeof(addr_remote))){
		perror("connect");
		return -1;
	}
	
	fd = fopen(file_name,"rb");
	if (fd == NULL){
		perror("fopen");
		return -1;
	}

	while(!feof(fd))
	{
		char buf[1024];
		int read_size = fread(buf,1,sizeof(buf),fd);
		if (read_size < 0){
			perror("fread");
			break;
		}
		if (write(sd,buf,read_size) != read_size){
			perror("write");
			break;
		}
	}

	fclose(fd);

	close(sd);
	return 0;
}
