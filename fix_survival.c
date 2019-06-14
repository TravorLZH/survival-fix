#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <windows.h>

const char *logpath="C:\\fixed_viruses.log";
FILE *logfile=NULL;
int nfixed_viruses=0;	// How many viruses have this program fixed
char buf[BUFSIZ];
char buf2[BUFSIZ];

void iterate_thru_directory(void)
{
	int ret;
	DIR *dir;
	struct dirent *ent;
	puts("Removing shortcuts...");
	system("del *.lnk");	// Delete symbolic link files (No error checks)
	ret=(int)getcwd(buf,BUFSIZ);	// Get the current directory
	if(ret==0){
		perror("getcwd() failed");
		exit(-1);
	}
	puts("Removing SURVIVAL.vbe...");
	unlink("SURVIVAL.vbe");		// Remove the virus file
	if((dir=opendir(buf))==NULL){
		perror("opendir() failed");
		exit(-1);
	}
	while((ent=readdir(dir))!=NULL){
		printf("Recovering %s...\n",ent->d_name);
		//sprintf(buf,"attrib -s -h -r \"%s\"",ent->d_name);
		//if((ret=system(buf))!=0){
		//	perror("system() failed");
		//	exit(-1);
		//}
		sprintf(buf2,"%s\\%s",buf,ent->d_name);
		SetFileAttributesA((const char*)buf2,
				FILE_ATTRIBUTE_NORMAL);
	}
	// Hide "System Volume Information" folder
	sprintf(buf2,"%s\\System Volume Information",buf);
	SetFileAttributesA((const char*)buf2,
			FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM);
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

void kill_wscript(void)
{
	// No error checks, sometimes `wscript.exe' is not running
	system("taskkill /f /im wscript.exe 2>NUL");
}

int main(int argc,char **argv)
{
	int i;
	logfile=fopen(logpath,"r");
	if(logfile!=NULL){	// If file exists
		fscanf(logfile,"%d",&nfixed_viruses);
		fclose(logfile);
	}
	system("color 0a");
	puts("SURVIVAL.vbe removal by Travor Liu <travor_lzh@outlook.com>");
	puts("===========================================================");
	kill_wscript();
	clean_temp();
	iterate_thru_directory();
	logfile=fopen(logpath,"w");
	nfixed_viruses++;
	fprintf(logfile,"%d",nfixed_viruses);
	fclose(logfile);
	printf("You have now removed %d SURVIVAL.vbe viruses",nfixed_viruses);
	system("pause");
	return 0;
}
