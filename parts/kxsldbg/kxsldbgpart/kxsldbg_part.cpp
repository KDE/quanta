#include "kxsldbg_part.h"

#include <kinstance.h>
#include <kaction.h>
#include <kstdaction.h>
#include <kfiledialog.h>
#include <kparts/genericfactory.h>

#include <qfile.h>
#include <qtextstream.h>

#include "../kxsldbg.h"
#include <kaction.h>
#include <kinstance.h>
#include <kiconloader.h>
#include <qmessagebox.h>
#include <klocale.h>
#include <kdeversion.h>
#if KDE_IS_VERSION(3,1,90)
#include <kinputdialog.h>
#else
#include <klineeditdlg.h>
#endif

// Qxsldbg specific includes
#include <qvariant.h>
#include <qfile.h>
#include <qstatusbar.h>
#include <qsplitter.h>
#include <qvbox.h>

#include "qxsldbgdoc.h"
#include "qxsldbgview.h"

#include <qmime.h>
#include <qdragobject.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qtextstream.h>
#include <qtextbrowser.h>
#include <qmessagebox.h>
#include <qlineedit.h>
#include <qdockwindow.h>
#include <qpushbutton.h>
#include <qinputdialog.h>
#include "xsldbgoutputview.h"
#include "xsldbgconfigimpl.h"
#include <kdebug.h>

typedef KParts::GenericFactory<KXsldbgPart> KXsldbgPartFactory;
K_EXPORT_COMPONENT_FACTORY( libkxsldbgpart, KXsldbgPartFactory )

