#include "parsejobtest.h"

#include <qtest_kde.h>

#include <tests/autotestshell.h>
#include <tests/testcore.h>
#include <interfaces/ilanguagecontroller.h>
#include <language/backgroundparser/backgroundparser.h>
#include <tests/testproject.h>
#include <language/duchain/duchainlock.h>
#include <language/duchain/duchain.h>
#include <language/duchain/parsingenvironment.h>

QTEST_KDEMAIN(Css::TestParseJob, GUI)

namespace Css {

void TestParseJob::init()
{
    KDevelop::AutoTestShell::init();
    m_core = new KDevelop::TestCore();
    m_core->initialize(KDevelop::Core::NoUi);
    m_projectController = new KDevelop::TestProjectController(m_core);
    m_core->setProjectController(m_projectController);
}

void TestParseJob::cleanup()
{
    m_core->cleanup();
    delete m_core;
}

class TestFile : public QObject
{
    Q_OBJECT
public:
    TestFile(QByteArray contents, KDevelop::TestProject *project, const QString &filename)
        : m_file(filename), m_ready(false)
    {
        m_file.open();
        m_file.write(contents);
        m_file.close();
        project->addToFileSet(KDevelop::IndexedString(m_file.fileName()));
    }

    ~TestFile()
    {
        if (m_topContext) {
            KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
            KDevelop::DUChain::self()->removeDocumentChain(m_topContext.data());
        }
    }

    void parse(KDevelop::TopDUContext::Features features)
    {
        kDebug() << "parsing" << m_file.fileName();
        KDevelop::DUChain::self()->updateContextForUrl(KDevelop::IndexedString(m_file.fileName()), features, this);
    }

    void waitForParsed()
    {
        QTime t;
        t.start();
        while (!m_ready) {
            Q_ASSERT(t.elapsed() < 60000);
            QTest::qWait(10);
        }
    }

    KDevelop::ReferencedTopDUContext topContext()
    {
        waitForParsed();
        return m_topContext;
    }

public slots:
    void updateReady(KDevelop::IndexedString url, KDevelop::ReferencedTopDUContext topContext)
    {
        qDebug() << url.str() << topContext.data();
        Q_ASSERT(url.str() == m_file.fileName());
        m_ready = true;
        m_topContext = topContext;
    }

private:
    QTemporaryFile m_file;
    bool m_ready;
    KDevelop::ReferencedTopDUContext m_topContext;
};


void TestParseJob::testSimple()
{
    KDevelop::TopDUContext::Features features = KDevelop::TopDUContext::VisibleDeclarationsAndContexts;

    KDevelop::TestProject* project = new KDevelop::TestProject;
    m_projectController->clearProjects();
    m_projectController->addProject(project);

    TestFile f("a { color: red; }", project, "XXXXXXX.css");
    f.parse(features);

    KDevelop::ReferencedTopDUContext top = f.topContext();

    KDevelop::DUChainReadLocker lock(KDevelop::DUChain::lock());
    QVERIFY(top);
    QVERIFY(top->parsingEnvironmentFile()->language() == KDevelop::IndexedString("Css"));
    QCOMPARE(top->childContexts().count(), 1);
}

void TestParseJob::testSimpleHtml()
{
    KDevelop::TopDUContext::Features features = KDevelop::TopDUContext::VisibleDeclarationsAndContexts;

    KDevelop::TestProject* project = new KDevelop::TestProject;
    m_projectController->clearProjects();
    m_projectController->addProject(project);

    TestFile f("<html><style>a { color: red; }</style></html>", project, "XXXXXXX.html");
    f.parse(features);

    KDevelop::ReferencedTopDUContext top = f.topContext();

    KDevelop::DUChainReadLocker lock(KDevelop::DUChain::lock());
    QVERIFY(top);
    QVERIFY(top->parsingEnvironmentFile()->language() == KDevelop::IndexedString("Css"));
    QCOMPARE(top->childContexts().count(), 1);
}

void TestParseJob::testHtmlTwoStyleElements()
{
    KDevelop::TopDUContext::Features features = KDevelop::TopDUContext::VisibleDeclarationsAndContexts;

    KDevelop::TestProject* project = new KDevelop::TestProject;
    m_projectController->clearProjects();
    m_projectController->addProject(project);

    TestFile f("<html><style>a { color: red; }</style><style>h1 { font-weight:normal; }</style></html>", project, "XXXXXXX.html");
    f.parse(features);

    KDevelop::ReferencedTopDUContext top = f.topContext();

    KDevelop::DUChainReadLocker lock(KDevelop::DUChain::lock());
    QVERIFY(top);
    QVERIFY(top->parsingEnvironmentFile()->language() == KDevelop::IndexedString("Css"));
    QCOMPARE(top->childContexts().count(), 2);
}


}

#include "parsejobtest.moc"
#include "moc_parsejobtest.cpp"
