#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QStringView>
#include <QList>
#include <QMap>
#include <QDebug>
#include <utility>

long long recursiveSearch(QString springs, QList<int> blocks);

int main(int argc, char *argv[]) {
    QTextStream out(stdout);

    QFile file("input");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal() << "Cannot open input";
        return 1;
    }

    QTextStream in(&file);

    long long sum_part1 = 0;
    long long sum_part2 = 0;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(' ');

        QString springs = parts[0];

        QList<int> blocks;
        for (const QString &part: parts[1].split(',')) {
            blocks.append(part.toInt());
        }

        QString springsX5 = springs;
        QList<int> blocksX5 = blocks;
        for (int i = 0; i < 4; i++) {
            springsX5.append("?" + springs);
            blocksX5.append(blocks);
        }

        sum_part1 += recursiveSearch(springs, blocks);;
        sum_part2 += recursiveSearch(springsX5, blocksX5);
    }

    out << "Sum part 1: " << sum_part1 << "\n";
    out << "Sum part 2: " << sum_part2 << "\n";

    return 0;
}

long long recursiveSearch(QString springs, QList<int> blocks) {
    static QMap<std::pair<QString, QList<int>>, long long> memo;

    if (memo.contains({springs, blocks})) {
        return memo[{springs, blocks}];
    }

    QString springsOrig = springs;
    QList<int> blocksOrig = blocks;

    long long combinations;

    if (blocks.isEmpty()) {
        combinations = !springs.contains('#');
    } else {
        while (!springs.isEmpty() && springs.front() == '.') {
            springs.removeFirst();
        }

        int blockToAllocate = blocks.takeFirst();

        int last = springs.indexOf('#');
        if (last == -1 || last > springs.length() - blockToAllocate) {
            last = springs.length() - blockToAllocate;
        }

        combinations = 0;
        for (int i = 0; i <= last; i++) {
            if (springs.sliced(i, blockToAllocate).contains('.')) {
                continue;
            }

            bool notAtEnd = (springs.length() - i - blockToAllocate > 0);
            if (notAtEnd && springs[i + blockToAllocate] == '#') {
                continue;
            }

            combinations += recursiveSearch(springs.last(springs.length() - i - blockToAllocate - notAtEnd), blocks);
        }
    }

    memo.insert({springsOrig, blocksOrig}, combinations);

    return combinations;
}
