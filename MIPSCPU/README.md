# do_mips_cpu

本文件夹主要是2022年我的计组实验仓库，用于记录我的计组实验成果。

# **实验一 / 单周期**

 

| **实验名称：** | **单周期CPU** | **日期：** | 2022年5月27日 |
| -------------- | ------------- | ---------- | ------------- |

| **班级：** | **HC002004** | **学号：** | **2020300071** | 姓名： | 杜高源 |
| ---------- | ------------ | ---------- | -------------- | ------ | ------ |

## 一、实验要求

1. 通过对ADDU单周期CPU的设计，完成基本模块：alu、pc、gpr、iml等模块的设计与仿真。
2. 利用1中实现的各个模块，设计一个能实现下列R型指令的单周期CPU: addu，subu，add，and，or，slt。

2. 在单周期CPU（R型指令）设计的基础上增加下列I型指令: addi，addiu，andi，ori，lui。

3. 在单周期CPU（I型指令）设计的基础上增加下列存储器操作指令: lw、sw。

4. 在单周期CPU（MEM型指令）设计的基础上增加下列跳转指令: beq、j、jal、jr。

## 二、实验过程：

### (一) ADDU单周期CPU模块设计

学期初参加了试点班的考核，对于ALU和有限状态机的研究有了一点了解。

在第一周，先实现了各个基本的模块设计，进行了addu、subu、add、and、or、slt指令的设计，并编写了相关的testbench进行初步的仿真和功能验证。

#### 1. PC模块

##### 1.1 模块图

![image-20220604213137499](C:\Users\shandaiwang\AppData\Roaming\Typora\typora-user-images\image-20220604213137499.png)

##### 1.2 模块信号

| 信号名 | I/O  | 描述           |
| ------ | ---- | -------------- |
| clock  | I    | 时钟信号       |
| reset  | I    | 复位信号       |
| npc    | I    | 下一条指令地址 |
| pc     | O    | 当前的指令地址 |

##### 1.3 模块代码

```verilog
//核心代码为：
always @(posedge clock or negedge reset)
    begin
        if (reset == 0)
            data <= 32'h00003000;//初设地址
        else
            data <= npc;
    end
//再把data交给pc
assign pc = data;
```

##### 1.4 功能介绍

更新当前的指令地址，如果`reset`为低电平，则初始化`pc`，否则用`npc`赋值。

#### 2. im模块

##### 2.1 模块图

![image-20220604213149414](C:\Users\shandaiwang\AppData\Roaming\Typora\typora-user-images\image-20220604213149414.png)

##### 2.2 模块信号

| 信号名      | 方向 | 描述 |
| ----------- | ---- | ---- |
| pc          | I    | 地址 |
| instruction | O    | 指令 |

##### 2.3 模块代码

```verilog
//核心代码：
	reg [31:0] ins_memory[1023:0]; //4k指令存储器
//低12位+两位零
    assign instruction = ins_memory[pc[11:0]>>2];

```

##### 2.5 功能介绍

输入信号pc为32位，但指令存储器ins_memory只有4KB（已给定），所以需要截取pc的低12位来作为ins_memory的地址；

又由于MIPS指令是固定长度的32位，而编址方式是按字节进行的，所以需要乘个4。

#### 3. gpr模块

##### 3.1 模块图

![image-20220604213314653](C:\Users\shandaiwang\AppData\Roaming\Typora\typora-user-images\image-20220604213314653.png)

##### 3.2 模块信号

| **信号名**       | **方向** | **描述**                       |
| ---------------- | -------- | ------------------------------ |
| a[31:0]          | O        | 读出的读寄存器1的值            |
| b[31:0]          | O        | 读出的读寄存器2的值            |
| clock            | I        | 时钟信号，上升沿有效           |
| reg_write        | I        | 写使能信号  1：有效；0：无效。 |
| rs[4:0]          | I        | 读寄存器1的编号                |
| rt[4:0]          | I        | 读寄存器2的编号                |
| data_write[31:0] | I        | 写数据                         |
| num_write[31:0]  | I        | 写寄存器编号                   |

