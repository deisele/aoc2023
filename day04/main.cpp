#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QList>
#include <QSet>
#include <QDebug>

int main(int argc, char *argv[]) {
    QTextStream out(stdout);

    QFile file("input");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal() << "Cannot open input";
        return 1;
    }

    QTextStream in(&file);

    int sum1 = 0;
    int sum2 = 0;
    QList<int> winCounts;
    QList<int> copies;

    while (!in.atEnd()) {
        QString line = in.readLine();

        QStringList parts = line.replace("|", ":").split(":");
        QStringList winningNumbersList = parts[1].simplified().split(" ");
        QStringList ourNumbersList = parts[2].simplified().split(" ");

        QSet<QString> winningNumbers(winningNumbersList.begin(), winningNumbersList.end());
        QSet<QString> ourNumbers(ourNumbersList.begin(), ourNumbersList.end());

        winCounts.append(ourNumbers.intersect(winningNumbers).count());
        copies.append(1);
    }

    for (int i = 0; i < winCounts.count(); i++) {
        if (winCounts[i] > 0) {
            sum1 += 1 << (winCounts[i] - 1);
        }

        for (int j = i + 1; j < i + winCounts[i] + 1; j++) {
            copies[j] += copies[i];
        }

        sum2 += copies[i];
    }

    out << "Sum 1: " << sum1 << "\n";
    out << "Sum 2: " << sum2 << "\n";

    return 0;
}
