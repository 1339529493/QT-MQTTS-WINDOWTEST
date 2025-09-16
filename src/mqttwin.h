#pragma once
#include "ui_mqttwin.h"
#include <QDebug>
#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QIntValidator>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QTextEdit>

class mqttwin : public QMainWindow {
    Q_OBJECT
    
public:
    mqttwin(QWidget* parent = nullptr);
    ~mqttwin();

private:
    Ui_mqttwin* ui;

    // 连接参数控件
    QLineEdit *hostEdit;
    QLineEdit *portEdit;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *clientIdEdit;
    QCheckBox *cleanSessionCheck;
    
    // 发布订阅控件
    QLineEdit *publishTopicEdit;
    QLineEdit *publishMessageEdit;
    QLineEdit *subscribeTopicEdit;
    
    // 按钮控件
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QPushButton *publishButton;
    QPushButton *subscribeButton;
    QPushButton *unsubscribeButton;
    QPushButton *clearButton;
    
    // 显示区域
    QTextEdit *messageDisplay;
    
    // 验证器
    QIntValidator *portValidator;

private slots:
    // 按钮点击的槽函数，slots就是用来声明槽函数的（感觉有没有都没影响啊，可能是可读性吧）
    void connectButtonClicked();
    void disconnectButtonClicked();
    void publishButtonClicked();
    void clearButtonClicked();
    void subscribeButtonClicked();
    void unsubscribeButtonClicked();
};