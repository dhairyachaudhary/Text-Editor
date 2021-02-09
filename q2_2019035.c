/*
	Name: Dhairya Chaudhary
	Roll No: 2019035
	Section: B
*/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/file.h>
#include <termios.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

struct textList {
	int index;
	struct textList*prev;
	struct textList*next;
	char s[200];
};

void editMenu();
void addTextLine(struct textList *temp);
void initEditor(char* fl);
void editorPrint();
void closeEditor();
void reader(char* fl);
void edit();
void addNew(struct textList *q, char t[]);
void removeText(struct textList *p);
void textEdit(struct textList *p);
void saveText();

struct textList *head;
char *file;

FILE *fp=NULL;

int getch(){
	int ip;
	struct termios oldt,
	newt;
	tcgetattr(STDIN_FILENO,&oldt);
	newt=oldt;
	newt.c_lflag&=~(ICANON|ECHO);
	tcsetattr(STDIN_FILENO,TCSANOW,&newt);
	ip = getchar();
	tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
	return ip;
}

void clearlist() {
	while(head->next!=NULL) {
		removeText(head->next);
	}
}

void editorPrint() {
	struct textList *temp=head;
	system("clear");
	while(temp->next!=NULL){
		temp=temp->next;
		printf("%s",temp->s);
	}
}

void editMenu(void) {
	printf("\n\n");
	printf("U- Move up\n");
	printf("D- Move down\n");
	printf("E- edit the current line\n");
	printf("A- Append new line at current location\n");
	printf("R- Remove line at current location\n");
	printf("S- Save and exit\n");
	printf("X- Exit\n");
}

void textEdit(struct textList *p) {
	printf("\nEnter the new contents for the line :\n");
	gets(p->s);
	printf("\nChanges made.\n");
}

void addNew(struct textList *txt, char arr[]) {
	struct textList*p=(struct textList*)malloc(sizeof(struct textList));
	struct textList *temp=txt->next;
	strcpy(p->s,arr);
	p->prev=txt;
	p->next=txt->next;

	if((txt->next)!=NULL) {
		((txt->next)->prev)=p;
		while(temp!=NULL) {
			(temp->index)++;

			temp=temp->next;
		}
	}

	txt->next=p;
	p->index = txt->index+1;
}

void addTextLine(struct textList *txt) {
	char ce[200];
	printf("\nenter the new line :\n");
	gets(ce);
	addNew(txt, "\n");
	addNew(txt, ce);
}

void reader(char* fl) {
	FILE *fle; 
    char filename[100], c; 
	fle=fopen(fl, "r");   

	system("clear");
	getch();
	//system("clear");
		fflush(stdin);


    c=fgetc(fle); 
    while (c != EOF) { 
        printf ("%c", c); 
        c = fgetc(fle); 
    } 
}

void removeText(struct textList *tx) {
	struct textList *temp=tx->next;
	((tx->prev->next))=tx->next;

	if(tx->next!=NULL) {
		((tx->next)->prev)=tx->prev;
		while(temp!=NULL) {
			(temp->index)--;

			temp=temp->next;
		}
	}
	free(tx);
}

void initEditor(char* fl) {
	char c;
	char ce[200];
	struct textList *buffer=head;


	if(fp!=NULL) {
		printf("\nThis file is already open in another instance.\nDo you want to continue? (Y/N) :");
		c=getch();
		if(c=='N'){
			return;
		} else{
			closeEditor();
		}
	}

	fflush(stdin);
	file=fl;
	fflush(stdin);
	clearlist();

	fp=fopen(file,"r");
	if(fp==NULL) {
		printf("\nFile not found. Create new? (Y/N) :");
		c=getchar();
		//getchar();
		if(c=='N'){
			return;
		} else {
			fp=fopen(file,"w");
			edit();
			return;
		}
	}

	if(feof(fp)) {
		return;
	}

	while((fgets(ce,201,fp))!=NULL) {
		addNew(buffer, ce);
		buffer=buffer->next;
	}

	edit();
}


