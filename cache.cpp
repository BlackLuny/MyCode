#include <iostream>
#include <deque>
using namespace std;
const unsigned int max_memeory_addr = 0xffff;
const unsigned int LEN_PER_PAGE = 0x40;
typedef struct info
{
	info(unsigned int addr):page_start_addr(addr&0xFFC0){};
	unsigned int page_start_addr;
}Info,*PInfo;
//在cache中查找是否有addr所在的页，如果有，则返回此页的迭代器，否则就返回指向end()的迭代器
deque<Info>::const_iterator checkHint(const deque<Info> &cache, unsigned int addr)
{
	size_t cnt = cache.size();
	unsigned int page_start = addr & 0xFFC0;
	deque<Info>::const_iterator itr;
	for (itr = cache.begin(); itr != cache.end(); ++itr)
	{
		if (page_start == (*itr).page_start_addr) return itr;
	}
	return itr;
}
int cache_hint(const deque<unsigned int>& mem_seq)
{
	deque<Info> caches;
	int cnt = 0;
	deque<unsigned int>::const_iterator itr;
	for (itr = mem_seq.begin(); itr != mem_seq.end(); ++itr)
	{
		unsigned int cur_addr = *itr;
		deque<Info>::const_iterator tmp_itr = checkHint(caches, cur_addr);
		if (tmp_itr == caches.end())
		{
			//没有命中
			caches.push_back(info(cur_addr));
			if (caches.size() > 8) caches.pop_front();
		}else{
			cnt++;
		}
	}
return cnt;
}
int main()
{
	deque<unsigned int> deq;
	deq.push_back(0x60);
	deq.push_back(0x61);
	deq.push_back(0x900);
	deq.push_back(0x900);
	deq.push_back(0x901);
	deq.push_back(0x2000);
	deq.push_back(0x2000);
	deq.push_back(0x2500);
	deq.push_back(0x2400);
	deq.push_back(0x2300);
	deq.push_back(0x2200);
	deq.push_back(0x2100);
	deq.push_back(0x2900);
	deq.push_back(0x67);
	int cnt = cache_hint(deq);
	cout << cnt << endl;
	return 0;
}