/*
 * c-practice 01 - 字符串四个基础函数
 * 作者：zklin · 2026-10-26 · OnlineGDB 调通
 * 本题踩过的坑（以后写代码前先想起它们）：
 *   1. while(p != 0) 检查的是指针地址，永远为真 —— 查字符要用 *p
 *   2. buf[i] = p 拷的是地址不是字符
 *   3. for(i=0; i<10, j>=0; i++) 逗号表达式只取最后一个条件
 *   4. 不拷 '\0'，printf 会一路读到垃圾
 *   5. 查错对象：strcpy 应查 *src，不是 *dst（垃圾内存碰巧放过我一次）
 *   6. 返回起点要在开头就存 ret，结尾 p 已经走到 '\0' 了
 *   7. const 口诀：只读的加 const，要写的不加
 */
#include <stdio.h>

size_t my_strlen(const char* s)
{
    const char* p = s;
    while (*p != 0) {
        p++;
    }
    return (size_t)(p - s);
}

char* my_strcpy(char *dst, const char *src)
{
    char* ret = dst;
    while (*src != 0) {
        *dst = *src;
        dst++;
        src++;
    }
    *dst = 0;
    return ret;
}

int my_strcmp(const char* a, const char* b)
{
    while (*a == *b && *a != 0) {
        a++;
        b++;
    }
    return *a - *b;
}

char* my_strcat(char *dst, const char* src)
{
    char *ret = dst;
    char *p = dst;
    while (*p != 0) {
        p++;
    }
    while (*src != 0) {
        *p = *src;
        p++;
        src++;
    }
    *p = 0;
    return ret;
}

int main(void) {
    char buf[64];
    char *r;

    printf("strlen(\"hello\") = %u\n", (unsigned)my_strlen("hello"));

    my_strcpy(buf, "hello");
    printf("after strcpy buf=\"%s\"\n", buf);

    printf("strcmp(\"abc\",\"abc\") = %d\n", my_strcmp("abc","abc"));
    printf("strcmp(\"abc\",\"abd\") = %d\n", my_strcmp("abc","abd"));
    printf("strcmp(\"abd\",\"abc\") = %d\n", my_strcmp("abd","abc"));

    r = my_strcat(buf, " world");
    printf("after strcat buf=\"%s\" ret=\"%s\"\n", buf, r);

    return 0;
}
