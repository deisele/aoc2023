#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QList>
#include <QMap>
#include <QDebug>
#include <compare>

struct Vec2 {
    int x = 0;
    int y = 0;

    int get(int i) const {
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
};

void slide(QStringList &platform, Vec2 direction);
int calculateWeight(QStringList &platform, Vec2 direction);

const Vec2 up = {0, -1};
const Vec2 down = {0, 1};
const Vec2 left = {-1, 0};
const Vec2 right = {1, 0};

int main(int argc, char *argv[]) {
    QTextStream out(stdout);

    QFile file("input");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal() << "Cannot open input";
        return 1;
    }

    QTextStream in(&file);

    QStringList platform;
    while (!in.atEnd()) {
        platform.append(in.readLine());
    }

    slide(platform, up);
    out << "Weight part 1: " << calculateWeight(platform, up) << "\n";

    QMap<QStringList, int> visitedPlatforms;
    int cycles = 0;
    while (!visitedPlatforms.contains(platform)) {
        visitedPlatforms[platform] = cycles++;

        for (Vec2 direction: {up, left, down, right}) {
            slide(platform, direction);
        }
    }

    int loopStart = visitedPlatforms[platform];
    int loopLength = cycles - loopStart;
    int finalPlatformIndex = loopStart + (1'000'000'000 - cycles) % loopLength;
    QStringList finalPlatform = visitedPlatforms.key(finalPlatformIndex);

    out << "Weight part 2: " << calculateWeight(finalPlatform, up) << "\n";

    return 0;
}

void slide(QStringList &platform, Vec2 direction) {
    int width = platform[0].length();
    int height = platform.length();

    for (int y = (direction == down? height - 1 : 0); 0 <= y && y < height; y += (direction == down? -1 : 1)) {
        for (int x = (direction == right? width - 1 : 0); 0 <= x && x < width; x += (direction == right? -1 : 1)) {
            if (platform[y][x] == 'O') {
                platform[y][x] = '.';

                Vec2 slidePos(x, y);
                while (0 <= slidePos.x && slidePos.x < width && 0 <= slidePos.y && slidePos.y < height && platform[slidePos.y][slidePos.x] == '.') {
                    slidePos += direction;
                }
                slidePos -= direction;

                platform[slidePos.y][slidePos.x] = 'O';
            }
        }
    }
}

int calculateWeight(QStringList &platform, Vec2 direction) {
    int width = platform[0].length();
    int height = platform.length();

    int axis = (direction.x != 0? 0 : 1);
    int flipFactor = direction.get(axis);
    int flipOffset = (flipFactor == -1? Vec2(width, height).get(axis) : 0);

    int weight = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (platform[y][x] == 'O') {
                weight += Vec2(x, y).get(axis) * flipFactor + flipOffset;
            }
        }
    }

    return weight;
}
