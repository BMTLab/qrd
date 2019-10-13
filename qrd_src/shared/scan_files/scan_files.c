#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdint.h>
#include <stddef.h>

#include "dirent.h"

#ifndef SCAN_FILES_H
#include "./include/scan_files.h"
#endif

char file_names[MAX_FILES][MAX_FILENAME_LENGTH];

inline const char* concat(const char* s1, const char* s2)
{
	char* result = malloc(strlen(s1) + strlen(s2) + 1);
	strcpy(result, s1);
	strcat(result, s2);

	return result != 0 ? result : NULL;
}

int scan_files(char *dir_path, int *number_of_files)
{
	DIR *dir;
	struct dirent *ent;
	int i = -2;
	number_of_files = -1;

	if ((dir = opendir(dir_path)))
	{
		while ((ent = readdir(dir)))
		{
			if (++i > 0)
			{
				printf("\r\n\t %u. %s", i, ent->d_name);
				strcpy(file_names[i], ent->d_name);
			}
		}
		free(ent);
		closedir(dir);
	}
	else
	{
		return 0;
	}

	/* Return count files in dir */
	number_of_files = i;

	/* Return 1 if success */
	return 1;
}
