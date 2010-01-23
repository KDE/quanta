#include "modeltest.h"

#include <qtest_kde.h>
#include <KDebug>
#include <KTextEditor/EditorChooser>
#include <KTextEditor/View>

#include "../model.h"

namespace Css {

int debugArea() { return 9043; /* static int s_area = KDebug::registerArea("kdevcsssupport"); return s_area; */ }

ModelTest::ModelTest()
{
}

void ModelTest::testCompletionRange()
{
    KTextEditor::Document* doc = KTextEditor::EditorChooser::editor()->createDocument(0);
    doc->setText("body{font-w:normal;}");
                //01234567890123456789 
    KTextEditor::View* view = doc->createView(0);
    CodeCompletionModel* model = new CodeCompletionModel(doc);

    KTextEditor::Cursor position(0, 9);
    KTextEditor::Range range = model->completionRange(view, position);
    kDebug() << range << doc->text(range);
    QCOMPARE(range, KTextEditor::Range(0, 5, 0, 11));
    QCOMPARE(doc->text(range), QString("font-w"));

    position = KTextEditor::Cursor(0, 10);
    range = model->completionRange(view, position);
    kDebug() << range << doc->text(range);
    QCOMPARE(range, KTextEditor::Range(0, 5, 0, 11));
    QCOMPARE(doc->text(range), QString("font-w"));


    position = KTextEditor::Cursor(0, 11);
    range = model->completionRange(view, position);
    kDebug() << range << doc->text(range);
    QCOMPARE(range, KTextEditor::Range(0, 5, 0, 11));
    QCOMPARE(doc->text(range), QString("font-w"));

    doc->setText("body{font-:normal;}");
                //01234567890123456789

    position = KTextEditor::Cursor(0, 9);
    range = model->completionRange(view, position);
    kDebug() << range << doc->text(range);
    QCOMPARE(range, KTextEditor::Range(0, 5, 0, 10));
    QCOMPARE(doc->text(range), QString("font-"));

    position = KTextEditor::Cursor(0, 10);
    range = model->completionRange(view, position);
    kDebug() << range << doc->text(range);
    QCOMPARE(range, KTextEditor::Range(0, 5, 0, 10));
    QCOMPARE(doc->text(range), QString("font-"));

    doc->setText("font-");
                //01234567890123456789

    position = KTextEditor::Cursor(0, 5);
    range = model->completionRange(view, position);
    kDebug() << range << doc->text(range);
    QCOMPARE(range, KTextEditor::Range(0, 0, 0, 5));
    QCOMPARE(doc->text(range), QString("font-"));

    doc->setText("font-weight:normal");
                //01234567890123456789

    position = KTextEditor::Cursor(0, 12);
    range = model->completionRange(view, position);
    kDebug() << range << doc->text(range);
    QCOMPARE(range, KTextEditor::Range(0, 12, 0, 18));
    QCOMPARE(doc->text(range), QString("normal"));

    position = KTextEditor::Cursor(0, 15);
    range = model->completionRange(view, position);
    kDebug() << range << doc->text(range);
    QCOMPARE(range, KTextEditor::Range(0, 12, 0, 18));
    QCOMPARE(doc->text(range), QString("normal"));
    delete doc;
}

void ModelTest::testCompletionRangeSecondLine()
{
    KTextEditor::Document* doc = KTextEditor::EditorChooser::editor()->createDocument(0);
    doc->setText("body{color:red;}\nbody{font-w:normal;}");
                //                  01234567890123456789
    KTextEditor::View* view = doc->createView(0);
    CodeCompletionModel* model = new CodeCompletionModel(doc);

    KTextEditor::Cursor position(1, 9);
    KTextEditor::Range range = model->completionRange(view, position);
    kDebug() << range << doc->text(range);
    QCOMPARE(range, KTextEditor::Range(1, 5, 1, 11));
    QCOMPARE(doc->text(range), QString("font-w"));
    delete doc;
}

void ModelTest::completionItems_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QStringList>("result");

    // | is the cursor

    QTest::newRow("element")
        << "body{font-|w:normal;}"
        << (QStringList() << "font-weight");

