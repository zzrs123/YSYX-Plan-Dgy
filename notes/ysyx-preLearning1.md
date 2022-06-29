# ysyx-preLearning1

## 1. Ubuntu20.04.4 安装

## 2. Exploration on Ubuntu20.04

之前为了机器人比赛，装过ubuntu1804，但也是之前的事情了。但这次装ubuntu换源的时候居然脑子抽风去粘贴了18的源过来，直接导致崩盘。

## 3. Installing Tools

注意先换源。

就是几个命令行：

```shell
sudo apt-get install build-essential    # build-essential packages, include binary utilities, gcc, make, and so on
sudo apt-get install man                # on-line reference manual
sudo apt-get install gcc-doc            # on-line reference manual for gcc
sudo apt-get install gdb                # GNU debugger
sudo apt-get install git                # revision control system
sudo apt-get install libreadline-dev    # a library used later
sudo apt-get install libsdl2-dev        # a library used later
sudo apt-get install llvm llvm-dev      # llvm project, which contains libraries used later
```



## 4. Vim

安装：

```shell
sudo apt-get install vim
```

获取vim教程有两种方式：

1. 终端输入 `vimtutor`即可看到教程，但是因为系统环境选的是英文，感觉有点费力。
2. 在网上搜 vim 教程。

用 Vim 创建一个文件

```shell
vim test
```

> 程序设计课上你学会了使用Visual Studio, 然后你可能会认为, 程序员就是这样写代码的了. 其实并不是,  程序员会追求那些提高效率的方法. 不是GUI不好, 而是你只是用记事本的操作方式来写代码. 所以你需要改变,  去尝试一些可以帮助你提高开发效率的工具.

> 在GNU/Linux中, 与记事本的操作方式相比, 学会`vim`的基本操作就已经可以大大提高开发效率. 还有各种插件来增强`vim`的功能, 比如可以在代码中变量跳转的`ctags`等等. 你可以花点时间去配置一下`vim`, 具体配置方式请STFW. 总之, "编辑器之神"可不是浪得虚名的.

话是这么说，我感觉挺麻烦的。

下面我要写一个Hello World程序，并用makefile编译它。

主要参考为：https://blog.csdn.net/hjxu2016/article/details/100672126

期间遇到了问题为：

```shell
zzrs123@zzrs123:~/testrc/test1$ make
makefile:8: *** missing separator.  Stop.
```

参考教程为https://blog.csdn.net/limanjihe/article/details/52231243

**问题就是命令之前需要TAB，而不是空格。**

 创建文件夹 

```shell

mkdir testrc
cd testrc
mkdir test1
cd test1
vim hello.c

```

编写 helloworld 程序

```c
#include<stdio.h>
int main(){
    printf("Hello World");
    return 0;
}
```

编写 makefile

```shell
vim makefile
```

```makefile
#语法规则
# target ...:pre-requisities...
# 目标文件...:先决条件
# targe 这一个或多个的目标文件依赖于 pre-requisities中的文件
# command, 下面是命令

hello:hello.c
        gcc hello.c
.PHONY:clean
        -rm hello
# .PHONY 意为 clean 是一个伪目标
# 而在 rm 前加入 - 意为，也许某些文件出现问题，但不要管，继续做后面的事情

# 也可以
#clean:
#  -rm hello
# 但不推荐这么做，如果在当前目录下存在一个clean，就不需要执行clean后面的规则了
```

<esc> + :wq! + Enter退出后，操作如下：

```shell
zzrs123@zzrs123:~/testrc/test1$ make
gcc hello.c
zzrs123@zzrs123:~/testrc/test1$ ls
a.out  hello.c  makefile
zzrs123@zzrs123:~/testrc/test1$ ./a.out
hello world!
```

## 5. gdb调试

代码：

```C
#include<stdio.h>
void sum(int m);

int main(){
        int i,n=0;
        sum(50);
        for(i = 1; i <= 50; i++){
                n +=i;
        }
        printf("The sum of 1 - 50 is %d \n",n);
        return 0;
}

void sum(int m){
        int i,n=0;
        for(i=1;i<=m;i++) n+=i;
        printf("The sum of 1 - m is %d\n",n);
}

```

参考教程：

https://blog.51cto.com/u_15262460/2882577

只能说超级详细。

mark一下， 0628看到了gdb调试完成，后面是make工程相关的内容。

回头总结一下这部分的内容，整个博文。

## 6. tmux 终端管理器

这个用来分屏十分方便。参考教程：

https://zhuanlan.zhihu.com/p/98384704

基本就可以通过一个终端来切换不同的端口，只需要通过命名来管理一下即可。

## 7.