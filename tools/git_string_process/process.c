#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>


static const TYPE_NEW =1;
static const TYPE_MOD =2;

int main(int argc,char* argv[])
{
	if (argc !=3){
		printf("argc err!\n inputFileName %d(new file) or %d(modify)\n",TYPE_NEW,TYPE_MOD);
		return -1;
	}
	const char* file_name=argv[1];
	const int show_type=atoi(argv[2]);

	FILE* fd = fopen(file_name,"rb");
	if (fd == NULL) {perror("fopen"); return -1;}
	
	while(!feof(fd))
	{
		int i;
		char buf[1024*2];
		char cmd[sizeof(buf)];
		char dirname[sizeof(buf)]="my_copy/";
		if (fgets(buf,sizeof(buf),fd)==NULL || strlen(buf)<5) continue;


		buf[strlen(buf)-1]='\0';
		const char* origin_name=&buf[13];

		if (buf[1]=='n'){
			strncat(dirname,origin_name,sizeof(dirname));
			for (i=strlen(dirname);i>0;i--)
				if (dirname[i] == '/'){dirname[i]='\0';break;}
			if (opendir(dirname)==NULL){
				snprintf(cmd,sizeof(cmd),"mkdir -p %s",dirname);
				system(cmd);
			}
			snprintf(cmd,sizeof(cmd),"cp %s %s",origin_name,dirname);
			if (show_type != TYPE_NEW) continue;
			printf("%s ",origin_name);
			system(cmd);
		}
		else if (buf[1]=='m'){
			if (show_type != TYPE_MOD) continue;
			printf("%s\n",origin_name);
		}
	}

	fclose(fd);
}
