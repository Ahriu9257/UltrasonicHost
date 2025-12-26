#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class SerialPortHandler : public QObject
{
    Q_OBJECT
public:
    explicit SerialPortHandler(QObject *parent = nullptr);
    ~SerialPortHandler();

    QStringList getAvailablePorts();
    bool openPort(const QString &portName, qint32 baudRate);
    void closePort();
    bool isOpen() const;

signals:
    void distanceReceived(double distance);
    void connectionStatusChanged(bool connected);
    void errorOccurred(const QString &error);

private slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);

private:
    void parseLine(const QByteArray &line);

    QSerialPort *m_serialPort;
    QByteArray m_receiveBuffer;
};

#endif // SERIALPORT_H
