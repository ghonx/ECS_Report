#include "sectionclass.h"



sectionClass::sectionClass(Section section, QString text, QFont font, Qt::Alignment alignment)
{
    p_section = section;
    p_text = text;
    p_font = font;
    p_alignment = alignment;
}

Section sectionClass::getSection()
{
    return p_section;
}


Qt::Alignment sectionClass::getAlignment()
{
    return p_alignment;
}


QFont sectionClass::getFont()
{
    return p_font;
}


QString sectionClass::getText()
{
    return p_text;
}
