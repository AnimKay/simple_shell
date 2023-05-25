#include "shell.h"

/**
 * bfree - frees a pointer and changes the address to null
 * @ptr: address of the pointer to be freed
 *
 * Return: 1 if freed and 0 if not.
 */
int bfree(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
		return (1);
	}
	return (0);
}
