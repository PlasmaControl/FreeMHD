#ifndef HierarchicalGridLayout_h
#define HierarchicalGridLayout_h

#include <QObject>

class HierarchicalGridLayoutTester : public QObject
{
  Q_OBJECT;
private Q_SLOTS:

  void addWidget();
  void addWidget_data();

  void rearrange();
  void rearrange_data();

  void interactiveResize();
};

#endif
