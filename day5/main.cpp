#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <sstream>

struct Mapper {
	long long from_in;
	long long from_out;
	long long len;
};

struct Seeder {
	long long start;
	long long len;
};

int main() {
	std::ifstream input {"input.txt"};

	std::vector<Seeder> seeds;
	std::vector<std::vector<Mapper>> maps;

	std::string seeds_str;
	std::getline(input, seeds_str);

	seeds_str = seeds_str.substr(7);

	std::stringstream seeds_ss {seeds_str};

	while(!seeds_ss.eof()) {
		long long start, len;
		seeds_ss >> start >> len;

		seeds.emplace_back(start, len);
	}

	input.ignore(100, '\n');

	while(input.peek() != EOF) {
		input.ignore(100, '\n');

		std::vector<Mapper> map;

		while(true) {
			std::string line;
			std::getline(input, line);

			if (line == "") break;

			std::stringstream ss {line};

			long long in, out, len;
			ss >> out >> in >> len;

			map.emplace_back(in, out, len);
		}

		maps.emplace_back(std::move(map));
	}

	long long least = 99999999999999;

	for (Seeder& seeder : seeds) {
		for (long long i2 = 0; i2 < seeder.len; ++i2) {
			long long s = seeder.start + i2;
			for (std::vector<Mapper>& map : maps) {
				for (Mapper& mapper : map) {
					if (s >= mapper.from_in && s < mapper.from_in + mapper.len) {
						s = (s - mapper.from_in + mapper.from_out);
						break;
					}
				}
			}

			if (least > s) least = s;
		}
	}

	std::cout << least << '\n';
}
