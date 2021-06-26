#include <klog.h>

void klog(int type, char *message) {
	if (type == 0) printf("[ OK ] Initialized %s\r\n", message);
	if (type == 1) printf("[ FAIL ] %s not initialized\r\n", message);
	if (type == 2) printf("[ WARN ] %s\r\n", message);
	if (type == 3) printf("[ INFO ] %s\r\n", message);
}
