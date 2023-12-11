#include <QFile>
#include <QTextStream>
#include <QString>
#include <QList>
#include <QDebug>
#include <utility>
#include <algorithm>
#include <cmath>

enum class Direction {
    Rows,
    Columns
};

void multiplyEmptyRowsAndColumns(QList<std::pair<int, int>> &galaxies, int multiplier);
void multiplyEmpty(QList<std::pair<int, int>> &galaxies, Direction direction, int multiplier);
long long totalPathLength(const QList<std::pair<int, int>> &galaxies);

int main(int argc, char *argv[]) {
    QTextStream out(stdout);

    QFile file("input");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal() << "Cannot open input";
        return 1;
    }

    QTextStream in(&file);

    QList<std::pair<int, int>> galaxies;
    int y = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();

        for (int x = 0; x < line.size(); x++) {
            if (line[x] == '#') {
                galaxies.append({x, y});
            }
        }

        y++;
    }

    QList<std::pair<int, int>> galaxiesPart1 = galaxies;
    multiplyEmptyRowsAndColumns(galaxiesPart1, 2);

    QList<std::pair<int, int>> galaxiesPart2 = galaxies;
    multiplyEmptyRowsAndColumns(galaxiesPart2, 1'000'000);

    out << "Path length sum part 1: " << totalPathLength(galaxiesPart1) << "\n";
    out << "Path length sum part 2: " << totalPathLength(galaxiesPart2) << "\n";

    return 0;
}

void multiplyEmptyRowsAndColumns(QList<std::pair<int, int>> &galaxies, int multiplier) {
    multiplyEmpty(galaxies, Direction::Rows, multiplier);

    std::ranges::sort(galaxies);
    multiplyEmpty(galaxies, Direction::Columns, multiplier);
}

void multiplyEmpty(QList<std::pair<int, int>> &galaxies, Direction direction, int multiplier) {
    int increment = 0;
    int last = 0;
    for (auto &[x, y]: galaxies) {
        int &c = (direction == Direction::Rows? y : x);

        if (c != last) {
            increment += (c - last - 1) * (multiplier - 1);
            last = c;
        }

        c += increment;
    }
}

long long totalPathLength(const QList<std::pair<int, int>> &galaxies) {
    long long pathLengthSum = 0;
    for (int i = 0; i < galaxies.size(); i++) {
        for (int j = i + 1; j < galaxies.size(); j++) {
            pathLengthSum += std::abs(galaxies[i].first - galaxies[j].first) + std::abs(galaxies[i].second - galaxies[j].second);
        }
    }

    return pathLengthSum;
}
