/*
 *
 *	Display Hex Digits from VHDL
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "sharedmem.h" 

struct int_bounds {
  int left;
  int right;
  char dir;
  unsigned int len;
};

struct ghdl_string {
  char *base;
  struct int_bounds *bounds;
};

int fd;
char* FILEPATH = SHAREDMEMORYFILE;	
void *map = NULL;

int init(int dummy)
{

	int result;
	struct SharedMemoryLayout* shm;
	int pid;
    char* const* argv = {NULL};

	/*	create the mmap'ed file we will use to communicate 
		with the gui						*/

	/*	1. see if file exists (simulation already running; exit)
									*/
    if(access(FILEPATH,F_OK) != -1)    
    {
		perror("Shared memory file exists. Another simulation is already running");
		exit(EXIT_FAILURE);
	}

	/*	2. open the file	*/

	fd = open(FILEPATH, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
    	if (fd == -1) {
		perror("Error opening file for writing");
		exit(EXIT_FAILURE);
    	}	

	/*	3. grow file	*/
	result = lseek(fd, SHAREDMEMORYSIZE-1, SEEK_SET);
   	if (result == -1) {
		close(fd);
		perror("Error calling lseek() to 'stretch' the file");
		exit(EXIT_FAILURE);
    }

	/*	4. write through at end of file	*/

	result = write(fd, "", 1);
    if (result != 1) {
		close(fd);
		perror("Error writing last byte of the file");
		exit(EXIT_FAILURE);
    }

	/*	5. mmap		*/

	map = mmap(0, SHAREDMEMORYSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
		close(fd);
		perror("Error mmapping the file");
		exit(EXIT_FAILURE);
    }	

	/*	6. initialise memory	*/
	
	shm = (struct SharedMemoryLayout*)map;

	shm->requestUIExit = 0;
	shm->uiExited = 0;	

	for (int i = 0; i < 8; i++)
	{
		shm->sevenSegmentData[i] = 1;	
	}

	/*	7. Fork off the gui process	*/

	pid = fork();

	if (pid == 0)
	{
		execvp("hexdisplay/hexdisplay-qt", argv);
	}

	return 0;
}

int shutdown(int dummy)
{

	struct SharedMemoryLayout* shm;
	
	/*	Kill gui thread by writing kill through to mmap'ed file	*/

	shm = (struct SharedMemoryLayout*)map;

	shm->requestUIExit = 1;

	/*	Wait for gui process to terminate	*/

	printf("Waiting for UI to exit...\n");
	while (!shm->uiExited){};

	/*	munmap memory	*/

    if (munmap(map, SHAREDMEMORYSIZE) == -1) {
		perror("Error un-mmapping the file\n");
    }

	/*	delete the shared memory file	*/
	if (unlink(FILEPATH) == -1) 
	{
		perror("Error deleting the memory mapped file\n");
	}

	return 0;
}

int display_hex_digits(int display, int segment, struct ghdl_string* val)
{
	char string[1024];

	struct SharedMemoryLayout* shm;
	
	shm = (struct SharedMemoryLayout*)map;

	strncpy(string, val->base, val->bounds->len);
	string[val->bounds->len] = '\0';

	//printf("Display: %d, segment: %d : %s\n", display, segment, string);

	if (strcmp(string,"'1'") == 0)
		shm->sevenSegmentData[segment] = 1;
	else
		shm->sevenSegmentData[segment] = 0;
	
	//printf("%d\n", shm->sevenSegmentData[segment]);
	
	return 0;
}

int hex_display_ui_exited(int dummy)
{
	struct SharedMemoryLayout* shm;
	
	shm = (struct SharedMemoryLayout*)map;
	
	return (shm->uiExited);

}
