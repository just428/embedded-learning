# 嵌入式 C 复习专项练习题（带答案）

> 适用：跟完翁恺 C 到「字符串函数」章、中断后复习用
> 用法：每一题先自己写，再对照「参考实现」；能在 PC 上 `gcc` 跑通才算过关
> 重点：指针（W2） > 字符串（W3） > 结构体（W4 预览）

---

## 第一部分 · 指针（W2，最重要）

### P1 用指针交换两个数
写 `swap(int *a, int *b)`，在主函数里交换 `x=3, y=5`，打印结果。
```c
#include <stdio.h>
void swap(int *a, int *b){ int t = *a; *a = *b; *b = t; }
int main(){ int x=3,y=5; swap(&x,&y); printf("%d %d\n",x,y); } // 5 3
```
要点：传值只改副本，传指针才能改外部变量——这是嵌入式里"函数里改全局/外设状态"的基础。

### P2 指针遍历数组求最大值
用指针（不用下标）求 `int a[5]={3,9,1,7,4}` 的最大值。
```c
int a[5]={3,9,1,7,4}; int *p=a, max=*p;
for(int i=1;i<5;i++) if(*(p+i)>max) max=*(p+i);
printf("%d\n", max); // 9
```

### P3 自己实现 strlen
不用库函数，写 `int my_strlen(const char *s)`。
```c
int my_strlen(const char *s){ int n=0; while(*s){ n++; s++; } return n; }
```
要点：`const char*` 表示不修改指向内容，只是读。

### P4 指针算术（必考）
`int a[3]; int *p=a;` 问：`p+1` 比 `p` 的地址大几个字节？`char *q` 呢？
```
int*   : +4 字节（sizeof(int)）
char*  : +1 字节（sizeof(char)）
```
这就是数组能用 `p+1` 访问下一个元素的根本原因。

### P5 函数指针 + 回调（HAL 库思想）
定义函数指针类型，把 `on_press` 注册成按键回调并调用。
```c
#include <stdio.h>
typedef void (*cb_t)(void);
void on_press(void){ printf("key pressed\n"); }
int main(){ cb_t handler = on_press; handler(); } // key pressed
```
要点：HAL 库的 `HAL_UART_RxCpltCallback` 就是这种"把函数当变量传"的机制。

### P6 const 与指针（面试常混）
区分下面三种写法：
```c
const int *p;    // 指向的内容不可改（p 可改指向）
int *const p;    // 指针本身不可改（指向固定，内容可改）
int const *p;    // 同 const int*，内容不可改
```

### P7 二级指针
`int a=10; int *p=&a; int **pp=&p;` 打印 `**pp`。
```c
printf("%d\n", **pp); // 10
```
要点：`pp` 存 `p` 的地址，`*pp` 是 `p`（即 `&a`），`**pp` 是 `a`。

### P8 指针访问结构体（接 W4）
用 `->` 访问结构体成员。
```c
typedef struct{ int id; int val; } Dev;
Dev d={1,88}; Dev *pd=&d;
printf("%d %d\n", pd->id, pd->val); // 1 88
```

### P9 数组名 vs 指针（易错）
`int arr[5]; int *p=arr;` 打印 `sizeof(arr)` 和 `sizeof(p)`（64 位机）。
```c
printf("%zu %zu\n", sizeof(arr), sizeof(p)); // 20  8
```
要点：数组名是"常量指针"，`sizeof` 拿到整个数组字节数；指针变量 `sizeof` 只是指针本身大小（8 字节）。

### P10 野指针防范
下面哪段会崩？怎么改？
```c
int *p;        // 未初始化，里面是随机地址
*p = 10;       // 往随机地址写 → 可能崩溃

// 正确写法：
int *p = NULL;
int x = 0;
p = &x;        // 用前指向有效内存
if (p) *p = 10;
```

### P11 用数组指针遍历二维数组
写 `void print(int (*p)[4], int rows)` 打印 3×4 数组；再用指针形式 `*(*(p+i)+j)` 打印一遍，确认结果完全一样。
```c
#include <stdio.h>
void print(int (*p)[4], int rows){
    for(int i=0;i<rows;i++){
        for(int j=0;j<4;j++) printf("%3d", p[i][j]);
        printf("\n");
    }
}
void print_ptr(int (*p)[4], int rows){      // 等价的指针写法
    for(int i=0;i<rows;i++){
        for(int j=0;j<4;j++) printf("%3d", *(*(p+i)+j));
        printf("\n");
    }
}
int main(){
    int a[3][4]={{1,2,3,4},{5,6,7,8},{9,10,11,12}};
    print(a,3); print_ptr(a,3);             // a 退化为 int(*)[4]
}
```
要点：`p[i][j]` 就是 `*(*(p+i)+j)` 的语法糖，两种写法完全等价。形参必须写列数 `4`，否则编译器不知道一行多长。

### P12 找出最大值的行和列（综合指针传参）
写 `int find_max(int (*p)[4], int rows, int *r, int *c)`，返回最大值，并用指针参数把行列"带回来"。
```c
int find_max(int (*p)[4], int rows, int *r, int *c){
    int max = p[0][0]; *r = 0; *c = 0;
    for(int i=0;i<rows;i++)
        for(int j=0;j<4;j++)
            if(p[i][j] > max){ max = p[i][j]; *r = i; *c = j; }
    return max;
}
// 调用：
int rr, cc;
int m = find_max(a, 3, &rr, &cc);
printf("max=%d at [%d][%d]\n", m, rr, cc);   // max=12 at [2][3]
```
要点：函数想"返回多个结果"就传指针进去——和 P1 的 swap 是完全同一个道理。

