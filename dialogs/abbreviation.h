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

  void saveTemplates();

public slots:
  void slotDTDChanged(const QString& newDTDName);

private slots:
  void slotTemplateSelectionChanged(QListViewItem*);
  void slotAddTemplate();
  void slotRemoveTemplate();

private:
  DTDStruct *m_dtd;
  QListViewItem *oldItem;
};

#endif
