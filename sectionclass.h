#ifndef SECTIONCLASS_H
#define SECTIONCLASS_H
#include<QtCore>
#include<QFont>
typedef enum {
    reportHeader = 0,
    pageHeader = 1,
    pageFooter = 2,
    reportFooter = 3
} Section;

class sectionClass
{
public:
    sectionClass(Section section, QString text, QFont font , Qt::Alignment alignment = Qt::AlignHCenter);
    Section getSection();
    QString getText();
    QFont getFont();
    Qt::Alignment getAlignment();
private:
    Section p_section;
    QString p_text;
    QFont p_font;
    Qt::Alignment p_alignment;
};

#endif // SECTIONCLASS_H
