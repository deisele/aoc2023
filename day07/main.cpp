#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QList>
#include <QDebug>
#include <algorithm>
#include <utility>
#include "HandPart1.h"
#include "HandPart2.h"

int main(int argc, char *argv[]) {
    QTextStream out(stdout);

    QFile file("input");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal() << "Cannot open input";
        return 1;
    }

    QTextStream in(&file);

    QList<std::pair<HandPart1, int>> handsWithBidsPart1;
    QList<std::pair<HandPart2, int>> handsWithBidsPart2;
    while (!in.atEnd()) {
        QString line = in.readLine();

        QStringList parts = line.split(" ");
        handsWithBidsPart1.append({HandPart1(parts[0]), parts[1].toInt()});
        handsWithBidsPart2.append({HandPart2(parts[0]), parts[1].toInt()});
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
