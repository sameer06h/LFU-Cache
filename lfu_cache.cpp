#include <iostream>
#include<list>
#include<map>
#include<algorithm>

using namespace std;


template <class K, class V>
class LfuCache {
private:
	unsigned int _sizeofcache = 5;
	map<K, pair<V, int>> lfcache;
	map<int, list<K>> freqlist;

public:
	  LfuCache(int sizeofcache) : _sizeofcache(sizeofcache) {}

	void print_freq_list() {
		for (auto i : freqlist) {
			cout << "freq: " << i.first << " keys:";
			for (auto v : i.second) {
				cout << " " << v;
			}
			cout << endl;
		}
	}

	void replace_cache_page(K key, V Value) {
		auto lfkey = freqlist.begin();
		cout << "removing key: " << lfkey->second.front() <<" frequency of key access: " << lfkey->first << endl;
		//removed the entry from the beginning of the list i.e the least frequenctly used page
		lfcache.erase(lfkey->second.front());
		lfkey->second.pop_front();
		if (lfkey->second.empty()) {
			freqlist.erase(lfkey->first);
		}
	}

	void insert_into_freqlist(int freq, K key) {
		//we just accessed a page from cache, delete its old frequency from the list
		//and add new entry with frequency + 1
		auto old_list_entry = freqlist.find(freq - 1);

		//remove old entry if it exisits
		if (old_list_entry != freqlist.end()) {
			// get the reference of list from the freqlist map
			auto old_flist = &(old_list_entry->second);
			//find the key in the list
			auto old_list_res = std::find(old_flist->begin(), old_flist->end(), key);
			if (old_list_res != old_flist->end()) {
				//remove the key from list
				old_flist->remove(key);
				//after removing the key from the list, list becomes empty ?, remove the list too.
				// wait... if there is nothing in the list why do we need an entry for that in frequency list map?
				// remove the entry for that frequency from the frequency map.
				if (old_flist->empty()) {
					freqlist.erase(freq - 1);
				}
			} 

		}

		//find if new frequency entry exists
		auto new_list_entry = freqlist.find(freq);
		if (new_list_entry != freqlist.end()) {
			//get reference of new_list from freqlist
			auto new_flist = &new_list_entry->second;
			new_flist->push_back(key);
		} else {
			// the frequency entry doesn`t exist in the frequency map add a new map entry.
			freqlist.insert(pair<int, list<K>>(freq, list<K>{ key }));
		}
	}

	void put(K key, V value) {
		if (lfcache.size() >= _sizeofcache) {
			//replace the page
			replace_cache_page(key, value);
		}
		lfcache.insert(pair<K, pair<V, int>>(key, pair<V,int>(value, 0)));
		// we add initial frequency of access for the page as 0,
		// mere add into map doesnt populate the frequency map, to populate the entry in to the frequencylist map we do a get below.
		get(key);

	}

	V get(K key) {
		auto res_pair = lfcache.find(key);
        if(res_pair == lfcache.end()) {
            return V(NULL);
        }
		(res_pair->second).second++;
		//cout << "Key:" << res_pair->first << " value:" << (res_pair->second).first << " frequency:" << (res_pair->second).second << endl;
		insert_into_freqlist((res_pair->second).second, res_pair->first);
		return (res_pair->second).first;
	}
};

int main()
{
	LfuCache<int,int> testcache(5);
    // not really caring about get return
    //
	testcache.put(1, 10);
	testcache.get(1);
	testcache.put(2, 20);
	testcache.put(3, 30);
	testcache.put(4, 40);
	testcache.put(5, 50);
	testcache.get(2);
	testcache.get(2);
	testcache.get(3);
	testcache.get(3);
	testcache.get(4);
	testcache.get(4);
	testcache.get(5);
	testcache.get(5);
    /*
     * by now map should look like:
     * [key, value, frequency]
     * [1, 10, 2]
     * [2, 20, 3]
     * [3, 30, 3]
     * [4, 40, 3]
     * [5, 50, 3]
     *
     * replace page case from top least frequently used page is [1, 10, 2]
     * [6, 60, 1]
     *
     * */
	testcache.print_freq_list();
	testcache.put(6, 60);
	testcache.print_freq_list();

    auto res = testcache.get(10);
    if( !res) {
        cout << "invalid key, page not found in cache" << endl;
    }
}
