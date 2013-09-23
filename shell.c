#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2
int getLine(char *prompt, char *buff, size_t sz) {
	int ch, extra;
	
	// Get line with buffer overrun protection.
	if (prompt != NULL) {
		printf("%s", prompt);
		fflush(stdout);
	}
	if (fgets(buff, sz, stdin) == NULL) return NO_INPUT;
	
	// If it was too long, there'll be no newline. In that case, we flush
	// to end of line so that excess doesn't affect the next call.
	if (buff[strlen(buff)-1] != '\n') {
		extra = 0;
		while (((ch = getchar()) != '\n') && (ch != EOF))
			extra = 1;
		return (extra == 1) ? TOO_LONG : OK;
	}

	// Otherwise remove newline and give string back to caller.
	buff[strlen(buff)-1] = '\0';
	return OK;
}

void clearScreen() {
	// print several blank lines
	for (int l=0; l<32; l++) {
		fputs("\n",stdout);
	}
	for (int l=0; l<32; l++) {
		fputs("\033[A\033[2K",stdout);
	}
	// rewind(stdout);
	// ftruncate(1,0);
}

int main(int argc, char *argv[])
{
	char prompt[] = "shell";
	int rc;
	char buff[255];
	pid_t pid;
	int status;
	
	strcat(strcat(prompt, (geteuid() == 0 ? "#" : "$")), " ");
	
	while (strncmp(buff, "exit", 4) != 0)
	{
		rc = getLine(prompt, buff, sizeof(buff));
		
		if (rc == NO_INPUT) {
			puts("No Input\n");
		}
		
		if (rc == TOO_LONG) {
			printf("Input too long [%s]\n", buff);
		}
		
		if (strncmp(buff, "clear", 5) == 0) {
			clearScreen();
		} else if (strncmp(buff, "exit", 4) == 0) {
			fputs("\n",stdout);
			exit(0);
		} else {
			printf("command: %s\n", buff);
			
			/**
			 * pid_t fork() spawns a new child process which is a clone of its parent
			 * in the parent, fork() returns the child's pid, whereas
			 * in the child, fork() returns a value of 0
			 */
			if ((pid = fork()) < 0) {
				perror("fork");
				continue;
			}
			
			if (pid == 0) {
				/*** child ***/
				puts("Hello world!\n");
				// execlp(buf, buf, (char *)0);
				// printf("Couldn't execute: %s", buf);
				exit(127);
			} else {
				/*** parent ***/
				if ((pid = waitpid(pid, &status, 0)) < 0)
					perror("waitpid");
				printf("%% ");
			}
		}
	}
	
	/*
	char buf[30];
	DIR *dp;
	struct dirent *dirp;
	bzero(buf, sizeof(buf));
	
	if (argc < 2) {
		fprintf(stderr, "usage: shell [options]\n");
		exit(1);
	}
	
	if ((dp = opendir(argv[1])) == NULL) {
		strncpy(buf, "can't open ", sizeof("can't open "));
		strcat(buf, argv[1]);
		strcat(buf, "\n");
		fprintf(stderr, buf);
		exit(1);
	}
	while ((dirp = readdir(dp)) != NULL)
		printf("%s\n", dirp->d_name);
	closedir(dp);
	*/
	
	exit(0);
}
