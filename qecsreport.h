#ifndef QECSREPORT_H
#define QECSREPORT_H
#include "layoutcol.h"
#include "sectionclass.h"
#include <QDebug>
#include <QDesktopServices>
#include <QFont>
#include <QPainter>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QTableView>

class QEcsReport : public QWidget {
    Q_OBJECT
public:
    QEcsReport(QTableView* tableView);
    void ExportPDF(QString url, bool WithOpen = true);
    void setTableView(QTableView* tableView);
    void addText(Section sec, QString txt, QFont font, Qt::Alignment alig);
    void setLayoutCol(int col, Qt::Alignment dir);
    void setGrudFont(QFont font);
    QFont gridFont();
    void setDefaultFont(QFont font);
    QFont defaultFont();
    void Preview();
    void Reset();
    void setHeaderCBackColor(QColor cl);
    void setPageSize(QPageSize::PageSizeId ps);
    void setOrientation(QPageLayout::Orientation orn);
    void setPageNumberFormat(QString pf = "%p/%P", Qt::Alignment alig = Qt::AlignCenter);
    void setPageNumberFont(QFont fnt);
    void setLogo(QString url, Qt::Alignment alig = Qt::AlignRight, int MaxHeight = 70, int StartLine = 70);

public slots:
    void print(QPrinter* p);

private:
    struct strct_PrinterMarge {
        int Marge_Top;
        int Marge_Bottom;
        int Marge_Left;
        int Marge_Right;
    };
    void drawGridHeader(QPainter* painter);
    void drawGridRows(QPainter* painter);
    void drawSectionReportHeader(QPainter* painter);
    void drawSectionHeader(QPainter* painter);
    void drawSectionReportFooter(QPainter* painter);
    void drawSectionFooter(QPainter* painter);
    void Init(QPainter* painter);
    void ChangePrinterSetting(QPrinter* prt);
    void execPrint(QPainter* painter);
    void DrawLogo(QPainter* painter);
    QTableView* p_tableView;
    QPrinter Cur_Printer;
    QVector<sectionClass*> sections;
    QVector<layoutCol*> layoutColumn;
    // strct_PrinterMarge PrinterMarge;
    Qt::Alignment getColAl(int col);
    QFont p_gridFont = QFont("Arial", 10);
    QFont p_defaultFont = QFont("Arial", 10);
    QFont p_PageNumber_Font = QFont("Arial", 8);
    int p_pageCount;
    int p_curPage;
    int p_tableWidth;
    int p_colCount;
    int p_pageWidth;
    qreal rowHeight;
    qreal p_pageHeight;
    qreal p_pageFooterHeight;
    qreal p_reportFooterHeight;
    qreal p_pageHeaderHeight;
    qreal p_reportHeaderHeight;
    qreal p_HorezantalMarg;
    qreal p_StarrtDetailY;
    qreal p_lastDetailY;
    QPageSize::PageSizeId p_PgSz = QPageSize::PageSizeId::A4;
    QPageLayout::Orientation p_Orientation = QPageLayout::Portrait;
    QColor p_headBackColor = 0xEDEDED;
    QString p_PageNumberFormat = "";
    Qt::Alignment p_PageNumberFormatAlignment = Qt::AlignCenter;
    QString p_LogoUrl = "";
    Qt::Alignment p_LogoAlignment = Qt::AlignRight;
    int p_LogoStartLine = 70;
    int p_logoMaxHeight = 70;

    const qreal LineWidth = .1;
    const qreal rowReaderHeight = 1.7;
    const qreal p_padding = 6;
};
#endif // QECSREPORT_H
