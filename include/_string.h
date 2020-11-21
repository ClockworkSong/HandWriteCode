#ifndef _STRING_H_
#define _STRING_H_

//#include <iostream>

/** 
 * @brief	字符串长度计算函数
 * @param	参数: str代表被计算的字符串
 *
 * @return	说明: 字符串str长度
 */
size_t mystrlen(const char* str);

/** 
 * @brief	字符串拷贝函数
 * @param	参数1: dst代表目标字符串，建议进行初始化
 * @param	参数2: src代表原始字符串
 *
 * @return	说明: 目标字符串dst首地址
 */
char* mystrcpy(char* dst, const char* src);

#endif// _STRING_H_
