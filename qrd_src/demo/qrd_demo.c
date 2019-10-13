#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifdef __unix__
#include <curses.h>
#define PAUSE()         \
	printf("\r\n\r\n"); \
	getchar();
#elif defined(_WIN32) || defined(WIN32)
#include <conio.h>
#define PAUSE()         \
	printf("\r\n\r\n"); \
	system("pause");
#endif

#include <png.h>
#include <time.h>

#include "include/qrd_demo.h"

#if USE_TICKS_INSTEAD_MILLISECONDS
#define START_COUNT_TIME() const long long t = get_ticks_count();
#define END_COUNT_TIME() get_ticks_count() - t;
#else
#define START_COUNT_TIME() const long long t = clock();
#define END_COUNT_TIME() clock() - t;
#endif


int main(void)
{
	int test_case_index = 0;
	char *dir_path = DIR_PATH_IMAGES;
	test_info info;

	printf("--------------------------------------------------------------------------------");
	printf("\r\n Light demo test \r\n");
	printf("\r\n\t 1. Decode certain image");
	printf("\r\n\t 2. Decode all images");
	printf("\r\n %s \r\n", dir_path);
	printf("\r\n Choose test: ");
	scanf("%d", &test_case_index);

	switch (test_case_index)
	{
	case 1:
		printf(run_demo_png(dir_path, open_and_choose_file(dir_path)).message);
		break;

	case 2:
		info = test_all_files(dir_path);

		for (int i = 0; i < info.number_of_files; i++)
			printf(info.content[i].message);

		printf("\r\n AVERAGE CLOCK: %f", info.average_clock);
		printf("\r\n NUMBER OF PASSED FILES: %u", info.number_of_passed);
		printf("\r\n NUMBER OF UNPASSED FILES: %u", info.number_of_unpassed);

#if SHOW_LIST_PASSED
		printf("\r\n PASSED FILES: %s \r\n", info.list_of_passed);
#endif
#if SHOW_LIST_UNPASSED
		printf("\r\n UNPASSED FILES: %s \r\n", info.list_of_unpassed);
#endif
#if SHOW_RESULT_FLAGS
		printf("\r\n FLAGS: %s \r\n", info.flags);
#endif
		break;

	default:
		printf("NOT CORRECT INPUT");
		break;
	}

	PAUSE();
	return SUCCESS;
}

static inline unsigned long long get_ticks_count()
{
	asm("rdtsc;");
}

static char *open_and_choose_file(const char *dir_path)
{
	int selected_file_index = 0;

	printf("\r\n Choose one of of exiting file in directory:\r\n");
	scan_files((const char *)dir_path, NULL);
	printf("\r\n\r\n Select a file: ");
	scanf("%hd", &selected_file_index);

	if (selected_file_index == 0)
		system("@cls||clear");

	return &file_names[selected_file_index];
}

static test_info test_all_files(const char *dir_path)
{
	int
		*number_of_files = 0,
		number_of_passed = 0,
		number_of_unpassed = 0,
		selected_file_index = 0;

	unsigned long long
		clock_sum = 0;

	char *flags = "\r\n";
	char *list_of_unpassed = "\r\n";
	char *list_of_passed = "\r\n";

	scan_files((const char *)dir_path, number_of_files);

	test_info current_test_info;

	for (selected_file_index = 0; selected_file_index < *number_of_files; selected_file_index++)
	{
		const char *file_name = &file_names[selected_file_index];
		const result info = run_demo_png(dir_path, file_name);

		if (info.status)
		{
			clock_sum += info.clock_diff;
			list_of_passed = concat(list_of_passed, concat("\r\n  ", file_name));
			flags = concat(flags, "1\r\n");
			number_of_passed++;
		}
		else
		{
			list_of_unpassed = concat(list_of_unpassed, concat("\r\n  ", file_name));
			flags = concat(flags, "0\r\n");
			number_of_unpassed++;
		}

		current_test_info.content[selected_file_index] = info;
	}

	current_test_info.number_of_files = number_of_files;
	current_test_info.number_of_passed = number_of_passed;
	current_test_info.number_of_unpassed = number_of_passed;
	current_test_info.average_clock = (double)clock_sum / number_of_passed;
	current_test_info.list_of_passed = list_of_passed;
	current_test_info.list_of_unpassed = list_of_unpassed;
	current_test_info.flags = flags;

	return current_test_info;
}

