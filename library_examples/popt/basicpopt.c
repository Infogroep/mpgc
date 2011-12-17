// standard C library headers
#include <stdio.h>
#include <stdlib.h>

// popt - command line argument passing
#include <popt.h>

int main(int argc, const char *argv[]) {
	char *echoString = malloc(1024);
	struct poptOption echo = {
		// long option name
		"echo",
		// short option name
		'e',
		// see man page, describes whether an option requires an argument, and of what
		// type it should be
		POPT_ARG_STRING,
		// address at which to store the pointer to the option argument
		&echoString,
		// 0 means don't return, just update flags
		0,
		// description for autohelp
		"echoes its argument to the standard output",
		// argument description for autohelp
		"the string to print to standard output"
	};


	// will store the result of poptGetNetxOpt during option parsing
	char currentOption;
	// context for parsing options
	poptContext optionsContext;
	// array of all available options
	struct poptOption optionsTable[] = {
		echo,
		// add automatic help and usage options
		// (--usage, --help and -?)
		POPT_AUTOHELP
		// mark end of the options table
		{NULL, 0, 0, NULL, 0}
	};
	// initialize popt context
	optionsContext = poptGetContext(argv[0], argc, argv, optionsTable, 0);

	// check whether arguments have been given, else print help info
	if (argc < 2) {		
		poptPrintUsage(optionsContext, stderr, 0);
		poptFreeContext(optionsContext);
		free(echoString);
		exit(1);
	}

	// process options
	while ((currentOption = poptGetNextOpt(optionsContext)) >= 0) {
		switch (currentOption) {
			case 'e':
				printf("echo: %s\n", echoString);
		}
	}
	poptFreeContext(optionsContext);
	free(echoString);
	return 0;
}
