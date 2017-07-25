#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QWidget>
#include <QXmlStreamReader>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget *parent=new QWidget();

    int x=0;
    int y=0;
    int width=0;
    int height=0;
    int windowCount=0;
    QString style;
    QString content;
    MainWindow *newWindow=nullptr;

    QFile file("record.xml");
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    if(file.exists()){
        QXmlStreamReader xmlReader(&file);
        xmlReader.readNext();
        while (!xmlReader.atEnd()) {
            if (xmlReader.isStartElement()) {
                if (xmlReader.name()=="window") {
                    windowCount++;
                }
                else if (xmlReader.name()=="x") {
                    x=xmlReader.readElementText().toInt();
                }
                else if (xmlReader.name()=="y") {
                    y=xmlReader.readElementText().toInt();
                }
                else if (xmlReader.name()=="width") {
                    width=xmlReader.readElementText().toInt();
                }
                else if (xmlReader.name()=="height") {
                    height=xmlReader.readElementText().toInt();
                }
                else if (xmlReader.name()=="style") {
                    style=xmlReader.readElementText();
                }
                else if (xmlReader.name()=="content") {
                    content=xmlReader.readElementText();
                    if (windowCount > 0){
                        newWindow=new MainWindow(parent);
                        newWindow->setGeometry(x,y,width,height);
                        newWindow->setStyleSheet(style);
                        newWindow->setContentTest(content);
                        newWindow->show();
                    }
                }
            }
            xmlReader.readNextStartElement();
        }
    }
    file.close();
    if(windowCount==0){
        (new MainWindow(parent))->show();
    }
    return a.exec();
}
