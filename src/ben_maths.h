#pragma once

// Lookup tables for integer angles and values, the original authors measured a large speedup over the libm calls

#ifndef M_PI
#define M_PI 3.141592654
#endif

// Angles must be in positive degrees. Computes scale * cos(angle)
int cosi(int scale, int angle);
int sini(int scale, int angle);
int sqrti(int x);
