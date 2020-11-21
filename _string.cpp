#include <string.h>

// 字符串长度
size_t mystrlen(const char* str)
{
	size_t cnt = 0;
	while(*str++ != 0) cnt ++;

	return cnt;
}

// 字符串拷贝
char* mystrcpy(char* dst, const char* src)
{
	char* s = dst;
	while(*dst++ == *src++);
	
	return s;
}

