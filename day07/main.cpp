#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QList>
#include <QDebug>
#include <algorithm>
#include <vector>
#include <utility>

namespace ranges = std::ranges;

enum class Card {
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

char cardToChar(Card card) {
    char labels[13] = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J' ,'Q', 'K', 'A'};

    return labels[(int)card];
}

class Hand {
    public:
        Hand() = delete;
        Hand(QString handStr) {
            for (QChar c: handStr) {
                if (c == '2') {
                    cards.push_back(Card::R2);
                } else if (c == '3') {
                    cards.push_back(Card::R3);
                } else if (c == '4') {
                    cards.push_back(Card::R4);
                } else if (c == '5') {
                    cards.push_back(Card::R5);
                } else if (c == '6') {
                    cards.push_back(Card::R6);
                } else if (c == '7') {
                    cards.push_back(Card::R7);
                } else if (c == '8') {
                    cards.push_back(Card::R8);
                } else if (c == '9') {
                    cards.push_back(Card::R9);
                } else if (c == 'T') {
                    cards.push_back(Card::R10);
                } else if (c == 'J') {
                    cards.push_back(Card::J);
                } else if (c == 'Q') {
                    cards.push_back(Card::Q);
                } else if (c == 'K') {
                    cards.push_back(Card::K);
                } else if (c == 'A') {
                    cards.push_back(Card::A);
                }
            }

            std::vector<Card> cardsSorted = cards;
            ranges::sort(cardsSorted, ranges::greater());

            // five of a kind
            if (cardsSorted[0] == cardsSorted[4]) {
                handType = 6;
                return;
            }

            // four of a kind
            if (cardsSorted[0] == cardsSorted[3] || cardsSorted[1] == cardsSorted[4]) {
                handType = 5;
                return;
            }

            // full house
            if ((cardsSorted[0] == cardsSorted[2] && cardsSorted[3] == cardsSorted[4]) || (cardsSorted[0] == cardsSorted[1] && cardsSorted[2] == cardsSorted[4])) {
                handType = 4;
                return;
            }

            // three of a kind
            if (cardsSorted[0] == cardsSorted[2] || cardsSorted[1] == cardsSorted[3] || cardsSorted[2] == cardsSorted[4]) {
                handType = 3;
                return;
            }

            // two pairs
            if ((cardsSorted[0] == cardsSorted[1] && (cardsSorted[2] == cardsSorted[3] || cardsSorted[3] == cardsSorted[4])) || (cardsSorted[1] == cardsSorted[2] && cardsSorted[3] == cardsSorted[4])) {
                handType = 2;
                return;
            }

            // one pair
            if (cardsSorted[0] == cardsSorted[1] || cardsSorted[1] == cardsSorted[2] || cardsSorted[2] == cardsSorted[3] || cardsSorted[3] == cardsSorted[4]) {
                handType = 1;
                return;
            }

            // high card
            handType = 0;
        }

        operator QString() const {
            QString str;
            for (Card card: cards) {
                str += cardToChar(card);
            }

            return str;
        }

        friend auto operator<=>(const Hand &a, const Hand &b) = default;

    private:
        int handType;
        std::vector<Card> cards;
};

int main(int argc, char *argv[]) {
    QTextStream out(stdout);

    QFile file("input");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal() << "Cannot open input";
        return 1;
    }

    QTextStream in(&file);

    QList<std::pair<Hand, int>> handsWithBids;
    while (!in.atEnd()) {
        QString line = in.readLine();

        QStringList parts = line.split(" ");
        handsWithBids.append({Hand(parts[0]), parts[1].toInt()});
    }

    ranges::sort(handsWithBids);

    int sum = 0;
    for (int multiplier = 1; multiplier <= handsWithBids.count(); multiplier++) {
        sum += multiplier * handsWithBids[multiplier - 1].second;
    }

    out << "Sum: " << sum << "\n";

    return 0;
}
