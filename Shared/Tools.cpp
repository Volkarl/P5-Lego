#include <math.h>
#include "Connectivity.h"
#include "Tools.h"

size_t intlen(int i) {
	if (i == 0) return 1;
	else if (i < 0) return 2 + static_cast<size_t>(log10(-i));
	else if (i > 0) return 1 + static_cast<size_t>(log10(i));

	return 0; // Compiler starts crying QQ
}

void numberToCharArray(unsigned char *array, int number)
{
	for (int i = intlen(number); i > 0; i--)
	{
		array[i] = static_cast<unsigned char>(number % 10);
		number /= 10;
	}
}

size_t insertNumberToArray(unsigned char *array, size_t offset, int number)
{
	size_t numberLen = intlen(number);
	unsigned char *numArray = new unsigned char[numberLen];
	numberToCharArray(numArray, number);

	strncpy( ((char*) array) + offset, (char*)numArray, numberLen + 2);
	array[offset] = PACKET_SEPERATOR; // TODO: Change

	return offset += numberLen + 1;
}
