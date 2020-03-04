
#include <sys/param.h>
#include <sys/tty.h>
#include <localenv.h>

#ifdef FIONREAD
empty (fd)
{
    long count;

    if (ioctl (fd, FIONREAD, &count) < 0)
	return 1;
    return count <= 0;
}
#endif FIONREAD
