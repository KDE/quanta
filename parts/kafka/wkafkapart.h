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
namespace Kate
{
	class View;
}
class Document;
class Node;
class Parser;
class kNodeAttrs;

#include <qmap.h>
#include <qobject.h>
#include <qptrdict.h>
#include "kafkahtmlpart.h"

/**
 * This class assures the synchronisation of the two trees : the quanta and the
 * kafka tree, with the help of the undo/redo class.
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

	/**
	 * @param _node The DOM::Node we want the attributess.
	 * @return Return the corresponding kNodeAttrs of the DOM::Node.
	 */
	kNodeAttrs *getAttrs(DOM::Node _domNode);

	/**
	 * Connects the domNode to the corresponding Quanta Node.
	 * @param _domNode The DOM::Node to connect to the Node.
	 * @param _node The Node to connect to the DOM::Node.
	 */
	void connectDomNodeToQuantaNode(DOM::Node _domNode, Node *_node);

	/**
	 * Disconnects the domNode from its corresponding Quanta Node.
	 * @param _domNode The DOM::Node to disconnect from its Node.
	 */
	void disconnectDomNodeFromQuantaNode(DOM::Node _domNode);

	/**
	 * Disconnects all the domNode from their Quanta Node.
	 */
	void disconnectAllDomNodes();

	/**
	 * This function build a kafka DOM:::Node from a Quanta Node.
	 * @param _node The node from which we build the DOM::Node
	 * @param insertNode Specifies if we should insert the Node.
	 * @return Returns if the insertion was successful if asked, else true.
	 */
	bool buildKafkaNodeFromNode(Node *_node, bool insertNode = true);

	/**
	 * This function returns the special XML character (e.g. space, �...)
	 * from its encoded form (e.g. &nbsp;)
	 * @return Returns the special character.
	 */
	QString getDecodedChar(QString encodedChar);

	/**
	 * This function returns the text decoded from its XML-encoded form.
	 * @param encodedText The text to decode.
	 * @return Returns the text decoded.
	 */
	QString getDecodedText(QString encodedText);

	/**
	 * This function synchronizes the Node from the DOM::Node. If the Node is a text Node,
	 * we try to keep its indentation while updating it.
	 * @param _node The Node to synchronize.
	 * @param _domNode The Node is synchronized from this DOM::Node.
	 */
	void buildNodeFromKafkaNode(Node *_node, DOM::Node _domNode);

	/**
	 * This function creates and synchronize a Node from the DOM::Node.
	 * It also create empty Nodes between Nodes.
	 * @param _domNode The Node returned is synchronized from this DOM::Node.
	 * @param _nodeParent The parent Node of the Node returned.
	 * @param addedClosingNode Returns if the closing Node was created.
	 * @param addedLeftEmptyNode Returns if an Empty Node was created next to the node returned.
	 * @param addedRightEmptyNode Returns if an Empty Node was created next to the node
	 * returned, or next to the closing Node if created.
	 * @param _nodePrev The Node the Node returned will be placed after. If null, it will
	 * be placed as the first child of _nodeParent.
	 * @param appendChild Specifies if a Node without previous Node should be placed as the last
	 * child of _nodeParent in case we don't know the previous Node.
	 * @return Returns a new Node created from the DOM::Node.
	*/
	Node * buildNodeFromKafkaNode(DOM::Node _domNode, Node *_nodeParent,
		bool &addedClosingNode, bool &addedLeftEmptyNode, bool &addedRightEmptyNode,
		Node *_nodePrev = 0L, bool appendChild = false);

	/**
	 * This function returns the XML-encoded character (e.g. &nbsp;)
	 * from the XML special character (e.g. space, �...)
	 * @param decodedChar The character to encode.
	 * @param previousDecodedchar The previous decoded character.
	 * @return Returns the XML-encoded character.
	 */
	QString getEncodedChar(QString decodedChar, QString previousDecodedChar);

	/**
	 * This function returns the text with all the special XML characters encoded.
	 * @param decodedText The text to encode.
	 * @param bLine The start line of the returned encoded text.
	 * @param bCol The start col of the returned encoded text.
	 * @param eLine Returns the end line of the returned encoded text.
	 * @param eCol Returns the end col of the return encoded text.
	 * @return Return the XML-encoded text.
	 */
	QString getEncodedText(QString decodedText, int bLine, int bCol, int &eLine, int &eCol);

	/**
	 * This function generates the code corresponding to the XML node.
	 * @param _node The Node we want to generate its code.
	 * @param bLine The start line of the returned generated code.
	 * @param bCol The start col of the returned generated code.
	 * @param eLine Returns the end line of the returned generated code.
	 * @param eCol Returns the end col of the return generated code.
	 * @return Returns the code.
	 */
	QString generateCodeFromNode(Node *_node, int bLine, int bCol, int &eLine, int &eCol);

	/**
	 * This function returns the next Node after _node.
	 * @param _node It is the Node from which we want the next Node.
	 * @param goingTowardsRootNode This boolean specifies if we should go up or down in the tree.
	 * For a normal use, It must be set to false at the
	 * beginning and then the same boolean must be used when using
	 * several times this function.
	 * @param endNode Specifies at which Node the search should end.
	 * @return Returns the next Node.
	 */
	Node *getNextNode(Node *_node, bool &goingTowardsRootNode, Node *endNode = 0L);

	/**
	 * This function search the corresponding quanta Node to the kafka DOM::Node
	 * @param _domNode The DOM::Node we seek its corresponding Node.
	 * @return The Node corresponding to _domNode.
	 */
	Node *searchCorrespondingNode(DOM::Node _domNode);

	/**
	 * Returns the cursor position in the kafka editor corresponding to the cursor
	 * position in the Quanta editor.
	 * @return Returns the cursor position in the kafka editor.
	 */
	int getKafkaCursorPosition();

	/**
	 * Returns the cursor position in the Quanta editor corresponding to the cursor
	 * position in the kafka editor.
	 * @param line Returns the line cursor position.
	 * @param col Returns the col cursor position.
	 */
	void getQuantaCursorPosition(int &line, int &col);

	/**
	 * In order to have khtml works whatever DTD is loaded, they must always exists
	 * and be valid.
	 */
	DOM::Node html, body, head;

