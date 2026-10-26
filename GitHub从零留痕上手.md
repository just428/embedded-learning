# GitHub 从零到每天留痕 · 逐步操作手册

> 配《嵌入式实习学习路线图 v2》使用 · 2026-10-25  
> 目标：**今天把点灯工程推上 GitHub**，之后每天顺手 push，攒出一份能挂简历的作品

## 你现在的状态（已在本机检测）

| 项目        | 状态                                    |
| --------- | ------------------------------------- |
| Git 是否安装  | ✅ 已装 **Git 2.55.0**                   |
| 用户名/邮箱    | ✅ 已配置 `just428` / `2422509353@qq.com` |
| GitHub 账号 | ✅ 已注册，用户名 `just428`                   |
| SSH 密钥    | ✅ 已生成并粘到 GitHub，改走 443 端口（校园网封 22）    |
| 远程仓库      | ✅ 已建 `just428/embedded-learning`      |
| 本地仓库      | ✅ `/d/Code/embedded-learning`（已首推）    |
| Git 配置    | ✅ 三条宽松配置已补（中文文件名 / 换行符 / 默认分支）        |

所以剩下要做的只有：**clone 到本地 → 把点灯工程放进去 → push**。前面的密钥、账号、远程仓库都已经通了。

---

## 步骤 0：打开命令行

本机装了 Git，**在任意文件夹空白处点右键 → 选「Open Git Bash Here」**。  
出来一个黑框，光标前面是 `$`，下面所有命令都在这里敲。

> 补充：如果没有这个右键菜单，按 `Win` 键搜 **Git Bash** 也能打开。  
> 本文所有代码块里 `#` 开头的是注释，不用敲。

先验证一下（复制粘贴后回车）：

```bash
git --version
# 预期输出：git version 2.55.0.windows.3
```

---

## 步骤 1：补两条配置（一次性的）

```bash
git config --global core.quotepath false
git config --global core.autocrlf false
git config --global init.defaultBranch main
```

解释：

- `core.quotepath false`：不然中文文件名会显示成 `\344\275\240` 这种乱码
- `core.autocrlf false`：不让 Git 偷偷改换行符，避免源文件被莫名其妙改动
- `init.defaultBranch main`：和 GitHub 默认分支名一致，避免第一次 push 报 `master`/`main` 不匹配

检查是否生效：

```bash
git config --global --list
# 预期看到类似：
# safe.directory=*
# user.name=just428
# user.email=2422509353@qq.com
# core.quotepath=false
# core.autocrlf=false
# init.defaultbranch=main
```

> 如果 `user.name` / `user.email` 你想改成别的（比如姓名拼音），用：  
> `git config --global user.name "YourName"` 和 `git config --global user.email "you@example.com"`

---

## 步骤 2：注册 GitHub 账号

1. 浏览器打开 **<https://github.com>** → 右上角 **Sign up**
2. 填邮箱（建议就用你 git 里配的那个 QQ 邮箱，保持一致）→ 密码 → 用户名
3. **用户名要认真取** —— 这会印在你简历上，也会被面试官搜到。建议：
   - 好选项：姓名拼音全拼（`linzhikang`）、`名字缩写+embedded`
   - 避免：`just428` 这种看不出人的中二 ID（如果这是你本名的一部分另说）
   - 用户名以后可以改，但旧链接会失效，最好一次到位
4. 完成人机验证 → 去邮箱点验证链接（**不验证的话后面 push 会失败**）
5. 后面调查问卷一路跳过，选 Free 免费计划

> 顺手做一件事：右上角头像 → **Settings → Billing → 申请 GitHub Student Developer Pack**（免费，用学校邮箱/学生证申请，能给一些免费服务器和工具额度，用不用得上不强求）

---

## 步骤 3：生成 SSH 密钥（免密码 push）

在 Git Bash 里执行，**连按三次回车**（不要设密码，直接回车）：

```bash
ssh-keygen -t ed25519 -C "2422509353@qq.com"
# 依次出现三行提示：
# Enter file in which to save the key:   → 直接回车
# Enter passphrase:                      → 直接回车
# Enter same passphrase again:           → 直接回车
```

看到类似这样的方块图案就成功了：

```
+--[ED25519 256]--+
|      .o+=*.     |
+----[SHA256]-----+
```

把公钥内容复制出来：

