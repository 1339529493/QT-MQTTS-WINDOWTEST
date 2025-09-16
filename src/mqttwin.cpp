#include "mqttwin.h"
#include "window_mqtt.h"
#include <QGroupBox>
#include <QMessageBox>

mqttwin::mqttwin(QWidget* parent)
    : QMainWindow(parent)
{
    // 设置窗口标题和大小
    setWindowTitle("MQTT Client");
    setMinimumSize(800, 600);
    
    // 创建中央部件
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    
    // 1. 连接参数组
    QGroupBox *connectionGroup = new QGroupBox("连接参数", centralWidget);
    QGridLayout *connectionLayout = new QGridLayout(connectionGroup);
    
    connectionLayout->addWidget(new QLabel("服务器地址:"), 0, 0);
    hostEdit = new QLineEdit("lh-world.icu", connectionGroup);
    connectionLayout->addWidget(hostEdit, 0, 1);
    
    connectionLayout->addWidget(new QLabel("端口:"), 0, 2);
    portEdit = new QLineEdit("1883", connectionGroup);
    portValidator = new QIntValidator(1, 65535, this);
    portEdit->setValidator(portValidator);
    connectionLayout->addWidget(portEdit, 0, 3);
    
    connectionLayout->addWidget(new QLabel("用户名:"), 1, 0);
    usernameEdit = new QLineEdit("lh_mqtts", connectionGroup);
    connectionLayout->addWidget(usernameEdit, 1, 1);
    
    connectionLayout->addWidget(new QLabel("密码:"), 1, 2);
    passwordEdit = new QLineEdit("1993047286", connectionGroup);
    passwordEdit->setEchoMode(QLineEdit::Password);
    connectionLayout->addWidget(passwordEdit, 1, 3);
    
    connectionLayout->addWidget(new QLabel("客户端ID:"), 2, 0);
    clientIdEdit = new QLineEdit("QMQTT_Client_1", connectionGroup);
    connectionLayout->addWidget(clientIdEdit, 2, 1);
    
    cleanSessionCheck = new QCheckBox("清除会话", connectionGroup);
    cleanSessionCheck->setChecked(true);
    connectionLayout->addWidget(cleanSessionCheck, 2, 2, 1, 2);
    
    mainLayout->addWidget(connectionGroup);
    
    // 2. 连接按钮行
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    connectButton = new QPushButton("连接", centralWidget);
    disconnectButton = new QPushButton("断开", centralWidget);
    disconnectButton->setEnabled(false);
    
    buttonLayout->addWidget(connectButton);
    buttonLayout->addWidget(disconnectButton);
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);

    connect(connectButton, &QPushButton::clicked, this, &mqttwin::connectButtonClicked);
    connect(disconnectButton, &QPushButton::clicked, this, &mqttwin::disconnectButtonClicked);
    
    // 3. 发布消息组
    QGroupBox *publishGroup = new QGroupBox("发布消息", centralWidget);
    QVBoxLayout *publishLayout = new QVBoxLayout(publishGroup);
    
    QHBoxLayout *topicLayout = new QHBoxLayout();
    topicLayout->addWidget(new QLabel("主题:"));
    publishTopicEdit = new QLineEdit("user1/topic", publishGroup);
    topicLayout->addWidget(publishTopicEdit);
    
    QHBoxLayout *messageLayout = new QHBoxLayout();
    messageLayout->addWidget(new QLabel("消息:"));
    publishMessageEdit = new QLineEdit("Hello MQTT!", publishGroup);
    messageLayout->addWidget(publishMessageEdit);
    
    publishButton = new QPushButton("发布", publishGroup);
    publishButton->setEnabled(false);

    publishLayout->addLayout(topicLayout);
    publishLayout->addLayout(messageLayout);
    publishLayout->addWidget(publishButton);
    
    mainLayout->addWidget(publishGroup);

    connect(publishButton, &QPushButton::clicked, this, &mqttwin::publishButtonClicked);
    
    // 4. 订阅主题组
    QGroupBox *subscribeGroup = new QGroupBox("订阅主题", centralWidget);
    QHBoxLayout *subscribeLayout = new QHBoxLayout(subscribeGroup);
    
    subscribeLayout->addWidget(new QLabel("主题:"));
    subscribeTopicEdit = new QLineEdit("user1/topic", subscribeGroup);
    subscribeLayout->addWidget(subscribeTopicEdit);
    
    subscribeButton = new QPushButton("订阅", subscribeGroup);
    unsubscribeButton = new QPushButton("取消订阅", subscribeGroup);
    subscribeButton->setEnabled(false);
    unsubscribeButton->setEnabled(false);
    
    subscribeLayout->addWidget(subscribeButton);
    subscribeLayout->addWidget(unsubscribeButton);
    
    mainLayout->addWidget(subscribeGroup);

    connect(subscribeButton, &QPushButton::clicked, this, &mqttwin::subscribeButtonClicked);
    connect(unsubscribeButton, &QPushButton::clicked, this, &mqttwin::unsubscribeButtonClicked);
    
    // 5. 消息显示区域
    QGroupBox *messageGroup = new QGroupBox("消息日志", centralWidget);
    QVBoxLayout *messageGroupLayout = new QVBoxLayout(messageGroup);
    
    messageDisplay = new QTextEdit(messageGroup);
    messageDisplay->setReadOnly(true);
    messageDisplay->setPlaceholderText("MQTT消息将显示在这里...");

    // 创建水平布局放置清空按钮（靠右对齐）
    QHBoxLayout *clearButtonLayout = new QHBoxLayout();
    clearButtonLayout->addStretch(); // 添加弹性空间，将按钮推到右边

    QPushButton *clearButton = new QPushButton("清空", messageGroup);
    clearButton->setFixedSize(80, 30); // 设置固定大小
    clearButtonLayout->addWidget(clearButton);

    // 将文本框和按钮布局添加到消息组
    messageGroupLayout->addWidget(messageDisplay);
    messageGroupLayout->addLayout(clearButtonLayout);

    mainLayout->addWidget(messageGroup, 1); // 设置拉伸因子为1，占据更多空间

    connect(clearButton, &QPushButton::clicked, this, &mqttwin::clearButtonClicked);
    
    // 6.设置中央部件
    setCentralWidget(centralWidget);
}

