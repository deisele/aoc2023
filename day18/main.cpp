#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QList>
#include <QMap>
#include <QPoint>
#include <QDebug>
#include <set>
#include <algorithm>

bool operator<(QPoint a, QPoint b) {
    if (a.x() != b.x()) {
        return a.x() < b.x();
    }

    return a.y() < b.y();
}

int main(int argc, char *argv[]) {
    QTextStream out(stdout);

    QFile file("input");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal() << "Cannot open input";
        return 1;
    }

    QTextStream in(&file);

    QPoint up = {0, -1};
    QPoint down = {0, 1};
    QPoint left = {-1, 0};
    QPoint right = {1, 0};

    QList<QPoint> directions = {right, down, left, up};

    struct DigStep {
        QPoint direction;
        int length;
    };
    QList<QList<DigStep>> digPlans(2);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(' ');

        QPoint directionPart1 = directions[QList<char>{'R', 'D', 'L', 'U'}.indexOf(parts[0][0].toLatin1())];
        int lengthPart1 = parts[1].toInt();

        QPoint directionPart2 = directions[parts[2].sliced(7, 1).toInt()];
        int lengthPart2 = parts[2].sliced(2, 5).toInt(nullptr, 16);

        digPlans[0].append({directionPart1, lengthPart1});
        digPlans[1].append({directionPart2, lengthPart2});
    }

    for (int i = 0; i < digPlans.length(); i++) {
        const QList<DigStep> &digPlan = digPlans[i];

        QPoint topLeft;
        QPoint bottomRight;
        int trenchLength = 0;

        QPoint pos;
        for (DigStep step: digPlan) {
            pos += step.direction * step.length;

            topLeft.setX(std::min(pos.x(), topLeft.x()));
            topLeft.setY(std::min(pos.y(), topLeft.y()));

            bottomRight.setX(std::max(pos.x(), bottomRight.x()));
            bottomRight.setY(std::max(pos.y(), bottomRight.y()));

            trenchLength += step.length;
        }

        QMap<std::set<QPoint>, char> trenchSegmentTypes = {
            {{up, down}   , '|'},
            {{left, right}, '-'},
            {{up, right}  , 'L'},
            {{left, up}   , 'J'},
            {{left, down} , '7'},
            {{down, right}, 'F'}
        };

        QList<QMap<int, char>> grid(bottomRight.y() - topLeft.y() + 1);

        pos = {0, 0};
        QPoint oldDirection = -digPlan.last().direction;
        for (DigStep step: digPlan) {
            grid[pos.y() - topLeft.y()][pos.x()] = trenchSegmentTypes[{oldDirection, step.direction}];
            oldDirection = -step.direction;
            for (int i = 0; i < step.length - 1; i++) {
                pos += step.direction;
                grid[pos.y() - topLeft.y()][pos.x()] = trenchSegmentTypes[{oldDirection, step.direction}];
            }
            pos += step.direction;
        }

        long long volume = trenchLength;
        for (int y = topLeft.y(); y <= bottomRight.y(); y++) {
            bool inside = false;
            int lastX = 0;
            for (auto [x, cell]: grid[y - topLeft.y()].asKeyValueRange()) {
                int gap = x - lastX - 1;
                if (inside && gap > 0) {
                    volume += gap;
                }
                lastX = x;

                if (cell == '|' || cell == 'L' || cell == 'J') {
                    inside = !inside;
                }
            }
        }

        out << "Volume part " << i + 1 << ": " << volume << "\n";
    }

    return 0;
}