##### 3.3 模块代码

```verilog
 reg [31:0] gp_registers[31:0];  //32个寄存器
    integer i;

    initial
    begin
        //通过for循环实现32位的赋值
        //这是一种简便写法。
        for (i = 0; i < 32; i = i + 1)
            gp_registers[i] = 32'b0;
    end
	//通过assign来赋值
    assign a = gp_registers[rs];
    assign b = gp_registers[rt];

    always @(posedge clock)
    begin
        if (reg_write == 1 && num_write != 0)
            gp_registers[num_write] <= data_write;
    end
```

##### 3.4 功能介绍

这个模块就是寄存器堆，其上有写入端口rs,rt，以及写入控制端口num_write,data_write，在clock的上升沿，如果reg_write有效且num_write也有效，把数据写进来。



#### 4. alu 模块

##### 4.1 模块图

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210611195202366.png)

##### 4.2 模块信号

| 信号名 | 方向 | 描述                 |
| ------ | ---- | -------------------- |
| a      | I    | 操作数1              |
| b      | I    | 操作数2              |
| aluop  | I    | 操作码               |
| c      | O    | 计算结果             |
| zero   | O    | 零信号，用于分支指令 |

##### 4.3 模块代码

```verilog
//实验一中先需要实现加法器
module alu(c,a,b);

    output [31:0] c;
    input [31:0] a;
    input [31:0] b;

    assign c = a + b;

endmodule


//R型指令
//即将加法器进行替换，增加其他运算功能
`include "header.v"
module alu(c,a,b,aluop);

    output reg signed [31:0] c;
    input signed [31:0] a;
    input signed [31:0] b;
    input [2:0] aluop;

    always @(a or b or aluop)
        case (aluop)
            `ADD: c = a + b;
            `SUB: c = a - b;
            `AND: c = a & b;
            `OR: c = a | b;
            `SLT: c = (a < b) ? 32'd1 : 32'd0;
            default: c = a + b;
        endcase
    
endmodule

//i型指令
`include "header.v"
module alu(c,a,b,aluop);

    output reg signed [31:0] c;
    input signed [31:0] a;
    input signed [31:0] b;
    input [2:0] aluop;

    always @(a or b or aluop)
        case (aluop)
            `ADD: c = a + b;
            `SUB: c = a - b;
            `AND: c = a & b;
            `OR: c = a | b;
            `SLT: c = (a < b) ? 32'd1 : 32'd0;
            `LUI: c = b << 16;
            default: c = a + b;
        endcase
    
endmodule

//MEM指令不需要改动alu

//j型指令
`include "header.v"
module alu(c,a,b,aluop,zero);

    output reg signed [31:0] c;
    input signed [31:0] a;
    input signed [31:0] b;
    input [2:0] aluop;
    output reg zero;


    always @(*)
    begin
        zero = 0;
        case (aluop)
            `ADD: c <= a + b;
            `SUB: c <= a - b;
            `AND: c <= a & b;
            `OR: c <= a | b;
            `SLT: c <= $signed(a) < $signed(b) ? 32'd1 : 32'd0;
            `LUI: c <= b << 16;
            `EQB: c <= b;
            default: c <= b;
        endcase
        zero <= (c == 0) ? 1 : 0;
    end
    
endmodule
```

##### 4.4 功能介绍

第一次实验写的就是加法器。后面需要把指令写进来。不同的计算指令分别给不同的表达式就可以。

#### 5. ADDU单周期CPU的顶层模块

##### 5.1  模块图

![image-20220605100229400](C:\Users\shandaiwang\AppData\Roaming\Typora\typora-user-images\image-20220605100229400.png)

##### 5.2 模块代码

```verilog
  //输入
    input clock;
    input reset;

    wire [31:0] npc;
    wire [31:0] pc;
    wire [31:0] instruction;
    wire [4:0] rs; //读寄存器1
    wire [4:0] rt; //读寄存器2
    wire [4:0] rd;
    wire reg_write;
    wire [31:0] a;
    wire [31:0] b;
    wire [31:0] c;
    pc PC(.pc(pc),.clock(clock),.reset(reset),.npc(npc));
    assign npc = pc + 4;
    im IM(.instruction(instruction),.pc(pc));
    assign rs = instruction [25:21];
	assign rt = instruction [20:16];
	assign rd = instruction [15:11];
    assign reg_write = 1;
    gpr GPR(.a(a),.b(b),.clock(clock),.reg_write(reg_write),.num_write(rd),.rs(rs),.rt(rt),.data_write(c));
    alu ALU(.c(c),.a(a),.b(b));