### P13 一维缓冲当二维用（嵌入式：显存按行操作）
有 `uint16_t fb[12]` 模拟一块 3 行 × 4 列的显存，用数组指针 `(uint16_t (*)[4])fb` 把它当二维数组看，写个函数把第 `row` 行刷成指定颜色，并验证 `fb[5]` 被改到。
```c
#include <stdio.h>
#include <stdint.h>
#define W 4
uint16_t fb[12] = {0};                        // 模拟 3×4 显存，内存连续

void fill_row(uint16_t (*p)[W], int row, uint16_t color){
    for(int c=0; c<W; c++) p[row][c] = color;
}
int main(){
    uint16_t (*p)[W] = (uint16_t (*)[W])fb;   // 一维缓冲 → 按行看
    fill_row(p, 1, 0xF800);                   // 把第 1 行刷成红色
    printf("fb[5] = %04X\n", fb[5]);          // F800（= 第1行第1列）
    printf("行长 = %zd 字节\n", (char*)&p[1][0] - (char*)&p[0][0]); // 8（4列×2字节）
}
```
要点：内存里根本没有"行"，`p[1][1]` 就是 `fb[1*4+1]`。LCD 显存、ADC 多通道缓冲都是这样"按行切"来访问的。

---

## 第二部分 · 字符串（W3）

### S1 自己实现 strcpy
```c
char* my_strcpy(char *d, const char *s){
    char *t=d;
    while((*t++ = *s++));
    return d;
}
```
陷阱：目标 `d` 必须够大，否则越界——嵌入式里多用 `snprintf` 替代。

### S2 自己实现 strcmp
```c
int my_strcmp(const char *a, const char *b){
    while(*a && *a==*b){ a++; b++; }
    return *a - *b;
}
```
返回值：0 相等，负数 `a<b`，正数 `a>b`。

### S3 自己实现 strcat
```c
char* my_strcat(char *d, const char *s){
    char *t=d;
    while(*t) t++;          // 先移到末尾 '\0'
    while((*t++ = *s++));
    return d;
}
```

### S4 sprintf 组包（嵌入式上报必用）
把温度、湿度拼成一个上报串（用 `snprintf` 防溢出）。
```c
char buf[64];
float temp=26.5f, hum=60.0f;
snprintf(buf, sizeof(buf), "{\"temp\":%.1f,\"hum\":%.1f}", temp, hum);
printf("%s\n", buf); // {"temp":26.5,"hum":60.0}
```

### S5 字符串反转（原地）
```c
#include <string.h>
void rev(char *s){
    int i=0, j=(int)strlen(s)-1;
    while(i<j){ char t=s[i]; s[i]=s[j]; s[j]=t; i++; j--; }
}
```

### S6 统计单词数（空格分隔）
```c
int words(const char *s){
    int n=0, in=0;
    while(*s){
        if(*s!=' ' && !in){ n++; in=1; }
        else if(*s==' ') in=0;
        s++;
    }
    return n;
}
```

---

## 第三部分 · 结构体 struct（W4 预览，翁恺第11章）

### T1 定义并初始化
```c
typedef struct{ int id; char name[16]; float score; } Stu;
Stu s = {1, "Tom", 88.5f};
```

### T2 结构体数组
```c
Stu cls[3] = {{1,"A",90},{2,"B",85},{3,"C",92}};
printf("%.1f\n", cls[1].score); // 85.0
```

### T3 结构体指针 + `->`
```c
Stu *p = &s;
printf("%s %.1f\n", p->name, p->score); // Tom 88.5
```

### T4 传指针修改成员
```c
void set_score(Stu *p, float v){ p->score = v; }
```

### T5 位域映射寄存器（衔接嵌入式 C）
```c
typedef struct{ unsigned EN:1; unsigned MODE:2; unsigned RES:5; } CR_t;
volatile CR_t *CR = (CR_t*)0x40021000; // 假设某寄存器地址
CR->EN = 1;   // 置位
```
注意：位域的位序是**编译器相关**的，跨平台/严格场合更推荐"位运算 + 掩码"（见之前五张图里的位运算）。

---

## 自测验收表

| 模块 | 你能独立写出并跑通 | 打勾 |
|---|---|---|
| 指针改值 / 传参(swap) | P1 | ☐ |
| 指针遍历数组 / 字符串 | P2 P3 | ☐ |
| 指针算术 / 数组名 vs 指针 | P4 P9 | ☐ |
| 函数指针回调 | P5 | ☐ |
| const 指针区分 | P6 | ☐ |
| 二级指针 | P7 | ☐ |
| 野指针防范 | P10 | ☐ |
| 二维数组传参 / 按行寻址 | P11~P13 | ☐ |
| 字符串四件套 strlen/strcpy/strcmp/strcat | S1~S3 | ☐ |
| sprintf 组包 | S4 | ☐ |
| 结构体定义/数组/指针/传参 | T1~T4 | ☐ |
| 位域映射寄存器 | T5 | ☐ |

> 全打勾 = 通用 C 复习完成，可以进嵌入式 C 专项（volatile / 位运算 / 寄存器映射 / 内存分区 / 函数指针）。

---

## 编译运行（PC 上验证）
```bash
gcc p1.c -o p1 && ./p1
```
建议每题单独存一个 `.c` 文件，调通就 `git add` 传到 GitHub——这就是你大二实习简历的作品集。
