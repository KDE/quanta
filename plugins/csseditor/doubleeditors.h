/***************************************************************************
                          doubleeditors.h  -  description
                             -------------------
    begin                : dom ago 3 2003
    copyright            : (C) 2003 by simone
    email                : simone@localhost
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 #ifndef DOUBLEEDITORS_H
 #define DOUBLEEDITORS_H
 #include <qhbox.h>
 class specialSB;
 class QSpinBox;
 class QComboBox;
 
 class doublePercentageEditor : public QHBox /*multipleSpinBox*/  {
     Q_OBJECT
   private:
     QSpinBox  *sbSx,
               *sbDx;
     QString   sxSBValue,
               dxSBValue;
   public:
	   doublePercentageEditor(QWidget *parent=0, const char *name=0);
	   ~doublePercentageEditor();

   public slots:
     void sxSBValueSlot(const QString&);
     void dxSBValueSlot(const QString&);

   signals:
     void valueChanged(const QString&);

};

class doubleComboBoxEditor : public QHBox  {
     Q_OBJECT
   private:
     QComboBox *cbSx,
               *cbDx;
     QString sxValue,
             dxValue;

   public:
	   doubleComboBoxEditor(QWidget *parent=0, const char *name=0);
	   ~doubleComboBoxEditor();
     QComboBox* getCbSx() const { return cbSx;}
     QComboBox* getCbDx() const { return cbDx;}
   public slots:
     void sxValueSlot(const QString&);
     void dxValueSlot(const QString&);

   signals:
     void valueChanged(const QString&);
};

class doubleLengthEditor : public QHBox  {
     Q_OBJECT
   private:
     specialSB *ssbSx,
               *ssbDx;
     QString ssbSxValue,
             ssbDxValue;
   public:
	   doubleLengthEditor(QWidget *parent=0, const char *name=0);
	   ~doubleLengthEditor();

   public slots:
     void ssbSxValueSlot(const QString&);
     void ssbDxValueSlot(const QString&);

   signals:
     void valueChanged(const QString&);  
};

#endif
