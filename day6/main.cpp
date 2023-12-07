#include <iostream>
#include <fstream>
#include <sstream>

int main() {
	std::ifstream input {"input.txt"};

	std::string line1, line2;
	std::getline(input, line1);
	std::getline(input, line2);

	line1 = line1.substr(5);
	line2 = line2.substr(9);

	std::stringstream ss1 {line1};
	std::stringstream ss2 {line2};

	int ret = 1;
	
	for (int i = 0; i < 1; ++i) {
		double time, distance;
		ss1 >> time;
		ss2 >> distance;

		std::cout << time << " " << distance << "\n";

		long long count = 0;

		for (long long t = 0; t <= time; ++t) {
			if ((time - t)*t > distance) {
				++count;
			}
		}

		ret *= count;
	}

	std::cout << ret << '\n';
}
