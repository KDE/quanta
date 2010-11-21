#include "modeltest.h"

#include <qtest_kde.h>
#include <KDebug>
#include <KTextEditor/EditorChooser>
#include <KTextEditor/View>

#include "../model.h"

namespace Css {

int debugArea() { return 9043; /* static int s_area = KDebug::registerArea("kdevcsssupport"); return s_area; */ }

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
    QTest::addColumn<QString>("type");
    QTest::addColumn<QString>("text");
    QTest::addColumn<QStringList>("result");

    // | is the cursor

    QTest::newRow("element") << "css"
        << "body{font-|w:normal;}"
        << (QStringList() << "font-weight");

    QTest::newRow("new element") << "css"
        << "body{|}"
        << (QStringList() << "font-weight");

    QTest::newRow("new element without value") << "css"
        << "body{font-w|}"
        << (QStringList() << "font-weight");

    QTest::newRow("new element without value and without brace") << "css"
          //01234567890123456789
        << "body{font-w|"
        << (QStringList() << "font-weight");

    QTest::newRow("field") << "css"
          //01234567890123456789
        << "body{font-weight:|normal;}"
        << (QStringList() << "normal" << "bold");

    QTest::newRow("field without expression") << "css"
        << "body{font-weight:|}"
        << (QStringList() << "normal" << "bold");

    QTest::newRow("field without expression and without brace") << "css"
          //012345678901234567
        << "body{font-weight:|"
        << (QStringList() << "normal" << "bold");

    QTest::newRow("field without expression and without brace with space") << "css"
          //0123456789012345678
        << "body{font-weight: |"
        << (QStringList() << "normal" << "bold");

    QTest::newRow("second field different property2") << "css"
        << "body{font-weight: n|; float: left; }"
        << (QStringList() << "normal" << "bold");

    QTest::newRow("second field different property") << "css"
        << "body{font-weight: normal; float: |}"
        << (QStringList() << "right" << "left");

    QTest::newRow("second field without value") << "css"
        << "body{font-weight: |; float: left}"
        << (QStringList() << "normal" << "bold");

    QTest::newRow("second field without value without semicolon") << "css"
        << "body{font-weight: | float: left}"
        << (QStringList() << "normal" << "bold");

    QTest::newRow("second field") << "css"
          //01234567890123456789
        << "body{font-weight:normal; |}"
        << (QStringList() << "font-weight");

    QTest::newRow("element second line") << "css"
        << "body{color:red;}\nbody{font|-w:normal;}"
        << (QStringList() << "font-weight");

    QTest::newRow("selector at start") << "css"
        << "|body{}"
        << (QStringList() << "body" << "a");

    QTest::newRow("selector") << "css"
        << "body{}|"
        << (QStringList() << "body" << "a");

    QTest::newRow("selector second line") << "css"
        << "body{font-weight: bolder;}|\nbody{font-weight: asdf;}"
        << (QStringList() << "body" << "a");

    QTest::newRow("selector with space") << "css"
        << "body{} |"
        << (QStringList() << "body" << "a");

    QTest::newRow("selector separated with space") << "css"
        << "body |{}"
        << (QStringList() << "body" << "a");
    
    QTest::newRow("selector separated with comma") << "css"
        << "body, |{}"
        << (QStringList() << "body" << "a");

    QTest::newRow("selector separated with space without braces") << "css"
        << "body |"
        << (QStringList() << "body" << "a");

    QTest::newRow("selector separated with comma without braces") << "css"
        << "body, |"
        << (QStringList() << "body" << "a");

    QTest::newRow("selector empty text") << "css"
        << "|"
        << (QStringList() << "body" << "a");

    //************************** HTML

    QTest::newRow("in empty style element") << "html"
        << "<html><style>|</style></html>"
        << (QStringList() << "body" << "a");

    QTest::newRow("outside style element") << "html"
        << "<html>|<style></style></html>"
        << (QStringList());

    QTest::newRow("empty html") << "html"
        << ""
        << (QStringList());

    QTest::newRow("html fields") << "html"
        << "<html><style>a { font-weight: b|; }</style></html>"
        << (QStringList() << "bold" << "normal");
}

void ModelTest::completionItems()
{
    KTextEditor::Document* doc = KTextEditor::EditorChooser::editor()->createDocument(0);

    QFETCH(QString, text);
    QFETCH(QString, type);

    KTextEditor::Cursor position;
    QString textBeforeCursor = text.left(text.indexOf('|'));
    position.setLine(textBeforeCursor.count('\n'));
    position.setColumn(textBeforeCursor.mid(textBeforeCursor.lastIndexOf('\n')).length());

    text.replace('|', "");

    QTemporaryFile file("XXXXXXXXX."+type);
    file.open();
    file.write(text.toUtf8());
    file.close();

    doc->openUrl(KUrl("file://"+QDir::current().absoluteFilePath(file.fileName())));

    QCOMPARE(doc->mimeType(), QString("text/")+type);

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
