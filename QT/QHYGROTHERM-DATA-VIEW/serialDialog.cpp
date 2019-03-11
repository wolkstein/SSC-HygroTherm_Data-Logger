#include "serialDialog.h"

#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
//#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QSpinBox>
#include <QString>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QCloseEvent>

serialDialog::serialDialog(QWidget *parent) :
	QDialog(parent),
	m_serialPortLabel(new QLabel(tr("Serieller Port:"))),
	m_serialPortComboBox(new QComboBox),
	m_waitResponseLabel(new QLabel(tr("Timeout, msec:"))),
	m_waitResponseSpinBox(new QSpinBox),
	m_requestLabel(new QLabel(tr("(G) hole, (D) lösche"))),
	m_requestLineEdit(new QLineEdit(tr("G"))),
	m_trafficLabel(new QLabel(tr("Keine Daten."))),
	m_statusLabel(new QLabel(tr("Status: wartend."))),
	m_textEdit(new QTextBrowser()),
	m_runButton(new QPushButton(tr("Sende Anfrage"))),
	m_saveButton(new QPushButton(tr("Daten speichern")))
{
	const auto infos = QSerialPortInfo::availablePorts();

	for (const QSerialPortInfo &info : infos)
		m_serialPortComboBox->addItem(info.portName());

	m_waitResponseSpinBox->setRange(0, 10000);
	m_waitResponseSpinBox->setValue(1000);

	auto mainLayout = new QGridLayout;
	mainLayout->addWidget(m_serialPortLabel, 0, 0);
	mainLayout->addWidget(m_serialPortComboBox, 0, 1);
	mainLayout->addWidget(m_waitResponseLabel, 1, 0);
	mainLayout->addWidget(m_waitResponseSpinBox, 1, 1);
	mainLayout->addWidget(m_requestLabel, 2, 0);
	mainLayout->addWidget(m_requestLineEdit, 2, 1, 1, 1);
	mainLayout->addWidget(m_runButton, 2, 2, 1, 1);
	mainLayout->addWidget(m_trafficLabel, 3, 0, 1, 4);
	mainLayout->addWidget(m_statusLabel, 4, 0, 1, 5);
	mainLayout->addWidget(m_saveButton, 6, 0, 2, 1);
	mainLayout->addWidget(m_textEdit,5, 0, 1, 6);
	setLayout(mainLayout);

	setWindowTitle(tr("USB Logger"));
	m_serialPortComboBox->setFocus();

	connect(m_runButton, &QPushButton::clicked, this, &serialDialog::transaction);
	connect(m_saveButton, &QPushButton::clicked, this, &serialDialog::saveData);
	connect(&m_thread, &MasterThread::response, this, &serialDialog::showResponse);
	connect(&m_thread, &MasterThread::error, this, &serialDialog::processError);
	connect(&m_thread, &MasterThread::timeout, this, &serialDialog::processTimeout);
	connect(&m_thread, &MasterThread::uplstatus, this, &serialDialog::showUplStatus);
}


void serialDialog::closeEvent(QCloseEvent *event){
	event->ignore();
	this->hide();

}

void serialDialog::transaction()
{
	setControlsEnabled(false);
	this->setCursor(Qt::WaitCursor);
	m_statusLabel->setText(tr("Status: Laufend, verbinde zum Port %1.")
						   .arg(m_serialPortComboBox->currentText()));
	m_thread.transaction(m_serialPortComboBox->currentText(),
						 m_waitResponseSpinBox->value(),
						 m_requestLineEdit->text());

}

void serialDialog::showResponse(const QString &s)
{
	setControlsEnabled(true);
	this->setCursor(Qt::ArrowCursor);
	m_trafficLabel->setText(tr("Verkehr, Datentransfer #%1:"
							   "\n\r-Anfrage: %2")
							.arg(++m_transactionCount)
							.arg(m_requestLineEdit->text()));
							//.arg(s));
	m_textEdit->setText(s);
}

void serialDialog::showUplStatus(const int &line){
	m_trafficLabel->setText(tr("Lese Zeichen Nr.:%1")
							.arg(line));
}

void serialDialog::processError(const QString &s)
{
	setControlsEnabled(true);
	this->setCursor(Qt::ArrowCursor);
	m_statusLabel->setText(tr("Status: Nicht laufend, %1.").arg(s));
	m_trafficLabel->setText(tr("Kein Verkehr."));
}

void serialDialog::processTimeout(const QString &s)
{
	setControlsEnabled(true);
	this->setCursor(Qt::ArrowCursor);
	m_statusLabel->setText(tr("Status: Laufend, %1.").arg(s));
	m_trafficLabel->setText(tr("Kein Verkehr."));
}

void serialDialog::setControlsEnabled(bool enable)
{
	m_runButton->setEnabled(enable);
	m_serialPortComboBox->setEnabled(enable);
	m_waitResponseSpinBox->setEnabled(enable);
	m_requestLineEdit->setEnabled(enable);
}

void serialDialog::saveData(){

	//QString filter = "Comma Separated List (*.csv)";
	//QString fileName = QFileDialog::getSaveFileName(this, "Speicher als CSV Datei", QDir::currentPath(), filter, &filter);
	QString fileName = QFileDialog::getSaveFileName(this, tr("Speichere als CSV Datei"), QDir::currentPath(), tr("Comma Separated List (*.csv)"));
	if (fileName != "") {
		QFile file(fileName);

		if (file.open(QIODevice::ReadWrite)) {
		   QTextStream stream(&file);
		   stream << this->m_textEdit->toPlainText();
		   file.flush();
		   file.close();
		}
		else {
		   QMessageBox::critical(this, tr("Fehler"), tr("Datei konnte nicht geschrieben werden"));
		   return;
		}
	}
}
