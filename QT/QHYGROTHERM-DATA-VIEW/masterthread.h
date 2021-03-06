#ifndef MASTERTHREAD_H
#define MASTERTHREAD_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>

class MasterThread : public QThread
{
	Q_OBJECT

public:
	explicit MasterThread(QObject *parent = nullptr);
	~MasterThread();

	void transaction(const QString &portName, int waitTimeout, const QString &request);

signals:
	void response(const QString &s);
	void uplstatus(const int &line);
	void error(const QString &s);
	void timeout(const QString &s);

private:
	void run() override;

	QString m_portName;
	QString m_request;
	int m_waitTimeout = 0;
	QMutex m_mutex;
	QWaitCondition m_cond;
	bool m_quit = false;
	bool close_port =false;
};

#endif // MASTERTHREAD_H
