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

class KConfig;
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
class NodeModifsSet;
class NodeEnhancer;
class DTDStruct;

#include <qmap.h>
#include <qobject.h>
#include <qptrdict.h>

#include "kafkahtmlpart.h"

/**
 * This class assures the synchronisation of the two trees : the quanta and the
 * kafka tree.
 * By Quanta Node, i mean Node (cf quanta/parser/node.h)
 * By Kafka Node, i mean DOM::Node (cf <dom/dom_node.h>)
 */
class KafkaDocument : public QObject
{
Q_OBJECT
public:

	/**
	 * Create a KafkaWidget.
	 */
	KafkaDocument(QWidget *parent, QWidget *widgetParent, const char *name);
	~KafkaDocument();


	/* ----------------------------------- LOADING/UNLOADING -------------------------------------------------*/

	/**
	 * Builds kafka's own tree from the Quanta tree.
	 * If the current Document is empty, it will create a minimum tree.
	 * @param the Document we want to load.
	 */
	void loadDocument(Document *doc);

	/**
	 * Unloads the kafka tree.
	 */
	void unloadDocument();

	/**
	 * Reloads the Document.
	 */
	void reloadDocument();

	/**
	 * Called to get KafkaDocument's state.
	 * @return Returns true if KafkaDocument is loaded.
	 */
	bool isLoaded() {return _docLoaded;}


	/* ----------------------------------- KAFKA<->QUANTA NODES LINK ----------------------------------*/

	/**
	 * @param _node The DOM::Node we want the attributess.
	 * @return Return the corresponding kNodeAttrs of the DOM::Node.
	 */
	kNodeAttrs *getAttrs(DOM::Node _domNode);

	/**
	 * This function search the corresponding quanta Node to the kafka DOM::Node
	 * @param _domNode The DOM::Node we seek its corresponding Node.
	 * @return The Node corresponding to _domNode.
	 */
	Node *getNode(DOM::Node _domNode);

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


	/* --------------------------- KAFKA/QUANTA NODES CREATION -----------------------------*/

	/**
	 * This function build a kafka DOM:::Node from a Quanta Node.
	 * @param _node The node from which we build the DOM::Node
	 * @param insertNode Specifies if we should insert the Node.
	 * @return Returns if the insertion was successful if asked, else true.
	 */
	bool buildKafkaNodeFromNode(Node *_node, bool insertNode = true);

	/**
	 * This function synchronizes the Node from the DOM::Node. If the Node is a text Node,
	 * we try to keep its indentation while updating it.
	 * @param _node The Node to synchronize.
	 * @param _domNode The Node is synchronized from this DOM::Node.
	 */
	void buildNodeFromKafkaNode(Node *_node, DOM::Node _domNode);

	/**
	 * This function creates and synchronize a Node from the DOM::Node. It adds
	 * the closing Node if necessary, and the node and its closing Node can surround Nodes
	 * and thus make them its childs. Usefull when adding a Node on a selected range of Nodes.
	 * It also create empty Nodes between Nodes.
	 * @param _domNode The Node returned is synchronized from this DOM::Node.
	 * @param _nodeParent The parent Node of the Node returned.
	 * @param _beginNode The new Node will be placed before or within _beginNode.
	 * @param beginOffset NOT IMLEMENTED If set to 0 or -1, the new Node will be placed before _beginNode,
	 * else _beginNode will be splitted at offset #beginOffset and the new Node will be placed
	 * inbetween.
	 * @param _endNode NOT IMPLEMENTED If not null and if the new Node has a closing tag, set the closing node
	 * after or within _endNode.
	 * @param endOffset NOT IMPLEMENTED If set to -1, the closing node will be placed after _endNode, else _endNode
	 * will be splitted at offset #endOffset and the closing Node will be placed inbetween.
	 * @param modifs The NodeModifSet to log the changes made.
	 * @return Returns the new main Node created from the DOM::Node.
	*/
	Node * buildNodeFromKafkaNode(DOM::Node _domNode, Node *_nodeParent,
		Node *_beginNode, int beginOffset, Node *_endNode, int endOffset,
		NodeModifsSet *modifs);