static result run_demo_png(char *dir_path, char *file_name)
{
	result current_result;
	current_result.name = (char *)file_name;
	current_result.status = FAILURE;
	current_result.clock_diff = 0;
	char *file_path = concat(dir_path, file_name);

	struct quirc *qr = quirc_new();
	{
		if (qr && check_if_png(file_path) && load_png(qr, file_path))
		{
			struct quirc_code *code = malloc(sizeof(struct quirc_code));
			struct quirc_data *data = malloc(sizeof(struct quirc_data));

			START_COUNT_TIME();

			const uint_fast8_t num_codes = quirc_count(qr);
			quirc_decode_error_t error = 1;
			const char *decoded_data = "";

			for (uint_fast8_t i = 0; i < num_codes; i++)
			{
				quirc_extract(qr, i, code);
				error = quirc_decode(code, data);

				if (strlen(data->payload) == 0)
					error = 1;
				else
					decoded_data = concat((const char *)decoded_data, (const char *)data->payload);

				if (num_codes > 0 && i < num_codes - 1)
					decoded_data = concat((const char *)decoded_data, ", ");
			}

			current_result.clock_diff = END_COUNT_TIME();

			if (code)
				free(code);

			if (error)
			{
				strncpy(
					current_result.decoded_data,
					(char *)quirc_strerror(error),
					strlen(quirc_strerror(error) + 1));

				sprintf(
					(char *)current_result.message,
					"\r\n   UNPASSED \r\n FILE: %s \r\n EXCEPTION: %s \r\n",
					current_result.name,
					(char *)current_result.decoded_data);
			}
			else
			{
				strncpy(
					current_result.decoded_data,
					decoded_data,
					strlen(decoded_data) + 1);

				sprintf(
					(char *)current_result.message,
					"\r\n   PASSED \r\n FILE: %s \r\n DECODED DATA: %s \r\n CLOCK: %llu \r\n",
					current_result.name,
					(char *)current_result.decoded_data,
					current_result.clock_diff);

				current_result.status = SUCCESS;
			}

			if (data)
				free(data);
		}
		else
		{
			perror("The object was not created, the file is not a png, or it is impossible to open it");
		}

		quirc_destroy(qr);

		return current_result;
	}
}

static int check_if_png(const char *file_path)
{
	FILE *infile = NULL;
	int is_png = FAILURE;

	unsigned char buf[PNG_BYTES_TO_CHECK];

	if ((infile = fopen(file_path, "rb")) == NULL)
		goto out;

	if (fread(buf, 1, PNG_BYTES_TO_CHECK, infile) != PNG_BYTES_TO_CHECK)
		goto out;

	if (png_sig_cmp(buf, (png_size_t)0, PNG_BYTES_TO_CHECK) == 0)
		is_png = SUCCESS;

	/* FALLTHROUGH */
out:
	if (infile)
		fclose(infile);

	return is_png;
}

static int load_png(struct quirc *qr, const char *file_path)
{
	FILE *infile = NULL;
	png_uint_16 trns = NULL;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	int is_png_loaded = FAILURE;
	uint_fast8_t number_passes = 1;

	if ((infile = fopen(file_path, "rb")) == NULL)
		goto out;

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
		goto out;

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		goto out;

	if (setjmp(png_jmpbuf(png_ptr)))
		goto out;

	png_init_io(png_ptr, infile);
	png_read_info(png_ptr, info_ptr);

	const png_byte color_type = png_get_color_type(png_ptr, info_ptr);
	const png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	const png_byte interlace_type = png_get_interlace_type(png_ptr, info_ptr);

	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png_ptr);

	if ((trns = png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)))
		png_set_tRNS_to_alpha(png_ptr);

	if (bit_depth == 16)
	{
#if PNG_LIBPNG_VER >= 10504
		png_set_scale_16(png_ptr);
#else
		png_set_strip_16(png_ptr);
#endif
	}

	if (trns || color_type & PNG_COLOR_MASK_ALPHA)
		png_set_strip_alpha(png_ptr);

	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png_ptr);

	if (color_type == PNG_COLOR_TYPE_PALETTE ||
		color_type == PNG_COLOR_TYPE_RGB ||
		color_type == PNG_COLOR_TYPE_RGB_ALPHA)
	{
		png_set_rgb_to_gray_fixed(png_ptr, 1, -1, -1);
	}

	if (interlace_type != PNG_INTERLACE_NONE)
		number_passes = png_set_interlace_handling(png_ptr);

	png_read_update_info(png_ptr, info_ptr);

	const uint_fast16_t width = png_get_image_width(png_ptr, info_ptr);
	const uint_fast16_t height = png_get_image_height(png_ptr, info_ptr);
	const uint_fast16_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	if (rowbytes != width)
	{
		fprintf(
			stderr,
			"load_png: expected row bytes to be %u but got %u \r\n",
			width, rowbytes);
		goto out;
	}

	if (quirc_resize(qr, width, height) < 0)
		goto out;

	uint_fast8_t *image = quirc_begin(qr, NULL, NULL);
	{
		for (uint_fast8_t pass = 0; pass < number_passes; pass++)
		{
			for (register uint_fast16_t y = 0; y < height; y++)
			{
				png_bytep row_pointer = image + y * width;
				png_read_rows(png_ptr, &row_pointer, NULL, 1);
			}
		}

		png_read_end(png_ptr, info_ptr);
	}

	is_png_loaded = SUCCESS;

out:
	/* FALLTHROUGH && CLEAN-UP */
	if (png_ptr)
	{
		if (info_ptr)
			png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		else
			png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
	}
	if (infile)
		fclose(infile);

	quirc_end(qr);

	return is_png_loaded;
}
