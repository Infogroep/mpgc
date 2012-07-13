/* Copyright 2012 Joris Vandermeersch
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

#include "daemon.h"
#include "server.h"
#include "options.h"

#include <stdio.h>
#include <signal.h>
#include <syslog.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void signal_handler(int sig)
{
	switch(sig)
	{
		case SIGHUP:
			syslog(LOG_WARNING, "received SIGHUP signal.");
			break;
		case SIGINT:
		case SIGTERM:
			syslog(LOG_INFO, "daemon exiting");
			daemon_shutdown();
			exit(EXIT_SUCCESS);
			break;
		default:
			syslog(LOG_WARNING, "unhandled signal %s", strsignal(sig));
			break;
	}
}

void daemon_shutdown()
{
	stop_server(server);
	close(pidFilehandle);
}

void mpgc_shutdown() {
	char str[10];
	char *pidfile = "/tmp/mpgc/mpgc.pid";
	pidFilehandle = open(pidfile, O_RDONLY);
	read(pidFilehandle, str, 10);
	kill(atoi(str), SIGTERM);
}

void daemonize(char *rundir, char *pidfile)
{
	int pid, sid, i;
	char str[10];
	struct sigaction newSigAction;
	sigset_t newSigSet;

	/* Check if parent process id is set */
	if (getppid() == 1)
	{
		/* PPID exists, therefore we are already a daemon */
		return;
	}

	/* Set signal mask - signals we want to block */
	sigemptyset(&newSigSet);
	sigaddset(&newSigSet, SIGCHLD);  /* ignore child - i.e. we don't need to wait for it */
	sigaddset(&newSigSet, SIGTSTP);  /* ignore Tty stop signals */
	sigaddset(&newSigSet, SIGTTOU);  /* ignore Tty background writes */
	sigaddset(&newSigSet, SIGTTIN);  /* ignore Tty background reads */
	sigprocmask(SIG_BLOCK, &newSigSet, NULL);   /* Block the above specified signals */

	/* Set up a signal handler */
	newSigAction.sa_handler = signal_handler;
	sigemptyset(&newSigAction.sa_mask);
	newSigAction.sa_flags = 0;

	/* Signals to handle */
	sigaction(SIGHUP, &newSigAction, NULL);     /* catch hangup signal */
	sigaction(SIGTERM, &newSigAction, NULL);    /* catch term signal */
	sigaction(SIGINT, &newSigAction, NULL);     /* catch interrupt signal */

	/* Fork*/
	pid = fork();

	if (pid < 0)
	{
		/* Could not fork */
		exit(EXIT_FAILURE);
	}

	if (pid > 0)
	{
		sleep(2);
		pidFilehandle = open(pidfile, O_RDONLY);
		read(pidFilehandle, str, 10);
		if(pid == atoi(str)) {
			/* Child created ok, so exit parent process */
			printf("child process created: %d\n", pid);
			exit(EXIT_SUCCESS);
		} else {
			printf("daemon already running! pid: %d\n", atoi(str));
			exit(EXIT_SUCCESS);
		}
	}

	/* Child continues */

	umask(027); /* Set file permissions 750 */

	/* Get a new process group */
	sid = setsid();

	if (sid < 0)
	{
		exit(EXIT_FAILURE);
	}

	/* close all descriptors */
	for (i = getdtablesize(); i >= 0; --i)
	{
		close(i);
	}

	/* Route I/O connections */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	chdir(rundir); /* change running directory */

	/* Ensure only one copy */
	pidFilehandle = open(pidfile, O_RDWR|O_CREAT, 0600);

	if (pidFilehandle == -1 )
	{
		/* Couldn't open lock file */
		syslog(LOG_INFO, "could not open PID lock file %s, exiting", pidfile);
		exit(EXIT_FAILURE);
	}

	/* Try to lock file */
	if (lockf(pidFilehandle,F_TLOCK,0) == -1)
	{
		/* Couldn't get lock on lock file */
		syslog(LOG_INFO, "could not lock PID lock file %s, exiting", pidfile);
		exit(EXIT_FAILURE);
	}

	/* Get and format PID */
	sprintf(str, "%d\n", getpid());

	/* write pid to lockfile */
	write(pidFilehandle, str, strlen(str));
}

int start_daemon(struct mpgc_config config)
{
	/* Debug logging
		 setlogmask(LOG_UPTO(LOG_DEBUG));
		 openlog(DAEMON_NAME, LOG_CONS, LOG_USER);
		 */

	/* Logging */
	setlogmask(LOG_UPTO(LOG_INFO));
	openlog(DAEMON_NAME, LOG_CONS | LOG_PERROR, LOG_USER);

	/* Deamonize */
	syslog(LOG_INFO, "daemonizing...");
	daemonize("/tmp/mpgc/", "/tmp/mpgc/mpgc.pid");

	/* needs to be done after daemonizing, because daemonizing closes all descriptors */
	syslog(LOG_INFO, "starting server");
	server = start_server(config.port);

	while (1)
	{
		// do stuff
		sleep(1);
	}
}

