
/***************************************************************************
                          xsldbgevent.h  -  event to notify app of
                                                   data from xsldbg
                             -------------------
    begin                : Fri Feb 1 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef XSLDBGEVENT_H
#define XSLDBGEVENT_H

#if defined WIN32
#include <libxsldbg/xsldbgwin32config.h>
#endif

#include <qevent.h>
#include <qlist.h>

#include "xsldbgmsg.h"

/* how many columns do we have */
#define XSLDBGEVENT_COLUMNS 3

class  XsldbgDebuggerBase;
class  XsldbgEventData;

class  XsldbgEventDataList : public QGList
{
public:
    XsldbgEventDataList(void)                             {}
    XsldbgEventDataList( const XsldbgEventDataList &l ) : QGList(l) {}
   ~XsldbgEventDataList(void)                             { clear(); }
    XsldbgEventDataList &operator=(const XsldbgEventDataList &l)
                        { return (XsldbgEventDataList&)QGList::operator=(l); }
    bool operator==( const XsldbgEventDataList &list ) const
    { return QGList::operator==( list ); }
    uint  count(void)   const               { return QGList::count(); }
    bool  isEmpty(void) const               { return QGList::count() == 0; }
    bool  insert( uint i, const XsldbgEventData *d){ return QGList::insertAt(i,(QCollection::Item)d); }
    void  inSort( const XsldbgEventData *d )       { QGList::inSort((QCollection::Item)d); }
    void  prepend( const XsldbgEventData *d )      { QGList::insertAt(0,(QCollection::Item)d); }
    void  append( const XsldbgEventData *d )       { QGList::append((QCollection::Item)d); }
    bool  remove( uint i )              { return QGList::removeAt(i); }
    bool  remove(void)                      { return QGList::remove((QCollection::Item)0); }
    bool  remove( const XsldbgEventData *d )       { return QGList::remove((QCollection::Item)d); }
    bool  removeRef( const XsldbgEventData *d )    { return QGList::removeRef((QCollection::Item)d); }
    void  removeNode( QLNode *n )       { QGList::removeNode(n); }
    bool  removeFirst(void)                 { return QGList::removeFirst(); }
    bool  removeLast(void)                  { return QGList::removeLast(); }
    XsldbgEventData *take( uint i )                { return (XsldbgEventData *)QGList::takeAt(i); }
    XsldbgEventData *take(void)                        { return (XsldbgEventData *)QGList::take(); }
    XsldbgEventData *takeNode( QLNode *n )         { return (XsldbgEventData *)QGList::takeNode(n); }
    void  clear(void)                       { QGList::clear(); }
    void  sort(void)                        { QGList::sort(); }
    int   find( const XsldbgEventData *d )         { return QGList::find((QCollection::Item)d); }
    int   findNext( const XsldbgEventData *d )     { return QGList::find((QCollection::Item)d,FALSE); }
    int   findRef( const XsldbgEventData *d )      { return QGList::findRef((QCollection::Item)d); }
    int   findNextRef( const XsldbgEventData *d ){ return QGList::findRef((QCollection::Item)d,FALSE);}
    uint  contains( const XsldbgEventData *d ) const { return QGList::contains((QCollection::Item)d); }
    uint  containsRef( const XsldbgEventData *d ) const
                                        { return QGList::containsRef((QCollection::Item)d); }
    XsldbgEventData *at( uint i )                  { return (XsldbgEventData *)QGList::at(i); }
    int   at(void) const                    { return QGList::at(); }
    XsldbgEventData *current(void)  const              { return (XsldbgEventData *)QGList::get(); }
    QLNode *currentNode(void)  const        { return QGList::currentNode(); }
    XsldbgEventData *getFirst(void) const              { return (XsldbgEventData *)QGList::cfirst(); }
    XsldbgEventData *getLast(void)  const              { return (XsldbgEventData *)QGList::clast(); }
    XsldbgEventData *first(void)                       { return (XsldbgEventData *)QGList::first(); }
    XsldbgEventData *last(void)                        { return (XsldbgEventData *)QGList::last(); }
    XsldbgEventData *next(void)                        { return (XsldbgEventData *)QGList::next(); }
    XsldbgEventData *prev(void)                        { return (XsldbgEventData *)QGList::prev(); }
    void  toVector( QGVector *vec )const{ QGList::toVector(vec); }
private:
    void  deleteItem( QCollection::Item d );
};






