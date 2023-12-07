#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QList>
#include <QDebug>
#include <algorithm>
#include <utility>
#include <vector>
#include <cassert>

enum class AocPart {
    One,
    Two
};

enum class Rank {
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

char cardPart1ToChar(Rank card) {
    char labels[13] = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J' ,'Q', 'K', 'A'};

    return labels[(int)card];
}

class Hand {
    public:
        Hand() = delete;
        Hand(QString handStr, AocPart part): part(part) {
            for (QChar c: handStr) {
                Rank rank;

                if ('2' <= c && c <= '9') {
                    rank = (Rank)(c.unicode() - '2');
                } else if (c == 'T') {
                    rank = Rank::R10;
                } else if (c == 'J') {
                    rank = Rank::J;
                } else if (c == 'Q') {
                    rank = Rank::Q;
                } else if (c == 'K') {
                    rank = Rank::K;
                } else if (c == 'A') {
                    rank = Rank::A;
                }

                cards.push_back(rank);
            }

            QList<int> rankCount(13);
            for (Rank card: cards) {
                rankCount[(int)card]++;
            }

            int jokerCount;
            if (part == AocPart::Two) {
                jokerCount = rankCount.takeAt((int)Rank::J);
            }

            std::ranges::sort(rankCount, std::ranges::greater());

            if (part == AocPart::Two) {
                // add jokers to the rank with the most cards
                rankCount[0] += jokerCount;
            }

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
            for (Rank card: cards) {
                str += cardPart1ToChar(card);
            }

            return str;
        }

        friend std::strong_ordering operator<=>(const Hand &a, const Hand &b) {
            assert(a.part == b.part);
            assert(a.cards.size() == b.cards.size());
            assert(a.cards.size() == 5);

            std::strong_ordering handTypeOrder = a.handType <=> b.handType;
            if (handTypeOrder != std::strong_ordering::equal) {
                return handTypeOrder;
            }

            for (int i = 0; i < (int)a.cards.size(); i++) {
                std::strong_ordering cardOrder = a.cards[i] <=> b.cards[i];
                if (cardOrder != std::strong_ordering::equal) {
                    if (a.part == AocPart::Two) {
                        if (a.cards[i] == Rank::J) {
                            return std::strong_ordering::less;
                        }

                        if (b.cards[i] == Rank::J) {
                            return std::strong_ordering::greater;
                        }
                    }

                    return cardOrder;
                }
            }

            return std::strong_ordering::equal;
        };

    private:
        AocPart part;
        int handType;
        std::vector<Rank> cards;
};

int main(int argc, char *argv[]) {
    QTextStream out(stdout);

    QFile file("input");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal() << "Cannot open input";
        return 1;
    }

    QTextStream in(&file);

    QList<std::pair<Hand, int>> handsWithBidsPart1;
    QList<std::pair<Hand, int>> handsWithBidsPart2;
    while (!in.atEnd()) {
        QString line = in.readLine();

        QStringList parts = line.split(" ");
        handsWithBidsPart1.append({Hand(parts[0], AocPart::One), parts[1].toInt()});
        handsWithBidsPart2.append({Hand(parts[0], AocPart::Two), parts[1].toInt()});
    }

    std::ranges::sort(handsWithBidsPart1);
    std::ranges::sort(handsWithBidsPart2);

    int sum1 = 0;
    int sum2 = 0;
    for (int multiplier = 1; multiplier <= handsWithBidsPart1.count(); multiplier++) {
        sum1 += multiplier * handsWithBidsPart1[multiplier - 1].second;
        sum2 += multiplier * handsWithBidsPart2[multiplier - 1].second;
    }

    out << "Sum 1: " << sum1 << "\n";
    out << "Sum 2: " << sum2 << "\n";

    return 0;
}
