#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QList>
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

    // mark path
    for (Vec2 pos: path) {
        maze[pos.y][pos.x] = 'X';
    }

    // find out if the loop is clockwise or counter-clockwise
    int netRightTurns = 0;
    for (int i = 0; i < path.length() - 1; i++) {
        Vec2 posPrev = path[(i - 1 + path.length()) % path.length()];
        Vec2 posCurr = path[i];
        Vec2 posNext = path[(i + 1) % path.length()];

        Vec2 inDir = posCurr - posPrev;
        Vec2 outDir = posNext - posCurr;

        // cross product
        netRightTurns += inDir.x * outDir.y - inDir.y * outDir.x;
    }

    bool clockwise = netRightTurns > 0;

    // mark and count all points inside the loop
    int pointsInside = 0;
    for (int i = 0; i < path.length() - 1; i++) {
        Vec2 posPrev = path[(i - 1 + path.length()) % path.length()];
        Vec2 posCurr = path[i];
        Vec2 posNext = path[(i + 1) % path.length()];

        Vec2 inDir = posCurr - posPrev;
        Vec2 outDir = posNext - posCurr;

        Vec2 paintDir = inDir.rotated90(clockwise);
        while (paintDir != outDir) {
            Vec2 pos = posCurr + paintDir;
            while (maze[pos.y][pos.x] != 'X') {
                // don't double count
                if (maze[pos.y][pos.x] != 'I') {
                    maze[pos.y][pos.x] = 'I';
                    pointsInside++;
                }

                pos += paintDir;
            }

            paintDir.rotate90(!clockwise);
        }
    }

    out << "Farthest distance: " << path.length() / 2 << "\n";
    out << "Points inside: " << pointsInside << "\n";

    return 0;
}