mqttwin::~mqttwin()
{
    delete ui; 

}

void mqttwin::connectButtonClicked()
{
    int ret;
    QString host = hostEdit->text();
    quint16 port = portEdit->text().toUShort();
    QString username = usernameEdit->text();
    QByteArray password = passwordEdit->text().toUtf8();
    QString clientId = clientIdEdit->text();
    bool cleanSession = cleanSessionCheck->isChecked();
    qDebug() << "host:" << host;
    qDebug() << "port:" << port;
    qDebug() << "username:" << username;
    qDebug() << "password:" << password;
    qDebug() << "clientId:" << clientId;
    qDebug() << "cleanSession:" << cleanSession;
    ret = mqtt_connect(host, port, clientId, username, password, cleanSession);
    if (!ret)
    {
        disconnectButton->setEnabled(true);
        subscribeButton->setEnabled(true);
        publishButton->setEnabled(true);
        connectButton->setEnabled(false);
    }
    else{
        QMessageBox::critical(nullptr, "error", "mqtt fail connect");
    }
}

void mqttwin::disconnectButtonClicked()
{
    mqtt_disconnect();
    disconnectButton->setEnabled(false);
    subscribeButton->setEnabled(false);
    unsubscribeButton->setEnabled(false);
    publishButton->setEnabled(false);
    connectButton->setEnabled(true);
}

void mqttwin::publishButtonClicked()
{
    QString publishData = publishMessageEdit->text();
    QString publishTopic = publishTopicEdit->text();
    mqtt_pub(publishTopic, publishData);
    qDebug() << "publishData:" << publishData;
    messageDisplay->insertPlainText("publish  : ");
    messageDisplay->insertPlainText(publishData);
    messageDisplay->insertPlainText("\n");
}

void mqttwin::clearButtonClicked()
{
    messageDisplay->clear();
}

void mqttwin::subscribeButtonClicked()
{
    QString topic = subscribeTopicEdit->text();
    mqtt_sub(topic, messageDisplay);
    subscribeButton->setEnabled(false);
    unsubscribeButton->setEnabled(true);
    subscribeTopicEdit->setEnabled(false);
}

void mqttwin::unsubscribeButtonClicked()
{
    QString topic = subscribeTopicEdit->text();
    mqtt_unsub(topic);
    subscribeButton->setEnabled(true);
    unsubscribeButton->setEnabled(false);
    subscribeTopicEdit->setEnabled(true);
}
