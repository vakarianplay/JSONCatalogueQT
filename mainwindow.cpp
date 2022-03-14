#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initUi();
    jsonOpen();
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUi()
{
    ui->pushButton_zoomin->setIcon(QIcon(":/icons/zoom-in.png"));
    ui->pushButton_zoomout->setIcon(QIcon(":/icons/zoom-out.png"));
    ui->pushButton_zoomfit->setIcon(QIcon(":/icons/fit.png"));
}

void MainWindow::on_treeWidget_currentItemChanged(QTreeWidgetItem *current,
                                                  QTreeWidgetItem *previous)
{
    QString converter = ui->treeWidget->currentItem()->text(1);

    if (!converter.isEmpty()) {
        imgPath = QDir::currentPath() + "/img/" + converter;
        scene->clear();
        imageViewer();

    } else {
        scene->clear();
    }
}

void MainWindow::imageViewer()
{
    QImage image(imgPath);
    item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene->setSceneRect(0, 0, image.width(), image.height());
    scene->addItem(item);
    ui->graphicsView->resetMatrix();
    ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
    imgPath.clear();
}

void MainWindow::treeViewer(QJsonDocument jsonCatalog)
{
    if (jsonCatalogError.errorString().toInt() == QJsonParseError::NoError) {
        QJsonArray jsonCatalogAr = QJsonValue(jsonCatalog.object().value("Base")).toArray();

        for (int i = 0; i < jsonCatalogAr.count(); i++) {
            QTreeWidgetItem *theme = new QTreeWidgetItem(ui->treeWidget);
            theme->setText(0, jsonCatalogAr.at(i).toObject().value("Theme").toString());

            QJsonArray elementsAr = jsonCatalogAr.at(i).toObject().value("Elements").toArray();
            qDebug() << elementsAr;
            for (int j = 0; j < elementsAr.count(); j++) {
                QTreeWidgetItem *elementsTheme = new QTreeWidgetItem(theme);
                elementsTheme->setText(0, elementsAr.at(j).toObject().value("name").toString());
                elementsTheme->setText(1, elementsAr.at(j).toObject().value("path").toString());
            }
        }
    }
}

void MainWindow::jsonOpen()
{
    QFile file(QDir::currentPath() + "/data/catalog.json");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QJsonDocument jsonCatalog = QJsonDocument::fromJson(QByteArray(file.readAll()),
                                                            &jsonCatalogError);
        treeViewer(jsonCatalog);
    }
}

void MainWindow::on_pushButton_zoomin_clicked()
{
    ui->graphicsView->scale(scaleFactor, scaleFactor);
}

void MainWindow::on_pushButton_zoomout_clicked()
{
    ui->graphicsView->scale(1 / scaleFactor, 1 / scaleFactor);
}

void MainWindow::on_pushButton_zoomfit_clicked()
{
    ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}