signals:
	/**
	 * Called whenever a DOM::Node get the focus
	 */
	void newCursorPosition(int col, int row);

	/**
	 * Called whenever a DOM::Node get the focus
	 */
	void nodeGetFocus(Node *_node);

	/**
	 * Called when the wKafkaPart is loaded.
	 */
	void loaded();

	/**
	 * Called when the wKafkaPart is unloaded.
	 */
	void unloaded();

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
	 * Called whenever a DOM::Node is inserted in the Kafka tree.
	 * @param _domNode is the Node inserted.
	 * @param insertChilds Specifies if the _domNode's child should be inserted
	 */
	void slotDomNodeInserted(DOM::Node _domNode, bool insertChilds);

	/**
	 * Called whenever DOM::Node's attributes are modified.
	 * @param _domNode is the Node modified.
	 */
	void slotDomNodeModified(DOM::Node _domNode);

	/**
	 * Called whenever a DOM::Node is about to be removed from the Kafka tree.
	 * @param _domNode is the Node to be deleted.
	 * @param deleteChilds Specifies if we should delete the child nodes of _node
	 */
	void slotDomNodeAboutToBeRemoved(DOM::Node _domNode, bool deleteChilds);

	/**
	 * Called whenever a DOM::Node get the focus
	 */
	void slotdomNodeNewCursorPos(DOM::Node _domNode, int offset);

	/**
	 * Called whenever the kafkaHTMLPart widget get/lost the focus.
	 */
	 void slotKafkaGetFocus(bool focus);

	 /**
	  * Called whenever the KTextEditor::View widget get the focus.
	  */
	  void slotQuantaGetFocus(Kate::View *view);

protected:

	/**
	 * The timer event, called by the kafkaUpdateTimer and quantaTimerUpdate which
	 * triggers the update of the other view.
	 */
	virtual void timerEvent(QTimerEvent *e );

private:
	/**
	 * Fits the Nodes position when a change occurs in the kafka tree.
	 * @param _startNode The Node where the update starts
	 * @param colMovement The number of column that should be
	 * added/retrieved from the column position
	 * @param lineMovement The number of lines that should be
	 * added/retrieved from the line position
	 * @param colEnd The column position where the update should stop
	 * @param lineEnd The line position where the update should stop
	 */
	void fitsNodesPosition(Node* _startNode, int colMovement,
		int lineMovement = 0, int colEnd = -2, int lineEnd = -2);

	/** For debugging purpose. It prints the quanta internal tree to stdout */
	void coutTree(Node *node, int indent);

	int kafkaUpdateTimer;
	int quantaUpdateTimer;
	QMap<QString, QString> decodedChars;
	QMap<QString, QString> encodedChars;
	QPtrDict<kNodeAttrs> domNodeProps;
	KafkaHTMLPart *_kafkaPart;
	Document *_currentDoc;

	bool _docLoaded;

};

#endif