```bash
cat ~/.ssh/id_ed25519.pub
# 输出一整行，形如：
# ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAI... 2422509353@qq.com
```

**完整复制这一整行**（从 `ssh-ed25519` 一直到邮箱结尾）。

粘到 GitHub：

1. 右上角头像 → **Settings**
2. 左侧 **SSH and GPG keys**
3. 右上角绿色 **New SSH key**
4. Title 随便填：`My Laptop`
5. Key type 保持 `Authentication Key`
6. Key 框里粘贴刚才那一整行
7. **Add SSH key**（会让你输一次 GitHub 密码确认）

回到 Git Bash 测试：

```bash
ssh -T git@github.com
# 第一次会问：Are you sure you want to continue connecting (yes/no/[fingerprint])?
# 输入 yes 回车
# 成功会看到：Hi 你的用户名! You've successfully authenticated, but GitHub does not provide shell access.
```

> ❌ 如果卡住或报 `Permission denied (publickey)`：说明公钥没粘全或邮箱没验证，重做步骤 3 的复制粘贴环节，重点看是不是多了空格或少了结尾。

### 3.1 如果报 `connect to host github.com port 22: Connection refused`

**这不是你的错，也不是 key 的问题**——是网络把 22 端口封了（校园网、部分运营商很常见）。修法是让 SSH 改走 443 端口（和网页同一个端口，基本封不了）。

新建/编辑 `C:\Users\zklin\.ssh\config`（这个文件没有扩展名），内容四行：

```
Host github.com
  HostName ssh.github.com
  Port 443
  User git
```

保存后**再测一次** `ssh -T git@github.com`，看到 `Hi 你的用户名!` 就通了。

> 配好之后 `git clone` / `git push` 的命令**一个字都不用改**，还是写 `git@github.com:...`，SSH 会自动走 443。  
> 这台机器已经配好了（2026-10-25 验证通过，GitHub 用户名为 `just428`）。

---

## 步骤 4：在 GitHub 上建仓库

1. 打开 <https://github.com> → 右上角 **+** → **New repository**
2. **Repository name**：`embedded-learning`
3. **Description**：`汕头大学 · 嵌入式学习记录 STM32 / RTOS / BLE`
4. 选 **Public**（重要：公开才能给 HR 看绿墙）
5. **勾选 Add a README file**
6. Add .gitignore 选 `None`（我们自己写）
7. License 选 None
8. 点绿色 **Create repository**

建好后页面会显示仓库地址，形如 `https://github.com/你的用户名/embedded-learning`。

---

## 步骤 5：把仓库拉到本机

**先决定放在哪**。建议建一个专门的目录，路径里**不要有空格**：

```bash
mkdir -p /d/Code
cd /d/Code
```

> 如果你想放别的地方，比如 `D:\学习\嵌入式`，在 Git Bash 里就是 `cd "/d/学习/嵌入式"`（中文路径要加引号）。  
> 小知识：Git Bash 里 D 盘写作 `/d/`，C 盘写作 `/c/`，和 Windows 的 `D:\` 是一回事。

克隆（把下面的 `你的用户名` 换成你真实的 GitHub 用户名）：

```bash
git clone git@github.com:你的用户名/embedded-learning.git
# 预期输出：
# Cloning into 'embedded-learning'...
# remote: Enumerating objects: 4, done.
# remote: Total 4 (delta 0), reused 0 (delta 0), 100% (4/4), done.
```

进去看看：

```bash
cd embedded-learning
ls -la
# 会看到 README.md 和一个 .git 隐藏文件夹
pwd
# 显示当前路径
```

---

## 步骤 6：整理点灯工程，放进仓库

### 6.1 先找到你的点灯工程

一般在 `Documents`、`Desktop` 或某个 STM32 教程文件夹里，找后缀是 `.uvprojx`（Keil5）或 `.ioc`（CubeMX）的文件。

### 6.2 建实验目录

> ⚠️ **先确认你在仓库里**：`pwd` 应该显示 `.../embedded-learning`。如果显示在 `/d/Code` 这种仓库外面，先 `cd embedded-learning`。否则 `01-gpio-blink` 会和仓库平级，push 上去仓库是空的（这坑已经踩过一次了）。

```bash
cd embedded-learning     # 如果还没进仓库
mkdir -p 01-gpio-blink
```

用 Windows 资源管理器把整个点灯工程文件夹**复制**到 `01-gpio-blink/` 里面。

### 6.3 必须删掉的垃圾文件

Keil 编译会生成一堆中间产物，这些**绝对不能传**（会把仓库撑爆，而且毫无意义）：

| 删掉                                          | 原因     |
| ------------------------------------------- | ------ |
| `Objects/`、`Listings/`、`Debug/`、`Release/`  | 编译中间产物 |
| `*.o`、`*.d`、`*.axf`、`*.map`、`*.htm`、`*.lnp` | 编译输出   |
| `*.bak`、`*.uvoptx.bak`                      | 备份文件   |
| `JLinkLog.txt`、`*.log`                      | 日志     |

**必须保留**：`.c` `.h` `.uvprojx`（工程本身）、`.ioc`（CubeMX 配置）、`.s`（启动文件）、`.md`、图片。

### 6.4 写 .gitignore 一劳永逸

在仓库**根目录**（不是 `01-gpio-blink` 里）执行：

```bash
touch .gitignore
notepad .gitignore
```

记事本打开后粘贴下面内容，**保存并关闭**：

```
# Keil 编译产物
Objects/
Listings/
Debug/
Release/
*.o
*.d
*.axf
*.map
*.htm
*.lnp
*.bak
*.uvoptx.bak

