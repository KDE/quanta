#ifndef MODELTEST_H
#define MODELTEST_H

#include <QObject>

#include <duchain/tests/duchaintestbase.h>

namespace Css
{
int debugArea();

class ModelTest : public DUChainTestBase
{
    Q_OBJECT

private slots:
    void testCompletionRange();
    void testCompletionRangeSecondLine();

    void completionItems_data();
    void completionItems();
};

}

#endif // MODELTEST_H
