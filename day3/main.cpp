#include <iostream>
#include <fstream>

#include <unordered_map>
#include <memory>
#include <optional>
#include <vector>

constexpr int BOARD_WIDTH = 140;
constexpr int BOARD_HEIGHT = 140;

class number {
public:
	int value = 0;
	bool summed = false;
};

class board {
public:
	std::optional<std::reference_wrapper<std::shared_ptr<number>>> get(int x, int y) {
		if (x < 0 || x >= BOARD_WIDTH) return {};
		if (y < 0 || y >= BOARD_HEIGHT) return {};

		return board[x][y];
	}

private:
	std::shared_ptr<number> board[BOARD_WIDTH][BOARD_HEIGHT];
};

int main() {
	std::ifstream input {"input.txt"};

	board b {};
	std::vector<std::pair<int,int>> symbols {};

	int y = 0;

	while(input.peek() != EOF) {
		std::string line;
		std::getline(input, line);

		std::shared_ptr<number> active_number;
		bool is_number = false;

		for (int x = 0; x < BOARD_WIDTH; ++x) {
			char c = line[x];

			// Dot
			if (c == '.') {
				is_number = false;
			}

			// Number
			else if (c >= '0' && c <= '9') {
				if (!is_number) {
					active_number = std::make_shared<number>();
					is_number = true;
				}

				active_number->value *= 10;
				active_number->value += c - '0';

				auto opt = b.get(x, y);

				if (opt.has_value())
					opt.value().get() = active_number;
			}

			// Symbol
			else if (c == '*') {
				is_number = false;
				symbols.emplace_back(x, y);
			}
		}

		++y;
	}

	long long sum = 0;

	for (auto& c : symbols) {
		auto& [x, y] = c;

		long long ratio = 1;
		int count = 0;

		for (int dx = -1; dx < 2; ++dx) {
			for (int dy = -1; dy < 2; ++dy) {
				auto opt = b.get(x + dx, y + dy);
				if (opt.has_value()) {
					auto& val = opt.value().get();

					if (val) {
						if (!val->summed) {
							val->summed = true;
							count += 1;
							ratio *= val->value;
						}
					}
				}
			}
		}

		std::cout << count << '\n';
		if (count == 2)
			sum += ratio;
	}

	std::cout << sum << '\n';
}
