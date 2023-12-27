#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QList>
#include <QQueue>
#include <QDebug>
#include <compare>
#include <tuple>

struct Vec2 {
    int x = 0;
    int y = 0;

    Vec2 rotated90(bool clockwise) {
        return clockwise? Vec2(-y, x) : Vec2(y, -x);
    }

    friend std::strong_ordering operator<=>(Vec2 a, Vec2 b) = default;

    friend Vec2 operator+(Vec2 a, Vec2 b) {
        return {a.x + b.x, a.y + b.y};
    }

    friend Vec2 operator-(Vec2 a, Vec2 b) {
        return {a.x - b.x, a.y - b.y};
    }

    Vec2 &operator+=(Vec2 b) {
        *this = *this + b;
        return *this;
    }

    Vec2 &operator-=(Vec2 b) {
        *this = *this - b;
        return *this;
    }

    friend Vec2 operator-(Vec2 a) {
        return {-a.x, -a.y};
    }
};

const Vec2 up = {0, -1};
const Vec2 down = {0, 1};
const Vec2 left = {-1, 0};
const Vec2 right = {1, 0};

const QList<Vec2> directions = {up, down, left, right};

int main(int argc, char *argv[]) {
    QTextStream out(stdout);

    QFile file("input");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal() << "Cannot open input";
        return 1;
    }

    QTextStream in(&file);

    QStringList grid;
    while (!in.atEnd()) {
        grid.append(in.readLine());
    }

    int width = grid[0].length();
    int height = grid.length();

    for (auto [part, maxJoin, maxStraight]: QList<std::tuple<int, int, int>>{{1, 3, 3}, {2, 6, 10}}) {
        QList<QList<QList<QList<int>>>> bestDistances(height, QList<QList<QList<int>>>(width, QList<QList<int>>(4, QList<int>(maxStraight + 1, -1))));

        QQueue<std::tuple<Vec2, Vec2, int, int>> update;
        update.enqueue({{width - 1, height - 1}, down, maxStraight, 0});
        update.enqueue({{width - 1, height - 1}, right, maxStraight, 0});

        while (!update.empty()) {
            auto [pos, direction, straightCount, distance] = update.dequeue();

            if (!(0 <= pos.x && pos.x < width && 0 <= pos.y && pos.y < height)) {
                continue;
            }

            int &bestDistance = bestDistances[pos.y][pos.x][directions.indexOf(direction)][straightCount];
            if (distance >= bestDistance && bestDistance != -1) {
                continue;
            }

            bestDistance = distance;

            int cellDistance = grid[pos.y][pos.x].toLatin1() - '0';

            if (straightCount > 0) {
                update.enqueue({pos - direction, direction, straightCount - 1, distance + cellDistance});
            }

            if (straightCount <= maxJoin) {
                for (bool clockwise: {false, true}) {
                    update.enqueue({pos - direction.rotated90(clockwise), direction.rotated90(clockwise), maxStraight - 1, distance + cellDistance});
                }
            }
        }

        int bestDistance = -1;
        for (const QList<int> &distances: bestDistances[0][0]) {
            for (int distance: distances.first(maxJoin + 1)) {
                if ((distance != -1 && distance < bestDistance) || bestDistance == -1) {
                    bestDistance = distance;
                }
            }
        }

        out << "Minimum heat loss part " << part << ": " << bestDistance << "\n";
    }

    return 0;
}
