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
	int hits,reads,writes,replaces,total,total_time;
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
}