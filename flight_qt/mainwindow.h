#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "allSoure.h"
#include <QMainWindow>
#include <QPropertyAnimation>
#include "searchpage.h"
#include "dbhelper.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    //登录功能
    void login();
    //页面切换
    void showRegisterPage();
    void showForgetPage();
    void backLogin();
    //从查询界面返回登录界面
    void showLogin();
    //注册
    void registerUser();
    //忘记密码
    void forgetUser();
private:
    Ui::MainWindow *ui;
    searchPage* searcheView;
};
#endif // MAINWINDOW_H
