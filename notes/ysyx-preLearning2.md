# ysyx-preLearning2

## 1. 复习C语言

在 pa0 的末尾，除了提醒我们牢记：

- STFW
- RTFM
- RTFSC

还提醒我们要复习C语言，这在后面会被用到。一生一芯要求掌握的C语言程度：递归、指针、链表。

推荐教程：

- [Linux C编程一站式学习](https://docs.huihoo.com/c/linux-c-programming/)
- [The C programming language (2nd Edition)](http://math.ecnu.edu.cn/~jypan/Teaching/ParaComp/books/The C Programming Language 2nd.pdf)
- [SEI CERT C Coding Standard](https://wiki.sei.cmu.edu/confluence/display/c/SEI+CERT+C+Coding+Standard)
- 本阶段相关资源:
  - 录播 
    - [C 语言拾遗 (1): 机制 (第一次录播, 录音效果较差)](https://www.bilibili.com/video/BV1qa4y1j7xk?p=1)
    - [C 语言拾遗 (2): 编程实践](https://www.bilibili.com/video/BV1qa4y1j7xk?p=2)
  - 课件 
    - [C 语言拾遗 (1): 机制](http://jyywiki.cn/ICS/2020/slides/2.slides)
    - [C 语言拾遗 (2): 编程实践](http://jyywiki.cn/ICS/2020/slides/3.slides)

此外，verilator 会编译出C++ 文件，也需要了解一点点C++ 的基本知识。这个我应该没啥问题。后续用到再说。

不知道为什么不能访问以上链接，下载了一个谷歌也不行。[如何在 Ubuntu 20.04 上安装 Google Chrome 网络浏览器](https://zhuanlan.zhihu.com/p/137114100)

不过C语言资源很多，不至于应付不过来。

## 2. 搭建 Verilator 仿真环境

### 2.1 安装

```bash 
# Prerequisites:
#sudo apt-get install git perl python3 make autoconf g++ flex bison ccache
#sudo apt-get install libgoogle-perftools-dev numactl perl-doc
#sudo apt-get install libfl2  # Ubuntu only (ignore if gives error)
#sudo apt-get install libfl-dev  # Ubuntu only (ignore if gives error)
#sudo apt-get install zlibc zlib1g zlib1g-dev  # Ubuntu only (ignore if gives error)

git clone https://github.com/verilator/verilator   # Only first time

# Every time you need to build:
unsetenv VERILATOR_ROOT  # For csh; ignore error if on bash
unset VERILATOR_ROOT  # For bash
cd verilator
git pull         # Make sure git repository is up-to-date
git tag          # See what versions exist
#git checkout master      # Use development branch (e.g. recent bug fixes)
#git checkout stable      # Use most recent stable release
#git checkout v{version}  # Switch to specified release version

autoconf         # Create ./configure script
./configure      # Configure and create Makefile
make -j `nproc`  # Build Verilator itself (if error, try just 'make')
sudo make install
```

查看版本：

```bash
verilator --version
# 输出 Verilator 4.210 2021-07-07 rev v4.210
```

安装成功。

![image-20220630200849323](/home/zzrs123/.config/Typora/typora-user-images/image-20220630200849323.png)

### 2.2 C++例程



[例程地址](https://verilator.org/guide/latest/example_cc.html#example-c-execution)

> 注我们是从源码下载编译得到的，所以例程需要在verilator文件夹下编译。 

参考教程：

1. [Unbuntu21.04安装Verilator --4.210](https://blog.csdn.net/qq_43209432/article/details/123049094)
2. 官网：差别是没有切换分支，切换分支才能下载4.210https://verilator.org/guide/latest/install.html



## 3. Verilator 的使用

上面的示例非常简单, 甚至算不上是一个真正的电路模块. 接下来我们编写一个真正的电路模块, 双控开关, 来进行测试. 编写如下的verilog代码:

```verilog
module top(
  input a,
  input b,
  output f
);
  assign f = a ^ b;
endmodule
```
