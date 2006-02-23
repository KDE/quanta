/***************************************************************************
                               wkafkapart.h
                             -------------------

    copyright            : (C) 2003, 2004 - Nicolas Deschildre
    email                : ndeschildre@kdewebdev.org
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

class Document;
class Node;
class Parser;
class kNodeAttrs;
class NodeModifsSet;
class NodeEnhancer;
struct DTDStruct;
class NodeSelectionInd;
class NodeModifsSet;

#include <qmap.h>
#include <qobject.h>
#include <qptrdict.h>

#include "kafkahtmlpart.h"

/**
 * This class assures the synchronisation of the two trees : the quanta and the
 * kafka tree.
 * By Quanta Node, i mean Node (cf quanta/parser/node.h)
 * By Kafka Node, i mean DOM::Node (cf <dom/dom_node.h>)
 * It is a singleton class.
 */
class KafkaDocument : public QObject
{
Q_OBJECT
public:
        /** Returns a reference to the KafkaDocument object */
        static KafkaDocument* const ref(QWidget *parent = 0L, QWidget *widgetParent = 0L, const char *name = 0L)
        {
          static KafkaDocument *m_ref;
          if (!m_ref) m_ref = new KafkaDocument(parent, widgetParent, name);
          return m_ref;
        }

	~KafkaDocument();


	/* ----------------------------- LOADING/UNLOADING -----------------------------------------*/

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
	 * Connects the domNode to the corresponding Quanta Node with an intermediate class :
	 * a kNodeAttrs which links the node and the DOM::Node.
	 * This is MANDATORY, even if node is null, to use this function at each
	 * DOM::Node insertion !!
	 * @param domNode The DOM::Node to connect to the Node.
	 * @param node The Node to connect to the DOM::Node. Can be null.
	 * @return Returns the kNodeAttr which links them.
	 */
	kNodeAttrs* connectDomNodeToQuantaNode(DOM::Node domNode, Node *node);

	/**
	 * Disconnects the domNode from its corresponding Quanta Node.
	 * @param _domNode The DOM::Node to disconnect from its Node.
	 */
	void disconnectDomNodeFromQuantaNode(DOM::Node _domNode);

	/**
	 * Disconnects all the domNode from their Quanta Node.
	 */
	void disconnectAllDomNodes();
                
        
    /* --------------------------- CURSOR AND SELECTION  HANDLING --------------------------*/

        /**
         * Set the cursor and selection coordinates.
         * @param nodeSelection The coordinates of the selection and cursor.
         */
        void setCursorAndSelection(NodeSelectionInd *nodeSelection);
         
        void setCursor(Node* node, int cursorOffset);

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
	 * @param node The Node to synchronize.
	 * @param domNode The Node is synchronized from this DOM::Node.
	 */
	void buildNodeFromKafkaNode(Node *node, DOM::Node domNode);

