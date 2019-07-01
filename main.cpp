#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QCanBus>
#include <QCanBusDevice>
#include <QCanBusFrame>
#include <iostream>
#include <canframeid.h>
#include <QTimer>
#include <QRandomGenerator>
#include <QDebug>


using namespace std;

// Global.
QObject *object;
QCanBusDevice *device;

int decodeFrame(const QCanBusFrame &frame)
{
    int value;
    const QByteArray payload = frame.payload();

    if(frame.isValid())
    {
        switch(frame.frameId())
        {
            case VEHICLE_SPEED:
            {
                unsigned short b3, b4;

                b3 = payload[3];
                b4 = payload[4];
                qDebug() << b3 << b4;
                value = b3 + b4;
                break;
            } 

        }

        return value;
    }

    value = 1;

    return value;
}

void checkFrames()
{
   
    // Read frames.
    while(device->framesAvailable() > 0)
    {

        QCanBusFrame frame = device->readFrame();

        if(frame.isValid())
        {
            switch(frame.frameId())
            {
                case VEHICLE_SPEED:

                    object->setProperty("speed", decodeFrame(frame));

                    break;


                default:
                    break;
            }
        }
    }
}

QCanBusDevice::Filter setCanFilter(const unsigned short &id)
{
    QCanBusDevice::Filter filter;

    filter.frameId = id;
    filter.frameIdMask = 0x7FFu; // Compare against all 11-bits of frame id.
    filter.format = QCanBusDevice::Filter::MatchBaseFormat;
    filter.type = QCanBusFrame::DataFrame;

    return filter;
}

void sendCan ()
{
    quint16 val_3, val_4;
    std::string send;
    quint16 val = QRandomGenerator::global()->bounded(0,250);

    if (val == 0){
        val_3 = 0;
        val_4 = 0;
        send = "cansend vcan0 316#1122330000667788";
    }
    else if (val < 16) {
        val_3 = val;
        val_4 = 0;
        send = "cansend vcan0 316#1122330" + QString::number(val_3,16).toStdString() + "00667788";
    }
    else if (val < 128){
        val_3 = val;
        val_4 = 0;
        send = "cansend vcan0 316#112233" + QString::number(val_3,16).toStdString() + "00667788";
    }
    else{
        val_3 = 127;
        val_4 = 255 - val;
        if (val_4 < 16){
            send = "cansend vcan0 316#112233" + QString::number(val_3,16).toStdString() + "0" + QString::number(val_4,16).toStdString() + "667788";
        }
        else {
            send = "cansend vcan0 316#112233" + QString::number(val_3,16).toStdString() + QString::number(val_4,16).toStdString() + "667788";
        }

    }

    system(send.c_str());

    //system("cansend vcan0 316#112233FF44556677");
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Load gauge UI.
    QQmlEngine engine;
    QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:/main.qml")));
    object = component.create();

    QTimer *canTimer = new QTimer;
    canTimer->start(1000);

    QObject::connect(canTimer, &QTimer::timeout, sendCan);

/************************************** CAN Bus functionality ***************************************/

    if(QCanBus::instance()->plugins().contains("socketcan"))
    {

        // Create CAN bus device and connect to can0 via SocketCAN plugin.
        device = QCanBus::instance()->createDevice("socketcan", "vcan0");

        device->connectDevice();

        // Apply filters to CAN Bus device.
        QList<QCanBusDevice::Filter> filterList;

        filterList.append(setCanFilter(VEHICLE_SPEED));

        bool fdKey = true;

        device->setConfigurationParameter(QCanBusDevice::RawFilterKey, QVariant::fromValue(filterList));
        device->setConfigurationParameter(QCanBusDevice::CanFdKey, fdKey);

        // Connect framesRecieved signal to slot function for reading frames.
        QObject::connect(device, &QCanBusDevice::framesReceived, checkFrames);

    }

    return app.exec();
}
