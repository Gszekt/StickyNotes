#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStatusBar>
#include <QTextEdit>
#include <QString>
#include <QFocusEvent>

#include "titlebar.h"
#include "styleoptionbar.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString getContentText();
    void setContentTest(QString);

private:
    TitleBar *titlebar;
    StyleOptionBar *styleOptionBar;
    QTextEdit *content;
    QStatusBar *statusBar;
    QVBoxLayout *content_layout;
    QWidget *parentW;
    QString style="background: rgb(243,243,243);";

    void initTitlebar();
    void setStyle(const QString &style);

private slots:
    void onNewBtnClicked();
    void onSettingBtnClicked();
    void onDeleteBtnClicked();
    void onCloseBtnClicked();
    void onStyleBtnClicked(QString btnName);

protected:
    virtual void closeEvent(QCloseEvent *event) override;
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
};

#endif // MAINWINDOW_H
