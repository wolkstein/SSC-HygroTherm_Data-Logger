#ifndef SERIALDIALOG_H
#define SERIALDIALOG_H

#include "masterthread.h"

#include <QDialog>
#include <QTextBrowser>

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
	void transaction();
	void saveData();
	void showResponse(const QString &s);
	void showUplStatus(const int &line);
	void processError(const QString &s);
	void processTimeout(const QString &s);
	void closeEvent(QCloseEvent *event);

private:
	void setControlsEnabled(bool enable);

private:
	int m_transactionCount = 0;
	QLabel *m_serialPortLabel = nullptr;
	QComboBox *m_serialPortComboBox = nullptr;
	QLabel *m_waitResponseLabel = nullptr;
	QSpinBox *m_waitResponseSpinBox = nullptr;
	QLabel *m_requestLabel = nullptr;
	QLineEdit *m_requestLineEdit = nullptr;
	QLabel *m_trafficLabel = nullptr;
	QLabel *m_statusLabel = nullptr;
	QPushButton *m_runButton = nullptr;
	QPushButton *m_saveButton = nullptr;

	QTextBrowser *m_textEdit = nullptr;

	MasterThread m_thread;
};

#endif // SERIALDIALOG_H
