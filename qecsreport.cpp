#include "qecsreport.h"
#include "qtextdocument.h"
#include <QToolBar>

QEcsReport::QEcsReport(QTableView* tableView) { p_tableView = tableView; }

void QEcsReport::ExportPDF(QString url, bool WithOpen)
{
    Cur_Printer.setOutputFormat(QPrinter::PdfFormat);
    ChangePrinterSetting(&Cur_Printer);
    Cur_Printer.setOutputFileName(url);
    QPainter* painter = new QPainter(&Cur_Printer);
    execPrint(painter);
    delete painter;
    if (WithOpen)
        QDesktopServices::openUrl(QUrl(url));
}

void QEcsReport::setTableView(QTableView* tableView)
{
    p_tableView = tableView;
}

void QEcsReport::addText(Section sec, QString txt, QFont font, Qt::Alignment alig)
{
    sections.append(new sectionClass(sec, txt, font, alig));
}

void QEcsReport::setLayoutCol(int col, Qt::Alignment dir)
{
    layoutColumn.append(new layoutCol(col, dir));
}

void QEcsReport::drawGridHeader(QPainter* painter)
{
    if (p_curPage == 1)
        drawSectionReportHeader(painter);
    drawSectionHeader(painter);
    painter->setFont(p_gridFont);
    painter->setPen(QPen(Qt::black, LineWidth));
    int x = Cur_Printer.pageLayout().margins().right() + p_HorezantalMarg;
    int y = p_StarrtDetailY + p_padding;
    int h = (painter->fontMetrics().height() + p_padding) * rowReaderHeight;
    for (int col = 0; col < p_colCount; ++col) {
        int tmpCol = col;
        if (p_tableView->layoutDirection() == Qt::RightToLeft)
            tmpCol = p_colCount - col - 1;
        int curColWidth = p_pageWidth * p_tableView->columnWidth(tmpCol) / p_tableWidth;
        if (p_tableView->columnWidth(tmpCol) > 0) {
            painter->fillRect(x, y, curColWidth, h, p_headBackColor);
            painter->drawRect(x, y, curColWidth, h);
            QString txt = p_tableView->model()->headerData(tmpCol, Qt::Horizontal).toString();
            painter->drawText(x + (p_padding / 2), y, curColWidth - p_padding, h, Qt::AlignCenter, txt);
            x += curColWidth;
        }
    }
    p_lastDetailY = y + h;
}

void QEcsReport::drawGridRows(QPainter* painter)
{
    painter->setFont(p_gridFont);
    painter->setPen(QPen(Qt::black, LineWidth));
    qreal GridAria = p_pageHeight - p_reportHeaderHeight - p_pageHeaderHeight - p_pageFooterHeight - rowHeight * rowReaderHeight - rowHeight;
    if (!p_PageNumberFormat.isEmpty())
        GridAria -= QFontMetrics(p_PageNumber_Font).height();
    for (int row = 0; row < p_tableView->model()->rowCount(); ++row) {
        int x = Cur_Printer.pageLayout().margins().right() + p_HorezantalMarg;
        for (int col = 0; col < p_colCount; ++col) {
            int tmpCol = col;
            if (p_tableView->layoutDirection() == Qt::RightToLeft)
                tmpCol = p_colCount - col - 1;
            int curColWidth = p_pageWidth * p_tableView->columnWidth(tmpCol) / p_tableWidth;
            if (curColWidth > 1) {
                int typ = p_tableView->model()->index(row, tmpCol).data(0).typeId();
                QString txt = p_tableView->model()->index(row, tmpCol).data(0).toString();
                if (typ == 6 | typ == 38) {
                    double l = p_tableView->model()->index(row, tmpCol).data(0).toDouble();
                    txt = QString("%1").arg(l);
                }
                Qt::Alignment align = getColAl(tmpCol);
                painter->drawRect(x, p_lastDetailY, curColWidth, rowHeight);
                painter->drawText(x + p_padding / 2, p_lastDetailY + p_padding / 2, curColWidth - p_padding, rowHeight - p_padding, align, txt);
                x += curColWidth;
            }
        }
        GridAria -= rowHeight;
        p_lastDetailY += rowHeight;
        if (GridAria < rowHeight) {
            drawSectionFooter(painter);
            Cur_Printer.newPage();
            ++p_curPage;
            p_StarrtDetailY = 30;
            drawGridHeader(painter);
            GridAria = p_pageHeight - p_pageHeaderHeight - p_pageFooterHeight - rowHeight * rowReaderHeight - rowHeight;
            if (!p_PageNumberFormat.isEmpty())
                GridAria -= QFontMetrics(p_PageNumber_Font).height();
        }
    }
    p_StarrtDetailY = p_lastDetailY;
    drawSectionReportFooter(painter);
    drawSectionFooter(painter);
}

