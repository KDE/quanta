#ifndef QTEST_CONNECTIONTEST
#define QTEST_CONNECTIONTEST

#include <QtCore/QObject>

class ConnectionTest : public QObject
{
Q_OBJECT
private slots:
    void init();
    void cleanup();

    void testStdOutput();
    void testShowStepInSource();
    void testMultipleSessions();
};

#endif
