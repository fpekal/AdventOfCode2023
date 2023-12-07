#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <algorithm>

enum HandType {
	HIGH_CARD,
	ONE_PAIR,
	TWO_PAIRS,
	THREE_OF_KIND,
	FULL,
	FOUR_OF_KIND,
	FIVE_OF_KIND
};

int card_to_int(char c) {
	switch(c) {
		case 'T':
			return 9;
		case 'J':
			return 0;
		case 'Q':
			return 10;
		case 'K':
			return 11;
		case 'A':
			return 12;
		default:
			return c - '1';
	}
}

class Hand {
public:
	std::array<int, 5> cards;
	long long bid;
	HandType type;

	Hand(const std::string& str) {
		parse_cards(str);
		detect_hand_type();

		parse_bid(str);
	}

private:
	void parse_bid(const std::string& str) {
		bid = std::atol(str.c_str() + 6);
	}

	void parse_cards(const std::string& str) {
		for (int i = 0; i < 5; ++i) {
			cards.at(i) = card_to_int(str[i]);
		}
	}

	void detect_hand_type() {
		std::array<int, 13> cards_num {};

		int jokers = 0;

		for (int c : cards) {
			if (c == 0) {
				jokers++;
			}
			else
				++cards_num.at(c);
		}

		std::array<int, 4> counts {};

		for (int c : cards_num) {
			if (c == 0) continue;
			if (c == 1) continue;

			++counts.at(c - 2);
		}

		if (counts.at(3) >= 1) {
			type = FIVE_OF_KIND;
		}

		else if (counts.at(2) >= 1) {
			type = FOUR_OF_KIND;
		}

		else if (counts.at(1) >= 1 && counts.at(0) >= 1) {
			type = FULL;
		}

		else if (counts.at(1) >= 1) {
			type = THREE_OF_KIND;
		}

		else if (counts.at(0) >= 2) {
			type = TWO_PAIRS;
		}

		else if (counts.at(0) == 1) {
			type = ONE_PAIR;
		}

		else type = HIGH_CARD;

		switch(jokers) {
		case 0:
			return;
		case 1:
			switch(type) {
			case HIGH_CARD:
				type = ONE_PAIR;
				return;
			case ONE_PAIR:
				type = THREE_OF_KIND;
				return;
			case TWO_PAIRS:
				type = FULL;
				return;
			case THREE_OF_KIND:
				type = FOUR_OF_KIND;
				return;
			case FOUR_OF_KIND:
				type = FIVE_OF_KIND;
				return;
			}
		case 2:
			switch(type) {
			case HIGH_CARD:
				type = THREE_OF_KIND;
				return;
			case ONE_PAIR:
				type = FOUR_OF_KIND;
				return;
			case THREE_OF_KIND:
				type = FIVE_OF_KIND;
				return;
			}
		case 3:
			switch(type) {
			case HIGH_CARD:
				type = FOUR_OF_KIND;
				return;
			case ONE_PAIR:
				type = FIVE_OF_KIND;
				return;
			}
		case 4: [[fallthrough]];
		case 5:
			type = FIVE_OF_KIND;
			return;
		}
	}
};

class HandComparator {
public:
	bool operator()(const Hand& h1, const Hand& h2) {
		if ((int)h1.type != (int)h2.type)
			return (int)h1.type < (int)h2.type;

		for (int i = 0; i < 5; ++i) {
			if (h1.cards[i] < h2.cards[i]) return true;
			if (h1.cards[i] > h2.cards[i]) return false;
		}

		return true;
	}
};

int main() {
	std::ifstream input {"input.txt"};

	std::vector<Hand> hands;

	while(input.peek() != EOF) {
		std::string line;
		std::getline(input, line);

		hands.emplace_back(line);
	}

	std::sort(hands.begin(), hands.end(), HandComparator{});

	long long sum = 0;
	long long i = 1;
	for (Hand& h : hands) {
		sum += h.bid * i;
		++i;
	}

	std::cout << sum << '\n';
}
