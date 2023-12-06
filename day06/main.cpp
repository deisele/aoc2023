#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QList>
#include <QDebug>

int main(int argc, char *argv[]) {
    QTextStream out(stdout);

    QFile file("input");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal() << "Cannot open input";
        return 1;
    }

    QTextStream in(&file);

    bool part1 = false;
    if (part1) {
        QList<int> times, distances;
        for (QList<int> *data: {&times, &distances}) {
            QList<QString> line = in.readLine().simplified().split(" ");
            line.removeFirst();

            for (const QString &numStr: line) {
                data->append(numStr.toInt());
            }
        }

        int winProduct = 1;
        for (int i = 0; i < times.count(); i++) {
            int wins = 0;
            for (int buttonHoldTime = 0; buttonHoldTime <= times[i]; buttonHoldTime++) {
                int distance = (times[i] - buttonHoldTime) * buttonHoldTime;
                if (distance > distances[i]) {
                    wins++;
                }
            }

            winProduct *= wins;
        }

        out << "Win product: " << winProduct << "\n";
    } else {
        long long int recordTime, recordDistance;
        for (long long int *data: {&recordTime, &recordDistance}) {
            *data = in.readLine().remove(" ").split(":")[1].toLongLong();
        }

        long long int wins = 0;
        for (long long int buttonHoldTime = 0; buttonHoldTime <= recordTime; buttonHoldTime++) {
            long long int distance = (recordTime - buttonHoldTime) * buttonHoldTime;
            if (distance > recordDistance) {
                wins++;
            }
        }

        out << "Wins: " << wins << "\n";
    }

    return 0;
}