# 编辑器 / 系统
.vscode/
.idea/
.DS_Store
Thumbs.db

# 日志
*.log
JLinkLog.txt
```

> 如果记事本保存时自动加了 `.txt` 后缀，在资源管理器里开启"文件扩展名"显示，手动把 `.gitignore.txt` 改名为 `.gitignore`（去掉 `.txt`）。

### 6.5 给实验写一个 README

```bash
touch 01-gpio-blink/README.md
notepad 01-gpio-blink/README.md
```

粘贴（按你的实际情况改）：

````markdown
# 01 GPIO 点灯

## 做了什么
配置 PC13 为推挽输出，500ms 翻转一次，板载 LED 闪烁。

## 关键代码
```c
HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
HAL_Delay(500);
````

## 学到的

- GPIO 输出模式：推挽 vs 开漏
- HAL_Delay 依赖 SysTick

## 踩坑

- 时钟没配置对，LED 不亮（下次先查 SystemClock_Config）

````

> 注意：Markdown 里代码块要用三个反引号 ``` 包起来，写嵌套时容易乱，直接在 GitHub 网页上编辑更省事（后面步骤 9 会讲）。

---

## 步骤 7：第一次提交（四步）

**第 1 步，看看 Git 认为你改了什么：**

```bash
git status
# 预期：红色字列出 01-gpio-blink/ 和 .gitignore（Untracked files）
````

**第 2 步，把改动加进暂存区：**

```bash
git add .
# 再 git status 一次，会看到文件变绿色了 = 已暂存
git status
```

**第 3 步，打一个提交记录：**

```bash
git commit -m "feat: STM32 GPIO 点灯实验，PC13 推挽输出"
# 预期输出：
# [main 1a2b3c4] feat: STM32 GPIO 点灯实验，PC13 推挽输出
#  3 files changed, 45 insertions(+)
```

commit message 格式记住三种就够：`feat:` 新内容 · `fix:` 修 bug · `docs:` 改文档。

**第 4 步，推到 GitHub：**

```bash
git push
# 第一次可能提示输入 yes 确认主机指纹，敲 yes
# 预期：
# Enumerating objects: 5, done.
# To github.com:你的用户名/embedded-learning.git
#  * [new branch]      main -> main
```

**刷新 GitHub 仓库页面** —— 你的点灯代码在线上了。这就是第一次留痕。

---

## 步骤 8：验证结果

页面上应该能看到：

1. 文件列表里有 `01-gpio-blink/`、`README.md`、`.gitignore`
2. 右上角显示 `1 commit`
3. README.md 的内容自动渲染在页面下方（Markdown 会自动排版）
4. 你个人主页 <https://github.com/你的用户名> 上，出现了绿色格子

---

## 步骤 9：以后怎么更新（两种方式任选）

### 方式 A：命令行（推荐，以后上班就用这个）

每次写完代码，四连：

```bash
cd /d/Code/embedded-learning
git pull              # 1. 先拉最新（换设备时必做）
# ...改代码...
git add .             # 2. 暂存
git commit -m "feat: xxx"   # 3. 提交
git push              # 4. 推送
```

