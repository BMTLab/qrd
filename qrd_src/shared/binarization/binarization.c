#include "binarization.h"

#define CONTOUR_BLACK (unsigned char) 0
#define CONTOUR_WHITE (unsigned char) 0xFF

static inline double sqr(double _x)
{
	return _x * _x;
}

void threshold(
	unsigned char* src,
	unsigned char* out,
	uint_fast16_t width,
	uint_fast16_t height)
{

	const uint_fast8_t  log_w_ext = 1;
	const uint_fast8_t  std_dev_max = 0x80;
	const uint_fast16_t width_w = width >> 3;
	const uint_fast16_t width_w_half = width_w >> 1;
	float k;

	register uint_fast16_t i, j;
	register uint_fast16_t x, y;

	uint_fast8_t  min_image;
	uint_fast16_t index;
	uint_fast16_t count_w;
	uint_fast16_t count_non_white;
	uint_fast16_t count_w_ext;

	int_fast16_t  x1, y1, x2, y2;
	int_fast16_t  x1_ext, x2_ext, y1_ext, y2_ext;

	unsigned long* integral_image;
	unsigned long  sum_image;
	unsigned long  sum_w;
	unsigned long  sum_w_ext;
	unsigned long  threshold;

	double dev = 0;
	double std_dev = 0;
	double average_w = 0;
	double average_w_ext = 0;

	/* Memory allocation for integral */
	integral_image = calloc(width * height, sizeof(unsigned long*));
	if (!integral_image) goto out;

	/* The calculation of the integral sum and min of the image */
	count_non_white = 0;
	min_image = src[0];

	for (i = 0; i < width; i++)
	{
		sum_image = 0;
		for (j = 0; j < height; j++)
		{
			index = j * width + i;
			sum_image += src[index];

			integral_image[index] = (i == 0)
				? sum_image
				: integral_image[index - 1] + sum_image;

			if (index != 0 && src[index] < min_image)
				min_image = src[index];

			if (src[index] < (unsigned char*) 0x04)
				count_non_white++;
		}
	}

	k = (float) count_non_white / (width * height);

	/* Loop for each pixel */
	for (i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			index = j * width + i;

			/* Finding the coordinates of the corners of the window */
			x1 = i - width_w_half;
			x2 = i + width_w_half;
			y1 = j - width_w_half;
			y2 = j + width_w_half;

			if (x1 < 0)
				x1 = 0;
			if (x2 >= width)
				x2 = width - 1;
			if (y1 < 0)
				y1 = 0;
			if (y2 >= height)
				y2 = height - 1;

			/* Finding the coordinates of the corners of the slightly larger window */
			x1_ext = x1 - (width_w >> log_w_ext);
			x2_ext = x2 + (width_w >> log_w_ext);
			y1_ext = y1 - (width_w >> log_w_ext);
			y2_ext = y2 + (width_w >> log_w_ext);

			if (x1_ext < 0)
				x1_ext = 0;
			if (x2_ext >= width)
				x2_ext = width - 1;
			if (y1_ext < 0)
				y1_ext = 0;
			if (y2_ext >= height)
				y2_ext = height - 1;

			/* Calculating the sum of pixel values for window and extended window */
			sum_w =
				integral_image[y2 * width + x2] -
				integral_image[y1 * width + x2] -
				integral_image[y2 * width + x1] +
				integral_image[y1 * width + x1];

			sum_w_ext =
				integral_image[y2_ext * width + x2_ext] -
				integral_image[y1_ext * width + x2_ext] -
				integral_image[y2_ext * width + x1_ext] +
				integral_image[y1_ext * width + x1_ext];

			/* Calculating the count of pixel for window and extended window */
			count_w = (y2 - y1) * (x2 - x1);
			count_w_ext = (y2_ext - y1_ext) * (x2_ext - x1_ext);

			/* Calculating the mean of pixel values for window and extended window */
			average_w = sum_w / count_w;
			average_w_ext = sum_w_ext / count_w_ext;

			/* Calculation of standard deviation */
			dev = 0;
			for (x = x1_ext; x < x2_ext; x++)
			{
				for (y = y1_ext; y < y2_ext; y++)
				{
					dev += sqr(src[y * width + x] - average_w_ext);
				}
			}
			std_dev = sqrt(dev / count_w_ext);

			/* Threshold Calculation */
			threshold =
				(1 - k) * average_w	+
				k * min_image +
				k * (std_dev / std_dev_max) * (average_w - min_image);

			/* Segmentation */
			out[index] = (unsigned long) src[index] <= (unsigned long) threshold
				? CONTOUR_BLACK
				: CONTOUR_WHITE;
		}
	}

out:
	/* Clean-up */
	if (integral_image) free(integral_image);
}
