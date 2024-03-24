#include "pieview.h"
#include <QPainter>

PieView::PieView(QWidget *parent) : QAbstractItemView(parent)
{

}

void PieView::paintEvent(QPaintEvent *)
{
    QPainter painter(viewport());
    painter.setPen(Qt::black);
    int x0 = 40;
    int y0 = 290;

    //右边窗口y坐标轴
    painter.drawLine(x0,y0,40,30); //两点之间确定直线 (x0,y0)点 与 (40,30)点
    painter.drawText(12,30,"销售数量");
    for(int i = 1; i < 5; i++)
    {
        painter.drawLine(-1,-i*50,1,-i*50);
        painter.drawText(-20,-i*50,tr("%1").arg(i*5));
    }
    //右边窗口x坐标轴
    painter.drawLine(x0,y0,300,290);
    painter.drawText(300,295,"品牌");
    int pos = x0+20;
    int row;
    //model()：这个函数返回当前视图的模型对象
    //model()->rowCount(rootIndex())：获取模型中根节点下的行数，即模型中的总行数。
    for(row=0;row<model()->rowCount(rootIndex()); row++) //遍历行
    {
        QModelIndex index = model()->index(row,0,rootIndex()); //第row行第0（也就是1）列 汽车品牌名称
        QString dep = model()->data(index).toString(); //品牌名称
        painter.drawText(pos,y0+20,dep);
        pos+=50;
    }

    int posN=x0+20;
    for(row=0;row<model()->rowCount(rootIndex());row++) //遍历行
    {
        QModelIndex index = model()->index(row,1,rootIndex()); //第row行第1列 销售数量
        int sell = model()->data(index).toDouble(); //根据该值确定方形高度
        int width = 10;
        QModelIndex colorIndex = model()->index(row,0,rootIndex()); //根据品牌名称的不同来更换颜色
        QColor color = QColor(model()->data(colorIndex,Qt::DecorationRole).toString()); //返回颜色
        if(selections->isSelected(index)) //当选择到了这个模型
            painter.setBrush(QBrush(color,Qt::Dense3Pattern)); //设置阴影
        else
            painter.setBrush(QBrush(color));
        painter.drawRect(QRect(posN,y0-sell*10,width,sell*10)); //画方形
        QRegion regionM(posN,y0-sell*10,width,sell*10); //QRegion 表示区域的类 // 创建一个QRegion对象，表示一个矩形区域 QRegion region(50, 50, 200, 100);
        RegionList<<regionM;
        posN+=50;
    }

}

void PieView::setSelectionModel(QItemSelectionModel *selectionModel)
{
    selections = selectionModel; //设置选择模型为QItemSelectionModel
}

QRegion PieView::itemRegion(QModelIndex index)
{
    QRegion region;
    if(index.column() == 1) //销售数量
        region = RegionList[index.row()];
    return region;
}

QModelIndex PieView::indexAt(const QPoint &point) const
{
    QPoint newPoint(point.x(),point.y());
    QRegion region;
    foreach (region, RegionList) {
       if(region.contains(newPoint))
       {
           int row = RegionList.indexOf(region);
           QModelIndex index = model()->index(row,1,rootIndex());
           return index;
       }
    }
    return QModelIndex();
}

//下面是未实现的纯虚函数的定义声明
QRect PieView::visualRect(const QModelIndex &index)const{}
void  PieView::scrollTo(const QModelIndex  &index,ScrollHint hint){}
QModelIndex PieView::moveCursor(CursorAction cursorAction,Qt:: KeyboardModifiers modifiers){}
int  PieView::horizontalOffset()const{}
int  PieView::verticalOffset()const{}
bool PieView::isIndexHidden(const QModelIndex &index)const{}
void PieView::setSelection(const QRect &rect,QItemSelectionModel::SelectionFlags command){}
QRegion PieView::visualRegionForSelection(const QItemSelection &selection)const{}
