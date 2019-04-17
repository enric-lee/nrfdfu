#define _GNU_SOURCE
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "conf.h"
#include "log.h"
#include "serialtty.h"
#include "dfu.h"

struct config conf;
int ser_fd;

static struct option options[] = {
	{ "help",	no_argument,		NULL, 'h' },
	{ "debug",      optional_argument,	NULL, 'd' },
	{ "port",       required_argument,	NULL, 'p' },
	{ NULL, 0, NULL, 0 }
};

static void usage(void)
{
	fprintf(stderr, "nrfserdfu [options]\n"
			"options:\n"
			"[-h --help]\n"
			"[-d --debug]\n"
			"[-p --port]\n");
}

static void main_options(int argc, char* argv[])
{
	int n = 0;
	while (n >= 0) {
		n = getopt_long(argc, argv, "hd::p:", options, NULL);
		if (n < 0)
			continue;
		switch (n) {
		case 'h':
			usage();
			break;
		case 'd':
			if (optarg == NULL)
				conf.debug = 1;
			else
				conf.debug = atoi(optarg);
			break;
		case 'p':
			strncpy(conf.serport, optarg, CONF_MAX_LEN);
			break;
		}
	}
}

int main(int argc, char *argv[])
{
	bool ret;
	main_options(argc, argv);

	FILE* fp1 = fopen("/home/br1/ble-radar-0.0-35-gc6cff41-DFU/ble-radar-0.0-35-gc6cff41.dat", "r");
	if (fp1 == NULL) {
		LOG_ERR("Error opening file");
		return EXIT_FAILURE;
	}

	FILE* fp2 = fopen("/home/br1/ble-radar-0.0-35-gc6cff41-DFU/ble-radar-0.0-35-gc6cff41.bin", "r");
	if (fp2 == NULL) {
		LOG_ERR("Error opening file");
		return EXIT_FAILURE;
	}

	LOG_INF("Port %s", conf.serport);
	ser_fd = serial_init(conf.serport);

	do {
		ret = dfu_ping();
		if (!ret)
			sleep(1);
	} while (!ret);

	dfu_set_packet_receive_notification(0);
	dfu_get_serial_mtu();
	dfu_select_object(1);
	dfu_create_object(1, 141);
	dfu_object_write(fp1);
	dfu_get_crc();
	dfu_object_execute();

	dfu_object_write_procedure(1, fp2);

	serial_fini(ser_fd);
}