#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QList>
#include <QDebug>
#include <compare>
#include <algorithm>

struct Cube {
    int z, x, y;

    friend constexpr std::strong_ordering operator<=>(Cube a, Cube b) = default;
};

struct Brick {
    Cube a, b;

    friend constexpr std::strong_ordering operator<=>(Brick a, Brick b) = default;
};

QList<Brick> dropBricks(QList<Brick> bricks);

int main() {
    QTextStream out(stdout);

    QFile file("input");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal() << "Cannot open input";
        return 1;
    }

    QList<Brick> bricks;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split("~");

        QStringList aParts = parts[0].split(",");
        Cube a = {aParts[2].toInt(), aParts[0].toInt(), aParts[1].toInt()};

        QStringList bParts = parts[1].split(",");
        Cube b = {bParts[2].toInt(), bParts[0].toInt(), bParts[1].toInt()};

        if (a.x > b.x || a.y > b.y || a.z > b.z || a.x < 0 || b.x > 9 || a.y < 0 || b.y > 9) {
            qFatal() << "Invalid input";
            return 1;
        }

        bricks.append({a, b});
    }

    std::sort(bricks.begin(), bricks.end());
    bricks = dropBricks(bricks);
    std::sort(bricks.begin(), bricks.end());

    int safeToRemove = 0;
    int totalFallenBricks = 0;

    for (int i = 0; i < bricks.size(); i++) {
        QList<Brick> testPile = bricks;
        testPile.remove(i);

        QList<Brick> droppedTestPile = dropBricks(testPile);

        int fallenBricks = 0;
        for (int j = 0; j < testPile.size(); j++) {
            if (testPile[j] != droppedTestPile[j]) {
                fallenBricks++;
            }
        }

        safeToRemove += (fallenBricks == 0);
        totalFallenBricks += fallenBricks;
    }

    out << "Safe to remove: " << safeToRemove << "\n";
    out << "Total fallen bricks: " << totalFallenBricks << "\n";

    return 0;
}

QList<Brick> dropBricks(QList<Brick> bricks) {
    QList<QList<int>> heightmap(10, QList<int>(10, 0));

    for (Brick &brick: bricks) {
        int newZ = 0;
        for (int x = brick.a.x; x <= brick.b.x; x++) {
            for (int y = brick.a.y; y <= brick.b.y; y++) {
                newZ = std::max(newZ, heightmap[x][y] + 1);
            }
        }

        brick.b.z -= brick.a.z - newZ;
        brick.a.z = newZ;

        for (int x = brick.a.x; x <= brick.b.x; x++) {
            for (int y = brick.a.y; y <= brick.b.y; y++) {
                heightmap[x][y] = std::max(heightmap[x][y], brick.b.z);
            }
        }
    }

    return bricks;
}
