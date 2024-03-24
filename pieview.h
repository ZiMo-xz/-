#ifndef PIEVIEW_H
#define PIEVIEW_H

#include <QWidget>
#include <QAbstractItemView>

class PieView : public QAbstractItemView
{
    Q_OBJECT
    //声明的很多QAbstractItemView纯虚函数，不一定都要实现，但一定要声明
public:
    explicit PieView(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    //为selections赋值
    void setSelectionModel(QItemSelectionModel *selectionModel);
    QRegion itemRegion(QModelIndex index);
    QRect visualRect(const QModelIndex &index) const;
    void scrollTo(const QModelIndex &index, ScrollHint hint);
    QModelIndex indexAt(const QPoint &point) const;
    QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers);
    int horizontalOffset() const;
    int verticalOffset() const;
    bool isIndexHidden(const QModelIndex &index) const;
    void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command);
    QRegion visualRegionForSelection(const QItemSelection &selection) const;
private:
    QItemSelectionModel *selections;
    QList<QRegion> RegionList;
signals:

public slots:

};

#endif // PIEVIEW_H
