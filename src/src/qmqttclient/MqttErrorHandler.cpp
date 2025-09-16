#include "MqttErrorHandler.h"


MqttErrorHandler::MqttErrorHandler(QMqttClient* client, QObject* parent)
        : QObject(parent), m_client(client)
{
    connect(m_client, &QMqttClient::errorChanged, this, &MqttErrorHandler::handleError);
    connect(m_client, &QMqttClient::connected, this, &MqttErrorHandler::handleconnected);
    // connect(m_client, &QMqttClient::connected, this, &MqttErrorHandler::handleconnected);
}

void MqttErrorHandler::handleconnected()
{
    qCritical() << "=========================================";
    qCritical() << "MQTT客户端链接中";
    qCritical() << "=========================================";
    qCritical() << "客户端ID:" << m_client->clientId();
    qCritical() << "服务器:" << m_client->hostname() << ":" << m_client->port();
    qCritical() << "连接状态:" << getConnectionStateString(m_client->state());
    qCritical() << "=========================================";
}

void MqttErrorHandler::handleError(QMqttClient::ClientError error)
{
    qCritical() << "=========================================";
    qCritical() << "MQTT客户端错误报告";
    qCritical() << "=========================================";
    qCritical() << "错误代码:" << error;
    qCritical() << "错误描述:" << getErrorDescription(m_client, error);
    qCritical() << "客户端ID:" << m_client->clientId();
    qCritical() << "服务器:" << m_client->hostname() << ":" << m_client->port();
    qCritical() << "连接状态:" << getConnectionStateString(m_client->state());
    qCritical() << "=========================================";
}

QString MqttErrorHandler::getErrorDescription(QMqttClient* client, QMqttClient::ClientError error)
{
    switch (error) {
    case QMqttClient::NoError: return "连接正常";
    case QMqttClient::InvalidProtocolVersion: return "协议版本不匹配";
    case QMqttClient::IdRejected: return "客户端ID被拒绝";
    case QMqttClient::ServerUnavailable: return "服务器不可用";
    case QMqttClient::BadUsernameOrPassword: return "认证失败";
    case QMqttClient::NotAuthorized: return "权限不足";
    case QMqttClient::TransportInvalid: return "传输层错误";
    case QMqttClient::ProtocolViolation: return "协议违规";
    case QMqttClient::UnknownError: return "未知错误";
    case QMqttClient::Mqtt5SpecificError: return "MQTT 5.0错误";
    default: return "未知错误码";
    }
}

QString MqttErrorHandler::getConnectionStateString(enum QMqttClient::ClientState state)
{
    switch (state) {
    case QMqttClient::Connecting: return "连接中";
    case QMqttClient::Connected: return "已连接";
    case QMqttClient::Disconnected: return "断开中";
    default: return "未知状态";
    }
}