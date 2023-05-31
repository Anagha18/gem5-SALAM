#include "../vadd_clstr_hw_defines.h"

// typedef unsigned array_t[256];

// void vadd(unsigned* a, unsigned* b, unsigned* c) {
// 	// int i, count;
// 	// // #pragma clang loop vectorize(enable)
// 	// #pragma clang loop vectorize_width(2)
// 	// for (i=0; i<256; i++) {
// 	// 	c[i] = a[i] + b[i];
// 	// }
// 	// count = 0;
// 	return;
// }

void top(unsigned* array_acc) {

	*(array_acc+0)=5;

	return;
}