void saveText() {
	struct textList *temp=head->next;
	fclose(fp);
	fp=fopen(file,"w");

	while(temp!=NULL) {
		fprintf(fp,"%s",temp->s);
		temp=temp->next;
	}
}

void closeEditor() {
	if(fp==NULL) {
		return;
	}

	fclose(fp);
	fp=NULL;
	clearlist();
}

void edit() {
	struct textList *temp=head->next;
	char c,d;

	system("clear");

	if(fp==NULL){
		printf("\nNo file is open\n");
		return;
	}

	system("clear");
	editorPrint();

	if(temp!=NULL){
		printf("\n \nCurrent Location: %d",temp->index);
	} else {
		temp=head;
	}

	editMenu();

	while(1) {
		c=getch();

		switch(c){
			case 'E' :
			textEdit(temp);
			editorPrint();
			break;

			case 'A' :
			addTextLine(temp);
			temp=temp->next;
			editorPrint();
			printf("\n\nCurrent Location:  %d",temp->index);
			break;

			case 'U' :
			if(temp==head) {
				printf("\nThe file is empty.");
				break;
			}

			if(temp->prev!=head) {
				temp=temp->prev;
				printf("\n\nCurrent Location: %d",temp->index);
			}
			break;

			case 'D' :
			if(temp->next!=NULL) {
				temp=temp->next;
				printf("\nCurrent Location: %d",temp->index);
			} else if(temp==head) {
				printf("\nThe file is empty.");
			}
			break;

			case 'R' :
			if(temp==head){
				printf("\nFile empty\n");
				break;
			}

			temp=temp->prev;
			removeText(temp->next);
			printf("\nLine deleted\n");
			editorPrint();

			if(temp->index){
				printf("\nCurrent Location: %d",temp->index);
			}

			if(((temp->prev)==NULL)&&(temp->next)!=NULL){
				temp=temp->next;
			} else if((temp==head)&&((temp->next)==NULL)){
				printf("\nFile empty");
			}
			break;

			case 'S' :
			saveText();
			closeEditor();
			return;
			break;

			case 'X' :
			printf("\nSave changes?(Y/N) :");
			while (1){
				d=getch();
				if(d=='Y'){
					saveText();
					break;
				} else if(d=='N'){
					break;
				}
			}
			closeEditor();
			return;
			break;
		}
	}
}


int main(int argc, char **argv){
	if  (argc<2){
		errno=71;
		perror("Error: ");
		//printf("%s\n", );
		exit(EXIT_FAILURE);
	}

	if (argc>2){
		errno=7;
		perror("Error: ");
		exit(EXIT_FAILURE);
	}

	int fd;

	fd = open(argv[1], O_RDWR);

	if (fd==-1){
		errno=9;
		perror("Error: ");
		exit(EXIT_FAILURE);
	}

	if (flock(fd, LOCK_EX|LOCK_NB) == -1) {
		if (errno == EWOULDBLOCK) {
			printf("File already in use. Press 1 to proceed, press 0 to exit.\n");
			int n;
			scanf("%d",&n);
			if (n==0){
				exit(EXIT_SUCCESS);
			} else {
                //do nothing
			}
		} else {
			errno=37;
			perror("Error: ");
			exit(EXIT_FAILURE);
		}
	}

	char c;

	head=(struct textList*)malloc(sizeof(struct textList));
	(head->next)=(head->prev)=NULL;
	(head->index)=0;

	while(1) {
		system("clear");
		printf("UwU's Text Editor!!! \n");
		printf("\nWelcome! Pick from the following options: \n");
		printf("L- Load file for editing\n");
		printf("E- Exit\n");

		c=getch();
		switch(c) {
			case 'L' :
			initEditor(argv[1]);
			break;

			case 'R' :
			reader(argv[1]);
			break;

			case 'E' :
			system("clear");
			exit(EXIT_SUCCESS);
			break;
		}
	}
}