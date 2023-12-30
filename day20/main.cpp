#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QList>
#include <QMap>
#include <QQueue>
#include <QDebug>
#include <utility>
#include <tuple>
#include <algorithm>
#include <cmath>

struct Node {
    char type = '\0';
    QStringList destinations;
    bool flipFlopState = false;
    QMap<QString, bool> lastInputs;
};

static long long leastCommonMultiple(const QList<int> &numbers);
static QList<std::pair<int, int>> factorize(int n);

int main(int argc, char *argv[]) {
    QTextStream out(stdout);

    QFile file("input");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal() << "Cannot open input";
        return 1;
    }

    QTextStream in(&file);

    QMap<QString, Node> nodes;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(" -> ");
        QString nodeName = parts[0];

        char firstChar = nodeName[0].toLatin1();
        if (firstChar == '%' || firstChar == '&') {
            nodeName.removeFirst();
            nodes[nodeName].type = firstChar;
        }

        nodes[nodeName].destinations = parts[1].split(", ");

        for (const QString &destination: nodes[nodeName].destinations) {
            nodes[destination].lastInputs[nodeName] = false;
        }
    }

    int pulseCountLow = 0;
    int pulseCountHigh = 0;

    for (int i = 0; i < 1000; i++) {
        QQueue<std::tuple<QString, QString, bool>> signalQueue;
        signalQueue.enqueue({"broadcaster", "button", false});

        while (!signalQueue.isEmpty()) {
            auto [destination, from, signal] = signalQueue.dequeue();

            //out << from << " -" << (signal? "high" : "low") << "-> " << destination << "\n";

            (signal? pulseCountHigh : pulseCountLow)++;

            Node &node = nodes[destination];

            if (node.type == '%') {
                if (signal) {
                    continue;
                }

                node.flipFlopState = !node.flipFlopState;
                signal = node.flipFlopState;
            } else if (node.type == '&') {
                node.lastInputs[from] = signal;

                signal = true;
                for (bool lastInput: node.lastInputs.values()) {
                    signal &= lastInput;
                }
                signal = !signal;
            }

            for (const QString &nextDestination: node.destinations) {
                signalQueue.enqueue({nextDestination, destination, signal});
            }
        }
    }

    out << "Pulse count product: " << pulseCountLow * pulseCountHigh << "\n";

    /*
    Visualizing the module graph with graphviz reveals, that it consists of four clusters, each implementing a 12 bit binary counter.
    All counters are hooked up in parallel to the broadcaster, so they all get incremented each time a low pulse is sent to the broadcaster.
    Every counter is coupled to a conjunction module, which takes some of the counter-bits as input. Once the counter reaches a value, where all of these bits are high, it gets reset to zero by the conjunction module.
    The outputs of these four conjunction modules are then inverted and finally aggregated in another conjunction module, which feeds its output into the module named rx.

    This means, that rx will receive a low pulse, only when all counters are reset at the same time. The number of button presses, at which this first happens, is the lowest common multiple of all counter periods.
    */

    QList<int> counterPeriods;

    for (QString counterFlipFlop: nodes["broadcaster"].destinations) {
        int counterPeriod = 0;

        bool nextFound = true;
        for (int i = 0; nextFound; i++) {
            bool isConnectedToConjunction = false;
            nextFound = false;

            for (const QString &destination: nodes[counterFlipFlop].destinations) {
                if (nodes[destination].type == '&') {
                    isConnectedToConjunction = true;
                } else {
                    counterFlipFlop = destination;
                    nextFound = true;
                }
            }

            counterPeriod |= isConnectedToConjunction << i;
        }

        counterPeriods.append(counterPeriod);
    }

    out << "First rx low pulse: " << leastCommonMultiple(counterPeriods) << "\n";

    return 0;
}

static long long leastCommonMultiple(const QList<int> &numbers) {
    QMap<int, int> primeFactors;

    for (int num: numbers) {
        for (auto [prime, count]: factorize(num)) {
            primeFactors[prime] = std::max(primeFactors[prime], count);
        }
    }

    long long lcm = 1;
    for (int prime: primeFactors.keys()) {
        lcm *= std::pow((long long)prime, (long long)primeFactors[prime]);
    }

    return lcm;
}

static QList<std::pair<int, int>> factorize(int n) {
    QList<std::pair<int, int>> factors;

    for (int i = 2; i <= n; i++) {
        int count = 0;

        while (n % i == 0) {
            count++;
            n /= i;
        }

        if (count > 0) {
            factors.append({i, count});
        }
    }

    return factors;
}
