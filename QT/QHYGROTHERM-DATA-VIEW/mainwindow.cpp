/***************************************************************************
**                                                                        **
** SSC-Hygrothemometer Datenansicht ist ein Daten Visualisations Plotter  **
** für das gleichnamige Thermometer von mir.                              **
**                                                                        **
****************************************************************************
**           Author: Michael Wolkstein                                    **
**  Website/Contact: github.com/ssc-hygrothermometer                      **
**             Date: 25.11.18                                             **
**          Version: 0.1                                                  **
****************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serialDialog.h"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextCodec>
#include <QTextDocument>


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  set_intern_Positon_change(false);
  ui->setupUi(this);
  //setupPlot();
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  ui->plot->setVisible(false);
  //connect( ui->action_ffne_CSV_Datei, &QAction::triggered, this, this->openFile() );
  csvModel = new QStandardItemModel(this);
  csvModel->setColumnCount(7);
  csvModel->setHorizontalHeaderLabels(QStringList() << "Datum Zeit" << "Raumtemperatur in C°" << "Relative Luftfeute in %"<<"Luftdruck in hPa"
                                      <<"Taupunkt 100%"<<"Absulute Luftfeuchte in g/m³"<<"Akku Spannung in V");
  ui->tableView->setModel(csvModel);
  ui->tableView->resizeColumnsToContents();

  connect(ui->plot, SIGNAL(axisClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelClick(QCPAxis*,QCPAxis::SelectablePart)));
  connect(ui->plot, SIGNAL(legendClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendClick(QCPLegend*,QCPAbstractLegendItem*)));

  qDebug() << "CSV Model init finished: " << csvModel->rowCount();
  // register the plot document object (only needed once, no matter how many plots will be in the QTextDocument):
  //QCPDocumentObject *plotObjectHandler = new QCPDocumentObject(this);
  //ui->textEdit->document()->documentLayout()->registerHandler(QCPDocumentObject::PlotTextFormat, plotObjectHandler);
  //ui->textEdit->document()->pageSize().setWidth(1260);
  //qDebug() << ui->textEdit->document()->documentLayout()->documentSize().width();
//  QPixmap bkgnd(":/img/log2.png");
//  bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
//  QPalette palette;
//  palette.setBrush(QPalette::Background, bkgnd);
//  this->setPalette(palette);
  QTextCursor cursor = ui->textEdit->textCursor();
  QTextImageFormat imageFormat;
  imageFormat.setName(":/img/log3.png");
  cursor.insertImage(imageFormat);


  const QList<int> standardSizes = QFontDatabase::standardSizes();
  foreach (int size, standardSizes)
  ui->comboSize->addItem(QString::number(size));
  ui->comboSize->setCurrentIndex(standardSizes.indexOf(QApplication::font().pointSize()));
  //ui->comboSize->setCurrentText("test");

  mySerialDialog = new serialDialog(this);

}

MainWindow::~MainWindow()
{
   delete ui;
   delete csvModel;
}

void MainWindow::setupPlot()
{
     QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    double starttime = QDateTime::fromString(csvModel->data(csvModel->index(1, 0)).toString(),"d.M.yyyy h:m:s").toTime_t();
    //double endtime = QDateTime::fromString(csvModel->data(csvModel->index(csvModel->rowCount(), 0)).toString(),"d.M.yyyy h:m:s").toTime_t();


    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables | QCP::iSelectAxes );

    ui->plot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom)); // period as decimal separator and comma as thousand separator

    ui->plot->legend->setVisible(ui->checkBoxLegende->isChecked());
    QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    ui->plot->legend->setFont(legendFont);
    ui->plot->legend->setBrush(QBrush(QColor(255,255,255,230)));

    // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
    ui->plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);

    // setup for graph 0: HUM RE%
    // will contain left maxwell-like function
    ui->plot->addGraph(ui->plot->xAxis, ui->plot->yAxis);
    ui->plot->graph(0)->setPen(QPen(Qt::blue));
    ui->plot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    //ui->plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    //ui->plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
    ui->plot->graph(0)->setName("(Links) Relative Luftfeuchte in %");

    // setup for graph 1: TEMP
    // will contain bottom maxwell-like function with error bars
    ui->plot->addGraph(ui->plot->xAxis, ui->plot->yAxis2);
    ui->plot->graph(1)->setPen(QPen(Qt::red));
    ui->plot->graph(1)->setName("(Rechts) Temperatur in °C");

    // setup for graph 1: DDawpoint
    // will contain bottom maxwell-like function with error bars
    ui->plot->addGraph(ui->plot->xAxis, ui->plot->yAxis2);
    ui->plot->graph(2)->setPen(QPen(Qt::darkRed));
    ui->plot->graph(2)->setName("(Rechts) Taupunkt 100% in °C");

    // setup for graph 1: AbsHum
    // will contain bottom maxwell-like function with error bars
    ui->plot->addGraph(ui->plot->xAxis, ui->plot->yAxis);
    ui->plot->graph(3)->setPen(QPen(Qt::darkBlue));
    ui->plot->graph(3)->setBrush(QBrush(QColor(0, 0, 255, 80)));
    ui->plot->graph(3)->setName("(Links) Luftfeuchte in g/m³");

    // setup for graph 1: mittlere luftfeuchte
    QPen blueDotPen;
    blueDotPen.setColor(QColor(30, 40, 255, 150));
    blueDotPen.setStyle(Qt::DashLine);
    blueDotPen.setWidthF(1);

    ui->plot->addGraph(ui->plot->xAxis, ui->plot->yAxis);
    ui->plot->graph(4)->setPen(blueDotPen);
    //ui->plot->graph(4)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 0.0), QBrush(QColor(0, 0, 255, 20)), 9));
    //ui->plot->graph(4)->setBrush(QBrush(QColor(0, 0, 255, 70)));
    ui->plot->graph(4)->setName("(Links) Durchschnitt Luftfeuchte in %");
    //ui->plot->graph(4)->setChannelFillGraph(ui->plot->graph(0));

    // setup for graph 1: mittlere luftfeuchte
    QPen idealDotPen;
    idealDotPen.setColor(QColor(30, 240, 25, 150));
    idealDotPen.setStyle(Qt::DashDotLine);
    idealDotPen.setWidthF(2);

    ui->plot->addGraph(ui->plot->xAxis, ui->plot->yAxis);
    ui->plot->graph(5)->setPen(idealDotPen);
    //ui->plot->graph(4)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 0.0), QBrush(QColor(0, 0, 255, 20)), 9));
    ui->plot->graph(5)->setBrush(QBrush(QColor(0, 250, 154, 100)));
    ui->plot->graph(5)->setName("(Links) ideale Luftfeuchte in %");
    ui->plot->graph(5)->setChannelFillGraph(ui->plot->graph(4));


    // generate hum data
    QVector<QCPGraphData> timeDataHum(csvModel->rowCount());
    QVector<QCPGraphData> timeDataTemp(csvModel->rowCount());
    QVector<QCPGraphData> timeDataDawPoint(csvModel->rowCount());
    QVector<QCPGraphData> timeDataAbsHum(csvModel->rowCount());
    QVector<QCPGraphData> timeDataIntegralHum(csvModel->rowCount());
    QVector<QCPGraphData> timeDataIdealHum(csvModel->rowCount());

    //color axis
    QPen axis_pen;
    axis_pen.setColor(QColor(Qt::blue));
    ui->plot->yAxis->setBasePen(axis_pen);
    ui->plot->yAxis->setTickLabelColor(QColor(Qt::blue));
    axis_pen.setColor(QColor(Qt::red));
    ui->plot->yAxis2->setBasePen(axis_pen);
    ui->plot->yAxis2->setTickLabelColor(QColor(Qt::red));


    float intHum = 0.0f;
    float middleIntHum = 0.0f;
    for (int i=0; i < csvModel->rowCount(); ++i)
    {

        double cvsTime = QDateTime::fromString(csvModel->data(csvModel->index(i, 0)).toString(),"d.M.yyyy h:m:s").toTime_t();
        float hum = csvModel->data(csvModel->index(i, 2)).toFloat();
        float temp = csvModel->data(csvModel->index(i, 1)).toFloat();
        float dawp = csvModel->data(csvModel->index(i, 4)).toFloat();
        float absH = csvModel->data(csvModel->index(i, 5)).toFloat();

        intHum = intHum + hum;

        middleIntHum = intHum / (i+1);
        //qDebug() << middleIntHum <<" : " << i <<" : " << intHum;

        //timeData[i].key = now + 24*3600*i;
        timeDataHum[i].key = cvsTime;
        timeDataTemp[i].key = cvsTime;
        timeDataDawPoint[i].key = cvsTime;
        timeDataAbsHum[i].key = cvsTime;
        timeDataIntegralHum[i].key = cvsTime;
        timeDataIdealHum[i].key = cvsTime;

        timeDataHum[i].value = static_cast<double>(hum);
        timeDataTemp[i].value = static_cast<double>(temp);
        timeDataDawPoint[i].value = static_cast<double>(dawp);
        timeDataAbsHum[i].value = static_cast<double>(absH);
        timeDataIntegralHum[i].value = static_cast<double>(middleIntHum);
        timeDataIdealHum[i].value = static_cast<double>(ui->spinBoxIdealHum->value());
    }

    ui->plot->graph(0)->data()->set(timeDataHum);
    ui->plot->graph(1)->data()->set(timeDataTemp);
    ui->plot->graph(2)->data()->set(timeDataDawPoint);
    ui->plot->graph(3)->data()->set(timeDataAbsHum);
    ui->plot->graph(4)->data()->set(timeDataIntegralHum);
    ui->plot->graph(5)->data()->set(timeDataIdealHum);

    //errorBars->setData(y1err);

    // activate top and right axes, which are invisible by default:
    ui->plot->xAxis2->setVisible(false);
    ui->plot->yAxis2->setVisible(true);

    //ui->plot->axisRect()->setRangeDragAxes(ui->plot->xAxis, ui->plot->yAxis2);
    //ui->plot->axisRect()->setRangeDragAxes(ui->plot->xAxis, ui->plot->yAxis);
    //ui->plot->axisRect()->setRangeZoomAxes();

    // set ranges appropriate to show data:
    // configure bottom axis to show date instead of number:
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("hh:mm:ss\ndd.MM.yyyy");
    ui->plot->xAxis->setTicker(dateTicker);
    ui->plot->xAxis->setRange(starttime, starttime + csvModel->rowCount()*60);

    ui->plot->xAxis2->setTicker(dateTicker);
    ui->plot->xAxis2->setRange(starttime, starttime + csvModel->rowCount()*60);

    ui->plot->yAxis->setRange(0, 70);
    ui->plot->yAxis2->setRange(0, 30);
    // set pi ticks on top axis:
    // add title layout element:
    ui->plot->plotLayout()->insertRow(0);
    QString mystarttitle = csvModel->data(csvModel->index(0, 0)).toString();
    QString myendtitle = csvModel->data(csvModel->index(csvModel->rowCount()-1, 0)).toString();
    QString mytitle = "SSC Hygro-Therm Log vom\n" + mystarttitle + " - " + myendtitle;
    //ui->plot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->plot, mytitle, QFont("sans", 12, QFont::Bold)));
    ui->plot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->plot, " ", QFont("sans", 3, QFont::Bold)));
    // set labels:
    ui->plot->xAxis->setLabel("Zeit & Datum");
    ui->plot->yAxis->setLabel("Luftfeuchte in % oder g/m³");
    ui->plot->yAxis2->setLabel("Temperatur in °C");
    // make ticks on bottom axis go outward:

    ui->plot->xAxis->setTickLength(0, 5);
    ui->plot->xAxis->setSubTickLength(0, 3);

    // make ticks on right axis go inward and outward:
    //ui->plot->yAxis2->setTickLength(3, 3);
    //ui->plot->yAxis2->setSubTickLength(1, 1);

}


void MainWindow::addDataFromFile(){
    QString filePath = QFileDialog::getOpenFileName(
                       this, tr("Open File"), "",
                       tr("CSV Liste (*.csv *.CSV *.txt *.TXT)"));
        if (!filePath.isEmpty())
            loadFile(filePath);
}




void MainWindow::loadFile(const QString &filePath){

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QFile file(filePath);

    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("Fehler"),
                             tr("Die Datei \n %1 \n konnte nicht gefunden werden.")
                             .arg(filePath));
        return;
    }
    else
    {

        QTextStream CSVTextStream (&file);
        unsigned int count = 0;

        while (!CSVTextStream.atEnd()){

            QString line = CSVTextStream.readLine();
			if( line.contains("HTTP") || line.isEmpty() ) break;
            // Adding to the model in line with the elements
            QList<QStandardItem *> standardItemsList;
            // consider that the line separated by semicolons into columns
            QStringList myItems = line.split(",");
            for(int i = 0; i < myItems.length();i++){
                QString item = myItems[i];
                standardItemsList.append(new QStandardItem(item));
            }


            if(count>2) // ignore first 3 lines
                csvModel->insertRow(csvModel->rowCount(), standardItemsList);
            count++;

        }

        file.close();
        ui->tableView->resizeColumnsToContents();
        this->setupPlot();
        ui->plot->setVisible(true);

    }

}

void MainWindow::clearCSVList(){
    csvModel->removeRows(0,csvModel->rowCount());
    ui->plot->clearGraphs();
    ui->plot->setVisible(false);
}

void MainWindow::axisLabelClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
    Q_UNUSED(part)
    if(axis->axisType() == QCPAxis::atLeft){
        ui->plot->axisRect()->setRangeDragAxes(ui->plot->xAxis, ui->plot->yAxis);
        ui->plot->axisRect()->setRangeZoomAxes(ui->plot->xAxis, ui->plot->yAxis);
    }

    if(axis->axisType() == QCPAxis::atRight){
        ui->plot->axisRect()->setRangeDragAxes(ui->plot->xAxis, ui->plot->yAxis2);
        ui->plot->axisRect()->setRangeZoomAxes(ui->plot->xAxis, ui->plot->yAxis2);
    }

}

void MainWindow::legendClick(QCPLegend *legend, QCPAbstractLegendItem *item){

    Q_UNUSED(legend)
    if (item) // only react if item was clicked (user could have clicked on border padding of legend where there is no item, then item is 0)
    {
        int toggle = -1;
        QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
        QString test = plItem->plottable()->name();
        if( test.contains("Relative Luftfeuchte in") ) toggle = 0;
        if( test.contains("Temperatur in ") ) toggle = 1;
        if( test.contains("Taupunkt") ) toggle = 2;
        if( test.contains("Luftfeuchte in g") ) toggle = 3;
        if( test.contains("(Links) Durchschnitt Luftfeuchte") ) toggle = 4;
        if( test.contains("(Links) ideale Luftfeuchte in") ) toggle = 5;

        qDebug() << "Legend click: on item: "<< plItem->plottable()->name() << " graph number:"<< toggle;
        if(toggle >=0){
            ui->plot->graph(toggle)->setVisible( !ui->plot->graph(toggle)->visible() );
            if(ui->plot->graph(toggle)->visible()) plItem->setTextColor(Qt::black);
            else plItem->setTextColor(Qt::gray);
            ui->plot->replot();
        }
    }

}



// hoolla

void MainWindow::on_plotUebertragen_clicked()
{
//  QTextCursor cursor = ui->textEdit->textCursor();
//  //qDebug() << "einfugen-klicked :";
//  cursor.insertText(QString(QChar::ObjectReplacementCharacter), QCPDocumentObject::generatePlotFormat(ui->plot, 630, 600));
//  ui->textEdit->setTextCursor(cursor);

    QImage myImage = ui->plot->toPixmap(630, 600,1.0).toImage();
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.insertImage(myImage);
}

void MainWindow::on_plotUebertragenFlach_clicked()
{
//  QTextCursor cursor = ui->textEdit->textCursor();
//  //qDebug() << "einfugen-klicked :";
//  cursor.insertText(QString(QChar::ObjectReplacementCharacter), QCPDocumentObject::generatePlotFormat(ui->plot, 630, 300));
//  ui->textEdit->setTextCursor(cursor);

    QImage myImage = ui->plot->toPixmap(630, 300,1.0).toImage();
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.insertImage(myImage);

}

// text editor tools
void MainWindow::on_toolButtonBold_clicked(){

    if(get_intern_Positon_change()){
        set_intern_Positon_change( false );
        return;
    }

    QTextCharFormat fmt;
    fmt.setFontWeight(ui->toolButtonBold->isChecked() ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::on_toolButtonUnterstrichen_clicked()
{
    if(get_intern_Positon_change()){
        set_intern_Positon_change( false );
        return;
    }

    QTextCharFormat fmt;
    fmt.setFontUnderline(ui->toolButtonUnterstrichen->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::on_toolButtonItalic_clicked()
{
    qDebug() << "italik-klicked :";
    if(get_intern_Positon_change()){
        set_intern_Positon_change( false );
        return;
    }

    QTextCharFormat fmt;
    fmt.setFontItalic(ui->toolButtonItalic->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::on_comboSize_currentIndexChanged(const QString &arg1)
{
    if(get_intern_Positon_change()){
        set_intern_Positon_change( false );
        return;
    }

    qreal pointSize = arg1.toFloat();
    if (arg1.toFloat() > 0) {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        mergeFormatOnWordOrSelection(fmt);
    }
}

void MainWindow::on_fontComboBox_currentIndexChanged(const QString &arg1)
{
     //qDebug() << "font-changed to :"<< arg1;
     QTextCharFormat fmt;
     fmt.setFontFamily(arg1);
     mergeFormatOnWordOrSelection(fmt);

}

void MainWindow::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = ui->textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    ui->textEdit->mergeCurrentCharFormat(format);
}
// end text editor tools

void MainWindow::on_actionExport_triggered()
{
  QString fileName = QFileDialog::getSaveFileName(this, "Export document as pdf...", qApp->applicationDirPath(), ".pdf");
  if (!fileName.isEmpty())
  {
    QPrinter printer;//(QPrinter::HighResolution);

    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    QMargins pageMargins(20, 20, 20, 20);
#if QT_VERSION < QT_VERSION_CHECK(5, 3, 0)
    printer.setFullPage(false);
    printer.setPaperSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Portrait);
    printer.setPageMargins(pageMargins.left(), pageMargins.top(), pageMargins.right(), pageMargins.bottom(), QPrinter::Millimeter);
#else
    QPageLayout pageLayout;
    pageLayout.setMode(QPageLayout::StandardMode);
    pageLayout.setOrientation(QPageLayout::Portrait);
    pageLayout.setPageSize(QPageSize(QPageSize::A4));
    pageLayout.setUnits(QPageLayout::Millimeter);

    pageLayout.setMargins(QMarginsF(pageMargins));
    printer.setPageLayout(pageLayout);
#endif
    ui->textEdit->document()->setPageSize(printer.pageRect().size());
    ui->textEdit->document()->print(&printer);
  }
}

void MainWindow::on_actionSpeichern_triggered(){
     qDebug() << "speichern-klicked :";

     QString fileName = QFileDialog::getSaveFileName(this, tr("Speicher als Open-Office Writer Dokument"),
                                                     QDir::currentPath(),
                                                     tr("Open-Office Writer (*.odt)"));
     if (!fileName.isEmpty())
     {
       QTextDocumentWriter writer(fileName);
       writer.setFormat("ODF");
       qDebug()<<writer.supportedDocumentFormats();
       writer.write( ui->textEdit->document());
     }
}

void MainWindow::on_actionOffnen_triggered(){
    qDebug() << "offnen-klicked :";

}

void MainWindow::on_action_Hilfe_triggered()
{
    qDebug() << "hilfe-klicked :";
//    QString url = QLatin1String("http://www.myoyxnline-hilde.de <a href=\"whatever\">text</a>");
//    QMessageBox::about(this,QString("Hilfe"), QString("Zeige online Hilfe\n" + url));
    QMessageBox msgBox;
    msgBox.setTextFormat(Qt::RichText);   //this is what makes the links clickable
    msgBox.setText("<center>SSC Hygro-Therm Plotter"
                   "<center>copyright 2018 by Michael Wolkstein"
				   "<center> <a href='https://github.com/wolkstein/SSC-HygroTherm_Data-Logger/wiki/Hilfe-Wiki'>Hilfe Wiki</a>");
    msgBox.setWindowTitle("Hilfe / Über");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::on_pushFoto_clicked()
{
  qDebug() << ui->plot->height() << ui->plot->width();
  if(ui->plot->height() < 10){
      QMessageBox::warning(this, tr("Fehler"),
                           tr("Keine Plot geladen oder zu flach angezeigt.<br> Lade eine CSV Datei oder ziehe den Plot größer auf."));
      return;
  }
  QImage myImage = ui->plot->toPixmap(ui->plot->width(),ui->plot->height(),3.0).toImage();

  myImage.save(QFileDialog::getSaveFileName(this, tr("Plot als Bild Speichern unter"),
                                            QDir::currentPath(),
                                            tr("Bild (*.png)")));

}


void MainWindow::on_checkBoxLegende_toggled()
{

     qDebug() << "toggled-klicked :" << ui->checkBoxLegende->isChecked();

//     if(ui->plot->height() < 10){
//         return;
//     }
     ui->plot->legend->setVisible(ui->checkBoxLegende->isChecked());
     ui->plot->replot();
}

void MainWindow::on_spinBoxIdealHum_valueChanged(const int &arg1){

    qDebug() << "ideal-changed to :"<< arg1 <<" csv model row count:" << csvModel->rowCount();

    // check csv model for data
    if(csvModel->rowCount() < 1) return;

    QVector<QCPGraphData> timeDataIdealHum(csvModel->rowCount());
    for (int i=0; i < csvModel->rowCount(); ++i)
    {

        double cvsTime = QDateTime::fromString(csvModel->data(csvModel->index(i, 0)).toString(),"d.M.yyyy h:m:s").toTime_t();

        timeDataIdealHum[i].key = cvsTime;

        timeDataIdealHum[i].value = static_cast<double>(ui->spinBoxIdealHum->value());
    }

    ui->plot->graph(5)->data()->set(timeDataIdealHum);
    ui->plot->replot();
}

// get feedback
void MainWindow::on_textEdit_cursorPositionChanged()
{
    const QList<int> standardSizes = QFontDatabase::standardSizes();

    QTextCursor cursor = ui->textEdit->textCursor();
    float textsize = cursor.charFormat().fontPointSize();
    bool bold = cursor.charFormat().font().bold();
    bool underline = cursor.charFormat().font().underline();
    bool kursive = cursor.charFormat().font().italic();

    qDebug() << "textedit-position :" << cursor.position() << "size: " << textsize <<"bold: " << bold;

    // size
    set_intern_Positon_change(true);
    ui->comboSize->setCurrentIndex(standardSizes.indexOf(cursor.charFormat().fontPointSize()));
    ui->toolButtonBold->setChecked(bold);
    ui->toolButtonUnterstrichen->setChecked(underline);
    ui->toolButtonItalic->setChecked(kursive);
    set_intern_Positon_change( false );

}

void MainWindow::on_actionUSB_Logger_triggered(){
	 mySerialDialog->show();
}
