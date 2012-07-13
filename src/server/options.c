/* Copyright 2011 Robrecht Dewaele
 *
 * This file is part of mpgc.
 *
 * mpgc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mpgc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with mpgc.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "options.h"

#include <popt.h>

#include <stdio.h>

/* string identifying lines logged by the mpgc daemon */
#define MPGC_IDENT "MPGC"
/* the mpgc root */
#define MPGC_ROOT "/tmp"

struct mpgc_config optionParse(int argc, const char *argv[]) {
	const char * root = MPGC_ROOT;
	const char * log_ident = MPGC_IDENT;
	int port = 8080;

	poptContext context;

	struct poptOption optionsTable[] = {
		{ "root", 'r', POPT_ARG_INT, &root, 0,
			"root directory", NULL },

		{ "ident", 'i', POPT_ARG_STRING, &log_ident, 0,
			"logifile identification", NULL },

		{ "port", 'p', POPT_ARG_STRING, &port, 0,
			"server port", NULL },

		POPT_AUTOHELP POPT_TABLEEND
	};

	context = poptGetContext(*argv, argc, argv, optionsTable, 0);

	// currently no options need special handling and no options return a value
	poptGetNextOpt(context);

	poptFreeContext(context);

	return (struct mpgc_config){ root, log_ident, port };
}
