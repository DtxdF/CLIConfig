#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int exists(char * filename) {
	int fd;

	if ((fd = open(filename, O_RDONLY)) == -1) {
		return (errno == ENOENT ? 0 : 0);
	
	}

	close(fd);

	return 1;

}
