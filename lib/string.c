#include "lib/string.h"
int strncmp ( char * s1, char * s2, int n)
{ 
	if(!n)return(0); //n为无符号整形变量;如果n为0,则返回0

	//在接下来的while函数中
	//第一个循环条件：--n,如果比较到前n个字符则退出循环
	//第二个循环条件：*s1,如果s1指向的字符串末尾退出循环
	//第二个循环条件：*s1 == *s2,如果两字符比较不等则退出循环
	while (--n && *s1 && *s1 == *s2)
	{ 
		s1++; //S1指针自加1,指向下一个字符
		s2++;//S2指针自加1,指向下一个字符
	} 
	return( *s1 - *s2 );//返回比较结果
}



//将数字转换成ASICA码
//数字前面的 0 不被显示出来, 比如 0000B800 被显示成 B800
char * itoa(char * str, int num)
{
	char *	p = str;
	char	ch;
	int	i;
	int	flag = false;
	//显示16进制前面的0x
	*p++ = '0';
	*p++ = 'x';
	//为0
	if(num == 0){
		*p++ = '0';
	}
	else{	//4位4位的分解出来
		for(i=28;i>=0;i-=4){		//从最高得4位开始
			ch = (num >> i) & 0xF;	//取得4位
			//if((ch > 0)){
				//flag = true;
			ch += '0';			//大于0就+'0'变成ASICA的数字
			if(ch > '9'){		//大于9就加上7变成ASICA的字母
				ch += 7;		
			}
			*p++ = ch;			//指针地址上记录下来。
			
		}
	}
	*p = 0;							//最后在指针地址后加个0用于字符串结束
	return str;
}

void *memset(void* src, uint8_t value, uint32_t size) {
   uint8_t* s = (uint8_t*)src;
   while (size-- > 0){
	   *s++ = value;
   }
}

/* 将src_起始的size个字节复制到dst_ */
void memcpy(void* dst_, const void* src_, uint32_t size) {
  
   uint8_t* dst = dst_;
   const uint8_t* src = src_;
   while (size-- > 0)
      *dst++ = *src++;
}

/* 将字符串从src_复制到dst_ */
char* strcpy(char* dst_, const char* src_) {
  
   char* r = dst_;		       // 用来返回目的字符串起始地址
   while((*dst_++ = *src_++));
   return r;
}

/* 返回字符串长度 */
uint32_t strlen(const char* str) {
  
   const char* p = str;
   while(*p++);
   return (p - str - 1);
}

/* 比较两个字符串,若a_中的字符大于b_中的字符返回1,相等时返回0,否则返回-1. */
int8_t strcmp (const char* a, const char* b) {
  
   while (*a != 0 && *a == *b) {
      a++;
      b++;
   }
/* 如果*a小于*b就返回-1,否则就属于*a大于等于*b的情况。在后面的布尔表达式"*a > *b"中,
 * 若*a大于*b,表达式就等于1,否则就表达式不成立,也就是布尔值为0,恰恰表示*a等于*b */
   return *a < *b ? -1 : *a > *b;
}

int memcmp(const void * s1, const void *s2, int n)
{
	if ((s1 == 0) || (s2 == 0)) { /* for robustness */
		return (s1 - s2);
	}

	const char * p1 = (const char *)s1;
	const char * p2 = (const char *)s2;
	int i;
	for (i = 0; i < n; i++,p1++,p2++) {
		if (*p1 != *p2) {
			return (*p1 - *p2);
		}
	}
	return 0;
}
char* strrchr(const char* str, int c)
{
   
    char* ret = NULL;
    while (*str)
    {
        if (*str == (char)c)//如果存在，ret保存地址
            ret = (char *)str;//强制转换指针类型，赋给ret 
        str++;
    }
    if ((char)c == *str) //考虑str第一个字符为'\0'的情况  
        ret = (char *)str; //强制转换指针类型，赋给ret  

    return ret;
}

char* strcat(char* strDest , const char* strSrc)
{
    char* address=strDest;
    while(*strDest)//是while(*strDest!=’\0’)的简化形式
    {
        strDest++;
    }
    while(*strDest++=*strSrc++);
    return address;//为了实现链式操作，将目的地址返回
}

