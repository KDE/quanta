#ifndef COLORREQUESTER_H
#define COLORREQUESTER_H

#include <kpushbutton.h>
#include <qcolor.h>

#include "minieditor.h"

class KLineEdit;
class KColorDialog;
class colorRequester : public miniEditor{
    Q_OBJECT
  public:
    colorRequester(QWidget *parent, const char* name=0);
    ~colorRequester();
    KLineEdit * lineEdit() const;
    KPushButton * button() const { return myButton; }
    virtual void connectToPropertySetter(propertySetter* p);
    //void setInitialValue(const QString&);
  public slots:
    void openColorDialog();
    void setInitialValue(/*const QString&*/);
  protected:
    void init();
  signals:
    void selectedColor(QColor);
    void textChanged(const QString&);
  private:
    mutable KColorDialog * myColorDialog;
    KPushButton *myButton;
    class colorRequesterPrivate;
          colorRequesterPrivate *d;
    QColor m_initialValue;      
};
#endif