	/**
	 * This function creates and synchronize a Node from the DOM::Node. It adds
	 * the closing Node if necessary, and the node and its closing Node can surround Nodes
	 * and thus make them its childs. Usefull when adding a Node on a selected range of Nodes.
	 * It also create empty Nodes between Nodes.
	 * @param domNode The Node returned is synchronized from this DOM::Node.
	 * @param nodeParent The parent Node of the Node returned.
	 * @param beginNode The new Node will be placed before or within _beginNode.
	 * @param beginOffset NOT IMLEMENTED If set to 0 or -1, the new Node will be placed before _beginNode,
	 * else _beginNode will be splitted at offset #beginOffset and the new Node will be placed
	 * inbetween.
	 * @param endNode NOT IMPLEMENTED If not null and if the new Node has a closing tag, set the closing node
	 * after or within endNode.
	 * @param endOffset NOT IMPLEMENTED If set to -1, the closing node will be placed after _endNode, else _endNode
	 * will be splitted at offset #endOffset and the closing Node will be placed inbetween.
	 * @param modifs The NodeModifSet to log the changes made.
	 * @return Returns the new main Node created from the DOM::Node.
	*/
	Node * buildNodeFromKafkaNode(DOM::Node domNode, Node *nodeParent,
		Node *beginNode, int beginOffset, Node *endNode, int endOffset,
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
	 * @param translateWhiteSpacesAndLineBreaks Specifies if whiteSpaces and line breaks should be
	 * compressed. Set it to false if we are inside PRE tag.
	 * @param removeLeftWhitespaces Specify if we should remove ALL whitespaces at the left of the string
	 * e.g. if the current text is the first child of a BLOCK Node (e.g. P)
	 * @param removeLeftWhitespaces Specify if we should remove ALL whitespaces at the right of the string
	 * e.g. if the current text is the last child of a BLOCK Node (e.g. P)
	 * @return Returns the text decoded.
	 */
	QString getDecodedText(const QString &encodedText, bool translateWhiteSpacesAndLineBreaks = true,
		bool removeLeftWhitespaces = false, bool removeRightWhitespaces = false);

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
	 * @param translateWhiteSpaces Specifies if we should translate the whitespaces
	 * into &nbsp; Set it to false for PRE Tag.
	 * @return Return the XML-encoded text.
	 */
	QString getEncodedText(const QString &decodedText, int bLine, int bCol, int &eLine, int &eCol,
		bool translateWhiteSpaces = true);

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
         * @param encodeText Specify, if node is a Text Node, if we should encode the text 
         * (e.g. whitespace => &nbsp;)
	 * @return Returns the code.
	 */
	QString generateCodeFromNode(Node *node, int bLine, int bCol, int &eLine, int &eCol, 
          bool encodeText = true);


	/* ------------------------- KAFKA<->QUANTA POSITION TRANSLATION -------------------------------*/

	/**
	 * Returns the kafka cursor position corresponding to the quanta cursor position.
	 * @param line The quanta line cursor position.
	 * @param col The quanta column cursor position.
	 * @param domNode Returns the DOM::Node in which the cursor is located.
	 * @param offset Returns the offset of the cursor.
	 */
	void translateQuantaIntoKafkaCursorPosition(uint line, uint col, DOM::Node &domNode, long &offset);
        
        /**
         * Returns the internal Node offset corresponding to the Quanta cursor position.
         * @param line The quanta line cursor position.
         * @param col The quanta column cursor position.
         * @param node Returns the node where is located the node internal offset.
         * @param offset Returns the node internal corresponding offset.
         */
    void translateQuantaIntoNodeCursorPosition(uint line, uint col, Node **node, long &offset);

	/**
	 * Returns the internal Node offset corresponding to the kafka cursor position.
	 * @param domNode The kafka DOM::Node cursor position.
	 * @param domNodeOffset The kafka offset cursor position.
         * @param node Returns the node where is located the node internal offset.
         * @param offset Returns the node internal corresponding offset.
	 */
	void translateKafkaIntoNodeCursorPosition(DOM::Node domNode, long domNodeOffset, Node **node, long &offset);

	/**
	 * Returns the quanta cursor position corresponding to the kafka cursor position.
	 * @param domNode The kafka DOM::Node cursor position.
	 * @param offset The kafka offset cursor position.
	 * @param line Returns the line cursor position.
	 * @param col Returns the col cursor position.
	 */
	void translateKafkaIntoQuantaCursorPosition(DOM::Node domNode, int offset, int &line, int &col);

	/**
	 * Returns the kafka cursor position corresponding to the internal Node offset.
	 * @param node The node.
	 * @param offset The internal offset of Node.
	 * @param domNode Returns the corresponding DOM::Node of node. Can be null.
	 * @param domNodeOffset Returns the offset inside the DOM::Node.
	 */
	void translateNodeIntoKafkaCursorPosition(Node *node, int offset, DOM::Node &domNode,
		long &domNodeOffset);
                
        /**
         * Returns the quanta cursor position corresponding to the internal Node offset.
         * @param node The node.
         * @param offset The internal offset of Node.
         * @param line Returns the line position of the corresponding Quanta cursor.
         * @param col Returns the col position of the corresponding Quanta cursor.
         */      
        void translateNodeIntoQuantaCursorPosition(Node *node, int offset, uint &line, uint &col);
         

	/** ----------------- DOM::NODE TREE MODIFICATIONS --------------------*/

	/**
	 * Insert a DOM::Node in the DOM::Node tree. It takes care to handle the exceptions and
	 * to postEnhance (cf htmlenhancer.h)
	 * @param node The node to insert.
	 * @param parent The new parent of node. If null, insert node at the top level.
	 * @param nextSibling The new next sibling of node. If null, append node at the end of the child list.
	 * @param rootNode The root DOM::Node of the DOM::Node tree. Useful when no parent is provided.
	 * @return Returns true if the operation was successfull.
	 */
	bool insertDomNode(DOM::Node node, DOM::Node parent = DOM::Node(),
		DOM::Node nextSibling = DOM::Node(), DOM::Node rootNode = DOM::Node());


	/**
	 * Removes a DOM::Node from the DOM::Node Tree. It takes care to handle the exceptions
	 * and to postUnenhance (cf htmlenhancer.h)
	 * @param node The Node to remove from the tree.
	 * @retun Returns true if the operation was successfull..
	 */
	bool removeDomNode(DOM::Node node);


	/** ------------------ DOM::NODE TREE NAVIGATION -----------------------------------------*/

	/**
	 * @param domNode The DOM::Node the search starts from.
	 * @return Returns the next previous sibling which has no special behavior (cf htmlenhancer.h)
	 * Sibling here (contrary to node.h) has the same meaning as in dom/dom_node.h
	 */
	DOM::Node getPrevSiblingNSpecial(DOM::Node domNode);

	/**
	 * @param domNode The DOM::Node the search starts from.
	 * @return Returns the next next sibling which has no special behavior (cf htmlenhancer.h)
	 * Sibling here (contrary to node.h) has the same meaning as in dom/dom_node.h
	 */
	DOM::Node getNextSiblingNSpecial(DOM::Node domNode);

	/* --------------------------------- MISCELLANEOUS ------------------------------------------*/

	/**
	 * Read the config.
	 * @param m_config The config to read.
	 */
	 void readConfig(KConfig *m_config);

	/**
	 * Returns the default DTD of the current Document.
	 */
	const DTDStruct* defaultDTD();

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
	 * Emitted when an error occurs when loading kafka. Can be called multiple times.
	 * @param node The node which caused this error.
	 */
	void loadingError(Node *node);

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
	 * @param domNode is the Node inserted.
	 * @param insertChilds Specifies if the _domNode's child should be inserted
         * @param modifs The changes made are logged into modifs.
	 */
	void slotDomNodeInserted(DOM::Node domNode, bool insertChilds, NodeModifsSet *modifs);

	/**
	 * Called whenever DOM::Node's attributes are modified.
	 * @param domNode is the Node modified.
         * @param modifs The changes made are logged into modifs.
	 */
	void slotDomNodeModified(DOM::Node domNode, NodeModifsSet *modifs);

	/**
	 * Called whenever a DOM::Node is about to be removed from the Kafka tree.
	 * @param domNode is the Node to be deleted.
	 * @param deleteChilds Specifies if we should delete the child nodes of _node
         * @param modifs The changes made are logged into modifs.
	 */
	void slotDomNodeAboutToBeRemoved(DOM::Node domNode, bool deleteChilds, NodeModifsSet *modifs);

	/**
	 * Called whenever a DOM::Node is moved.
	 * @param domNode The DOM::Node to move.
	 * @param newParent The new parent DOM::Node of domNode.
	 * @param before domNode will be inserted before before.
         * @param modifs The changes made are logged into modifs.
	 */
	void slotDomNodeIsAboutToBeMoved(DOM::Node domNode, DOM::Node newParent, DOM::Node before, NodeModifsSet *modifs);

	/**
	 * Called whenever a DOM::Node get the focus
	 */
	void slotdomNodeNewCursorPos(DOM::Node _domNode, int offset);
    
    void slotCut();
    void slotCopy();
    void slotPaste();

    void slotCut(Node* startNode, int startOffset, Node* endNode, int endOffset, 
                 Node** cursorNode, long cursorOffset, QString const& plainText);
    void slotCopy(Node* startNode, int startOffset, Node* endNode, int endOffset, QString const& plainText);

public:
	//use a QPtrList aferwards
	NodeEnhancer *mainEnhancer;
    
private:
	/**
	 * Create a KafkaWidget.
	 */
	KafkaDocument(QWidget *parent, QWidget *widgetParent, const char *name);

  QMap<QString, QString> decodedChars;
	QMap<QString, QString> encodedChars;
	QPtrDict<kNodeAttrs> domNodeProps;
	QGuardedPtr<KafkaWidget> m_kafkaPart;
	Document *m_currentDoc;
	bool _docLoaded;

};

#endif
