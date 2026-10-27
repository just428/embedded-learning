/*
 * 02 memcpy / memmove —— 2026-10-27 独立完成
 * 验收输出（逐字）：
 *   [naive] ababab
 *   [move ] ababcd
 *   [back ] cdefef
 *   [copy ] hello
 *   [ints ] 10 20 30 40 50
 *
 * 今日踩坑留痕（最重要）：
 *  1) void* 两不能：不能解引用(*)、不能算术(++)。函数内必须转 char* 才能按字节走。
 *  2) void* 是泛型指针：任何类型指针(int*/struct*)都能直接传进来，不用强转。
 *  3) src 只被读 → const char*；dst 要被写 → char*（只读加 const，要写不加）。
 *  4) 指针分工：ret 记住起点永远不动，return ret；别让“记起点的人”去当“干活的笔”。
 *  5) size_t 无符号：`for(size_t i=n-1; i>=0; i--)` 会绕回 42 亿 → d[巨值] 段错误(exit 139)。
 *     倒序用哨兵写法 `for(size_t i=n; i>0; i--){ size_t k=i-1; ... }` 或 `while(n--) d[n]=s[n];`。
 *  6) 重叠处理：目标在源后面(d>=s)会踩自己尾巴 → 倒序拷；否则正序就行。
 *  7) 编译错看第一条(病根)；exit 139 = SIGSEGV 段错误，编译期不报错、运行期才炸，板上即 HardFault。
 */

#include <stdio.h>

void *my_memcpy(void *dst, const void *src, size_t n)
{
    char *ret = (char *)dst;          // ret 记住起点，全程不动
    const char *s = (const char *)src; // void* 内部转 char* 才能下标/解引用；src 只读加 const
    for (size_t i = 0; i < n; i++) {
        ret[i] = s[i];                 // 朴素版：从前往后逐字节拷（重叠时会读脏）
    }
    return ret;                        // 还起点，类型 void* 与签名一致
}

void *my_memmove(void *dst, const void *src, size_t n)
{
    char *d = (char *)dst;
    const char *s = (const char *)src;
    if (d < s) {                       // 目标在源前面（或没重叠）→ 正序安全
        for (size_t i = 0; i < n; i++) {
            d[i] = s[i];
        }
    } else {                           // 目标在源后面且重叠 → 倒序防踩尾巴
        while (n--) {                  // n 自己往下数，n=0 自然停，避开无符号下溢
            d[n] = s[n];
        }
    }
    return d;
}

int main(void)
{
    char b1[32] = "abcdef";
    char b2[32] = "abcdef";
    char b3[32] = "abcdef";

    my_memcpy(b1 + 2, b1, 4);        // 目标区在后，和源重叠！(朴素版读脏)
    printf("[naive] %s\n", b1);

    my_memmove(b2 + 2, b2, 4);       // 同一个重叠场景（倒序救回）
    printf("[move ] %s\n", b2);

    my_memmove(b3, b3 + 2, 4);       // 目标区在前，反方向重叠（正序安全）
    printf("[back ] %s\n", b3);

    char s[16] = "hello", d[16] = "XXXXXXXX";
    my_memcpy(d, s, 6);              // 不重叠，拷 6 字节（含 '\0'）
    printf("[copy ] %s\n", d);

    int a[5] = {10, 20, 30, 40, 50}, t[5] = {0};
    my_memmove(t, a, sizeof(a));     // 一次搬 20 字节，验证 void* 通用
    printf("[ints ] ");
    for (int i = 0; i < 5; i++) printf("%d ", t[i]);
    printf("\n");

    return 0;
}
