#ifndef QRD_DEMO_H
#define QRD_DEMO_H

#define SUCCESS 1
#define FAILURE 0

#include "qrd_demo_config.h"
#include "../../shared/quirc/lib/quirc.h"
#include "../../shared/scan_files/include/scan_files.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct result_t
	{
		char *name;
		char decoded_data[MAX_DECODED_SYMBOLS];
		char message[MAX_MESSAGE_LEN];
		int status;
		unsigned long long clock_diff;
	} result;

	typedef struct test_info_t
	{
		int number_of_files;
		int number_of_passed;
		int number_of_unpassed;
		double average_clock;
		char *flags;
		char *list_of_passed;
		char *list_of_unpassed;
		result content[MAX_FILES];
	} test_info;

	int main(void);

	static char *open_and_choose_file(const char *);
	static test_info test_all_files(const char *);
	static result run_demo_png(char *, char *);
	static int check_if_png(const char *);
	static int load_png(struct quirc *, const char *);

#ifdef __cplusplus
}
#endif

#endif /* QRD_DEMO_H */
