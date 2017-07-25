#include "mainwindow.h"

#include <QWidget>
#include <QList>
#include <QDebug>
#include <QFile>
#include <QXmlStreamWriter>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    this->parentW=parent;
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::Window|Qt::FramelessWindowHint);
    this->setCentralWidget(new QWidget());

    initTitlebar();
    styleOptionBar=new StyleOptionBar(this);
    content=new QTextEdit(this);
    statusBar=new QStatusBar(this);

    this->setFocusProxy(this->content);

    connect(styleOptionBar,SIGNAL(styleBtnClickedSignal(QString)),this,SLOT(onStyleBtnClicked(QString)));
    styleOptionBar->setVisible(false);

    content->setFocusPolicy(Qt::StrongFocus);
    content->setStyleSheet("background-color:transparent;"
                           "font-size:14px;"
                           "border:none;"
                           "font-family:'Microsoft Yahei'");

    statusBar->setFixedHeight(8);
    statusBar->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    QVBoxLayout *main_layout=new QVBoxLayout(this);
    main_layout->addWidget(titlebar);
    main_layout->addWidget(styleOptionBar);

    content_layout=new QVBoxLayout(this);
    content_layout->addWidget(content);
    content_layout->setContentsMargins(12,0,12,4);
    main_layout->addLayout(content_layout);
    main_layout->addWidget(statusBar);

    main_layout->setContentsMargins(0,0,0,0);
    centralWidget()->setLayout(main_layout);
}

MainWindow::~MainWindow(){
    delete content;
    delete styleOptionBar;
    delete titlebar;
}

QString MainWindow::getContentText(){
    return content->toPlainText();
}

void MainWindow::setContentTest(QString text){
    this->content->append(text);
}

void MainWindow::initTitlebar(){
    titlebar=new TitleBar(this);
    titlebar->setBtnsVisible(false);
    connect(titlebar,SIGNAL(newBtnClickedSignal()),this,SLOT(onNewBtnClicked()));
    connect(titlebar,SIGNAL(settingBtnClickedSignal()),this,SLOT(onSettingBtnClicked()));
    connect(titlebar,SIGNAL(deleteBtnClickedSignal()),this,SLOT(onDeleteBtnClicked()));
    connect(titlebar,SIGNAL(closeBtnClickedSignal()),this,SLOT(onCloseBtnClicked()));
}

void MainWindow::onNewBtnClicked(){
    MainWindow *newWindow=new MainWindow(this->parentW);
    newWindow->setGeometry(this->pos().x()+20,
                           this->pos().y()+20,
                           this->size().width(),
                           this->size().height());
    newWindow->setStyleSheet(this->style);
    newWindow->show();
    newWindow->setFocus();
}

void MainWindow::onSettingBtnClicked(){
    this->styleOptionBar->setVisible(!styleOptionBar->isVisible());
    this->titlebar->setVisible(false);
    this->styleOptionBar->setFocus();
    //this->content_layout->setContentsMargins(12,0,12,4);
}

void MainWindow::onDeleteBtnClicked(){
    this->hide();
    bool isAllDeleted=true;
    QList<MainWindow *> windows= this->parentWidget()->findChildren<MainWindow *>();
    if(!windows.isEmpty()){
        foreach (MainWindow *mainwindow, windows) {
            if(mainwindow->isVisible()){
                isAllDeleted=false;
                break;
            }
        }
    }
    if(isAllDeleted)
        this->onCloseBtnClicked();
}

void MainWindow::onCloseBtnClicked(){
    this->close();
}

void MainWindow::onStyleBtnClicked(QString btnName){
    this->style=btnName;
    this->setStyleSheet(btnName);
    this->styleOptionBar->setVisible(false);
    this->titlebar->setVisible(true);
    this->content->setFocus();
}

void MainWindow::closeEvent(QCloseEvent *event){
    QList<MainWindow *> windows= this->parentWidget()->findChildren<MainWindow *>();
    if(!windows.isEmpty()){
        QFile file("record.xml");
        file.open(QIODevice::WriteOnly);
        QXmlStreamWriter xmlWriter(&file);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();
        xmlWriter.writeStartElement("windows");
        foreach (MainWindow *mainwindow, windows) {
            if(mainwindow->isVisible()){
                xmlWriter.writeStartElement("window");
                xmlWriter.writeTextElement("x",QString::number(mainwindow->pos().x()));
                xmlWriter.writeTextElement("y",QString::number(mainwindow->pos().y()));
                xmlWriter.writeTextElement("width",QString::number(mainwindow->size().width()));
                xmlWriter.writeTextElement("height",QString::number(mainwindow->size().height()));
                xmlWriter.writeTextElement("style",mainwindow->style);
                xmlWriter.writeTextElement("content",mainwindow->getContentText());
                xmlWriter.writeEndElement();
            }
        }
        xmlWriter.writeEndElement();
        xmlWriter.writeEndDocument();
        file.close();
    }
    return QWidget::closeEvent(event);
}

void MainWindow::enterEvent(QEvent *event){
    this->titlebar->setBtnsVisible(true);
    this->content->setFocus();
    return QWidget::enterEvent(event);
}

void MainWindow::leaveEvent(QEvent *event){
    this->titlebar->setBtnsVisible(false);
    this->styleOptionBar->setVisible(false);
    this->content->clearFocus();
    return QWidget::leaveEvent(event);
}