### 方式 B：VS Code 图形界面（零基础更友好）

1. 装 VS Code（免费）→ 打开它，用菜单 **文件 → 打开文件夹** 打开 `embedded-learning`
2. 左侧第三个图标（源代码管理，像分叉的线）
3. 改完文件后，在消息框里写一句 commit message
4. 点 **√ 提交** → 点 **同步更改**
5. 效果完全一样

> 建议：**写 README 和笔记用 VS Code**，它支持 Markdown 实时预览，比记事本舒服太多。装 `Markdown Preview Enhanced` 插件即可。  
> 也可以直接在 GitHub 网页上点文件 → 铅笔图标在线编辑，适合临时改 README。

---

## 步骤 10：Markdown 速成（写 README 只需要这 6 个）

| 效果   | 写法                                |    |    |      |     |     |          |
| ---- | --------------------------------- | -- | -- | ---- | --- | --- | -------- |
| 一级标题 | `# 标题`                            |    |    |      |     |     |          |
| 二级标题 | `## 小标题`                          |    |    |      |     |     |          |
| 加粗   | `**重点**`                          |    |    |      |     |     |          |
| 代码   | 单个反引号 `` `HAL_Delay()` ``         |    |    |      |     |     |          |
| 代码块  | 三个反引号开头 + 语言名（如 \`\`\`c），再三个反引号结尾 |    |    |      |     |     |          |
| 表格   | \`                                | 列1 | 列2 | `换行` | --- | --- | \` 换行数据行 |
| 图片   | `![说明](img/wiring.jpg)`           |    |    |      |     |     |          |
| 列表   | `- 事项`                            |    |    |      |     |     |          |
| 打勾列表 | `- [x] 已完成` / `- [ ] 未完成`         |    |    |      |     |     |          |

---

## 步骤 11：把仓库养成"简历级"的目录

```
embedded-learning/
├── README.md              # 总纲 + 进度打卡表
├── notes/                 # 每周笔记
│   └── 2026-10-26.md
├── c-practice/            # C 手写题 12 题（每题一个 .c）
│   ├── 01-string/
│   ├── 04-bit-macro/
│   ├── 05-ringbuffer/
│   └── README.md
├── 01-gpio-blink/
│   ├── README.md
│   ├── src/ inc/
│   └── img/wiring.jpg
├── 02-exti-key/
├── 03-timer-pwm/
├── 04-uart/
├── 05-i2c-oled/
├── 06-spi/
└── 07-adc-dma/
```

命名规则：**两位数编号 + 英文短横线**，整齐且排序正确。每个实验目录都要有 README。

两个完整项目以后单独开仓库：`stm32-rtos-datalogger`、`esp32-ble-node`。

---

## 步骤 12：把总 README 改成进度打卡表

在 GitHub 仓库页面点 `README.md` → 右上铅笔图标 → 替换成下面的模板 → 底部 **Commit changes**：

```markdown
# embedded-learning

汕头大学 · 通信工程 | 嵌入式学习记录
目标：2027 暑假嵌入式实习（STM32 / RTOS / BLE）

## 进度打卡

| 模块 | 内容 | 状态 |
|------|------|------|
| GPIO | 点灯 / 按键 / 外部中断 | 完成 |
| 定时器 | 定时中断 / PWM 呼吸灯 | 进行中 |
| UART | printf 重定向 / 环形缓冲接收 | 待开始 |
| I2C | OLED 显示 | 待开始 |
| SPI | Flash / TFT | 待开始 |
| ADC+DMA | 电压采集 | 待开始 |
| FreeRTOS | 任务 / 队列 / 信号量 | 待开始 |
| ESP32 BLE | GAP / GATT / 自定义服务 | 待开始 |

## C 练手写题

- [x] 字符串函数 strlen/strcpy/strcmp/strcat
- [x] 位操作宏 SET_BIT / CLR_BIT
- [ ] memcpy / memmove
- [ ] 环形缓冲区
- [ ] 单链表反转
- [ ] 联合体大小端转换

## 目录说明

- `notes/` 每周学习笔记，记录卡住的地方和解法
- `c-practice/` C 语言手写题，每题一个独立 .c + 预期输出
- `01-xx/` 外设实验，每个目录含 README + 接线图