KXsldbgPart::KXsldbgPart( QWidget *parentWidget, const char * /*widgetName*/,
                                  QObject *parent, const char *name,
                                  const QStringList & /*args*/ )
  : KParts::ReadOnlyPart(parent, name), DCOPObject("KXsldbgPart")
{
    currentLineNo = 0;
    currentColumnNo = 0;
    inspector = 0L;
    debugger = 0L;
    configWidget = 0L;

    // we need an instance
    setInstance( KXsldbgPartFactory::instance() );
    QVBox *frame = new QVBox(parentWidget);
    QHBox *h = new QHBox(frame);
    newXPath = new QLineEdit(h);
    xPathBtn = new QPushButton(i18n("Goto XPath"), h);
/* Disable searching as searching documentation is not ready
    h = new QHBox(frame);
    newSearch = new QLineEdit(h);
    searchBtn = new QPushButton(i18n("Search"), h);
*/
    h = new QHBox(frame);
    newEvaluate = new QLineEdit(h);
    evaluateBtn = new QPushButton(i18n("Evaluate"), h);

    QSplitter *splitter = new QSplitter(QSplitter::Vertical, frame);

    m_editWidget = new QXsldbgView( splitter);
    connect(m_editWidget, SIGNAL(cursorPositionChanged(int, int)),
            this, SLOT(editWidget_cursorPositionChanged(int, int)));
    connect(m_editWidget,SIGNAL(openFile(QString, int, int)), 
	    this,  SLOT(emitOpenFile(QString, int, int)));
    m_editWidget->setDocument(0L);
    outputview = new XsldbgOutputView(splitter);
    setWidget(frame);

    docDictionary.setAutoDelete(TRUE);

    // create our actions
    KStdAction::open(this, SLOT(fileOpen()), actionCollection());

    // set our XML-UI resource file
    setXMLFile("kxsldbg_part.rc");
   (void) new KAction( i18n("Configure..."),
                        "configure", Key_C,
                        this, SLOT(configureCmd_activated()),
                        actionCollection(), "configureCmd" );

    (void) new KAction( i18n("Inspect..."),
                        "find", Key_I,
                        this, SLOT(inspectorCmd_activated()),
                        actionCollection(), "inspectCmd" );


    // Motions commands
    (void) new KAction( i18n("Run"),
                        "run", Key_F5,
                        this, SLOT(runCmd_activated()),
                        actionCollection(), "runCmd" );

    (void) new KAction( i18n("Continue"),
                        "1downarrow", Key_F4,
                        this, SLOT(continueCmd_activated()),
                        actionCollection(), "continueCmd" );

    (void) new KAction( i18n("Step"),
                        "step", Key_F8,
                        this, SLOT(stepCmd_activated()),
                        actionCollection(), "stepCmd" );

    (void) new KAction( i18n("Next"),
                        "next", Key_F10,
                        this, SLOT(nextCmd_activated()),
                        actionCollection(), "nextCmd" );

    (void) new KAction( i18n("Step Up"),
                        "xsldbg_stepup", Key_F6,
                        this, SLOT(stepupCmd_activated()),
                        actionCollection(), "stepupCmd" );

    (void) new KAction( i18n("Step Down"),
                        "xsldbg_stepdown", Key_F7,
                        this, SLOT(stepCmd_activated()),
                        actionCollection(), "stepdownCmd" );

    // Breakpoint commands
    (void) new KAction( i18n("Break"),
                        "xsldbg_break", Key_F2,
                        this, SLOT(breakCmd_activated()),
                        actionCollection(), "breakCmd" );

    (void) new KAction( i18n("Enable/Disable"),
                        "xsldbg_enable", Key_F3,
                        this, SLOT(enableCmd_activated()),
                        actionCollection(), "enableCmd" );

    (void) new KAction( i18n("Delete"),
                        "xsldbg_delete", Key_Delete,
                        this, SLOT(deleteCmd_activated()),
                        actionCollection(), "deleteCmd" );

    (void) new KAction( i18n("&Source"),
                        "xsldbg_source", Key_S,
                        this, SLOT(sourceCmd_activated()),
                        actionCollection(), "sourceCmd" );

    (void) new KAction( i18n("&Data"),
                        "xsldbg_data", Key_D,
                        this, SLOT(dataCmd_activated()),
                        actionCollection(), "dataCmd" );

    (void) new KAction( i18n("&Output"),
                        "xsldbg_output", Key_O,
                        this, SLOT(outputCmd_activated()),
                        actionCollection(), "outputCmd" );

    (void) new KAction( i18n("Reload Current File From Disk"),
                        "xsldbg_refresh", CTRL + Key_F5,
                        this, SLOT(refreshCmd_activated()),
                        actionCollection(), "refreshCmd" );

    /* tracing and walking */
    (void) new KAction( i18n("Walk Through Stylesheet..."),
                        Key_W,
                        this, SLOT(walkCmd_activated()),
                        actionCollection(), "walkCmd" );
    (void) new KAction( i18n("Stop Wal&king Through Stylesheet"),
                        Key_K,
                        this, SLOT(walkStopCmd_activated()),
                        actionCollection(), "walkStopCmd" );
    (void) new KAction( i18n("Tr&ace Execution of Stylesheet"),
                        Key_A,
                        this, SLOT(traceCmd_activated()),
                        actionCollection(), "traceCmd" );
    (void) new KAction( i18n("Stop Tracing of Stylesheet"),
                        Key_K,
                        this, SLOT(traceStopCmd_activated()),
                        actionCollection(), "traceStopCmd" );

    (void) new KAction( i18n("&Evaluate Expression..."),
                        Key_E,
                        this, SLOT(evaluateCmd_activated()),
                        actionCollection(), "evaluateCmd" );

    (void) new KAction( i18n("Goto &XPath..."),
                        Key_X,
                        this, SLOT(gotoXPathCmd_activated()),
                        actionCollection(), "gotoXPathCmd" );

    (void) new KAction( i18n("Lookup SystemID..."),
                        0,
                        this, SLOT(slotLookupSystemID()),
                        actionCollection(), "lookupSystemID" );

    (void) new KAction( i18n("Lookup PublicID..."),
                        0,
                        this, SLOT(slotLookupPublicID()),
                        actionCollection(), "lookupPublicID" );


    /*
    (void) new KAction( i18n("Exit KXsldbg"),
                        "xsldbg_output", CTRL + Key_Q,
                        this, SLOT(exitCmd_activated()),
                        actionCollection(), "exitCmd" );
    */
    connect( xPathBtn, SIGNAL( clicked() ),
             this, SLOT( slotGotoXPath() ) );
    connect( evaluateBtn, SIGNAL( clicked() ),
             this, SLOT( slotEvaluate() ) );
/*

    connect( searchBtn, SIGNAL( clicked() ),
             this, SLOT( slotSearch() ) );
*/

}


