#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	/* Our process ID and Session ID */
	pid_t pid, sid;

	/* Fork off the parent process */
	pid = fork();
	if (pid < 0) {
		exit(EXIT_FAILURE);
	}
	/* If we got a good PID, then we can exit the parent process. */
	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

	/* Change the file mode mask */
	umask(0);

	/* Open any logs here (omitted) */


	/* Create a new SID for the child process */
	sid = setsid();
	if (sid < 0) {
		/* Log the failure */
		exit(EXIT_FAILURE);
	}

	/* Change the current working directory */
	if ((chdir("/")) < 0) {
		/* Log the failure */
		exit(EXIT_FAILURE);
	}

	/* Close out the standard file descriptors */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	/* Daemon-specific initialization goes here */
	int i = 3;
	openlog(*argv, LOG_PID, LOG_USER);
	syslog(LOG_INFO, "Starting the big loop.\n");

	/* The Big Loop */
	while (i--) {
		/* Do some task here ... */
		syslog(LOG_INFO, "Big loop iterations left: %d\n", i);

		sleep(5); /* wait */
	}
	syslog(LOG_INFO, "Big loop ended. Closing daemon.");
	closelog();

	exit(EXIT_SUCCESS);
}
