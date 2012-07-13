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

#include "server.h"

#include <stdlib.h>
#include <string.h>

#include <microhttpd.h>

static int answer_to_connection(
		__attribute__((unused)) void *cls,
		struct MHD_Connection *connection,
		__attribute__((unused)) const char *url,
		__attribute__((unused)) const char *method,
		__attribute__((unused)) const char *version,
		__attribute__((unused)) const char *upload_data,
		__attribute__((unused)) size_t *upload_data_size,
		__attribute__((unused)) void **con_cls)
{
	const char *page = "<html><body>Hello, browser! mpgcd reporting!</body></html>";
	struct MHD_Response *response;
	int ret;

	response = MHD_create_response_from_buffer(
			strlen(page),
			(void *)page,
			MHD_RESPMEM_PERSISTENT);
	ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
	MHD_destroy_response(response);

	return ret;
}

struct mpgc_server * start_server(int port) {
	struct mpgc_server * server = malloc(sizeof(struct mpgc_server));

	server->daemon = MHD_start_daemon(
			MHD_USE_SELECT_INTERNALLY,
			port,
			NULL,
			NULL,
			&answer_to_connection,
			NULL,
			MHD_OPTION_END);

	if (!server->daemon) {
		free(server);
		return NULL;
	}

	return server;
}

void stop_server(struct mpgc_server * server) {
	MHD_stop_daemon(server->daemon);
	free(server);
}