KXsldbgPart::~KXsldbgPart()
{
}


KAboutData *KXsldbgPart::createAboutData()
{
    // the non-i18n name here must be the same as the directory in
    // which the part's rc file is installed ('partrcdir' in the
    // Makefile)
    KAboutData *aboutData = new KAboutData("kxsldbgpart", I18N_NOOP("KXsldbgPart"), "0.1");
    aboutData->addAuthor("Keith Isdale", 0, "k_isdale@tpg.com.au");
    return aboutData;
}

bool KXsldbgPart::openFile()
{
    // m_file is always local so we can use QFile on it
    QFile file(m_file);
    if (file.open(IO_ReadOnly) == false)
        return false;

    // our example widget is text-based, so we use QTextStream instead
    // of a raw QDataStream
    QTextStream stream(&file);
    QString str;
    while (!stream.eof())
        str += stream.readLine() + "\n";

    file.close();

    // now that we have the entire file, display it
    // widget()->setText(str);

    // just for fun, set the status bar
    emit setStatusBarText( m_url.prettyURL() );

    return true;
}


void KXsldbgPart::fileOpen()
{
    // this slot is called whenever the File->Open menu is selected,
    // the Open shortcut is pressed (usually CTRL+O) or the Open toolbar
    // button is clicked
    QString file_name = KFileDialog::getOpenFileName();

    if (file_name.isEmpty() == false)
        openURL(file_name);
}


bool KXsldbgPart::checkDebugger()
{
  bool result = debugger != 0L;
  if (!result){
    QMessageBox::information(0L, i18n("Debugger Not Ready"),
			      i18n("Configure and start the debugger first!"),
			      QMessageBox::Ok);
  }

  return result;
}


void KXsldbgPart::lookupSystemID( QString systemID)
{
   bool ok = FALSE;
   if (!checkDebugger())
     return;

   if (systemID.isEmpty()){
#if KDE_IS_VERSION(3, 1, 90)
     systemID = KInputDialog::getText(
			      i18n( "Lookup SystemID" ),
			      i18n( "Please enter SystemID to find:" ),
			      QString::null, &ok,
			      m_editWidget);
#else
     systemID = QInputDialog::getText(
			      i18n( "Lookup SystemID" ),
			      i18n( "Please enter SystemID to find:" ),
			      QLineEdit::Normal, QString::null, &ok,
			      m_editWidget);
#endif
   }else{
     ok = TRUE;
   }
  if ( ok && !systemID.isEmpty() ){
      // user entered something and pressed ok
	QString msg(QString("system %1").arg(systemID));  // noTr
	debugger->fakeInput(msg, TRUE);
  }

}


void KXsldbgPart::lookupPublicID(QString publicID)
{
   bool ok = FALSE;
   if (!checkDebugger())
     return;

   if (publicID.isEmpty()){
#if KDE_IS_VERSION(3, 1, 90)
     publicID = KInputDialog::getText(
			      i18n( "Lookup PulicID" ),
			      i18n( "Please enter PublicID to find:" ),
			      QString::null, &ok, m_editWidget );
#else
     publicID = QInputDialog::getText(
			      i18n( "Lookup PulicID" ),
			      i18n( "Please enter PublicID to find:" ),
			      QLineEdit::Normal, QString::null, &ok, m_editWidget );
#endif
   }else{
     ok = TRUE;
   }
   if ( ok && !publicID.isEmpty()){
     // user entered something and pressed ok
	QString msg(QString("public %1").arg(publicID));  // noTr
	debugger->fakeInput(msg, TRUE);
  }
}


