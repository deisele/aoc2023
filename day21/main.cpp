#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QList>
#include <QQueue>
#include <QPoint>
#include <QDebug>
#include <utility>

int main() {
    QTextStream out(stdout);

    QFile file("input");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal() << "Cannot open input";
        return 1;
    }

    QTextStream in(&file);

    QStringList grid;
    QPoint start;

    while (!in.atEnd()) {
        QString line = in.readLine();

        int x = line.indexOf('S');
        if (x != -1) {
            start = {x, (int)grid.length()};
        }

        grid.append(line);
    }

    int gridSize = grid.length();

    QList<QPoint> directions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    QQueue<QPoint> queue;
    queue.enqueue(start);
    grid[start.y()][start.x()] = '+';

    while (!queue.isEmpty()) {
        QPoint current = queue.dequeue();

        for (QPoint direction: directions) {
            QPoint next = current + direction;

            if ((0 <= next.x() && next.x() < gridSize && 0 <= next.y() && next.y() < gridSize) && grid[next.y()][next.x()] == '.') {
                grid[next.y()][next.x()] = '+';
                queue.enqueue(next);
            }
        }
    }

    std::pair<int, int> diamondA = {0, 0};
    int diamondB = 0;

    for (int y = 0; y < gridSize; y++) {
        for (int x = 0; x < gridSize; x++) {
            if (grid[y][x] != '+') {
                if ((QPoint(x, y) - start).manhattanLength() <= gridSize / 2) {
                    ((x + y) % 2? diamondA.second : diamondA.first)++;
                } else {
                    diamondB++;
                }
            }
        }
    }

    for (long long steps: {64, 26501365}) {
        long long n = steps / gridSize;
        bool parity = steps % 2;

        long long reachablePlots = (steps + 1) * (steps + 1)
                                   - ((n & ~1) + 1) * ((n & ~1) + 1) * (parity? diamondA.second : diamondA.first)
                                   - ((n + 1) & ~1) * ((n + 1) & ~1) * (parity? diamondA.first : diamondA.second)
                                   - n * (n + 1) * diamondB;

        out << "Plots reachable within exactly " << steps << " steps: " << reachablePlots << "\n";
    }

    return 0;
}
