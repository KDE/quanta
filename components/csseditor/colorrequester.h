#ifndef COLORREQUESTER_H
#define COLORREQUESTER_H
#include <qhbox.h>
#include <kpushbutton.h>
#include <qcolor.h>

class KLineEdit;
class KColorDialog;
class colorRequester : public QHBox{
    Q_OBJECT
  public:
    colorRequester(QWidget *parent, const char* name=0);
    ~colorRequester();
    KLineEdit * lineEdit() const;
    KPushButton * button() const { return myButton; }
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
