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

#pragma once

#include <microhttpd.h>

struct mpgc_server {
	struct MHD_Daemon * daemon;
};

struct mpgc_server * start_server(int port);

void stop_server(struct mpgc_server * server);