	/* ------------------------------- TEXT ENTITIES ENCODING/DECODING ---------------------*/

	/**
	 * This function returns the special XML character (e.g. space, �...)
	 * from its encoded form (e.g. &nbsp;)
	 * @return Returns the special character.
	 */
	QString getDecodedChar(const QString &encodedChar);

	/**
	 * This function returns the text decoded from its XML-encoded form.
	 * @param encodedText The text to decode.
	 * @return Returns the text decoded.
	 */
	QString getDecodedText(const QString &encodedText);

	/**
	 * This function returns the XML-encoded character (e.g. &nbsp;)
	 * from the XML special character (e.g. space, �...)
	 * @param decodedChar The character to encode.
	 * @param previousDecodedchar The previous decoded character.
	 * @return Returns the XML-encoded character.
	 */
	QString getEncodedChar(const QString &decodedChar, const QString &previousDecodedChar);

	/**
	 * This function returns the text with all the special XML characters encoded.
	 * @param decodedText The text to encode.
	 * @param bLine The start line of the returned encoded text.
	 * @param bCol The start col of the returned encoded text.
	 * @param eLine Returns the end line of the returned encoded text.
	 * @param eCol Returns the end col of the return encoded text.
	 * @return Return the XML-encoded text.
	 */
	QString getEncodedText(const QString &decodedText, int bLine, int bCol, int &eLine, int &eCol);

	/**
	 * This function behaves essentially like the above function except that it doesn't
	 * return the position of the encoded text.
	 */
	QString getEncodedText(const QString &decodedText);

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


	/* ----------------------------------- KAFKA<->QUANTA POSITION TRANSLATION -------------------------------*/

	/**
	 * Returns the kafka cursor position corresponding to the quanta cursor position.
	 * @param line The quanta line cursor position.
	 * @param col The quanta column cursor position.
	 * @param domNode Returns the DOM::Node in which the cursor is located.
	 * @param offset Returns the offset of the cursor.
	 */
	void translateQuantaIntoKafkaCursorPosition(uint line, uint col, DOM::Node &domNode, int &offset);

	/**
	 * Returns the quanta cursor position corresponding to the kafka cursor position.
	 * @param domNode The kafka DOM::Node cursor position.
	 * @param offset The kafka offset cursor position.
	 * @param line Returns the line cursor position.
	 * @param col Returns the col cursor position.
	 */
	void translateKafkaIntoQuantaCursorPosition(DOM::Node domNode, int offset, int &line, int &col);


	/* --------------------------------- MISCELLANEOUS ------------------------------------------*/

	/**
	 * Read the config.
	 * @param m_config The config to read.
	 */
	 void readConfig(KConfig *m_config);

	/**
	 * Returns the default DTD of the current Document.
	 */
	DTDStruct* defaultDTD();

	/**
	 * @return Returns the current KafkaWidget.
	 */
	KafkaWidget *getKafkaWidget() {return m_kafkaPart;}

	/**
	 * @return Returns the current Document.
	 */
	Document *getCurrentDoc() {return m_currentDoc;}

	/**
	 * Prints in stdout the current Node tree + Node->DOM::Node->Node relationship.
	 * @node The startNode
	 * @indent The number of little dots per parent relationship.
	 */
	void coutLinkTree(Node *node, int indent);

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

public:
	//use a QPtrList aferwards
	NodeEnhancer *mainEnhancer;

private:
	QMap<QString, QString> decodedChars;
	QMap<QString, QString> encodedChars;
	QPtrDict<kNodeAttrs> domNodeProps;
	KafkaWidget *m_kafkaPart;
	Document *m_currentDoc;
	bool _docLoaded;

};

#endif
