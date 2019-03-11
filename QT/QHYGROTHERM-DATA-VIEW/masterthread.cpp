#include "masterthread.h"

#include <QtSerialPort/QSerialPort>
#include <QTime>
#include <QDebug>

MasterThread::MasterThread(QObject *parent) :
	QThread(parent)
{
}

MasterThread::~MasterThread()
{
	m_mutex.lock();
	m_quit = true;
	close_port = true;
	m_cond.wakeOne();
	m_mutex.unlock();
	wait();
}

void MasterThread::transaction(const QString &portName, int waitTimeout, const QString &request)
{
	const QMutexLocker locker(&m_mutex);
	m_portName = portName;
	m_waitTimeout = waitTimeout;
	m_request = request;
	if (!isRunning())
		start();
	else
		m_cond.wakeOne();
}

void MasterThread::run()
{
	bool currentPortNameChanged = false;

	m_mutex.lock();
	QString currentPortName;
	if (currentPortName != m_portName) {
		currentPortName = m_portName;
		currentPortNameChanged = true;
	}

	int currentWaitTimeout = m_waitTimeout;
	QString currentRequest = m_request;
	m_mutex.unlock();
	QSerialPort serial;


	if (currentPortName.isEmpty()) {
		emit error(tr("Kein Verbindung eingestellt"));
		return;
	}
	if(close_port) serial.close();
	while (!m_quit) {
		if (currentPortNameChanged) {
			serial.close();
			serial.setPortName(currentPortName);

			if (!serial.open(QIODevice::ReadWrite)) {
				emit error(tr("Kann nicht verbinden %1, Fehler Nr.: %2")
						   .arg(m_portName).arg(serial.error()));
				return;
			}
			else
			{
				//qDebug() << "serial is open";

				if(!serial.setBaudRate(QSerialPort::Baud115200))
					qDebug()<<serial.errorString();

				if(!serial.setDataBits(QSerialPort::Data8))
					qDebug()<<serial.errorString();

				if(!serial.setParity(QSerialPort::NoParity))
					qDebug()<<serial.errorString();

				if(!serial.setStopBits(QSerialPort::OneStop))
					qDebug()<<serial.errorString();

				if(!serial.setFlowControl(QSerialPort::SoftwareControl))
					qDebug()<<serial.errorString();
			}
		}
		// write request
		const QByteArray requestData = currentRequest.toUtf8();
		serial.write(requestData);
		if (serial.waitForBytesWritten(m_waitTimeout)) {
			// read response
			if (serial.waitForReadyRead(currentWaitTimeout)) {
				QByteArray responseData = serial.readAll();
				int dlines = 0;
				while (serial.waitForReadyRead(50)){
					responseData += serial.readAll();
					dlines++;
					this->uplstatus(dlines);

				}

				const QString response = QString::fromUtf8(responseData);
				emit this->response(response);
			} else {
				emit timeout(tr("Wait read response timeout %1")
							 .arg(QTime::currentTime().toString()));
			}
		} else {
			emit timeout(tr("Wait write request timeout %1")
						 .arg(QTime::currentTime().toString()));
		}
		m_mutex.lock();
		m_cond.wait(&m_mutex);
		if (currentPortName != m_portName) {
			currentPortName = m_portName;
			currentPortNameChanged = true;
		} else {
			currentPortNameChanged = false;
		}
		currentWaitTimeout = m_waitTimeout;
		currentRequest = m_request;
		m_mutex.unlock();
	}
}
