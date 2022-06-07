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
	//����д���ܴ���
	int total;
	//���Ĵ���
	int reads;
	//д�Ĵ���
	int writes;
	//���д���
	int hits;
	//�滻���Ե�����滻����
	int replaces;
	//����ʱ��
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
			//�ڶ�̬����������һ�����
		}
	}
	void read(int addr){//ʵ�ֶ�����
		cout << "-- read from " << addr;
		reads++;//��������+1
		total++;//�ܴ���+1

		int tag = (tag / set_size) % set_count;
		
		int index = (tag / set_size) % set_count;

		total_time = CACHE_READ_TIME;//ʱ��

		for(int i = index; i < index + set_size; i++){
			if(blocks[i].is_valid && blocks[i].tag ==tag){//˵��������
				hits ++;
				cout << "read finish, hit!" << endl;
				return;
			}
		}
		//����ѭ���У����������˳���������Ϊ������
		cout << " miss!" << endl;
		
		// ���ڴ����
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
		//���ƥ��
		cout << "cache is full.Now let's replace" <<endl;
		//��������滻�������ݷ���block
		int ran = rand()%set_size + index;
		//�滻����+1
		replaces++;

		//������Чλ
		blocks[ran].is_valid = true;
		//��־λ
		blocks[ran].tag = tag;
		cout << "replace finished!" << endl;

	}
	void write(int addr){//ʵ��д����
		cout << "-- write to " << addr << endl;

		writes++;
		total++;

		//Ѱ��Ŀ��cache
		int tag = addr / block_size;
		//�����ַ
		int index = (tag / set_size) % set_count;

		total_time += CACHE_READ_TIME;

		for (int i = 0; i < index + set_size; i++){
			if(!blocks[i].is_valid){
				blocks[i].is_valid = true;
				blocks[i].tag = tag;
				//��ʱ��Ϊ����
				hits++;
				cout << "write finished.hit!" << endl;



			}
		}
		
	}
}