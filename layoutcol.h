#ifndef LAYOUTCOL_H
#define LAYOUTCOL_H
#include <QtCore>

class layoutCol
{
public:
    layoutCol(int col,Qt::Alignment dir);
    Qt::Alignment getAlignment();
    int getCol();
private:
    Qt::Alignment p_dir;
    int p_col;

};


#endif // LAYOUTCOL_H
