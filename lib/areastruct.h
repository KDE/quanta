/***************************************************************************
    begin                : Fri Jun 17 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#ifndef AREASTRUCT_H
#define AREASTRUCT_H

#include <qpoint.h>

struct AreaStruct{
  AreaStruct() {end = start = QPoint(-1, -1);}
  AreaStruct(int bl, int bc, int el, int ec) {set(bl, bc, el, ec);}
  AreaStruct(const QPoint& start, const QPoint& end) {this->start = start; this->end = end;}
  AreaStruct(const AreaStruct& area) {start = area.start; end = area.end;}
  
  inline void set(int bl, int bc, int el, int ec) {start = QPoint(bl, bc); end = QPoint(el, ec);}
  
  inline void setStart(int bl, int bc) {start = QPoint(bl, bc);}
  
  inline void setEnd(int el, int ec) {end = QPoint(el, ec);}
  
  QPoint start;
  QPoint end;
  
  bool notEmpty() const {return (start.x() < end.x()) || 
                                (start.x() == end.x() && start.x() < end.x());};
  
  // compatible for old code
  inline int bLine() const {return start.x();};
  inline int bCol() const {return start.y();};
  inline int eLine() const {return end.x();};
  inline int eCol() const {return end.y();};
  
  /** Returns 0 if the (line,col) is inside the area specified by the other arguments,
      -1 if it is before the area and 1 if it is after. */
  static int isBetween(int line, int col, int bLine, int bCol, int eLine, int eCol)
  {
    if (line < bLine || (line == bLine && (col < bCol) )) 
      return -1; //it is before
      
    if (line > eLine || (line == eLine && (col > eCol) )) 
      return +1; //it is after
      
    return 0;
  }
  
  /** Returns 0 if the (line, col) is inside the area,
      -1 if it is before the area and 1 if it is after. */
  static int isBetween(int line, int col, const AreaStruct &area)
  {
    return isBetween(line, col, area.bLine(), area.bCol(), area.eLine(), area.eCol());
  }
  
  /** Returns 0 if the (line, col) is inside the area,
      -1 if it is before the area and 1 if it is after. */
  static int isBetween(const QPoint & qp, const AreaStruct &area)
  {
    return isBetween(qp.x(), qp.y(), area);
  }

};

#endif
