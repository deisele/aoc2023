#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QList>
#include <QMap>
#include <QDebug>
#include <cstdint>

uint8_t hash(const QString &str);

int main(int argc, char *argv[]) {
    QTextStream out(stdout);

    QFile file("input");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal() << "Cannot open input";
        return 1;
    }

    QTextStream in(&file);

    if (in.atEnd()) {
        qFatal() << "Cannot read line";
        return 1;
    }

    QString line = in.readLine();
    QStringList steps = line.split(',');

    QMap<QString, int> lensToFocalLength;
    QList<QList<QString>> boxes(256);

    int sumPart1 = 0;
    for (const QString &step: steps) {
        sumPart1 += hash(step);

        char operation = (step.contains('=')? '=' : '-');
        QStringList parts = step.split(operation);
        QString label = parts[0];
        QList<QString> &box = boxes[hash(label)];

        if (operation == '-') {
            box.removeOne(label);
        } else {
            int focalLength = parts[1].toInt();
            lensToFocalLength[label] = focalLength;

            if (!box.contains(label)) {
                box.append(label);
            }
        }
    }

    int sumPart2 = 0;
    for (int boxNumber = 0; boxNumber < boxes.length(); boxNumber++) {
        const QList<QString> &box = boxes[boxNumber];

        for (int lensIndex = 0; lensIndex < box.length(); lensIndex++) {
            const QString &label = box[lensIndex];

            sumPart2 += (boxNumber + 1) * (lensIndex + 1) * lensToFocalLength[label];
        }
    }

    out << "Sum part 1: " << sumPart1 << "\n";
    out << "Sum part 2: " << sumPart2 << "\n";

    return 0;
}

uint8_t hash(const QString &str) {
    uint8_t result = 0;

    for (QChar c: str) {
        result += c.toLatin1();
        result *= 17;
    }

    return result;
}
