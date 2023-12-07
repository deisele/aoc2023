#ifndef HANDPART2_H
#define HANDPART2_H

#include <QString>
#include <QVector>
#include <algorithm>
#include <vector>

enum class CardPart2 {
    J,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    R8,
    R9,
    R10,
    Q,
    K,
    A
};

char cardPart2ToChar(CardPart2 card) {
    char labels[13] = {'J', '2', '3', '4', '5', '6', '7', '8', '9', 'T' ,'Q', 'K', 'A'};

    return labels[(int)card];
}

class HandPart2 {
    public:
        HandPart2() = delete;
        HandPart2(QString handStr) {
            for (QChar c: handStr) {
                if (c == 'J') {
                    cards.push_back(CardPart2::J);
                }else if (c == '2') {
                    cards.push_back(CardPart2::R2);
                } else if (c == '3') {
                    cards.push_back(CardPart2::R3);
                } else if (c == '4') {
                    cards.push_back(CardPart2::R4);
                } else if (c == '5') {
                    cards.push_back(CardPart2::R5);
                } else if (c == '6') {
                    cards.push_back(CardPart2::R6);
                } else if (c == '7') {
                    cards.push_back(CardPart2::R7);
                } else if (c == '8') {
                    cards.push_back(CardPart2::R8);
                } else if (c == '9') {
                    cards.push_back(CardPart2::R9);
                } else if (c == 'T') {
                    cards.push_back(CardPart2::R10);
                } else if (c == 'Q') {
                    cards.push_back(CardPart2::Q);
                } else if (c == 'K') {
                    cards.push_back(CardPart2::K);
                } else if (c == 'A') {
                    cards.push_back(CardPart2::A);
                }
            }

            QVector<int> rankCount(13);
            for (CardPart2 card: cards) {
                rankCount[(int)card]++;
            }

            int jokerCount = rankCount.takeFirst();

            std::ranges::sort(rankCount, std::ranges::greater());

            // five of a kind
            if (rankCount[0] + jokerCount >= 5) {
                handType = 6;
                return;
            }

            // four of a kind
            if (rankCount[0] + jokerCount >= 4) {
                handType = 5;
                return;
            }

            // full house
            if (rankCount[0] + jokerCount >= 3 && rankCount[1] + rankCount[0] + jokerCount - 3 >= 2) {
                handType = 4;
                return;
            }

            // three of a kind
            if (rankCount[0] + jokerCount >= 3) {
                handType = 3;
                return;
            }

            // two pairs
            if (rankCount[0] + jokerCount >= 2 && rankCount[1] + + rankCount[0] + jokerCount - 2 >= 2) {
                handType = 2;
                return;
            }

            // one pair
            if (rankCount[0] + jokerCount >= 2) {
                handType = 1;
                return;
            }

            // high card
            handType = 0;
        }

        operator QString() const {
            QString str;
            for (CardPart2 card: cards) {
                str += cardPart2ToChar(card);
            }

            return str;
        }

        friend auto operator<=>(const HandPart2 &a, const HandPart2 &b) = default;

    private:
        int handType;
        std::vector<CardPart2> cards;
};

#endif
