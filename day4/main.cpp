#include <iostream>
#include <fstream>

#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include <numeric>

class ticket {
public:
	int game_id = 0;
	int score = 0;

	ticket() {
		game_id = m_next_id;
		m_next_id++;
	}

	ticket(const ticket& other) {
		game_id = other.game_id;
		score = other.score;
	}

	void parse_line(const std::string& line) {
		std::stringstream ss {line};

		ss.ignore(100, ':');

		std::unordered_set<int> numbers;

		for (int i = 0; i < 10; ++i) {
			int number = 0;
			ss >> number;

			numbers.insert(number);
		}

		ss.get();
		ss.get();

		for (int i = 0; i < 25; ++i) {
			int number = 0;
			ss >> number;

			if (numbers.contains(number)) {
				score++;
			}
		}
	}

	static int m_next_id;
};

int ticket::m_next_id = 0;

int main() {
	std::ifstream input {"input.txt"};


	long long sum = 0;

	std::unordered_map<int, ticket> tickets_map;

	while(input.peek() != EOF) {
		std::string line;
		std::getline(input, line);

		ticket t;
		t.parse_line(line);

		tickets_map.insert({t.game_id, t});
	}

	std::vector<int> unused_tickets;
	unused_tickets.resize(ticket::m_next_id);
	std::iota(unused_tickets.begin(), unused_tickets.end(), 0);

	sum += ticket::m_next_id;

	for (;unused_tickets.size() > 0;) {
		int new_ticket = unused_tickets.front();
		unused_tickets.erase(unused_tickets.begin());

		auto& ticket_info = tickets_map.at(new_ticket);

		sum += ticket_info.score;

		for (int i = ticket_info.game_id + 1; i < ticket_info.game_id + ticket_info.score + 1; ++i) {
			if (i < ticket::m_next_id)
				unused_tickets.push_back(i);
		}


	}

	std::cout << sum << '\n';
}
