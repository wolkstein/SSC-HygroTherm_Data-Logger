#ifndef SERIALDIALOG_H
#define SERIALDIALOG_H

#include "masterthread.h"

#include <QDialog>
#include <QTextBrowser>
//#include <QString>

class QLabel;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QComboBox;
class QTextBrowser;


class serialDialog : public QDialog
{
	Q_OBJECT

public:
	explicit serialDialog(QWidget *parent = nullptr);

private slots:
	void transaction(const QString &s);
	void saveData();
	void showResponse(const QString &s);
	void showUplStatus(const int &line);
	void processError(const QString &s);
	void processTimeout(const QString &s);
	void closeEvent(QCloseEvent *event);
	void getDataFromDevice();
	void setTimeOnDevice();
	void deleteDataOnDevice();	
	void updateSerialInfo();

private:
	void setControlsEnabled(bool enable);

	QString m_anfrage = "";
	int m_transactionCount = 0;
	QLabel *m_serialPortLabel = nullptr;
	QComboBox *m_serialPortComboBox = nullptr;
	QLabel *m_waitResponseLabel = nullptr;
	QSpinBox *m_waitResponseSpinBox = nullptr;
	QPushButton *m_setzteUhrzeit = nullptr;
	QPushButton *m_deleteDataFile = nullptr;
	QLabel *m_trafficLabel = nullptr;
	QLabel *m_statusLabel = nullptr;
	QPushButton *m_holeDatenBtn = nullptr;
	QPushButton *m_saveButton = nullptr;

	QTextBrowser *m_textEdit = nullptr;

	MasterThread m_thread;
};

#endif // SERIALDIALOG_H