/**
 * This class is used to convert a message from xsldbg into a simple data type
 *
 * @short convertor of xsldbg message to a data class
 *
 * @author Keith Isdale <k_isdale@tpg.com.au>
 */
class  XsldbgEventData {

 public:
  XsldbgEventData(void);
  ~XsldbgEventData(void);


  /**
   * Set the text for the column specified
   *
   * @param column 0 =< @p column < XSLDBGEVENT_COLUMNS
   * @param text The text value to store in column indicated
   */
  void setText(int column, QString text);


  /**
   * Get the text from the column specified
   *
   * @returns QString::null if invalid column number
   *
   * @param column 0 =< @p column < XSLDBGEVENT_COLUMNS
   *
   */
  QString getText(int column);


  /**
   * Set the integer value for the column specified
   *
   * @param column 0 =< @p column < XSLDBGEVENT_COLUMNS
   * @param value The value to store in column indicated
   */
  void setInt(int column, int value);


  /**
   * Get the integer value from the column specified
   *
   * @returns -1 if invalid column number
   *
   * @param column 0 =< @p column < XSLDBGEVENT_COLUMNS
   *
   */
  int  getInt(int column);

 private:
  /** Below are the messages that this class will support
      Values are mapped left to right ie the first QString value maps
      to textValues[0], the second mapps to textValues[1]
      the third maps to textValues[2] etc.. */
  QString textValues[XSLDBGEVENT_COLUMNS];

  /**
     Both int and bool values are mapped to intValues in the same manner as
     stated above */
  int intValues[XSLDBGEVENT_COLUMNS];

  /** - - - - - - The message/signal types supported   - - - - - - */
  // /** line number and/or file name changed */
  //  void lineNoChanged(QString /* fileName */, int /* lineNumber */, bool /* breakpoint */);
  // These data items are mapped to attributes of this class with the same name


  //	/** Show a message in debugger window */
  //	void showMessage(QString /* msg*/);
  //   These data item is mapped to the text attribute


  //	/** Add breakpoint to view, First parameter is QString::null
  //			to indicate start of breakpoint list notfication */
  //	void breakpointItem(QString /* fileName*/, int /* lineNumber */, QString /*templateName*/,
  //                                        bool /* enabled */, int /* id */);
  // These data items are mapped to attributes of this class with the same name


  //	/** Add global variable to view, First parameter is QString::null
  //        to indicate start of global variable list notfication */
  //	void globalVariableItem(QString /* name */, QString /* fileName */, int /* lineNumber */);
  // These data items are mapped to attributes of this class with the same name


  //	/** Add local variable to view, First parameter is QString::null
  //			to indicate start of local variable list notfication */
  //	void localVariableItem(QString /*name */, QString /* templateContext*/,
  //								QString /* fileName */, int /*lineNumber */);
  // These data items are mapped to attributes of this class with the same name


  //	/** Add template to view, First parameter is QString::null
  //			to indicate start of template list notfication */
  //	void templateItem(QString /* name*/, QString /*mode*/, QString /* fileName */, int /* lineNumber */);

  //	/** Add source to view, First parameter is QString::null
  //			to indicate start of source list notfication */
  //	void sourceItem(QString /* fileName */, QString /* parentFileName */, int /*lineNumber */);

  //	/** Add parameter to view, First parameter is QString::null
  //			to indicate start of parameter list notfication */
  //	void parameterItem(QString /* name*/, QString /* value */);

