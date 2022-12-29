#include "layoutcol.h"

layoutCol::layoutCol(int col, Qt::Alignment dir)
{
    p_dir = dir;
    p_col = col;
}

Qt::Alignment layoutCol::getAlignment()
{
    return p_dir;
}

int layoutCol::getCol()
{
    return p_col;
}

