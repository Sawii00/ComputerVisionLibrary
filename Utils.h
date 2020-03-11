#include <cstdint>

uint64_t power(uint64_t n, uint64_t e) {
	while (e > 1) {
		n *= n;
		e--;
	}
	return n;
}
