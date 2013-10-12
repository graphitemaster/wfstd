#include "wfSystemInfo.h"
#include <stdio.h>

int main() 
{
	printf("RDTSC:   %d\n", (int)wfCPUHasRDTSC());
	printf("Altivec: %d\n", (int)wfCPUHasALTIVEC());
	printf("MMX:     %d\n", (int)wfCPUHasMMX());
	printf("3DNow:   %d\n", (int)wfCPUHas3DNOW());
	printf("SSE:     %d\n", (int)wfCPUHasSSE());
	printf("SSE2:    %d\n", (int)wfCPUHasSSE2());
	printf("SSE3:    %d\n", (int)wfCPUHasSSE3());
	printf("SSE4.1:  %d\n", (int)wfCPUHasSSE41());
	printf("SSE4.2:  %d\n", (int)wfCPUHasSSE42());
	printf("NEON:    %d\n", (int)wfCPUHasNEON());
	
	return 0;
}
