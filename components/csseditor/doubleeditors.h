/***************************************************************************
                          doubleeditors.h  -  description
                             -------------------
    begin                : dom ago 3 2003
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
 #ifndef DOUBLEEDITORS_H
 #define DOUBLEEDITORS_H
 #include <qhbox.h>
 
 class mySpinBox;
 class specialSB;
 class QSpinBox;
 class QComboBox;
 
class doubleEditorBase : public QHBox {
    Q_OBJECT
  protected:
    QString   m_sxValue,
                   m_dxValue;
                   
  public:
    doubleEditorBase(QWidget *parent=0, const char *name=0);
    virtual ~doubleEditorBase(){}
    virtual void setInitialValue(){}

   public slots:
     void sxValueSlot(const QString&);
     void dxValueSlot(const QString&);

   signals:
     void valueChanged(const QString&);
}; 
 
class doublePercentageEditor : public doubleEditorBase {
    Q_OBJECT
  private:
    mySpinBox  *m_sbSx,
                      *m_sbDx;
                      
  public:
    doublePercentageEditor(QWidget *parent=0, const char *name=0);
    virtual ~doublePercentageEditor();
    virtual void setInitialValue(QString sx, QString dx);
};

class doubleComboBoxEditor : public doubleEditorBase {
    Q_OBJECT
  private:
    QComboBox *m_cbSx,
                         *m_cbDx;
                         
  public:
    doubleComboBoxEditor(QWidget *parent=0, const char *name=0);
    virtual ~doubleComboBoxEditor();
    QComboBox* cbSx() const { return m_cbSx;}
    QComboBox* cbDx() const { return m_cbDx;}
};

class doubleLengthEditor : public doubleEditorBase {
    Q_OBJECT
  private:
    specialSB *m_ssbSx,
                     *m_ssbDx;

  public:
    doubleLengthEditor(QWidget *parent=0, const char *name=0);
    virtual ~doubleLengthEditor();
    virtual void setInitialValue(QString sx, QString dx);
};

#endif
