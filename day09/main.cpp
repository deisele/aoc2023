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

    int sum1 = 0;
    int sum2 = 0;

    while (!in.atEnd()) {
        QStringList parts = in.readLine().split(" ");

        QList<int> input;
        foreach (QString part, parts) {
            input.append(part.toInt());
        }

        QList<QList<int>> differences;
        differences.append(input);

        while (differences.last().count() > 0) {
            const QList<int> &curr = differences.last();
            QList<int> diff;

            for (int i = 0; i < curr.count() - 1; i++) {
                diff.append(curr[i + 1] - curr[i]);
            }

            differences.append(diff);
        }

        // extrapolate forwards
        differences.last().append(0);
        for (int i = differences.count() - 2; i >= 0; i--) {
            differences[i].append(differences[i].last() + differences[i + 1].last());
        }

        // extrapolate backwards
        differences.last().prepend(0);
        for (int i = differences.count() - 2; i >= 0; i--) {
            differences[i].prepend(differences[i].first() - differences[i + 1].first());
        }

        sum1 += differences[0].last();
        sum2 += differences[0].first();
    }

    out << "Sum 1: " << sum1 << "\n";
    out << "Sum 2: " << sum2 << "\n";

    return 0;
}
