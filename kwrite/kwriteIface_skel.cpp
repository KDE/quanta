/****************************************************************************
**
** DCOP Skeleton created by dcopidl2cpp from kwriteIface.kidl
**
** WARNING! All changes made in this file will be lost!
**
*****************************************************************************/

#include "./kwriteIface.h"

#include <kdatastream.h>
#include <qasciidict.h>


static const int KWriteIface_fhash = 43;
static const char* const KWriteIface_ftable[43][3] = {
    { "void", "cursorLeft()", "cursorLeft()" },
    { "void", "shiftCursorLeft()", "shiftCursorLeft()" },
    { "void", "cursorRight()", "cursorRight()" },
    { "void", "shiftCursorRight()", "shiftCursorRight()" },
    { "void", "wordLeft()", "wordLeft()" },
    { "void", "shiftWordLeft()", "shiftWordLeft()" },
    { "void", "wordRight()", "wordRight()" },
    { "void", "shiftWordRight()", "shiftWordRight()" },
    { "void", "home()", "home()" },
    { "void", "shiftHome()", "shiftHome()" },
    { "void", "end()", "end()" },
    { "void", "shiftEnd()", "shiftEnd()" },
    { "void", "up()", "up()" },
    { "void", "shiftUp()", "shiftUp()" },
    { "void", "down()", "down()" },
    { "void", "shiftDown()", "shiftDown()" },
    { "void", "scrollUp()", "scrollUp()" },
    { "void", "scrollDown()", "scrollDown()" },
    { "void", "topOfView()", "topOfView()" },
    { "void", "bottomOfView()", "bottomOfView()" },
    { "void", "pageUp()", "pageUp()" },
    { "void", "shiftPageUp()", "shiftPageUp()" },
    { "void", "pageDown()", "pageDown()" },
    { "void", "shiftPageDown()", "shiftPageDown()" },
    { "void", "top()", "top()" },
    { "void", "shiftTop()", "shiftTop()" },
    { "void", "bottom()", "bottom()" },
    { "void", "shiftBottom()", "shiftBottom()" },
    { "int", "numLines()", "numLines()" },
    { "QString", "text()", "text()" },
    { "QString", "currentTextLine()", "currentTextLine()" },
    { "QString", "textLine(int)", "textLine(int num)" },
    { "QString", "currentWord()", "currentWord()" },
    { "QString", "word(int,int)", "word(int x,int y)" },
    { "void", "insertText(QString,bool)", "insertText(QString txt,bool mark)" },
    { "void", "setCursorPosition(int,int,bool)", "setCursorPosition(int line,int col,bool mark)" },
    { "bool", "isOverwriteMode()", "isOverwriteMode()" },
    { "void", "setOverwriteMode(bool)", "setOverwriteMode(bool b)" },
    { "int", "currentLine()", "currentLine()" },
    { "int", "currentColumn()", "currentColumn()" },
    { "bool", "loadFile(QString,int)", "loadFile(QString name,int flags)" },
    { "bool", "writeFile(QString)", "writeFile(QString name)" },
    { 0, 0, 0 }
};

