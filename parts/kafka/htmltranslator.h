/***************************************************************************
                               htmltranslator.h
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

#ifndef HTMLTRANSLATOR_H
#define HTMLTRANSLATOR_H

#include <kurl.h>

#include "nodetranslator.h"

class Node;
class WKafkaPart;
class KStandardDirs;

/**
 * This class takes care of the following tasks for [X]HTML DTDs :
 * - Add a TBODY DOM::Node to TABLE DOM::Node if necessary.
 * - Update the src attr of the DOM::Node.
 * - Display an little icon for scripts.
 */

class HTMLTranslator : public NodeTranslator
{
public:
	HTMLTranslator(WKafkaPart *_wkafkapart);
	virtual ~HTMLTranslator();

	/**
	 * This function modify the DOM::Node of the node.
	 * The DOM::Node must be built before calling this
	 * function.
	 * @param node The Node we want to "translate".
	 * @param parentDNode the parent DOM::Node of the root DOM::Node of node.
	 * @param nextDNode the DOM::Node next to the root DOM::Node of node.
	 */
	virtual bool translateNode(Node *node, DOM::Node parentDNode, DOM::Node nextDNode);

	/**
	 * Set the base URL of the current document to update src attrs.
	 * @param baseURL The base URL of the current Document.
	 */
	void setBaseURL(KURL baseURL) {m_baseURL = baseURL;}

	/**
	 * Get the current base URL.
	 * @return Returns the current base URL.
	 */
	KURL baseURL() {return m_baseURL;}

	/**
	 * @return Specifies if we should show icons for scripts.
	 */
	bool showIconsForScripts() {return m_showIconForScripts;}

	/**
	 * Set if we should these little icons.
	 * @param showIcons Should we show these icons?
	 */
	void showIconsForScripts(bool showIcons) {m_showIconForScripts = showIcons;}
	
private:
	KURL m_baseURL;
	bool m_showIconForScripts;
	WKafkaPart *m_wkafkapart;
	KStandardDirs *m_stddirs;
};

#endif
