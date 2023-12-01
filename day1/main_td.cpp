#include <iostream>
#include <fstream>

#include <atomic>

#include <unordered_map>

#include "pool.h"

std::unordered_map<uint64_t, int> map;
void map_setup() {
  // one
  map[0x6f6e65] = 1;
  map[0x31] = 1;
  // two
  map[0x74776f] = 2;
  map[0x32] = 2;
  // three
  map[0x7468726565] = 3;
  map[0x33] = 3;
  // four
  map[0x666f7572] = 4;
  map[0x34] = 4;
  // five
  map[0x66697665] = 5;
  map[0x35] = 5;
  // six
  map[0x736978] = 6;
  map[0x36] = 6;
  // seven
  map[0x736576656e] = 7;
  map[0x37] = 7;
  // eight
  map[0x6569676874] = 8;
  map[0x38] = 8;
  // nine
  map[0x6e696e65] = 9;
  map[0x39] = 9;
}

// Task for one thread.
// If there will be 10 tasks, one task will check 1 in 10 inputs.
// For example, 1, 11, 21, 31, etc.
void task(const std::string& filename, int task_max, std::atomic<int>& out, int task_id) {
	std::ifstream input {filename};

	// Move task to its location in a file
	for (int i = 0; i < task_id; ++i) {
		std::string tmp;
		std::getline(input, tmp);
	}

	int sum = 0;

	while(input.peek() != EOF) {
		std::string line;
		std::getline(input, line);
		line += "     ";

		int left = 0;
		int right = 0;

		uint64_t hash = 0;


    for (auto c : line) {
      hash <<= 8;
      hash |= c;
      auto iter = map.find(hash & 0xff);

      if (iter == map.end())
				iter = map.find(hash & 0xffffff);
      if (iter == map.end())
				iter = map.find(hash & 0xffffffff);
      if (iter == map.end())
				iter = map.find(hash & 0xffffffffff);
      if (iter == map.end())
				continue;
      if (!left) {
				left = iter->second;
      }
      right = iter->second;
    }

		sum += left * 10 + right;

		// Shift to the next line
		for (int i = 0; i < task_max - 1; ++i) {
			std::string tmp;
			std::getline(input, tmp);
		}
	}

	out += sum;
}

int main() {
	static constexpr int threads = 4;

	map_setup();

	tp::thread_pool<threads> pool;
	std::atomic<int> sum;

	std::function<void(int)> func = std::bind_front(task, "input.txt", threads, std::ref(sum));

	for (int i = 0; i < threads; ++i) {
		pool.add_task(std::bind_front(func, i));
	}

	pool.join();

	std::cout << sum << '\n';
}
