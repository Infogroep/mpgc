// A simple program that computes the square root of a number
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "TutorialConfig.h"
// the following header actually lives in a subdirectory, but cmake will take
// care of this, and make sure the compiler can find this header
// (personally I forsee problems when different headers have the same
// filename ...)
#include "mysqrt.h"

int main (int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stdout,"%s Version %d.%d\n",
				argv[0],
				Tutorial_VERSION_MAJOR,
				Tutorial_VERSION_MINOR);
		fprintf(stdout,"Usage: %s number\n",argv[0]);
		return 1;
	}
	double inputValue = atof(argv[1]);
	double outputValue = mysqrt(inputValue);
	fprintf(stdout,"The square root of %g is %g\n",
			inputValue, outputValue);
	return 0;
}
