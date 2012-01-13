#include <stdlib.h>
#include <ppl4.h>

#include "winmusik.h"

int main (int argc,char **argv)
{
	ppldd ret;
	File ff;
	ret=ChopID3Tags_internal("/mp3/mp3/105/017.mp3");
	printf ("ret: %u\n",ret);
	return 0;
}
