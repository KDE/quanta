/***************************************************************************
                          visualframeeditor.h  -  description
                             -------------------
    begin                : mar mar 25 2003
    copyright            : (C) 2003 by gulmini luciano
    email                : gulmini.luciano@student.unife.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef VISUALFRAMEEDITOR_H
#define VISUALFRAMEEDITOR_H

#include "treenode.h"
#include "selectablearea.h"
#include <qhbox.h>

/**
  *@author gulmini luciano
  */


class VisualFrameEditor : public QHBox  {
   Q_OBJECT
   private:
      enum MarkupLanguage{XHTML,HTML};
      tree *m_internalTree;
      SelectableArea* m_firstInsertedSA;
      QStringList m_existingStructure;
      MarkupLanguage m_markupLanguage;

      void buildInternalTree(const QString &parent);
      void setGeometries(const QString &l);
      void drawGUI(treeNode *n, QWidget* parent);
      QStringList convertAsterisks(const QString &s, int d);

      QString createFrameTag(areaAttribute *a);
      QString formatStructure();
      QString RCvalue(treeNode *n);
      void createStructure(treeNode* n);

   public:
      VisualFrameEditor( QWidget * parent = 0, const char * name = 0);
      ~VisualFrameEditor();
      void draw() { repaint(); }
      void loadExistingStructure(const QStringList &list);
      QString framesetStructure();
      void removeNode(const QString &l);
      void split(const QString &l, int n, SplitType type);
      void setMarkupLanguage(const QString& s);
      tree* internalTree() { return m_internalTree;}

   protected:
      virtual void paintEvent ( QPaintEvent * );
   signals:
     void areaSelected(const QString &);
};

#endif
