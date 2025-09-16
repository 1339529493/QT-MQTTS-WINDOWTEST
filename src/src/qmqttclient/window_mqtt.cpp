#include "window_mqtt.h"
#include "MqttErrorHandler.h"
#include <QThread>
#include <QCoreApplication>
#include <QTimer>

QMqttClient *window_mqtt = nullptr;
MqttErrorHandler *Mqtt_message_handler = nullptr;
QMqttSubscription *sub_handler = nullptr;
static int mqttclient_init()
{
    window_mqtt = new QMqttClient();
    Mqtt_message_handler = new MqttErrorHandler(window_mqtt);
    return 0;
}

int mqtt_connect(QString& hostName, quint16 port, QString& clientId, QString& username, QByteArray& password, bool cleanSession)
{
    if (!window_mqtt)
    {
        mqttclient_init();
    }
    window_mqtt->setHostname(hostName);
    window_mqtt->setPort(port);
    window_mqtt->setClientId(clientId);
    window_mqtt->setUsername(username);
    window_mqtt->setPassword(password);
    window_mqtt->setCleanSession(cleanSession);
    // window_mqtt->connectToHost();
    QSslCertificate cert = QSslCertificate(get_cert());
    QSslConfiguration sslConfig;
    sslConfig.setCaCertificates({cert});
    window_mqtt->connectToHostEncrypted(sslConfig);

    // 方式一:使用QEventLoop + QTimer（推荐）   //没有事件等待mqtt链接会在未连接上就退出mqtt_connect进入主循环导致connect链接卡死，也不会触发connect信号进入槽函数
    QEventLoop loop;
    QTimer::singleShot(2000, &loop, &QEventLoop::quit);//添加loop循环事件在10000ms后触发quit事件
    QObject::connect(window_mqtt, &QMqttClient::connected, &loop, &QEventLoop::quit);
    loop.exec();

    // // 方式二：或者使用QElapsedTimer（更精确）
    // QElapsedTimer timer;
    // timer.start();
    // while(timer.elapsed() < 2000) { //ms级
    //     QCoreApplication::processEvents();  // 在循环中继续处理QT事件(防止卡死，不然这里mqtt一直连不上)
    //     if (window_mqtt->state() == QMqttClient::Connected)
    //     {
    //         qDebug() << "---->" << window_mqtt->state();
    //         break;
    //     }
    //     QThread::usleep(1000); // 减少CPU占用，sleep也会使页面卡住，一般不要使用
    // }

    if (window_mqtt->state() == QMqttClient::Connected)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

void mqtt_disconnect()
{
    window_mqtt->disconnectFromHost();
}

int mqtt_sub(QString& sub_topic, QTextEdit *message_text)
{
    sub_handler = window_mqtt->subscribe(sub_topic);
    QObject::connect(sub_handler,QMqttSubscription::messageReceived,window_mqtt,[=](QMqttMessage msg){
        message_text->insertPlainText("subscribe : ");
        // message_text->insertPlainText(QString(msg.id()));
        message_text->insertPlainText(msg.payload());
        message_text->insertPlainText("\n");
    });
    return 0;
}

void mqtt_unsub(QString& sub_topic)
{
    window_mqtt->unsubscribe(sub_topic);
}

int mqtt_pub(QString& pub_topic, QString& pub_memssage)
{
    window_mqtt->publish(QMqttTopicName(pub_topic.toUtf8()),QByteArray(pub_memssage.toUtf8()));
    return 0;
}