## 硬件

STM32 开发板 + ST-Link + SSD1306 OLED + AHT20 温湿度
```


## 步骤 14：常见报错速查表

| 报错                                                                     | 原因                                       | 解决                                                               |
| ---------------------------------------------------------------------- | ---------------------------------------- | ---------------------------------------------------------------- |
| `Permission denied (publickey)`                                        | SSH key 没配好或没验证邮箱                        | 重做步骤 3，确认邮箱已验证                                                   |
| `ssh: connect to host github.com port 22: Connection refused`          | 网络屏蔽了 22 端口（校园网/运营商常见），**不是 key 的问题**    | 改走 443 端口，见下方「22 端口被墙的修法」                                        |
| `Support for password authentication was removed`                      | 用了 HTTPS 地址还输密码                          | 换成 SSH 地址：`git remote set-url origin git@github.com:用户名/仓库名.git` |
| `src refspec main does not match any`                                  | 还没 commit 就 push                         | 先 `git add .` + `git commit`                                     |
| `failed to push some refs`                                             | 远程有本地没有的提交                               | 先 `git pull --rebase` 再 `git push`                               |
| `LF will be replaced by CRLF` 警告                                       | 换行符提示，无害                                 | 已配置 `core.autocrlf false`，忽略即可                                   |
| 中文文件名显示 `\344\275`                                                     | 没配 quotepath                             | `git config --global core.quotepath false`                       |
| 提示文件超过 100MB                                                           | 塞了编译产物或视频                                | 先把它加进 `.gitignore`；视频传 B 站/网盘，README 放链接                         |
| `git` 不是内部或外部命令                                                        | 在 cmd 里敲了 Git 命令                         | 用 Git Bash，不是 cmd/PowerShell                                     |
| `fatal: not a git repository (or any of the parent directories): .git` | 不在仓库目录里（提示符没有 `(main)`）                  | `cd` 进 `embedded-learning`；git 只往上找 `.git`，不会往下找                 |
| `bash: cd: ... No such file or directory`（路径明明存在）                      | **Git Bash 区分大小写**，`/d/code` ≠ `/d/Code` | 别手打，敲前几个字母按 **Tab** 补全                                           |
| `bash: git: command not found`                                         | 粘贴时命令被截断，或不在 Git Bash 里                  | 用**右键**粘贴（`Ctrl+V` 在 Git Bash 无效），再 `git --version` 确认           |

**救命命令**（改崩了想回滚）：

```bash
git log --oneline          # 看历史，每行前面那串是版本号
git reset --hard 版本号     # 回到那个版本（会丢弃之后的改动，慎用）
git checkout -- 文件名      # 只撤销某个文件的改动
```

---

## 步骤 15：进阶（第一周之后再说，不急）

- **个人主页名片**：新建一个和用户名同名的仓库（如 `用户名/用户名`），里面的 README 会显示在你主页顶部，相当于一张自我介绍名片
- **GitHub Pages**：可以把周笔记变成个人博客（搜索 `jekyll` 或直接用 Docsify），现在不用搞，`notes/` 就够了
- **简历怎么挂**：简历的"专业技能"下面写一行 GitHub 链接 + 一句"30+ 篇学习记录，含 X 个完整项目"。**只挂有 README 的仓库**
- **别 clone 别人的工程原样传**：那叫复制不叫作品，所有代码必须自己敲过、调通过

---

## 今天的最小行动清单（打勾）

- [x] Git Bash 里敲通 `git --version`
- [x] 补三条 `git config`（步骤 1）
- [x] 注册 GitHub 账号 + 邮箱验证
- [x] 生成 SSH key 并粘到 GitHub，`ssh -T` 通了（22 被封，已改走 443）
- [x] 建 `embedded-learning` 仓库（Public + 勾选 README）
- [x] `git clone` 到本地
- [x] 点灯工程拷进去，建 `.gitignore` 和实验 README
- [x] `git add .` → `git commit` → `git push`
- [x] 刷新页面看到代码，第一次留痕达成

**2026-10-25 全部完成** —— 仓库 <https://github.com/just428/embedded-learning> ，首推 210 个文件（commit `fc0dde0`），总 README 打卡表随后补上（`4342d8b`）。

做完这九项，你就有第一条 GitHub 记录了。**之后每天只要 4 条命令、30 秒的事。**
