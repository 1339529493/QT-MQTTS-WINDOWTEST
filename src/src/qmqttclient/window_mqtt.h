#ifndef _WINDOW_MQTT_H_
#define _WINDOW_MQTT_H_

#include <QtMqtt/QMqttClient>
#include <QTextEdit>

const char *get_cert();
int mqtt_connect(QString& hostName, quint16 port, QString& clientId, QString& username, QByteArray& password, bool cleanSession);
void mqtt_disconnect();
int mqtt_sub(QString& sub_topic, QTextEdit *message_text);
void mqtt_unsub(QString& sub_topic);
int mqtt_pub(QString& pub_topic, QString& pub_memssage);

#endif 