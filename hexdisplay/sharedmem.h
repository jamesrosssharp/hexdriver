/*
		   Shared memory layout
										*/

struct __attribute__((__packed__)) SharedMemoryLayout 
{

	int sevenSegmentData[8];
	int requestUIExit;
	int uiExited;

};

#define SHAREDMEMORYSIZE sizeof(struct SharedMemoryLayout)
#define SHAREDMEMORYFILE "/tmp/hexdisp.mem"
