// SPDX-License-Identifier: BSD-3-Clause

#include <string.h>

char *strcpy(char *destination, const char *source)
{
	int index = 0;
	while (*(source + index) != '\0')
	{
		*(destination + index) = *(source + index);
		index++;
	}
	*(destination + index) = '\0';
	return destination;
}

char *strncpy(char *destination, const char *source, size_t len)
{
	size_t limit = strlen(source);
	if (limit > len)
	{
		int index = 0;
		while (len)
		{
			*(destination + index) = *(source + index);
			index++;
			len--;
		}
	}
	else
		strcpy(destination, source);
	return destination;
}

char *strcat(char *destination, const char *source)
{
	size_t len = strlen(destination);
	strcpy(destination + len, source);
	return destination;
}

char *strncat(char *destination, const char *source, size_t len)
{
	size_t limit = strlen(source);
	if (len > limit)
		strcat(destination, source);
	else
	{
		char *aux = destination;
		size_t length = 0;
		while (*destination != '\0')
		{
			destination++;
			length++;
		}
		destination = aux;
		int i = length;
		while (len)
		{
			destination[i] = *source;
			source++;
			len--;
			i++;
		}
		destination[i] = '\0';
	}
	return destination;
}

int strcmp(const char *str1, const char *str2)
{
	if (strlen(str1) > strlen(str2))
		return 1;
	else if (strlen(str1) < strlen(str2))
		return -1;
	while (*str1 != '\0' && *str2 != '\0')
	{
		if (*str2 > *str1)
			return -1;
		else if (*str1 > *str2)
			return 1;
		str1++;
		str2++;
	}

	return 0;
}

int strncmp(const char *str1, const char *str2, size_t len)
{
	char copy_str1[len];
	char copy_str2[len];
	size_t len1 = strlen(str1);
	size_t len2 = strlen(str2);

	if (len2 > len && len1 > len)
	{
		int limit = len;
		int i = 0;
		while (limit)
		{
			copy_str1[i] = *(str1 + i);
			copy_str2[i] = *(str2 + i);
			limit--;
			i++;
		}
		copy_str1[i] = '\0';
		copy_str2[i] = '\0';
		return strcmp(copy_str1, copy_str2);
	}
	else
	{
		if (len >= len1 && len >= len2)
			return strcmp(str1, str2);
		else if (len1 > len && len >= len2)
		{
			int limit = len;
			int i = 0;
			while (limit)
			{
				copy_str1[i] = *(str1 + i);
				limit--;
				i++;
			}
			copy_str1[i] = '\0';
			return strcmp(copy_str1, str2);
		}
		else
		{
			int limit = len;
			int i = 0;
			while (limit)
			{
				copy_str2[i] = *(str2 + i);
				limit--;
				i++;
			}
			copy_str2[i] = '\0';
			return strcmp(str1, copy_str2);
		}
	}
}

size_t strlen(const char *str)
{
	size_t i = 0;

	for (; *str != '\0'; str++, i++)
		;

	return i;
}

char *strchr(const char *str, int c)
{
	char *result = (char *)str;
	int i = *str;
	while (i != c && i != '\0')
	{
		str++;
		result = (char *)str;
		i = *str;
	}
	if (i == c)
		return result;
	else
		return NULL;
}

char *strrchr(const char *str, int c)
{
	char *aux = strchr(str, c);
	char *result = NULL;
	while (aux != NULL)
	{
		result = aux;
		aux = strchr(aux + 1, c);
	}
	return result;
}

char *strstr(const char *haystack, const char *needle)
{
	char *aux = strchr(haystack, *needle);
	char *result = aux;
	size_t match = strlen(needle);
	int i = 0;
	while (aux)
	{
		if (match == 0)
			break;
		if (*aux == *(needle + i))
		{
			i++;
			aux++;
			match--;
		}
		else
		{
			match = strlen(needle);
			aux = strchr(aux + 1, *needle);
			result = aux;
			i = 0;
		}
	}
	return result;
}

char *strrstr(const char *haystack, const char *needle)
{
	char *aux = strstr(haystack, needle);
	char *result = NULL;
	while (aux != NULL)
	{
		result = aux;
		aux = strstr(aux + 1, needle);
	}
	return result;
}

void *memcpy(void *destination, const void *source, size_t num)
{
	size_t index;
	if ((unsigned long)source % sizeof(long) == 0 &&
		num % sizeof(long) == 0 && (unsigned long)destination % sizeof(long) == 0)
	{
		for (index = 0; index < num / sizeof(long); index++)
		{
			*((long *)destination + index) = *((const long *)source + index);
		}
	}
	else
	{
		index = 0;
		for (; index < num;)
		{
			*((char *)destination + index) = *((const char *)source + index);
			index++;
		}
	}
	return destination;
}

void *memmove(void *destination, const void *source, size_t num)
{
	size_t index;
	if ((unsigned long)destination < (unsigned long)source)
	{
		return memcpy(destination, source, num);
	}
	if ((unsigned long)source % sizeof(long) == 0 &&
		num % sizeof(long) == 0 && (unsigned long)destination % sizeof(long) == 0)
	{
		for (index = num / sizeof(long); index > 0; index--)
		{
			*((long *)destination + index - 1) = *((const long *)source + index - 1);
		}
	}
	else
	{
		index = num;
		for (; index > 0;)
		{
			*((char *)destination + index - 1) = *((const char *)source + index - 1);
			index--;
		}
	}
	return destination;
}

int memcmp(const void *ptr1, const void *ptr2, size_t num)
{
	size_t index;
	if (ptr1 == ptr2)
		return 0;
	else
	{
		for (index = 0; index < num; index++)
		{
			if ((*(char *)ptr1) == (*(char *)ptr2))
			{
				ptr1 = (char *)ptr1 + 1;
				ptr2 = (char *)ptr2 + 1;
			}
		}
	}
	if (index == num)
		return 0;
	else
		return *(char *)ptr1 - *(char *)ptr2;
}

void *memset(void *source, int value, size_t num)
{
	size_t limit = num;
	unsigned char *p = (unsigned char *)source;
	while (limit)
	{
		*p++ = (unsigned char)value;
		limit--;
	}
	return source;
}