  //	/** Add callStack to view, First parameter is QString::null
  //			to indicate start of callstack list notfication */
  //	void callStackItem(QString /* tempalteName*/, QString /* fileName */, int /* lineNumber */);

  //	/** Add entity to view, First parameter is QString::null
  //			to indicate start of entity list notfication */
  //	void entityItem(QString /*SystemID*/,  QString /*PublicID*/);

  //	/* Show the URI for SystemID or PublicID requested */
  //	void resolveItem(QString /*URI*/);

  //    /* Display a integer option value First parameter is QString::null
  //			to indicate start of option list notification */
  //   void intOptionItem(QString /* name*/, int /* value */);

  //    /* Display a string option value. First parameter is QString::null
  //			to indicate start of option list notification */
  //   void stringOptionItem(QString /* name*/, QString /* value */);

};


/**
 * This class is posted to the applications event queue. When the application
 *  has time to process the event this class then aids in emitting
 *   the relavant signals for the event.
 *
 * @short Emit signals to QT application via debugger base class
 *
 * @author Keith Isdale <k_isdale@tpg.com.au>
 */
class  XsldbgEvent : public QEvent {

 public:
  XsldbgEvent(XsldbgMessageEnum type, const void *data);
  ~XsldbgEvent(void);

  /** Main control for emitting messages, use this from the application
      inside its event processing function */
  void emitMessage(XsldbgDebuggerBase *debugger);

  /** Emit a single message. It uses handleXXX to do the actual emitting
     of signal from debugger */
  void emitMessage(XsldbgEventData *eventData);

 private:
  /** Create the XsldbgEventData for this message. Is used by our constructor
     it uses handleXXX function to fill in the approriate values in
     the XsldbgEventData provided */
  XsldbgEventData * createEventData(XsldbgMessageEnum type, const  void *msgData);

  /** The following functions are directly related to the eventual signals that
      will be emitted ie the signal
             lineNoChanged(QString, int bool)
	is mapped to
	      handleLineNoChanged(XsldbgEventData *, void *)
    */
  void handleLineNoChanged(XsldbgEventData *eventData, const  void *msgData);
  void handleShowMessage(XsldbgEventData *eventData, const  void *msgData);
  void handleBreakpointItem(XsldbgEventData *eventData, const  void *msgData);
  void handleGlobalVariableItem(XsldbgEventData *eventData, const  void *msgData);
  void handleLocalVariableItem(XsldbgEventData *eventData, const  void *msgData);
  void handleTemplateItem(XsldbgEventData *eventData, const  void *msgData);
  void handleSourceItem(XsldbgEventData *eventData, const  void *msgData);
  void handleIncludedSourceItem(XsldbgEventData *eventData, const  void *msgData);
  void handleParameterItem(XsldbgEventData *eventData, const  void *msgData);
  void handleCallStackItem(XsldbgEventData *eventData, const  void *msgData);
  void handleEntityItem(XsldbgEventData *eventData, const  void *msgData);
  void handleResolveItem(XsldbgEventData *eventData, const  void *msgData);
  void handleIntOptionItem(XsldbgEventData *eventData, const  void *msgData);
  void handleStringOptionItem(XsldbgEventData *eventData, const  void *msgData);


 private:

  /** What type is the items in list */
  XsldbgMessageEnum itemType;

  /** A flag that gets set once the list has been filled with approriate
    XsldbgEventData */
  bool beenCreated;

  /** This is a volitile value that is only valid for the duration
    of the constuctor. It will be set to 0L immediately after */
  const void *data;

  /** This is a volitile value only valid for duration of emitMessage
     function. It will be set to 0L imedediately after */
  XsldbgDebuggerBase *debugger;

  /** This is the data associated with this event
      each data item in the list will be of the type required
      by the "type" this event
   */
   class  XsldbgEventDataList list;
    };




#endif
