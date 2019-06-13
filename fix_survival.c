#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

const char *logpath="C:\\fixed_viruses.log";
FILE *logfile=NULL;
int nfixed_viruses=0;	// How many viruses have this program fixed
char buf[BUFSIZ];

void iterate_thru_directory(void)
{
	int ret;
	DIR *dir;
	struct dirent *ent;
	ret=(int)getcwd(buf,BUFSIZ);	// Get the current directory
	if(ret==0){
		perror("getcwd() failed");
		exit(-1);
	}
	if((dir=opendir(buf))==NULL){
		perror("opendir() failed");
		exit(-1);
	}
	while((ent=readdir(dir))!=NULL){
		printf("%s\n",ent->d_name);
		sprintf(buf,"attrib -s -h -r %s",ent->d_name);
		if((ret=system(buf))!=0){
			perror("system() failed");
			exit(-1);
		}
	}
	unlink("SURVIVAL.vbe");		// Remove the virus file
}

void clean_temp(void)
{
	char *tmp;
	if((tmp=getenv("TMP"))==NULL){
		perror("getenv() failed");
		exit(-1);
	}
	sprintf(buf,"%s\\SURVIVAL.vbe",tmp);
	unlink(buf);	// No error checks needed here
}

int main(int argc,char **argv)
{
	logfile=fopen(logpath,"r");
	if(logfile!=NULL){	// If file exists
		fscanf(logfile,"%d",&nfixed_viruses);
		fclose(logfile);
	}
	printf("Dummy...\n");
	clean_temp();
	iterate_thru_directory();
	logfile=fopen(logpath,"w");
	nfixed_viruses++;
	fprintf(logfile,"%d",nfixed_viruses);
	return 0;
}
