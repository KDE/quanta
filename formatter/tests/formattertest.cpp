#include "formattertest.h"

#include <QTest>

#include "../sgmlformatter.h"
#include "../xmlformatter.h"
#include <KDebug>
#include <htmlformatter.h>
#include <QFile>

QTEST_MAIN(Xml::TestFormatter)

Xml::TestFormatter::TestFormatter()
{

}

void Xml::TestFormatter::testHtml()
{
    /*
    QFile f("/media/usb_large/tests/html/yahoo.html");
    f.open(QIODevice::ReadOnly);
    QTextStream stream(&f);
    QString test = stream.readAll();
    kDebug() << test.length();
    //QString test = "<body>\n text <?php \n blah \n ?>\n</body>";
    Xml::HtmlFormatter formatter;
    //formatter.setDebugEnabled(true);
    test = formatter.formatSource(test);
    kDebug() << test.length();
    */
}


void Xml::TestFormatter::testXml()
{
    /*
    QString test = "<!doctype html><html><meta><body>text</body></html>";
    XmlFormatter formatter;
    formatter.formatSource(test);
    */
}

#include "formattertest.moc"
