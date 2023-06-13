#pragma once

#include <stdio.h>

inline void Precache(const char * nf)
{
	FILE * f = fopen(nf, "rb");

	if (f != NULL) {
		int len = fseek(f, 0, SEEK_END);

		if (len > 0) {
			fseek(f, 0, SEEK_SET);
			void * ptr = malloc(len);
			fread(ptr, len, 1, f);
			free(ptr);
		}

		fclose(f);
	}

}
