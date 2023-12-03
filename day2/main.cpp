#include <iostream>
#include <fstream>
#include <sstream>
#include <charconv>

bool are_cubes_possible(int num, const std::string& color) {
	char c = color[0];

	switch (c) {
	case 'r':
		return num <= 12;
	case 'g':
		return num <= 13;
	case 'b':
		return num <= 14;
	}

	return false;
}

bool is_game_possible(const std::string& game) {
	std::string tmp;
	std::stringstream ss {game};
	ss >> tmp >> tmp; // Get rid of Game id
	
	bool game_possible = true;

	// One set
	while (true) {

		int num;
		std::string color;

		std::string num_str;
		ss >> num_str >> color;

		std::from_chars(num_str.c_str(), num_str.c_str() + num_str.length(), num);

		bool cubes_possible = are_cubes_possible(num, color);

		if (!cubes_possible)
			game_possible = false;

		if (ss.eof())
			break;
	}

	return game_possible;
}

int get_power(const std::string& game) {
	std::string tmp;
	std::stringstream ss {game};
	ss >> tmp >> tmp; // Get rid of Game id
	
	int r = 0, g = 0, b = 0;
	
	// One set
	while (true) {

		int num;
		std::string color;

		std::string num_str;
		ss >> num_str >> color;

		std::from_chars(num_str.c_str(), num_str.c_str() + num_str.length(), num);

		if (color[0] == 'r')
			if (r < num)
				r = num;

		if (color[0] == 'g')
			if (g < num)
				g = num;

		if (color[0] == 'b')
			if (b < num)
				b = num;

		if (ss.eof())
			break;
	}

	return r * g * b;
}

int main() {
	std::ifstream input {"input.txt"};

	long long sum = 0;

	while(input.peek() != EOF) {
		std::string game;
		std::getline(input, game);

		int power = get_power(game);

		sum += power;
	}

	std::cout << sum << '\n';
}
