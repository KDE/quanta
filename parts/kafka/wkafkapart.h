/***************************************************************************
                               wkafkapart.h
                             -------------------

    copyright            : (C) 2003 - Nicolas Deschildre
    email                : nicolasdchd@ifrance.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef WKAFKAPART_H
#define WKAFKAPART_H

namespace DOM
{
	class Node;
}
class KafkaHTMLPart;
class Document;
class Node;
class Parser;

#include <qmap.h>
#include <qobject.h>

#include "kafkahtmlpart.h"

/**
 * This class assures the synchronisation of the two trees : the quanta and the
 * kafka tree.
 */
class WKafkaPart : public QObject
{
Q_OBJECT
public:
	WKafkaPart(QWidget *parent, QWidget *widgetParent, const char *name);
	~WKafkaPart();

	/**
	 * Builds kafka's own tree from the Quanta tree.
	 * If the current Document is empty, it will create a minimum tree.
	 * @param the Document we want to load.
	 */
	void loadDocument(Document *doc);

	/**
	 * Unloads kafka tree.
	 */
	void unloadDocument();

	/**
	 * Called to get WKafkaPart's state.
	 * @return Returns true if WKafkaPart is loaded.
	 */
	bool isLoaded() {return _docLoaded;}

	/**
	 * @return Returns the current KafkaHTMLPart.
	 */
	KafkaHTMLPart *getKafkaPart() {return _kafkaPart;}

	/**
	 * @return Returns the current Document.
	 */
	Document *getCurrentDoc() {return _currentDoc;}

public slots:
	/**
	 * Updates the Kafka tree when possible from the modified Quanta tree,
	 * else reloads the kafka tree.
	 * @param tryToUpdate Specifies wheter or not we should try to update
	 * the Kafka tree instead of rebuilding it from scratch.
	 */
	void slotUpdateKafkaTree(bool tryToUpdate = true);

	/**
	 * Updates the Quanta tree from the modified Kafka tree.
	 */
	void slotUpdateQuantaTree();

	/**
	 * Called whenever a DOM::Node is inserted in the Quanta tree.
	 */
	void slotDomNodeInserted(DOM::Node _domNode);

	/**
	 * Called whenever DOM::Node's attributes are modified.
	 */
	void slotDomNodeModified(DOM::Node _domNode);

	/**
	 * Called whenever a DOM::Node is about to be removed from the Quanta tree.
	 */
	void slotDomNodeAboutToBeRemoved(DOM::Node _domNode);

	/**
	 * Called whenever a DOM::Node get the focus
	 */
	void slotDomNodeGetFocus(DOM::Node _domNode);

signals:
	/**
	 * Called when a DOM::Node get the focus
	 */
	void newCursorPosition(int col, int row);

private:
	QString getSpecialChar(QString encodedChar);
	QString getEncodedChar(QString specialChar, bool encodeWhiteSpaces = true);
	QMap<QString, QString> specialChars;
	QMap<QString, QString> encodedChars;
	KafkaHTMLPart *_kafkaPart;
	Document *_currentDoc;
	Node *_rootNode;
	void coutTree(Node *node, int indent);
	bool _docLoaded;
	void synchronizeXMLTag(Node* _node);
	void synchronizeTextTag(Node* _node);
	Node *getNextNode(Node *_node, bool &goingTowardsRootNode);
	Node *searchCorrespondingNode(DOM::Node _domNode);
};

#endif
