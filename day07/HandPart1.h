#ifndef HANDPART1_H
#define HANDPART1_H

#include <QString>
#include <algorithm>
#include <vector>

enum class CardPart1 {
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    R8,
    R9,
    R10,
    J,
    Q,
    K,
    A
};

char cardPart1ToChar(CardPart1 card) {
    char labels[13] = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J' ,'Q', 'K', 'A'};

    return labels[(int)card];
}

class HandPart1 {
    public:
        HandPart1() = delete;
        HandPart1(QString handStr) {
            for (QChar c: handStr) {
                if (c == '2') {
                    cards.push_back(CardPart1::R2);
                } else if (c == '3') {
                    cards.push_back(CardPart1::R3);
                } else if (c == '4') {
                    cards.push_back(CardPart1::R4);
                } else if (c == '5') {
                    cards.push_back(CardPart1::R5);
                } else if (c == '6') {
                    cards.push_back(CardPart1::R6);
                } else if (c == '7') {
                    cards.push_back(CardPart1::R7);
                } else if (c == '8') {
                    cards.push_back(CardPart1::R8);
                } else if (c == '9') {
                    cards.push_back(CardPart1::R9);
                } else if (c == 'T') {
                    cards.push_back(CardPart1::R10);
                } else if (c == 'J') {
                    cards.push_back(CardPart1::J);
                } else if (c == 'Q') {
                    cards.push_back(CardPart1::Q);
                } else if (c == 'K') {
                    cards.push_back(CardPart1::K);
                } else if (c == 'A') {
                    cards.push_back(CardPart1::A);
                }
            }

            int rankCount[13] = {0};
            for (CardPart1 card: cards) {
                rankCount[(int)card]++;
            }

            std::ranges::sort(rankCount, std::ranges::greater());

            // five of a kind
            if (rankCount[0] == 5) {
                handType = 6;
                return;
            }

            // four of a kind
            if (rankCount[0] == 4) {
                handType = 5;
                return;
            }

            // full house
            if (rankCount[0] == 3 && rankCount[1] == 2) {
                handType = 4;
                return;
            }

            // three of a kind
            if (rankCount[0] == 3) {
                handType = 3;
                return;
            }

            // two pairs
            if (rankCount[0] == 2 && rankCount[1] == 2) {
                handType = 2;
                return;
            }

            // one pair
            if (rankCount[0] == 2) {
                handType = 1;
                return;
            }

            // high card
            handType = 0;
        }

        operator QString() const {
            QString str;
            for (CardPart1 card: cards) {
                str += cardPart1ToChar(card);
            }

            return str;
        }

        friend auto operator<=>(const HandPart1 &a, const HandPart1 &b) = default;

    private:
        int handType;
        std::vector<CardPart1> cards;
};

#endif