void KXsldbgPart::slotLookupSystemID()
{
  lookupSystemID("");
}

void KXsldbgPart::slotLookupPublicID()
{
  lookupPublicID("");
}

void KXsldbgPart::configureCmd_activated()
{
  if (debugger == 0L){
    /* We must have a valid debugger */
    if (inspector == 0L){
      createInspector();
    }
    if (!checkDebugger())
      return;
  }

  if (configWidget == 0L){
    configWidget = new XsldbgConfigImpl( debugger, 0L );
    CHECK_PTR( configWidget );
  }
  if (configWidget != 0L)
    configWidget->refresh();
    configWidget->show();
}

void
KXsldbgPart::runCmd_activated()
{
    if ( checkDebugger() )
        debugger->slotRunCmd();
}

void KXsldbgPart::inspectorCmd_activated()
{
  if (inspector == 0L)
    createInspector();

  if (checkDebugger() && (inspector != 0L)){
    inspector->show();
  }
}


void KXsldbgPart::createInspector()
{
    if ( inspector == 0L ) {
      debugger = new XsldbgDebugger();
      CHECK_PTR( debugger );
      if ( debugger != 0L ) {
	       if (outputview){
		 connect(debugger,
			 SIGNAL( showMessage(QString /* msg*/)),
			 outputview,
		  SLOT(slotProcShowMessage(QString /* msg*/)));
	       }
	      inspector = new XsldbgInspector( debugger );
	      CHECK_PTR( inspector );
	      debugger->setInspector( inspector );
	      if (inspector != 0L){
		/*process line number and/or file name changed */
		connect(debugger,
			SIGNAL( lineNoChanged
				(QString /* fileName */ ,
				 int /* lineNumber */ ,
				 bool /* breakpoint */ ) ),
			this,
			SLOT(lineNoChanged
			     ( QString /* fileName */ ,
			       int /* lineNumber */ ,
			       bool /* breakpoint */ ) ) );
		connect(m_editWidget,
			SIGNAL(addBreakPoint(int /*lineNumber */)),
			this,
			SLOT(addBreakPoint(int /* lineNumber */)));
		connect(m_editWidget,
			SIGNAL(enableBreakPoint(int /*lineNumber */)),
			this,
			SLOT(enableBreakPoint(int /* lineNumber */)));
		connect(m_editWidget,
			SIGNAL(deleteBreakPoint(int /*lineNumber */)),
			this,
			SLOT(deleteBreakPoint(int /* lineNumber */)));
		connect(debugger,
			SIGNAL(breakpointItem(QString /* fileName*/,
					      int /* lineNumber */,
					      QString /*templateName*/,
					      QString /* modeName */,
					      bool /* enabled */,
					      int /* id */)),
			this,
			SLOT( breakpointItem(QString /* fileName*/,
					     int /* lineNumber */,
					     QString /*templateName*/,
					     QString /* modeName */,
					     bool /* enabled */,
					     int /* id */)));
		connect(debugger, SIGNAL(resolveItem(QString /*URI*/)),
			this, SLOT(slotProcResolveItem(QString /*URI*/)));
	      }
        }
    }
}

void KXsldbgPart::emitOpenFile(QString file, int line, int row)
{
    QByteArray params;
    QDataStream stream(params, IO_WriteOnly);
    stream << file << line << row;
    emitDCOPSignal("openFile(QString,int,int)", params); 
}
void KXsldbgPart::continueCmd_activated()
{
    if ( checkDebugger() )
        debugger->slotContinueCmd();

}

void KXsldbgPart::stepCmd_activated()
{
    if ( checkDebugger() )
        debugger->slotStepCmd();
}

void KXsldbgPart::nextCmd_activated()
{
    if ( checkDebugger() )
        debugger->fakeInput("next", true);  // noTr
}


void KXsldbgPart::stepupCmd_activated()
{
    if ( checkDebugger() )
      debugger->fakeInput("stepup", TRUE);  // noTr
}


