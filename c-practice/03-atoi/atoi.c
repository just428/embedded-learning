/*********************************************************************
 * 03 atoi / 简单 itoa —— 串口命令解析基本功
 * 手写实现，不依赖 stdlib / string。
 *
 * 今日踩坑记录（2026-10-28）：
 *  atoi:
 *   1) 参数用 const char* 收字符串，别套 memcpy 的 void*。
 *      void* 是“泛型门”，atoi 只吃字符串，不需要泛型；且 void* 不能
 *      解引用、不能做算术，强行 (int)dst 只会把指针变整数。
 *   2) 千万别写 int s = (int)dst; 把指针强转成 int —— 之后 *s 解引用的
 *      已不是你的字符串，结果全输出 0。参数本身就是 char*，直接用它走。
 *   3) val / neg 要先 int 声明再用，否则编译报 undeclared。
 *   4) 连续多位数字用 while 累积（val = val*10 + (*s-'0')），不是 if
 *      （if 只处理一位）。判断“是数字”用 *s>='0' && *s<='9'。
 *  itoa:
 *   5) 第二参数是 char *dst（指针），写 char dst 只能装一个字符。
 *   6) dst[i] = n%10 漏了 + '0' —— 写进去的是整数不是字符，打印乱码 'p'。
 *      正确：dst[i] = (n%10) + '0'。
 *   7) 反转条件 l < r，写成 l < i 会让 r 减成负数、访问 dst[-1] 越界
 *      → 段错误 exit 139（和题②的 139 同源：越界访问）。
 *   8) 函数声明返回 char*，末尾必须 return dst; 否则返回垃圾指针。
 *********************************************************************/

#include <stdio.h>

int my_atoi(const char *s) {
    int val = 0, neg = 0;
    while (*s == ' ') s++;                          /* 跳过前导空格 */
    if (*s == '-') { neg = 1; s++; }                /* 负号：记一下，跳过 */
    else if (*s == '+') { s++; }                    /* 正号：直接跳过 */
    while (*s >= '0' && *s <= '9') {                /* 连续数字累积 */
        val = val * 10 + (*s - '0');                /* 字符数字 → 整数 */
        s++;
    }
    return neg ? -val : val;
}

char* my_itoa(int n, char *dst) {
    int neg = 0, i = 0;
    if (n < 0) { neg = 1; n = -n; }                 /* 先记负号，当正数处理 */
    if (n == 0) { dst[0] = '0'; dst[1] = '\0'; return dst; }  /* 0 的特例 */

    while (n > 0) {                                 /* 从个位往左取，暂时是反的 */
        dst[i++] = (n % 10) + '0';                  /* 取个位 + '0' 变字符 */
        n /= 10;                                    /* 砍掉个位 */
    }
    if (neg) dst[i++] = '-';                        /* 负号放末尾 */
    dst[i] = '\0';                                  /* 收尾 */

    for (int l = 0, r = i - 1; l < r; l++, r--) {   /* 反转成正常顺序 */
        char t = dst[l]; dst[l] = dst[r]; dst[r] = t;
    }
    return dst;                                     /* 必须返回 */
}

int main(void) {
    printf("atoi(\"123\")  = %d\n", my_atoi("123"));
    printf("atoi(\"-456\") = %d\n", my_atoi("-456"));
    printf("atoi(\"+78\")  = %d\n", my_atoi("+78"));
    printf("atoi(\"  42\")  = %d\n", my_atoi("  42"));
    printf("atoi(\"0\")     = %d\n", my_atoi("0"));

    char b1[16], b2[16], b3[16];
    my_itoa(123,  b1); printf("itoa(123)  = %s\n", b1);
    my_itoa(-456, b2); printf("itoa(-456) = %s\n", b2);
    my_itoa(0,    b3); printf("itoa(0)    = %s\n", b3);
    return 0;
}
