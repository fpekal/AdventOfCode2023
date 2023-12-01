#include <iostream>
#include <fstream>

#include <atomic>

#include "pool.h"

std::string tab[] {
	"zero",
	"one",
	"two",
	"three",
	"four",
	"five",
	"six",
	"seven",
	"eight",
	"nine"
};

// Checks if the beginning of `p1` is equal to `p2`
bool my_compare_cstr(const char* p1, int len1, const char* p2, int len2) {
	if (len1 < len2) return false;

	for (int i = 0; i < len2; ++i) {
		if (p1[i] != p2[i]) return false;
	}

	return true;
}

// Finds the number in `str` at `pos`. If no number, return -1
int find_number(const std::string& str, int pos) {
	if (str[pos] >= '0' && str[pos] <= '9')
		return str[pos] - '0';

	for (int i = 0; i < 10; ++i) {
		const std::string& testing = tab[i];

		auto found = my_compare_cstr(str.c_str() + pos, str.length() - pos, testing.c_str(), testing.length());

		if (found)
			return i;
	}

	return -1;
}

// Task for one thread.
// If there will be 10 tasks, one task will check 1 in 10 inputs.
// For example, 1, 11, 21, 31, etc.
void task(const std::string& filename, int task_max, std::atomic<int>& out, int task_id) {
	std::ifstream input {filename};

	// Move task to its locstion in a file
	for (int i = 0; i < task_id; ++i) {
		std::string tmp;
		std::getline(input, tmp);
	}

	int sum = 0;

	while(input.peek() != EOF) {
		std::string line;
		std::getline(input, line);

		int left;
		int right;

		// Go from left to right
		for (int i = 0; i < line.length(); ++i) {
			int found = find_number(line, i);

			if (found != -1) {
				left = found;
				break;
			}
		}

		// Go from right to left
		for (int i = line.length() - 1; i >= 0; --i) {
			int found = find_number(line, i);

			if (found != -1) {
				right = found;
				break;
			}
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

	tp::thread_pool<threads> pool;
	std::atomic<int> sum;

	std::function<void(int)> func = std::bind_front(task, "input.txt", threads, std::ref(sum));

	for (int i = 0; i < threads; ++i) {
		pool.add_task(std::bind_front(func, i));
	}

	pool.join();

	std::cout << sum << '\n';
}