void KXsldbgPart::stepdownCmd_activated()
{
    if ( checkDebugger() )
      debugger->fakeInput("stepdown", TRUE);  // noTr
}


void KXsldbgPart::dataCmd_activated()
{
    if ( checkDebugger() )
        debugger->slotDataCmd();
}

void
KXsldbgPart::sourceCmd_activated()
{
    if ( checkDebugger() )
        debugger->slotSourceCmd();
}

void
KXsldbgPart::outputCmd_activated()
{
  if ( ( inspector != 0L ) &&  checkDebugger() && ( configWidget != 0L ) ){
    debugger->setOutputFileActive(TRUE);
    lineNoChanged( configWidget->getOutputFile(), 1, false );
  }
}

void KXsldbgPart::refreshCmd_activated()
{

  if (currentFileName.length() > 0 ){

    QXsldbgDoc *docPtr = docDictionary[currentFileName];
    if (docPtr != 0L){
      docPtr->load(currentFileName);
      m_editWidget->setDocument(docPtr);
      m_editWidget->repaintAll();
      /* Reload breakpoints from xsldbg */
      if ( checkDebugger() ){
	debugger->fakeInput("showbreak", TRUE);  // noTr
      }
    }
  }
}

void KXsldbgPart::enableCmd_activated()
{
  if ( checkDebugger() ){
    debugger->slotEnableCmd( currentFileName, currentLineNo);
  }
}

void KXsldbgPart::deleteCmd_activated()
{
  if ( checkDebugger() ){
        debugger->slotDeleteCmd( currentFileName, currentLineNo);
  }
}

void KXsldbgPart::breakCmd_activated()
{
  if ( checkDebugger() ){
    debugger->slotBreakCmd( currentFileName, currentLineNo);
  }
}

void KXsldbgPart::evaluateCmd_activated()
{
#if KDE_IS_VERSION(3,1,90)
  QString expression = KInputDialog::getText(i18n("Evalute Expression"), i18n("XPath:"));
#else
  QString expression = KLineEditDlg::getText(i18n("Evalute Expression"), i18n("XPath:"));
#endif
  if (checkDebugger()  && (expression.length() > 0)){
    debugger->slotCatCmd( expression);
  }
}

void KXsldbgPart::gotoXPathCmd_activated()
{
#if KDE_IS_VERSION(3,1,90)
  QString xpath = KInputDialog::getText(i18n("Goto XPath"), i18n("XPath:"));
#else
  QString xpath = KLineEditDlg::getText(i18n("Goto XPath"), i18n("XPath:"));
#endif
  if (checkDebugger() && xpath.length() > 0){
    debugger->slotCdCmd( xpath );
  }
}

void
KXsldbgPart::lineNoChanged(QString fileName, int lineNumber, bool breakpoint)
{
  if ( fileName.isEmpty() ) {
    qDebug( "Empty file Name" );  // noTr
    return;
  }

  if ( lineNumber > 0 )
    lineNumber--;           /* Take into account the actual cursor position */

  if ( currentFileName != fileName){
    QXsldbgDoc *docPtr = docDictionary[fileName];
    if (docPtr == 0L){
      docPtr = new QXsldbgDoc();
      if (docPtr != 0L){
	docPtr->load(fileName);
      docDictionary.insert(fileName, docPtr);
      m_editWidget->setDocument(docPtr);
      currentFileName = fileName;
      }
    }else{
      m_editWidget->setDocument(docPtr);
      currentFileName = fileName;
    }
  }
  /* Did we stop at a breakpoint if so move the marker */
  if (breakpoint) {
    m_editWidget->setMarkerPosition(lineNumber, 0);
    QByteArray params;
    QDataStream stream(params, IO_WriteOnly);
    stream << currentFileName << lineNumber;
    emitDCOPSignal("debuggerPositionChanged(QString,int)", params);
  }

  /* Move cursor and update status bar */
  m_editWidget->setCursorPosition( lineNumber, 0 );
}

