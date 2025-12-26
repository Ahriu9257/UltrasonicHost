#include "serialport.h"
#include <QDebug>

SerialPortHandler::SerialPortHandler(QObject *parent)
    : QObject(parent)
    , m_serialPort(new QSerialPort(this))
{
    connect(m_serialPort, &QSerialPort::readyRead,
            this, &SerialPortHandler::handleReadyRead);
    connect(m_serialPort, &QSerialPort::errorOccurred,
            this, &SerialPortHandler::handleError);
}

SerialPortHandler::~SerialPortHandler()
{
    closePort();
}

QStringList SerialPortHandler::getAvailablePorts()
{
    QStringList list;
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts())
        list << info.portName();
    return list;
}

bool SerialPortHandler::openPort(const QString &portName, qint32 baudRate)
{
    if (m_serialPort->isOpen()) closePort();

    m_serialPort->setPortName(portName);
    m_serialPort->setBaudRate(baudRate);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (m_serialPort->open(QIODevice::ReadWrite)) {
        emit connectionStatusChanged(true);
        return true;
    } else {
        emit errorOccurred("Failed to open port: " + m_serialPort->errorString());
        return false;
    }
}

void SerialPortHandler::closePort()
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
        emit connectionStatusChanged(false);
    }
    m_receiveBuffer.clear();
}

bool SerialPortHandler::isOpen() const
{
    return m_serialPort->isOpen();
}

void SerialPortHandler::handleReadyRead()
{
    m_receiveBuffer.append(m_serialPort->readAll());

    while (true) {
        int idx = m_receiveBuffer.indexOf('\n');
        if (idx < 0) break;

        QByteArray line = m_receiveBuffer.left(idx);
        m_receiveBuffer.remove(0, idx + 1);
        parseLine(line);
    }
}

void SerialPortHandler::parseLine(const QByteArray &line)
{
    QString text = QString::fromUtf8(line).trimmed();
    if (text.isEmpty()) return;

    qDebug() << "RAW:" << text;

    double distance = -1.0;

    if (text.contains(':')) {
        auto parts = text.split(':');
        if (parts.size() == 2) {
            bool ok;
            distance = parts[1].toDouble(&ok);
            if (!ok) return;
        }
    } else if (text.contains('=')) {  // 支持 Distance=65
        auto parts = text.split('=');
        if (parts.size() == 2) {
            bool ok;
            distance = parts[1].toDouble(&ok);
            if (!ok) return;
        }
    } else {
        bool ok;
        distance = text.toDouble(&ok);
        if (!ok) return;
    }

    if (distance >= 0 && distance <= 500)
        emit distanceReceived(distance);
}

void SerialPortHandler::handleError(QSerialPort::SerialPortError error)
{
    if (error != QSerialPort::NoError && error != QSerialPort::TimeoutError)
        emit errorOccurred("Serial error: " + m_serialPort->errorString());
}
