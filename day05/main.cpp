#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QList>
#include <QDebug>
#include <limits>

struct RangeMap {
    long long int rangeFromStart;
    long long int rangeToStart;
    long long int rangeLen;
};

long long int applyMap(const QList<QList<RangeMap>> &mapList, long long int num);

int main(int argc, char *argv[]) {
    QTextStream out(stdout);

    QFile file("input");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal() << "Cannot open input";
        return 1;
    }

    QTextStream in(&file);

    QList<long long int> seeds;
    for (const QString &seed: in.readLine().split(":")[1].simplified().split(" ")) {
        seeds.append(seed.toLongLong());
    }
    in.readLine();

    QList<QList<RangeMap>> mapList;

    QList<RangeMap> ranges;

    while (!in.atEnd()) {
        QString line = in.readLine();

        if (line.contains(":")) {
            ranges.clear();
            continue;
        }

        if (!line.isEmpty()) {
            QStringList parts = line.split(" ");
            RangeMap rangeMap = { .rangeFromStart = parts[1].toLongLong(),
                                  .rangeToStart = parts[0].toLongLong(),
                                  .rangeLen = parts[2].toLongLong()
                                };

            ranges.append(rangeMap);
        }

        if (line.isEmpty() || in.atEnd()) {
            mapList.append(ranges);
            continue;
        }
    }

    long long int closestLocation1 = std::numeric_limits<long long int>::max();
    for (long long int seed: seeds) {
        long long int location = applyMap(mapList, seed);
        if (location < closestLocation1) {
            closestLocation1 = location;
        }
    }

    long long int closestLocation2 = std::numeric_limits<long long int>::max();
    for (int i = 0; i < seeds.count(); i += 2) {
        for (long long int seed = seeds[i]; seed < seeds[i] + seeds[i + 1]; seed++) {
            long long int location = applyMap(mapList, seed);
            if (location < closestLocation2) {
                closestLocation2 = location;
            }
        }
    }

    out << "Closest location 1: " << closestLocation1 << "\n";
    out << "Closest location 2: " << closestLocation2 << "\n";

    return 0;
}

long long int applyMap(const QList<QList<RangeMap>> &mapList, long long int num) {
    for (const QList<RangeMap> &ranges: mapList) {
        for (const RangeMap &rangeMap: ranges) {
            if (rangeMap.rangeFromStart <= num && num < rangeMap.rangeFromStart + rangeMap.rangeLen) {
                num = rangeMap.rangeToStart + (num - rangeMap.rangeFromStart);
                break;
            }
        }
    }

    return num;
}
