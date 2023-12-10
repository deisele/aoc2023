#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QList>
#include <QHash>
#include <QDebug>
#include <compare>

struct Vec2 {
    int x = 0;
    int y = 0;

    Vec2 rotated90(bool clockwise) {
        return clockwise? Vec2(-y, x) : Vec2(y, -x);
    }

    void rotate90(bool clockwise) {
        *this = rotated90(clockwise);
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

    friend size_t qHash(Vec2 v, size_t seed) {
        return qHashMulti(seed, v.x, v.x);
    }
};

int main(int argc, char *argv[]) {
    QTextStream out(stdout);

    QFile file("input");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal() << "Cannot open input";
        return 1;
    }

    QTextStream in(&file);

    QList<QString> maze;
    while (!in.atEnd()) {
        QString line = "." + in.readLine() + ".";
        maze.append(line);
    }

    int width = maze[0].length();

    QString padLine(width, '.');
    maze.prepend(padLine);
    maze.append(padLine);

    int height = maze.length();

    Vec2 up = {0, -1};
    Vec2 down = {0, 1};
    Vec2 left = {-1, 0};
    Vec2 right = {1, 0};

    QList<Vec2> directions = {up, down, left, right};

    QMap<QChar, QList<Vec2>> pipeSegmentTypes = {
        {'|', {up, down}},
        {'-', {left, right}},
        {'L', {up, right}},
        {'J', {left, up}},
        {'7', {left, down}},
        {'F', {down, right}}
    };

    // find start
    Vec2 start;
    for (int y = 0; y < height; y++) {
        int x = maze[y].indexOf('S');
        if (x != -1) {
            start = {x, y};
            break;
        }
    }

    // try all directions until we find a valid path that loops back to start
    QList<Vec2> path;
    for (Vec2 dir: directions) {
        path = {start};

        Vec2 pos = start + dir;

        while (pos != start) {
            QChar symbol = maze[pos.y][pos.x];
            if (!pipeSegmentTypes.contains(symbol)) {
                break;
            }

            QList<Vec2> connectedDirections = pipeSegmentTypes[symbol];
            int inDirectionIdx = connectedDirections.indexOf(-dir);
            if (inDirectionIdx == -1) {
                break;
            }
            dir = connectedDirections[!inDirectionIdx];

            path.append(pos);
            pos += dir;
        }

        if (pos == start) {
            break;
        }
    }

    // fill in the correct symbol for the start
    for (auto [symbol, connectedDirections]: pipeSegmentTypes.asKeyValueRange()) {
        QList<Vec2> prevAndNext = {path.last(), path[1]};
        if (prevAndNext.contains(start + connectedDirections[0]) && prevAndNext.contains(start + connectedDirections[1])) {
            maze[start.y][start.x] = symbol;
            break;
        }
    }

    // convert path to set for faster lookup
    QSet<Vec2> pointsOnPath(path.begin(), path.end());

    int pointsInside = 0;
    for (int y = 0; y < height; y++) {
        bool inside = false;

        for (int x = 0; x < width; x++) {
            if (pointsOnPath.contains({x, y})) {
                if (pipeSegmentTypes[maze[y][x]].contains(up)) {
                    inside = !inside;
                }

                continue;
            }

            if (inside) {
                pointsInside++;
            }
        }
    }

    out << "Farthest distance: " << path.length() / 2 << "\n";
    out << "Points inside: " << pointsInside << "\n";

    return 0;
}
