/**
 *
 * Andras Mantia
 **/

#ifndef ABBREVIATION_H
#define ABBREVIATION_H

#include <qwidget.h>
#include <abbreviations.h>

class QListViewItem;
struct DTDStruct;

class Abbreviation : public Abbreviations
{
  Q_OBJECT
public:
  Abbreviation(QWidget *parent, const char *name = 0);

  ~Abbreviation();

private slots:
  void slotDTDChanged(const QString& newDTDName);
  void slotTemplateSelectionChanged(QListViewItem*);

private:
  DTDStruct *m_dtd;
};

#endif
