#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<syslog.h>
#include<stdlib.h>

#define __log_msg(...)  syslog(LOG_INFO, __VA_ARGS__ )  
#define __log_err(...)  syslog(LOG_ERR, __VA_ARGS__ )

#define TST_FILE	"tst.txt"

#define LEN(arr)	(sizeof(arr)/sizeof(arr[0]))

int main(int argc, char** argb)
{
	openlog("Testing", 0, LOG_USER);
	const char msg[] = "Hello World\n";
	char buf[32];
	int fd = open(TST_FILE, O_RDWR|O_APPEND|O_CREAT, S_IROTH|S_IWOTH|S_IRGRP|S_IWGRP|S_IRUSR|S_IWUSR);
	if(write(fd, msg, LEN(msg)) == -1) {
		__log_err("write: %s\n", strerror(errno));
		closelog();
		exit(1);
	}

	if(read(fd, buf, 32) == -1){
		__log_err("read: %s\n", strerror(errno));
		closelog();
		exit(1);
	}
	
	if(!strcmp(msg, buf)) {
		__log_msg("match\n");
	} else {
		__log_msg("not match\n");
	}

	return 0;
}