```

##### 5.3 功能介绍

顶层模块就是连线，参照给出的CPU模块图进行端口之间的连接。然后实例化，实例化个人倾向于上面我的代码里的逐个传参，不容易出错。

### (二) R型指令单周期CPU

#### 0. 相较于ADDU的变化

首先是指令增加，不同的指令（就R型而言），funct字段不同，对于运算alu中的工作也不相同；比如减法和加法的运算必不相同。

> （来自课件）特别注意，ALU的输入同时传递给各个运算功能，各个运算功**能并行工作**。最后通过一个多路选择电路把所需的运算结果选择出来作为ALU的输出。

要增加的地方则是：

- 加法器增加一个类似switch的结构，选择要执行的运算方式。
- 增加ctrl模块，对funct译码得到运算方式的控制信号

#### 1.  ctrl 模块

##### 1.1 模块信号

|  信号名   | 方向 |       描述       |
| :-------: | :--: | :--------------: |
|    op     |  I   |     信号类型     |
|   funct   |  I   |    功能码信号    |
| reg_write |  O   |  寄存器堆写使能  |
|   aluop   |  O   | 用于运算方式选择 |

##### 1.2 模块代码

```verilog
`include "header.v"
module ctrl(reg_write,aluop,op,funct);

    output reg_write;
    output [2:0] aluop;
    input [5:0] op;
    input [5:0] funct;

    assign reg_write = (op == `op_R)? 1 : 0;
    assign aluop = (op == `op_R) ?  (funct == `funct_addu) ? `ADD:
                                    (funct == `funct_subu) ? `SUB:
                                    (funct == `funct_add) ? `ADD:
                                    (funct == `funct_and) ? `AND:
                                    (funct == `funct_or) ? `OR:
                                    (funct == `funct_slt) ? `SLT:
                                                            `ADD:
                                    `ADD;

endmodule
```

##### 1.3  功能介绍

根据输入的指令功能字段funct，判断应该输出哪个aluop来进行相关的运算。

#### 2. R型指令单周期CPU的顶层模块

##### 2.1 模块图

图为数据通路：

![image-20220605102738511](C:\Users\shandaiwang\AppData\Roaming\Typora\typora-user-images\image-20220605102738511.png)

##### 2.2 模块代码

```verilog
	//输入
    input clock;
    input reset;

    wire [31:0] npc;
    wire [31:0] pc;
    wire [31:0] instruction;
    wire [4:0] rs; //读寄存器1
    wire [4:0] rt; //读寄存器2
    wire [4:0] rd;
    wire reg_write;
    wire [31:0] a;  
    wire [31:0] b;
    wire [31:0] c;
    wire [2:0] aluop;
    wire [5:0] op;
    wire [5:0] funct;
    pc PC(.pc(pc),.clock(clock),.reset(reset),.npc(npc));
    assign npc = pc + 4;
    im IM(.instruction(instruction),.pc(pc));
    assign op = instruction [31:26];
    assign rs = instruction [25:21];
	assign rt = instruction [20:16];
	assign rd = instruction [15:11];
    assign funct = instruction [5:0];
    ctrl CTRL(.reg_write(reg_write),.aluop(aluop),.op(op),.funct(funct));
    gpr GPR(.a(a),.b(b),.clock(clock),.reg_write(reg_write),.num_write(rd),.rs(rs),.rt(rt),.data_write(c));
    alu ALU(.c(c),.a(a),.b(b),.aluop(aluop));

```

