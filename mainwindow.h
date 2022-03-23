#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "customview.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMainWindow>
#include <QPixmap>
#include <QTreeView>
#include <QTreeWidget>
#include <QtGui>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void imageViewer();
    void treeViewer(QJsonDocument jsonCatalog);
    void jsonOpen();
    void initUi();
    void searching();

    void on_pushButton_zoomin_clicked();

    void on_pushButton_zoomout_clicked();

    void on_pushButton_zoomfit_clicked();

    void on_lineEdit_search_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QTreeWidget *treeView;
    QGraphicsItem *item;
    QGraphicsScene *scene;
    QModelIndex indexopen;

    double scaleFactor = 1.1;
    QString imgPath;

    QJsonParseError jsonCatalogError;
};

#endif // MAINWINDOW_H
