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
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>

#include <errno.h>
#include <string.h>
enum mpgc_error {
	MPGC_MAIN_FORK = -255,
	MPGC_MAIN_SETSID,
	MPGC_MAIN_ROOT
};
#define mpgc_err_check(ret, exp, err) if (ret == (exp)) { mpgc_error(err, __func__); }
static void mpgc_error(enum mpgc_error err, const char * fname) {
	char * std_errstr = strerror(errno);
	const char * mpgc_errstr;

	/* select mpgc error message */
	switch (err) {
		case MPGC_MAIN_FORK:
			mpgc_errstr = "creating child to become daemon";
			break;
		case MPGC_MAIN_SETSID:
			mpgc_errstr = "creating a new session";
			break;
		case MPGC_MAIN_ROOT:
			mpgc_errstr = "changing daemon root directory";
			break;
		default:
			mpgc_errstr = "UNKNOWN";
			break;
	}

	/* print error message and quit */
	fprintf(stderr, "MPGC ERROR %s @ %s: %s\n", mpgc_errstr, fname, std_errstr);
	exit(err);
}

/* "easter egg" because I felt like it */
#include <stdbool.h>
#include <time.h>

static inline void random_init(void) {
	static bool first_call = true;
	if (first_call) {
		srand((unsigned)time(NULL));
		first_call = false;
	}
}

static const char * random_hello(void) {
	const char * greeting;

	random_init();

	switch (rand() % 3) {
		case 0:
			greeting = "I love the smell of napalm in the morning.";
			break;
		case 1:
			greeting = "That's a pretty fucking good milkshake. I don't know if "
				"it's worth five dollars but it's pretty fucking good.";
			break;
		case 2:
			greeting = "We ride together, we die together. Bad boys for life.";
			break;
		default:
			greeting = "I'm not a puppet, I'm a real boy!";
	}

	return greeting;
}

static const char * random_bye(void) {
	const char * greeting;

	random_init();

	switch (rand() % 3) {
		case 0:
			greeting = "May the force be with you.";
			break;
		case 1:
			greeting = "Zed's dead baby, Zed's dead.";
			break;
		case 2:
			greeting = "So long, and thanks for all the fish.";
			break;
		default:
			greeting = "Live long and prosper. \\\\//";
	}

	return greeting;
}

#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#define MPGC_IDENT "MPGC"
#define MPGC_ROOT "/"

static void become_daemon(void) {
	/* process and session ID's*/
	pid_t pid, sid;

	/* fork, and exit parent when successful */
	mpgc_err_check(-1, pid = fork(), MPGC_MAIN_FORK);
	if (pid > 0) { exit(EXIT_SUCCESS); }

	/* now in child process */
	openlog(MPGC_IDENT, LOG_CONS | LOG_PID, LOG_DAEMON);

	umask(0);	
	mpgc_err_check(-1, sid = setsid(), MPGC_MAIN_SETSID);
	mpgc_err_check(-1, chdir(MPGC_ROOT), MPGC_MAIN_ROOT);

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

	/* we are now a daemon */
	syslog(LOG_INFO, random_hello());
}

int main(int argc, const char *argv[]) {
	struct mpgc_config config;
	struct mpgc_server * server;

	config = optionParse(argc, argv);
	printf("port: %d\n", config.port);
	
	// daemonize this process
	become_daemon();

	server = start_server(config.port);

	// XXX DEBUG: daemon testing: sleep and quit
	sleep(7);

	syslog(LOG_INFO, random_bye());
	stop_server(server);
	closelog();

	return EXIT_SUCCESS;
}
