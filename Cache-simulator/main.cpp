#include<iostream>
#include<stdio.h>
#include<stdlib.h>





#include<vector>
using namespace std;

struct block{
	bool is_valid;
	int tag;
	void *data;
};
#define MEM_READ_TIME 100
#define CACHE_READ_TIME 1

class Cache{
	public:

	int cache_size;
	int block_size;
	int set_size;

	int set_count;
	//读、写的总次数
	int total;
	//读的次数
	int reads;
	//写的次数
	int writes;
	//命中次数
	int hits;
	//替换策略的随机替换次数
	int replaces;
	//访问时间
	int total_time;

	vector<struct block> blocks;

	void init(int p_cache_size, int p_block_size, int p_set_size){
		cache_size = p_cache_size;
		block_size = p_block_size;
		set_size = p_set_size;
		
		total = 0;
		reads = 0;
		writes = 0;
		hits = 0;
		replaces = 0;

		total_time = 0;

		set_count = cache_size / block_size / set_size;

		for(int i = 0; i < set_count * set_size; i++){
			struct block b;
		

			b.is_valid = false;
			b.tag = -1;
			blocks.push_back(b);
			//在动态数组后再添加一个块儿
		}
	}
	void read(int addr){//实现读操作
		cout << "-- read from " << addr;
		reads++;//读操作数+1
		total++;//总次数+1

		int tag = (tag / set_size) % set_count;
		
		int index = (tag / set_size) % set_count;

		total_time = CACHE_READ_TIME;//时间

		for(int i = index; i < index + set_size; i++){
			if(blocks[i].is_valid && blocks[i].tag ==tag){//说明命中了
				hits ++;
				cout << "read finish, hit!" << endl;
				return;
			}
		}
		//上面循环中，若命中则退出，下面则为不命中
		cout << " miss!" << endl;
		
		// 从内存加载
		total_time += MEM_READ_TIME;

		for(int i = index; i < index + set_size; i++){
			if(!blocks[i].is_valid){
				blocks[i].is_valid = true;
				blocks[i].tag = tag;

				cout << "-- --- -- memory loaded" << endl;
			}
			else{
				cout << "memory failed!" << endl;
			}
		}
		//随机匹配
		cout << "cache is full.Now let's replace" <<endl;
		//采用随机替换，将数据放入block
		int ran = rand()%set_size + index;
		//替换次数+1
		replaces++;

		//更改有效位
		blocks[ran].is_valid = true;
		//标志位
		blocks[ran].tag = tag;
		cout << "replace finished!" << endl;

	}
	void write(int addr){//实现写操作
		cout << "-- write to " << addr << endl;

		writes++;
		total++;

		//寻找目标cache
		int tag = addr / block_size;
		//计算地址
		int index = (tag / set_size) % set_count;

		total_time += CACHE_READ_TIME;

		for (int i = 0; i < index + set_size; i++){
			if(!blocks[i].is_valid){
				blocks[i].is_valid = true;
				blocks[i].tag = tag;
				//此时即为命中
				hits++;
				cout << "write finished.hit!" << endl;



			}
		}
		
	}
}