void QEcsReport::drawSectionReportHeader(QPainter* painter)
{
    DrawLogo(painter);
    for (int i = 0; i < sections.count(); ++i) {
        if (sections[i]->getSection() == reportHeader) {
            painter->setFont(sections[i]->getFont());
            int h = painter->fontMetrics().height();
            int x = Cur_Printer.pageLayout().margins().right() + p_HorezantalMarg;
            painter->drawText(QRect(x + p_padding / 2, p_StarrtDetailY + p_padding / 2, p_pageWidth + p_padding, h + p_padding), sections[i]->getAlignment(), sections[i]->getText());
            p_StarrtDetailY = p_StarrtDetailY + h;
        }
    }
}

void QEcsReport::drawSectionHeader(QPainter* painter)
{
    for (int i = 0; i < sections.count(); ++i) {
        if (sections[i]->getSection() == pageHeader) {
            painter->setFont(sections[i]->getFont());
            int h = painter->fontMetrics().height();
            int x = Cur_Printer.pageLayout().margins().right() + p_HorezantalMarg;
            painter->drawText(QRect(x + p_padding / 2, p_StarrtDetailY + p_padding / 2, p_pageWidth + p_padding, h + p_padding), sections[i]->getAlignment(), sections[i]->getText());
            p_StarrtDetailY = p_StarrtDetailY + h;
        }
    }
}

void QEcsReport::drawSectionReportFooter(QPainter* painter)
{
    for (int i = 0; i < sections.count(); ++i) {
        if (sections[i]->getSection() == reportFooter) {
            painter->setFont(sections[i]->getFont());
            int h = painter->fontMetrics().height();
            int x = Cur_Printer.pageLayout().margins().right() + p_HorezantalMarg;
            painter->drawText(QRect(x + p_padding / 2, p_StarrtDetailY + p_padding / 2, p_pageWidth + p_padding, h + p_padding), sections[i]->getAlignment(), sections[i]->getText());
            p_StarrtDetailY = p_StarrtDetailY + h;
        }
    }
}

void QEcsReport::drawSectionFooter(QPainter* painter)
{
    p_StarrtDetailY = p_pageHeight - p_pageFooterHeight;
    for (int i = 0; i < sections.count(); ++i) {
        if (sections[i]->getSection() == pageFooter) {
            painter->setFont(sections[i]->getFont());
            int h = painter->fontMetrics().height();
            int x = Cur_Printer.pageLayout().margins().right() + p_HorezantalMarg;
            painter->drawText(QRect(x + p_padding / 2, p_StarrtDetailY + p_padding / 2, p_pageWidth + p_padding, h + p_padding), sections[i]->getAlignment(), sections[i]->getText());
            p_StarrtDetailY = p_StarrtDetailY + h;
        }
    }
    if (!p_PageNumberFormat.isEmpty()) {
        painter->setFont(p_PageNumber_Font);
        QString pg = p_PageNumberFormat;
        pg.replace("%p", QVariant(p_curPage).toString(), Qt::CaseSensitive)
            .replace("%P", QVariant(p_pageCount).toString(), Qt::CaseSensitive);
        int h = painter->fontMetrics().height();
        int x = Cur_Printer.pageLayout().margins().right() + p_HorezantalMarg;
        painter->drawText(QRect(x + p_padding / 2, p_StarrtDetailY + p_padding / 2, p_pageWidth + p_padding, h + p_padding), p_PageNumberFormatAlignment, pg);
    }
}

void QEcsReport::Init(QPainter* painter)
{
    // initial Value
    rowHeight = QFontMetrics(p_gridFont).height() + p_padding;
    p_HorezantalMarg = 50;
    p_tableWidth = 0;
    p_pageHeaderHeight = 0;
    p_reportHeaderHeight = 100;
    if (p_PageNumberFormat.isEmpty())
        p_pageFooterHeight = p_padding;
    else
        p_pageFooterHeight = QFontMetrics(p_PageNumber_Font).height() + p_padding * 2;
    p_reportFooterHeight = 0;
    Cur_Printer.setFullPage(true);
    p_colCount = p_tableView->model()->columnCount();
    for (int c = 0; c < p_colCount; ++c)
        p_tableWidth += p_tableView->columnWidth(c);
    p_pageWidth = Cur_Printer.width() - Cur_Printer.pageLayout().margins().left() - (p_HorezantalMarg * 2);
    p_pageHeight = Cur_Printer.height() - Cur_Printer.pageLayout().margins().top() - Cur_Printer.pageLayout().margins().bottom();
    p_curPage = 1;
    p_StarrtDetailY = 30;
    p_lastDetailY = p_StarrtDetailY;
    p_pageCount = 1;
    //-------------------------------------------
    for (int i = 0; i < sections.count(); ++i) {
        painter->setFont(sections[i]->getFont());
        int h = painter->fontMetrics().height();
        if (sections[i]->getSection() == pageFooter)
            p_pageFooterHeight += h;
        if (sections[i]->getSection() == pageHeader)
            p_pageHeaderHeight += h;
        if (sections[i]->getSection() == reportHeader)
            p_reportHeaderHeight += h;
        if (sections[i]->getSection() == reportFooter)
            p_reportFooterHeight += h;
    }
    qreal GridAria = p_pageHeight - p_reportHeaderHeight - p_pageHeaderHeight - p_pageFooterHeight - rowHeight * rowReaderHeight - rowHeight;
    if (!p_PageNumberFormat.isEmpty())
        GridAria -= QFontMetrics(p_PageNumber_Font).height();
    for (int row = 0; row < p_tableView->model()->rowCount(); ++row) {
        GridAria -= rowHeight;
        if (GridAria < rowHeight) {
            ++p_pageCount;
            GridAria = p_pageHeight - p_pageHeaderHeight - p_pageFooterHeight - rowHeight * rowReaderHeight - rowHeight;
            if (!p_PageNumberFormat.isEmpty())
                GridAria -= QFontMetrics(p_PageNumber_Font).height();
        }
    }
    if (p_reportFooterHeight > GridAria)
        ++p_pageCount;
}

