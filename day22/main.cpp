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

    QTextStream in(&file);

    QList<Brick> bricks;

    while (!in.atEnd()) {
        QString line = in.readLine();

        QStringList parts = line.split("~");

        QStringList aParts = parts[0].split(",");
        Cube a = {aParts[2].toInt(), aParts[0].toInt(), aParts[1].toInt()};

        QStringList bParts = parts[1].split(",");
        Cube b = {bParts[2].toInt(), bParts[0].toInt(), bParts[1].toInt()};

        if (a.z > b.z) {
            std::swap(a, b);
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
    for (Brick &brick: bricks) {
        int minZ = 1;

        for (const Brick &other: bricks) {
            if (other.a.z < brick.a.z && other.b.x >= brick.a.x && other.a.x <= brick.b.x && other.b.y >= brick.a.y && other.a.y <= brick.b.y) {
                minZ = std::max(minZ, other.b.z + 1);
            }
        }

        int diff = brick.a.z - minZ;
        brick.a.z -= diff;
        brick.b.z -= diff;
    }

    return bricks;
}
