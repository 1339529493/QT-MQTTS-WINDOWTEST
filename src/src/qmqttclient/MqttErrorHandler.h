#include <QDebug>
#include <QtMqtt/QMqttClient>

class MqttErrorHandler : public QObject
{
    // Q_OBJECT
private:
    QMqttClient* m_client;

public:
    explicit MqttErrorHandler(QMqttClient* client, QObject* parent = nullptr);

private slots:
    void handleError(QMqttClient::ClientError error);
    void handleconnected();
    void handledisconnected();
    void handlesubscribe();
    void handleunsubscribe();
    void handlepublish();
    static QString getErrorDescription(QMqttClient* client, QMqttClient::ClientError error);
    static QString getConnectionStateString(enum QMqttClient::ClientState state);
};
