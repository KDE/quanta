 
#ifndef XSLDBGDOC  
#define XSLDBGDOC 
 
#include <qstring.h> 
 
class XsldbgDoc { 
 public: 
  XsldbgDoc(); 
  XsldbgDoc(QString &fileName, QString &text); 
  ~XsldbgDoc(); 
 
  QString getText();  
  QString getFileName(); 
 
  void setRow(int row); 
  int getRow(); 
  void setColumn(int column); 
  int getColumn(); 
 
 private: 
  QString text; 
  QString fileName; 
  int row, column; 
 
}; 
 
#endif 