##### 2.3 功能介绍

连线，将上述各个部分的信号端口连接起来，实例化。仿真功能没有存图，可以看最后j指令的仿真验证。

### (三) i型指令单周期CPU

#### 0. 相较于R型指令的不同

差别在于操作数，R型指令只对寄存器操作数进行操作，而I型引入了立即数。

对于alu模块，需要增加一个lui，同时还需要对imm字段进行扩展，扩展按情况分为符号扩展和直接扩展。

要增加的地方：

- alu模块增加lui指令
- ALU模块还需要引入选择，选择操作数是立即数还是寄存器
- 新写一个扩展模块实现imm的扩展
- gpr模块写寄存器入口处增加选择，选择结果写入rd还是rt。

#### 1. ext模块

##### 1.1 模块图

![img](https://img-blog.csdnimg.cn/20210611195247805.png)

##### 1.2 模块代码

```verilog
module ext (
	input [15:0] immediate, 
	input ExtSel, 
	output [31:0] extended_immediate
    );
	assign extended_immediate = (ExtSel)?{{16{immediate[15]}}, immediate[15:0]}
											:{{16{1'b0}}, immediate[15:0]};
endmodule
```

##### 1.3 功能介绍

将16位数扩展为32位立即数，可以选择0扩展或者符号扩展。

#### 2. I型指令单周期CPU的顶层模块

##### 2.1 模块图

![image-20220605104413110](C:\Users\shandaiwang\AppData\Roaming\Typora\typora-user-images\image-20220605104413110.png)

##### 2.2 模块代码

```verilog
   //输入
    input clock;
    input reset;

    wire [31:0] npc;
    wire [31:0] pc;
    wire [31:0] instruction;
    wire [4:0] rs; //读寄存器1
    wire [4:0] rt; //读寄存器2
    wire [4:0] rd;
    wire reg_write;
    wire [31:0] a;  
    wire [31:0] b;
    wire [31:0] b1;
    wire [31:0] c;
    wire [2:0] aluop;
    wire [5:0] op;
    wire [5:0] funct;
    wire regdst;
	wire extop;
	wire alusrc;
	wire [15:0] imm;
    wire [31:0] eximm;
    wire [4:0] num_write;

    pc PC(.pc(pc),.clock(clock),.reset(reset),.npc(npc));
    assign npc = pc + 4;
    im IM(.instruction(instruction),.pc(pc));
    assign op = instruction [31:26];
    assign rs = instruction [25:21];
	assign rt = instruction [20:16];
	assign rd = instruction [15:11];
    assign funct = instruction [5:0];
    assign imm = instruction [15:0];
	assign num_write = regdst ? rd : rt;
    ctrl CTRL(.reg_write(reg_write),.aluop(aluop),.op(op),.funct(funct),.regdst(regdst),.extop(extop),.alusrc(alusrc));
    ext EXT(.immediate(imm), .ExtSel(extop), .extended_immediate(eximm));
    gpr GPR(.a(a),.b(b),.clock(clock),.reg_write(reg_write),.num_write(num_write),.rs(rs),.rt(rt),.data_write(c));
    assign b1 = alusrc ? eximm : b;
    alu ALU(.c(c),.a(a),.b(b1),.aluop(aluop));
```

##### 2.3 功能验证

时间紧张，i指令没有存图，可以看最后j指令完成后整体的仿真效果。

### (四) mem指令单周期CPU

#### 0. 相较于I/R指令的不同

指令格式和I型一样，所以mem指令中需要alu模块的内容都可以使用I型的逻辑来平替。

需要增加的是两条lw、sw指令，相应的需要增加数据存储器模块dm。

要增加的地方：

- 数据存储器模块dm；
- GPR模块的写数据处增加一个多路选择器，确定写数据是ALU的计算结果还是DM模块的输出数据；
- DM模块的address输入连接ALU模块的输出；
- 把GPR[rt]连接到DM模块的数据输入端口。

#### 1. dm模块

##### 1.1 模块图

![image-20220605095053285](C:\Users\shandaiwang\AppData\Roaming\Typora\typora-user-images\image-20220605095053285.png)

##### 1.2 模块信号

| 信号名    | 方向 | 描述       |
| --------- | ---- | ---------- |
| data_out  | O    | 数据输出   |
| clock     | I    | 时钟信号   |
| mem_write | I    | 写使能信号 |
| address   | I    | 地址       |
| data_in   | I    | 数据输入   |

##### 1.3 模块代码

```verilog
module dm(data_out,clock,mem_write,address,data_in);

    output [31:0] data_out;
    input clock;
    input mem_write;
    input [31:0] address;
    input [31:0] data_in;

    reg [31:0] data_memory[1023:0]; //4K数据存储器

    assign data_out = data_memory[address[11:2]];
    always @(posedge clock)
        begin
            if (mem_write)
                data_memory[address[11:2]] <= data_in;
        end

endmodule
```

##### 1.4 功能介绍

用于读取和写入数据，当写使能有效时，按照地址写入数据，当读有效时，按照地址读取出数据。

#### 2. mem指令单周期CPU的顶层模块

##### 2.1 模块图

![image-20220605111604217](C:\Users\shandaiwang\AppData\Roaming\Typora\typora-user-images\image-20220605111604217.png)

##### 2.2 模块代码

```verilog
  //输入
    input clock;
    input reset;

    wire [31:0] npc;
    wire [31:0] pc;
    wire [31:0] instruction;
    wire [4:0] rs; //读寄存器1
    wire [4:0] rt; //读寄存器2
    wire [4:0] rd;
    wire reg_write;
    wire [31:0] a;  
    wire [31:0] b;
    wire [31:0] b1;
    wire [31:0] c;
    wire [2:0] aluop;
    wire [5:0] op;
    wire [5:0] funct;
    wire regdst;
	wire extop;
	wire alusrc;
	wire [15:0] imm;
    wire [31:0] eximm;
    wire [4:0] num_write;
    wire memwrite;
    wire memread;
    //wire [31:0] data_in;
	wire [31:0] data_out;
    wire [31:0] bus_out;

    pc PC(.pc(pc),.clock(clock),.reset(reset),.npc(npc));
    assign npc = pc + 4;
    im IM(.instruction(instruction),.pc(pc));
    assign op = instruction [31:26];
    assign rs = instruction [25:21];
	assign rt = instruction [20:16];
	assign rd = instruction [15:11];
    assign funct = instruction [5:0];
    assign imm = instruction [15:0];
	assign num_write = regdst ? rd : rt;
    ctrl CTRL(.reg_write(reg_write),.aluop(aluop),.op(op),.funct(funct),.regdst(regdst),.extop(extop),.alusrc(alusrc),.memwrite(memwrite),.memread(memread));
    ext EXT(.immediate(imm), .ExtSel(extop), .extended_immediate(eximm));
    gpr GPR(.a(a),.b(b),.clock(clock),.reg_write(reg_write),.num_write(num_write),.rs(rs),.rt(rt),.data_write(bus_out));
    assign b1 = alusrc ? eximm : b;
    alu ALU(.c(c),.a(a),.b(b1),.aluop(aluop));
    dm DM(.data_out(data_out),.clock(clock),.mem_write(memwrite),.address(c),.data_in(b));
    assign bus_out = memread ? data_out : c;
```

### (五) j型指令单周期CPU

#### 0. 相较于之前的不同

j型指令有：beq，j，jal，jr。

j指令与i、R、mem都不太相似，是一种新的指令格式。由于引进了跳转功能，下一条指令地址也不能按照此前pc中设计的pc+4，而是可以根据需求跳转到指定位置。

此外，jal指令的返回地址需要写入GPR[31]。u

要增加的地方：

- 增加一个npc模块，共4个输入源
  - pc+4
  - gpr模块得到的
  - 16位立即数
  - 26位的立即数
- beq指令要求alu增加zero标志
- 写入数据选择器增加pc+4

#### 1. npc模块

##### 1.1 模块代码

```verilog
    output reg [31:0] npc;
    input [31:0] npc_t;  //npc_t = pc+4
    input [25:0] instr_index;
    input [31:0] offset;  //指令低16位符号扩展
    input [31:0] a;  //alu模块a输出
    input zero;   //alu模块zero输出
    input [1:0] s;  //ctrl模块产生，确定当前指令类型

    always @(*)
		begin
		npc = npc_t;
			case(s)
				`NONE: npc = npc_t;
				`BEQ:
					begin
						if(zero)
							npc = npc_t + {offset[29:0], 2'b00};
						else
							npc = npc_t;
					end
				`J_JAL: npc = {npc_t[31:28], instr_index, 2'b00};
				`JR: npc = a;
				
				default:	npc = 32'hxxxxxxxx;
			endcase
		end
```

##### 1.2 功能介绍

根据ctrl的信号，确定指令类型，计算下一条指令的地址。

#### 2. j型指令单周期CPU的顶层模块

##### 2.1 模块图

![image-20220605112427177](C:\Users\shandaiwang\AppData\Roaming\Typora\typora-user-images\image-20220605112427177.png)

##### 2.2 模块代码

```verilog
   //输入
    input clock;
    input reset;

    wire [31:0] npc;
    wire [31:0] pc;
    wire [31:0] instruction;
    wire [4:0] rs; //读寄存器1
    wire [4:0] rt; //读寄存器2
    wire [4:0] rd;
    wire reg_write;
    wire [31:0] a;  
    wire [31:0] b;
    wire [31:0] b1;
    wire [31:0] c;
    wire [2:0] aluop;
    wire [5:0] op;
    wire [5:0] funct;
    wire regdst;
	wire extop;
	wire alusrc;
	wire [15:0] imm;
    wire [31:0] eximm;
    wire [4:0] num_write;
    wire memwrite;
    wire memread;
    //wire [31:0] data_in;
	wire [31:0] data_out;
    wire [31:0] bus_out;
    wire [31:0] npc_t;
    wire [25:0] instr_index;
    wire zero;
    wire [1:0] s;
    wire r31;

    pc PC(.pc(pc),.clock(clock),.reset(reset),.npc(npc));
    assign npc_t = pc + 4;
    im IM(.instruction(instruction),.pc(pc));
    assign op = instruction [31:26];
    assign rs = instruction [25:21];
	assign rt = instruction [20:16];
	assign rd = instruction [15:11];
    assign funct = instruction [5:0];
    assign imm = instruction [15:0];
    assign instr_index = instruction [25:0];
    ctrl CTRL(.reg_write(reg_write),.aluop(aluop),.op(op),.funct(funct),.regdst(regdst),.extop(extop),.alusrc(alusrc),.memwrite(memwrite),.memread(memread),.s(s),.r31(r31));
    assign num_write = r31 ? 5'd31 : (regdst ? rd : rt);
    ext EXT(.immediate(imm),.ExtSel(extop),.extended_immediate(eximm));
    gpr GPR(.a(a),.b(b),.clock(clock),.reg_write(reg_write),.num_write(num_write),.rs(rs),.rt(rt),.data_write(bus_out));
    //assign b1 = alusrc ? eximm : b;
    assign b1 = r31 ? pc + 4 : (alusrc ? eximm : b);
    alu ALU(.c(c),.a(a),.b(b1),.aluop(aluop),.zero(zero));
    npc NPC(.npc(npc),.npc_t(npc_t),.instr_index(instr_index),.offset(eximm),.a(a),.zero(zero),.s(s));
    dm DM(.data_out(data_out),.clock(clock),.mem_write(memwrite),.address(c),.data_in(b));
    //assign bus_out = r31 ? npc_t : (memread ? data_out : c);
    assign bus_out = memread ? data_out : c;
```

对于以上所有代码的波形仿真在（六）。

### (六) .asm的波形仿真

#### 1. 导入txt文件

把.asm文件放入Mars中，点击上方run中的assemble，得到汇编语言的机器码。

![image-20220604221202158](C:\Users\shandaiwang\AppData\Roaming\Typora\typora-user-images\image-20220604221202158.png)

再点击上方图标自左向右第五个（0101），准备导出十六进制的txt文件。注意弹出框的右侧ascii格式改为hex格式（16进制）。

![image-20220604221345169](C:\Users\shandaiwang\AppData\Roaming\Typora\typora-user-images\image-20220604221345169.png)

点击接着可以看到导出的txt：这就是前四周我们看到的那种机器指令形式。每一行就是一个微指令。

![image-20220604221746870](C:\Users\shandaiwang\AppData\Roaming\Typora\typora-user-images\image-20220604221746870.png)

#### 2. testbench编写

接下来重点就是编写testbench，可以用`radmeh()`函数来读取这个txt，作为信号激励进行功能验证。

```Verilog
s_cycle_cpu CPU(CLOCK,RESET);
always
   #5 CLOCK = ~CLOCK;  
initial
    $readmemh("code_fibonacci2.txt", CPU.IM.ins_memory); 
    //用code_fibonacci3.txt初始化指令存储器  	   
integer i;
initial
   begin
	  CLOCK = 1; RESET = 1;
	  #2 RESET = 0;
	     for(i=0;i<32;i=i+1)
		    CPU.GPR.gp_registers[i] = 0;   
	  #4 RESET = 1;	
   end   
```

#### 3. fabonacci2.asm的波形验证

得到的波形图：

![image-20220604215829006](C:\Users\shandaiwang\AppData\Roaming\Typora\typora-user-images\image-20220604215829006.png)

![image-20220604220927759](C:\Users\shandaiwang\AppData\Roaming\Typora\typora-user-images\image-20220604220927759.png)

说明功能正常。modelsim中对于上面的机器格式依次进行了波形展示。16进制机器格式一个时钟周期内执行一个，其他模块信号随之进行变化。

## 三、遇到的问题和解决方法：

1. 第一次实验addu的问题很大，因为不清楚设计流程，并且写完自信满满地直接提交，出现了很多红道道。然后回来调试，发现是一些小细节没有注意，比如pc需要初设地址 `data <= 32'h00003000;//初设地址`。老师还强调过，给忘了。

2. 接下来地R/I/MEM都比较顺利，其中MEM可以复用I型指令地运算我一开始是没有想到的。j指令更难一些，首先是解决npc模块多输入的选择处理问题，我使用了如下方式：

   ```verilog
   always @(*)
   		begin
   		npc = npc_t;
   			case(s)
   				`NONE: npc = npc_t;
   				`BEQ:
   					begin
   						if(zero)
   							npc = npc_t + {offset[29:0], 2'b00};
   						else
   							npc = npc_t;
   					end
   				`J_JAL: npc = {npc_t[31:28], instr_index, 2'b00};
   				`JR: npc = a;
   				
   				default:	npc = 32'hxxxxxxxx;
   			endcase
   		end
   ```

   接着就是beq指令要求的zero，要始终是0确实很麻烦。

   ```verilog
    zero <= (c == 0) ? 1 : 0;
   ```

   最后是.asm的波形仿真，一开始满屏全是红线，后来发现是指令存储器的读取上出现了问题。修改之后正常。

## 四、实验总结

- 在平台提交的很痛苦，出现了各种时序、信号控制的错误，后面不得不自己写tb代码仿真。
- 通过这次实验，从verilog 代码层面实践了单周期CPU的设计，对于理论课程中的处理器部分、指令集部分有了更深入的了解。
- 同时从0开始搭建，由ADDU实现一个单指令单周期的CPU，逐步增加模块，复杂度递增，但是有迹可循，整合了我此前很多方面的理解，让我对从体系结构到微体系结构有了一个连贯一点的认识。
- 对于前面四周的硬件实验也有了一些巩固，比如hex格式的文件表示的指令后续是如何被执行的，这次试验就给了我答案，是通过对指令的译码分别控制alu、mem、dm等进行工作。