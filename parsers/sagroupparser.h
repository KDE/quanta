/***************************************************************************
                          sagroupparser.h  -  description
                             -------------------
    begin                : Wed Feb 11 2004
    copyright            : (C) 2004 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SAGROUPPARSER_H
#define SAGROUPPARSER_H

//qt includes
#include <qobject.h>

//forward definitions
class QTimer;
class Document;
class Node;
class SAParser;

/**
  This class is used to parse for special area (script) groups in the node tree.
 */
class SAGroupParser : public QObject
{
Q_OBJECT
public:
  public:
    SAGroupParser(SAParser *parent, Document *write, Node *startNode, Node *endNode, bool synchronous, bool parsingLastNode, bool paringLastGroup);
    ~SAGroupParser() {};

    QTimer *m_parseForGroupTimer;

  public slots:
    void slotParseForScriptGroup();

  signals:
    void rebuildStructureTree(bool);
    void cleanGroups();
    void parsingDone(SAGroupParser*);

  private:
    void parseForScriptGroup(Node *node);

    bool m_lastGroupParsed;
    bool m_parsingLastNode;
    bool m_synchronous;
    SAParser *m_parent;
    Node* g_node;
    Node* g_endNode;
    Document *m_write;
    int m_count;
};

#endif