Qt::Alignment QEcsReport::getColAl(int col)
{
    for (int i = 0; i < layoutColumn.count(); ++i) {
        Qt::Alignment r = Qt::AlignLeft;
        if (p_tableView->layoutDirection() == Qt::RightToLeft) {
            r = Qt::AlignRight;
        }
        if (layoutColumn[i]->getCol() == col)
            r = layoutColumn[0]->getAlignment();
        return r;
    }
}

void QEcsReport::ChangePrinterSetting(QPrinter* prt)
{
    QPageLayout pl;
    pl.setOrientation(p_Orientation);
    // pl.setPageSize(p_PgSz);
    pl.setPageSize(QPageSize(p_PgSz));
    prt->setPageLayout(pl);
    prt->setFullPage(true);
}

void QEcsReport::execPrint(QPainter* painter)
{
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
    Init(painter);
    drawGridHeader(painter);
    drawGridRows(painter);
    painter->end();
}

void QEcsReport::DrawLogo(QPainter* painter)
{
    if (!p_LogoUrl.isEmpty()) {
        QPixmap pm;
        pm.load(p_LogoUrl);
        p_StarrtDetailY = p_LogoStartLine;
        int pxH = p_logoMaxHeight;
        int pxW = pxH * pm.width() / pm.height();
        int x = Cur_Printer.pageLayout().margins().left() + 40;
        if (p_LogoAlignment == Qt::AlignRight)
            x = Cur_Printer.width() - Cur_Printer.pageLayout().margins().left() * 2 - pxW - 40;
        else if (p_LogoAlignment == Qt::AlignCenter)
            x = (Cur_Printer.width() - pxW) / 2;
        painter->drawPixmap(x, 60, pxW, pxH, pm);
    }
}

void QEcsReport::setGrudFont(QFont font)
{
    p_gridFont = font;
}

QFont QEcsReport::gridFont()
{
    return p_gridFont;
}

void QEcsReport::setDefaultFont(QFont font)
{
    p_defaultFont = font;
}

QFont QEcsReport::defaultFont()
{
    return p_defaultFont;
}

void QEcsReport::Preview()
{
    Cur_Printer.setOutputFormat(QPrinter::NativeFormat);
    QPrintPreviewDialog* preview = new QPrintPreviewDialog(&Cur_Printer);
    QList<QToolBar*> toolbarlist = preview->findChildren<QToolBar*>();
    if (!toolbarlist.isEmpty()) {
        toolbarlist.first()->removeAction(toolbarlist.first()->actions().at(6));
        toolbarlist.first()->removeAction(toolbarlist.first()->actions().at(6));
        toolbarlist.first()->removeAction(toolbarlist.first()->actions().at(6));
        toolbarlist.first()->removeAction(toolbarlist.first()->actions().at(17));
        // toolbarlist.first()->actions().at(17)->setDisabled(true);
    }
    connect(preview, SIGNAL(paintRequested(QPrinter*)), this,
        SLOT(print(QPrinter*)));
    preview->setWindowState(Qt::WindowMaximized);
    preview->exec();
    preview->deleteLater();
}

void QEcsReport::Reset()
{
    p_tableWidth = 0;
    sections.clear();
}

void QEcsReport::setHeaderCBackColor(QColor cl)
{
    p_headBackColor = cl;
}

void QEcsReport::setPageSize(QPageSize::PageSizeId ps)
{
    p_PgSz = ps;
}

void QEcsReport::setOrientation(QPageLayout::Orientation orn)
{
    p_Orientation = orn;
}

void QEcsReport::setPageNumberFormat(QString pf, Qt::Alignment alig)
{
    p_PageNumberFormat = pf.trimmed();
    p_PageNumberFormatAlignment = alig;
}

void QEcsReport::setPageNumberFont(QFont fnt)
{
    p_PageNumber_Font = fnt;
}

void QEcsReport::setLogo(QString url, Qt::Alignment alig, int MaxHeight, int StartLine)
{
    p_LogoUrl = url.trimmed();
    p_LogoAlignment = alig;
    p_LogoStartLine = StartLine;
    p_logoMaxHeight = MaxHeight;
}

void QEcsReport::print(QPrinter* p)
{
    ChangePrinterSetting(p);
    QPainter* painter = new QPainter(p);
    execPrint(painter);
    delete painter;
}
