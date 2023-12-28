#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QList>
#include <QMap>
#include <QDebug>
#include <utility>
#include <algorithm>

struct Rule {
    char var;
    char relation;
    int number;

    QString matchAction;
};

struct Workflow {
    QList<Rule> rules;
    QString defaultAction;
};

long long countAcceptedCombinations(const QMap<QString, Workflow> &workflows, QMap<char, std::pair<int, int>> machinePartRange, QString action);
bool splitRange(std::pair<int, int> &range, char relation, int number, bool match);

int main(int argc, char *argv[]) {
    QTextStream out(stdout);

    QFile file("input");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal() << "Cannot open input";
        return 1;
    }

    QTextStream in(&file);

    QMap<QString, Workflow> workflows;

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.isEmpty()) {
            break;
        }

        QStringList partsA = line.split('{');

        QString name = partsA[0];
        QStringList rules = partsA[1].chopped(1).split(',');

        Workflow workflow;
        workflow.defaultAction = rules.takeLast();

        for (const QString &rule: rules) {
            QStringList partsB = rule.split(':');
            char relation = (partsB[0].contains('<')? '<' : '>');
            QStringList partsC = partsB[0].split(relation);

            workflow.rules.append({partsC[0][0].toLatin1(), relation, partsC[1].toInt(), partsB[1]});
        }

        workflows[partsA[0]] = workflow;
    }

    QList<QMap<char, int>> machineParts;
    while (!in.atEnd()) {
        QString line = in.readLine();

        QMap<char, int> machinePart;
        QStringList assignments = line.mid(1).chopped(1).split(',');
        for (const QString &assignment: assignments) {
            QStringList parts = assignment.split('=');

            machinePart[parts[0][0].toLatin1()] = parts[1].toInt();
        }

        machineParts.append(machinePart);
    }

    int sum = 0;

    for (const QMap<char, int> &machinePart: machineParts) {
        QMap<char, std::pair<int, int>> machinePartRange;
        for (auto [var, value]: machinePart.asKeyValueRange()) {
            machinePartRange[var] = {value, value};
        }

        if (countAcceptedCombinations(workflows, machinePartRange, "in") == 1) {
            for (int val: machinePart.values()) {
                sum += val;
            }
        }
    }

    out << "Sum part 1: " << sum << "\n";

    QMap<char, std::pair<int, int>> machinePartRangeAll;
    for (char var: {'x', 'm', 'a', 's'}) {
        machinePartRangeAll[var] = {1, 4000};
    }
    out << "Accepted combinations (part 2): " << countAcceptedCombinations(workflows, machinePartRangeAll, "in") << "\n";

    return 0;
}

long long countAcceptedCombinations(const QMap<QString, Workflow> &workflows, QMap<char, std::pair<int, int>> machinePartRange, QString action) {
    if (action == "R") {
        return 0;
    }

    if (action == "A") {
        long long product = 1;
        for (auto [lo, hi]: machinePartRange.values()) {
            product *= hi - lo + 1;
        }
        return product;
    }

    long long sum = 0;

    const Workflow &workflow = workflows.value(action);
    for (const Rule &rule: workflow.rules) {
        QMap<char, std::pair<int, int>> matchedMachinePartRange = machinePartRange;
        bool matchedRangeValid = splitRange(matchedMachinePartRange[rule.var], rule.relation, rule.number, true);

        if (matchedRangeValid) {
            sum += countAcceptedCombinations(workflows, matchedMachinePartRange, rule.matchAction);
        }

        bool rangeValid = splitRange(machinePartRange[rule.var], rule.relation, rule.number, false);
        if (!rangeValid) {
            return sum;
        }
    }

    sum += countAcceptedCombinations(workflows, machinePartRange, workflow.defaultAction);

    return sum;
}

bool splitRange(std::pair<int, int> &range, char relation, int number, bool match) {
    auto &[lo, hi] = range;

    if (match) {
        if (relation == '<') {
            hi = std::min(hi, number - 1);
        } else {
            lo = std::max(lo, number + 1);
        }
    } else {
        if (relation == '<') {
            lo = std::max(lo, number);
        } else {
            hi = std::min(hi, number);
        }
    }

    return hi - lo >= 0;
}