void KXsldbgPart::editWidget_cursorPositionChanged( int lineNumber, int columnNumber)
{
  if ((currentLineNo != lineNumber + 1) || (columnNumber != currentColumnNo )) {
    currentLineNo = lineNumber + 1;
    currentColumnNo = columnNumber;
    QByteArray params;
    QDataStream stream(params, IO_WriteOnly);
    stream << currentFileName << lineNumber << columnNumber;
    emitDCOPSignal("editorPositionChanged(QString,int,int)", params);
  }

}

void  KXsldbgPart::addBreakPoint(int lineNumber)
{
  if ( checkDebugger() ){
    debugger->slotBreakCmd( currentFileName, lineNumber);
  }
}

void  KXsldbgPart::enableBreakPoint(int lineNumber)
{
  if ( checkDebugger() ){
    debugger->slotEnableCmd( currentFileName, lineNumber);
  }
}


void  KXsldbgPart::deleteBreakPoint(int lineNumber)
{
  if ( checkDebugger() ){
        debugger->slotDeleteCmd( currentFileName, lineNumber);
  }
}



void KXsldbgPart::slotSearch()
{
  if ((newSearch != 0L)  && checkDebugger() ) {
    QString msg(QString("search \"%1\"").arg(newSearch->text()));  // noTr
 		debugger->fakeInput(msg, false);
  }
}


void KXsldbgPart::slotEvaluate()
{
  if ((newEvaluate != 0L) && checkDebugger() ){
    debugger->slotCatCmd( newEvaluate->text() );
  }
}

void KXsldbgPart::slotGotoXPath()
{
  if ((newXPath != 0L) && checkDebugger() ){
    debugger->slotCdCmd( newXPath->text() );
  }
}



void KXsldbgPart::slotProcResolveItem(QString URI)
{
  if (!URI.isEmpty()){
    QMessageBox::information(m_editWidget, i18n("SystemID or PublicID Resolution Result"),
        i18n("SystemID or PublicID has been resolved to\n.%1").arg(URI),
	QMessageBox::Ok);
  }
}

void  KXsldbgPart::breakpointItem(QString fileName, int lineNumber ,
			      QString /*templateName*/, QString /* modeName */,
			      bool  enabled , int /* id */)
{

  if (fileName == 0L){
    /* Go through all documents and remove all breakpoints */
    QDictIterator<QXsldbgDoc> it(docDictionary);
    QXsldbgTextLine *item;
    QXsldbgDoc *docPtr;
    while (it.current()){
      docPtr = it.current();
      for (int index = 0; index < docPtr->lineCount(); index++){
	item = docPtr->getText(index);
	if (item != 0L)
	  item->deleteBreakPoint();
      }
      ++it;
    }
  }else{
    QXsldbgDoc *docPtr = docDictionary[fileName];
    QXsldbgTextLine *item;
    /* Load the file if need */
    if (docPtr == 0L){
      docPtr = new QXsldbgDoc();
      if (docPtr != 0L){
	docPtr->load(fileName);
	docDictionary.insert(fileName, docPtr);
      }
    }
    if (docPtr != 0L){
      item = docPtr->getText(lineNumber);
      if (item != 0L)
	item->enableBreakPoint(enabled);
    }
  }

  /* This causes a fair amount of flicker for a large amount
     of breakpoints */
  if (m_editWidget != 0L){
    m_editWidget->repaintAll();
  }
}



void KXsldbgPart::walkCmd_activated()
{
  if (checkDebugger()){
    debugger->slotWalkCmd();
  }
}

void KXsldbgPart::walkStopCmd_activated()
{
  if (checkDebugger()){
    debugger->slotWalkStopCmd();
  }
}

void KXsldbgPart::traceCmd_activated()
{
  if (checkDebugger()){
    debugger->slotTraceCmd();
  }
}

void KXsldbgPart::traceStopCmd_activated()
{
  walkStopCmd_activated();
}


#include "kxsldbg_part.moc"
