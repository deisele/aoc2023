#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QList>
#include <QDebug>

int findFirstReflection(const QStringList &pattern, int distance);
int hammingDistance(const QString &a, const QString &b);
QStringList transposed(const QStringList &pattern);

int main(int argc, char *argv[]) {
    QTextStream out(stdout);

    QFile file("input");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal() << "Cannot open input";
        return 1;
    }

    QTextStream in(&file);

    QList<QStringList> patterns(1);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.isEmpty()) {
            patterns.append(QStringList());
        } else {
            patterns.last().append(line);
        }
    }

    int sumPart1 = 0;
    int sumPart2 = 0;
    for (const QStringList &pattern: patterns) {
        sumPart1 += 100 * findFirstReflection(pattern, 0) + findFirstReflection(transposed(pattern), 0);
        sumPart2 += 100 * findFirstReflection(pattern, 1) + findFirstReflection(transposed(pattern), 1);
    }

    out << "Sum part 1: " << sumPart1 << "\n";
    out << "Sum part 2: " << sumPart2 << "\n";

    return 0;
}

int findFirstReflection(const QStringList &pattern, int distance) {
    for (int i = 1; i < pattern.length(); i++) {
        int a = i - 1;
        int b = i;
        int distanceSum = 0;

        while ((distanceSum += hammingDistance(pattern[a], pattern[b])) <= distance) {
            if (a == 0 || b == pattern.length() - 1) {
                if (distanceSum == distance) {
                    return i;
                }
                break;
            }

            a--;
            b++;
        }
    }

    return 0;
}

int hammingDistance(const QString &a, const QString &b) {
    int dist = 0;

    for (int i = 0; i < a.length(); i++) {
        dist += (a[i] != b[i]);
    }

    return dist;
}

QStringList transposed(const QStringList &pattern) {
    QStringList transposedPattern(pattern[0].length());

    for (const QString &line: pattern) {
        for (int i = 0; i < line.length(); i++) {
            transposedPattern[i].append(line[i]);
        }
    }

    return transposedPattern;
}
