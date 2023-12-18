#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QList>
#include <QQueue>
#include <QDebug>
#include <utility>
#include <compare>

struct Vec2 {
    int x = 0;
    int y = 0;

    int &get(int i) {
        return (i == 0? x : y);
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

int numEnergizedTiles(const QStringList &grid, Vec2 startingPosition, Vec2 startingDirection);

Vec2 up = {0, -1};
Vec2 down = {0, 1};
Vec2 left = {-1, 0};
Vec2 right = {1, 0};

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

    out << "Energized tiles part 1: " << numEnergizedTiles(grid, Vec2(0, 0), right) << "\n";

    int maxEnergizedTiles = 0;
    for (Vec2 startingDirection: {up, down, left, right}) {
        int startingDirectionAxis = (startingDirection.x == 0);

        for (int i = 0; i < (startingDirectionAxis == 1? width : height); i++) {
            Vec2 startingPosition;
            startingPosition.get(startingDirectionAxis) = (startingDirection.get(startingDirectionAxis) == -1? (startingDirectionAxis == 0? width : height) - 1 : 0);
            startingPosition.get(!startingDirectionAxis) = i;

            int energizedTiles = numEnergizedTiles(grid, startingPosition, startingDirection);
            if (energizedTiles > maxEnergizedTiles) {
                maxEnergizedTiles = energizedTiles;
            }
        }
    }

    out << "Energized tiles part 2: " << maxEnergizedTiles << "\n";

    return 0;
}

int numEnergizedTiles(const QStringList &grid, Vec2 startingPosition, Vec2 startingDirection) {
    int width = grid[0].length();
    int height = grid.length();

    QList<QList<QList<Vec2>>> visited(height, QList<QList<Vec2>>(width));

    QQueue<std::pair<Vec2, Vec2>> queue;
    queue.enqueue({startingPosition, startingDirection});

    int energizedTiles = 0;

    while (!queue.isEmpty()) {
        auto [pos, direction] = queue.dequeue();

        while (0 <= pos.x && pos.x < width && 0 <= pos.y && pos.y < height && !visited[pos.y][pos.x].contains(direction)) {
            energizedTiles += visited[pos.y][pos.x].isEmpty();

            visited[pos.y][pos.x].append(direction);

            QChar c = grid[pos.y][pos.x];
            if (c == '/' || c == '\\') {
                std::swap(direction.x, direction.y);
                if (c == '/') {
                    direction = -direction;
                }
            } else if ((c == '-' && direction.y != 0) || (c == '|' && direction.x != 0)) {
                std::swap(direction.x, direction.y);
                queue.enqueue({pos - direction, -direction});
            }

            pos += direction;
        }
    }

    return energizedTiles;
}
