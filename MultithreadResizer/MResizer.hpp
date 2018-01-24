#ifndef M_RESIZER
#define M_RESIZER
#include <vector>

void resize(std::vector<char> &srcRaw, std::vector<char> &dstRaw, int width, int height, int numberOfThreads = 0);
#endif