    QTest::newRow("new element")
        << "body{|}"
        << (QStringList() << "font-weight");

    QTest::newRow("new element without value")
        << "body{font-w|}"
        << (QStringList() << "font-weight");

    QTest::newRow("new element without value and without brace")
          //01234567890123456789
        << "body{font-w|"
        << (QStringList() << "font-weight");

    QTest::newRow("field")
          //01234567890123456789
        << "body{font-weight:|normal;}"
        << (QStringList() << "normal" << "bold");

    QTest::newRow("field without expression")
        << "body{font-weight:|}"
        << (QStringList() << "normal" << "bold");

    QTest::newRow("field without expression and without brace")
          //012345678901234567
        << "body{font-weight:|"
        << (QStringList() << "normal" << "bold");

    QTest::newRow("field without expression and without brace with space")
          //0123456789012345678
        << "body{font-weight: |"
        << (QStringList() << "normal" << "bold");

    QTest::newRow("second field different property2")
        << "body{font-weight: n|; float: left; }"
        << (QStringList() << "normal" << "bold");

    QTest::newRow("second field different property")
        << "body{font-weight: normal; float: |}"
        << (QStringList() << "right" << "left");

    QTest::newRow("second field without value")
        << "body{font-weight: |; float: left}"
        << (QStringList() << "normal" << "bold");

    QTest::newRow("second field without value without semicolon")
        << "body{font-weight: | float: left}"
        << (QStringList() << "normal" << "bold");

    QTest::newRow("second field")
          //01234567890123456789
        << "body{font-weight:normal; |}"
        << (QStringList() << "font-weight");

    QTest::newRow("element second line")
        << "body{color:red;}\nbody{font|-w:normal;}"
        << (QStringList() << "font-weight");

    QTest::newRow("selector at start")
        << "|body{}"
        << (QStringList() << "body" << "a");

    QTest::newRow("selector")
        << "body{}|"
        << (QStringList() << "body" << "a");

    QTest::newRow("selector second line")
        << "body{font-weight: bolder;}|\nbody{font-weight: asdf;}"
        << (QStringList() << "body" << "a");

    QTest::newRow("selector with space")
        << "body{} |"
        << (QStringList() << "body" << "a");

    QTest::newRow("selector separated with space")
        << "body |{}"
        << (QStringList() << "body" << "a");
    
    QTest::newRow("selector separated with comma")
        << "body, |{}"
        << (QStringList() << "body" << "a");

    QTest::newRow("selector separated with space without braces")
        << "body |"
        << (QStringList() << "body" << "a");

    QTest::newRow("selector separated with comma without braces")
        << "body, |"
        << (QStringList() << "body" << "a");

    QTest::newRow("selector empty text")
        << "|"
        << (QStringList() << "body" << "a");
}

void ModelTest::completionItems()
{
    KTextEditor::Document* doc = KTextEditor::EditorChooser::editor()->createDocument(0);

    QFETCH(QString, text);
    KTextEditor::Cursor position;
    QString textBeforeCursor = text.left(text.indexOf('|'));
    position.setLine(textBeforeCursor.count('\n'));
    position.setColumn(textBeforeCursor.mid(textBeforeCursor.lastIndexOf('\n')).length());
    text.replace('|', "");
    doc->setText(text);

    KTextEditor::View* view = doc->createView(0);
    CodeCompletionModel* model = new CodeCompletionModel(doc);

    QCOMPARE(model->rowCount(), 0);
    model->completionInvoked(view, model->completionRange(view, position), KTextEditor::CodeCompletionModel::ManualInvocation);
    QStringList completions;
    for (int i=0; i < model->rowCount(); ++i) {
        completions << model->data(model->index(i, CodeCompletionModel::Name), Qt::DisplayRole).toString();
    }
    kDebug() << "completions" << completions;
    QFETCH(QStringList, result);
    foreach (const QString &i, result) {
        QVERIFY(completions.contains(i));
    }

    delete doc;
}

}

QTEST_KDEMAIN(Css::ModelTest, GUI)

#include "modeltest.moc"