bool KWriteIface::process(const QCString &fun, const QByteArray &data, QCString& replyType, QByteArray &replyData)
{
    static QAsciiDict<int>* fdict = 0;
    if ( !fdict ) {
	fdict = new QAsciiDict<int>( KWriteIface_fhash, TRUE, FALSE );
	for ( int i = 0; KWriteIface_ftable[i][1]; i++ )
	    fdict->insert( KWriteIface_ftable[i][1],  new int( i ) );
    }
    int* fp = fdict->find( fun );
    switch ( fp?*fp:-1) {
    case 0: { // void cursorLeft()
	replyType = KWriteIface_ftable[0][0]; 
	cursorLeft( );
    } break;
    case 1: { // void shiftCursorLeft()
	replyType = KWriteIface_ftable[1][0]; 
	shiftCursorLeft( );
    } break;
    case 2: { // void cursorRight()
	replyType = KWriteIface_ftable[2][0]; 
	cursorRight( );
    } break;
    case 3: { // void shiftCursorRight()
	replyType = KWriteIface_ftable[3][0]; 
	shiftCursorRight( );
    } break;
    case 4: { // void wordLeft()
	replyType = KWriteIface_ftable[4][0]; 
	wordLeft( );
    } break;
    case 5: { // void shiftWordLeft()
	replyType = KWriteIface_ftable[5][0]; 
	shiftWordLeft( );
    } break;
    case 6: { // void wordRight()
	replyType = KWriteIface_ftable[6][0]; 
	wordRight( );
    } break;
    case 7: { // void shiftWordRight()
	replyType = KWriteIface_ftable[7][0]; 
	shiftWordRight( );
    } break;
    case 8: { // void home()
	replyType = KWriteIface_ftable[8][0]; 
	home( );
    } break;
    case 9: { // void shiftHome()
	replyType = KWriteIface_ftable[9][0]; 
	shiftHome( );
    } break;
    case 10: { // void end()
	replyType = KWriteIface_ftable[10][0]; 
	end( );
    } break;
    case 11: { // void shiftEnd()
	replyType = KWriteIface_ftable[11][0]; 
	shiftEnd( );
    } break;
    case 12: { // void up()
	replyType = KWriteIface_ftable[12][0]; 
	up( );
    } break;
    case 13: { // void shiftUp()
	replyType = KWriteIface_ftable[13][0]; 
	shiftUp( );
    } break;
    case 14: { // void down()
	replyType = KWriteIface_ftable[14][0]; 
	down( );
    } break;
    case 15: { // void shiftDown()
	replyType = KWriteIface_ftable[15][0]; 
	shiftDown( );
    } break;
    case 16: { // void scrollUp()
	replyType = KWriteIface_ftable[16][0]; 
	scrollUp( );
    } break;
    case 17: { // void scrollDown()
	replyType = KWriteIface_ftable[17][0]; 
	scrollDown( );
    } break;
    case 18: { // void topOfView()
	replyType = KWriteIface_ftable[18][0]; 
	topOfView( );
    } break;
    case 19: { // void bottomOfView()
	replyType = KWriteIface_ftable[19][0]; 
	bottomOfView( );
    } break;
    case 20: { // void pageUp()
	replyType = KWriteIface_ftable[20][0]; 
	pageUp( );
    } break;
    case 21: { // void shiftPageUp()
	replyType = KWriteIface_ftable[21][0]; 
	shiftPageUp( );
    } break;
    case 22: { // void pageDown()
	replyType = KWriteIface_ftable[22][0]; 
	pageDown( );
    } break;
    case 23: { // void shiftPageDown()
	replyType = KWriteIface_ftable[23][0]; 
	shiftPageDown( );
    } break;
    case 24: { // void top()
	replyType = KWriteIface_ftable[24][0]; 
	top( );
    } break;
    case 25: { // void shiftTop()
	replyType = KWriteIface_ftable[25][0]; 
	shiftTop( );
    } break;
    case 26: { // void bottom()
	replyType = KWriteIface_ftable[26][0]; 
	bottom( );
    } break;
    case 27: { // void shiftBottom()
	replyType = KWriteIface_ftable[27][0]; 
	shiftBottom( );
    } break;
    case 28: { // int numLines()
	replyType = KWriteIface_ftable[28][0]; 
	QDataStream _replyStream( replyData, IO_WriteOnly );
	_replyStream << numLines( );
    } break;
    case 29: { // QString text()
	replyType = KWriteIface_ftable[29][0]; 
	QDataStream _replyStream( replyData, IO_WriteOnly );
	_replyStream << text( );
    } break;
    case 30: { // QString currentTextLine()
	replyType = KWriteIface_ftable[30][0]; 
	QDataStream _replyStream( replyData, IO_WriteOnly );
	_replyStream << currentTextLine( );
    } break;
    case 31: { // QString textLine(int)
	int arg0;
	QDataStream arg( data, IO_ReadOnly );
	arg >> arg0;
	replyType = KWriteIface_ftable[31][0]; 
	QDataStream _replyStream( replyData, IO_WriteOnly );
	_replyStream << textLine(arg0 );
    } break;
    case 32: { // QString currentWord()
	replyType = KWriteIface_ftable[32][0]; 
	QDataStream _replyStream( replyData, IO_WriteOnly );
	_replyStream << currentWord( );
    } break;
    case 33: { // QString word(int,int)
	int arg0;
	int arg1;
	QDataStream arg( data, IO_ReadOnly );
	arg >> arg0;
	arg >> arg1;
	replyType = KWriteIface_ftable[33][0]; 
	QDataStream _replyStream( replyData, IO_WriteOnly );
	_replyStream << word(arg0, arg1 );
    } break;
    case 34: { // void insertText(QString,bool)
	QString arg0;
	bool arg1;
	QDataStream arg( data, IO_ReadOnly );
	arg >> arg0;
	arg >> arg1;
	replyType = KWriteIface_ftable[34][0]; 
	insertText(arg0, arg1 );
    } break;
    case 35: { // void setCursorPosition(int,int,bool)
	int arg0;
	int arg1;
	bool arg2;
	QDataStream arg( data, IO_ReadOnly );
	arg >> arg0;
	arg >> arg1;
	arg >> arg2;
	replyType = KWriteIface_ftable[35][0]; 
	setCursorPosition(arg0, arg1, arg2 );
    } break;
    case 36: { // bool isOverwriteMode()
	replyType = KWriteIface_ftable[36][0]; 
	QDataStream _replyStream( replyData, IO_WriteOnly );
	_replyStream << isOverwriteMode( );
    } break;
    case 37: { // void setOverwriteMode(bool)
	bool arg0;
	QDataStream arg( data, IO_ReadOnly );
	arg >> arg0;
	replyType = KWriteIface_ftable[37][0]; 
	setOverwriteMode(arg0 );
    } break;
    case 38: { // int currentLine()
	replyType = KWriteIface_ftable[38][0]; 
	QDataStream _replyStream( replyData, IO_WriteOnly );
	_replyStream << currentLine( );
    } break;
    case 39: { // int currentColumn()
	replyType = KWriteIface_ftable[39][0]; 
	QDataStream _replyStream( replyData, IO_WriteOnly );
	_replyStream << currentColumn( );
    } break;
    case 40: { // bool loadFile(QString,int)
	QString arg0;
	int arg1;
	QDataStream arg( data, IO_ReadOnly );
	arg >> arg0;
	arg >> arg1;
	replyType = KWriteIface_ftable[40][0]; 
	QDataStream _replyStream( replyData, IO_WriteOnly );
	_replyStream << loadFile(arg0, arg1 );
    } break;
    case 41: { // bool writeFile(QString)
	QString arg0;
	QDataStream arg( data, IO_ReadOnly );
	arg >> arg0;
	replyType = KWriteIface_ftable[41][0]; 
	QDataStream _replyStream( replyData, IO_WriteOnly );
	_replyStream << writeFile(arg0 );
    } break;
    default: 
	return DCOPObject::process( fun, data, replyType, replyData );
    }
    return TRUE;
}

QCStringList KWriteIface::interfaces()
{
    QCStringList ifaces = DCOPObject::interfaces();
    ifaces += "KWriteIface";
    return ifaces;
}

QCStringList KWriteIface::functions()
{
    QCStringList funcs = DCOPObject::functions();
    for ( int i = 0; KWriteIface_ftable[i][2]; i++ ) {
	QCString func = KWriteIface_ftable[i][0];
	func += ' ';
	func += KWriteIface_ftable[i][2];
	funcs << func;
    }
    return funcs;
}


