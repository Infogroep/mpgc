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
#include "server.h"

#include <stdio.h>

int main(int argc, const char *argv[]) {
	struct mpgc_config config;
	struct mpgc_server * server;

	config = optionParse(argc, argv);
	printf("port: %d\n", config.port);

	server = start_server(config.port);

	// don't return immediately
	getchar();

	stop_server(server);
	return 0;
}
