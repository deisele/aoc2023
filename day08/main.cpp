#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QList>
#include <QMap>
#include <QDebug>
#include <utility>
#include <cmath>

long long leastCommonMultiple(const QList<int> &numbers);
QList<std::pair<int, int>> factorize(int n);

int main(int argc, char *argv[]) {
    QTextStream out(stdout);

    QFile file("input");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal() << "Cannot open input";
        return 1;
    }

    QTextStream in(&file);

    QString directions = in.readLine();
    in.readLine();

    QMap<QString, std::pair<QString, QString>> paths;
    while (!in.atEnd()) {
        QString line = in.readLine();

        line.remove("=");
        line.remove(",");
        line.remove("(");
        line.remove(")");

        QStringList parts = line.simplified().split(" ");

        paths.insert(parts[0], {parts[1], parts[2]});
    }

    int stepsPart1 = 0;

    QString position = "AAA";
    for (int i = 0; position != "ZZZ"; i = (i + 1) % directions.size()) {
        stepsPart1++;
        position = (directions[i] == 'L'? paths[position].first : paths[position].second);
    }

    QStringList startingPositions;
    for (const QString &pos: paths.keys()) {
        if (pos.endsWith("A")) {
            startingPositions.append(pos);
        }
    }

    QList<int> stepsPart2;

    for (QString position: startingPositions) {
        int steps = 0;

        for (int i = 0; !position.endsWith("Z"); i = (i + 1) % directions.size()) {
            steps++;
            position = (directions[i] == 'L'? paths[position].first : paths[position].second);
        }

        stepsPart2.append(steps);
    }

    out << "Part 1: " << stepsPart1 << "\n";
    out << "Part 2: " << leastCommonMultiple(stepsPart2) << "\n";

    return 0;
}

long long leastCommonMultiple(const QList<int> &numbers) {
    QMap<int, int> primeFactors;

    for (int num: numbers) {
        for (auto [prime, count]: factorize(num)) {
            primeFactors[prime] = std::max(primeFactors[prime], count);
        }
    }

    long long lcm = 1;
    for (int prime: primeFactors.keys()) {
        lcm *= std::pow((long long)prime, (long long)primeFactors[prime]);
    }

    return lcm;
}

QList<std::pair<int, int>> factorize(int n) {
    QList<std::pair<int, int>> factors;

    for (int i = 2; i <= n; i++) {
        int count = 0;

        while (n % i == 0) {
            count++;
            n /= i;
        }

        if (count > 0) {
            factors.append({i, count});
        }
    }

    return factors;
}
