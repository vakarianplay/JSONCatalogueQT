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

    QPixmap pixmap(":/icons/search.png");
    QPixmap pixmapScaled = pixmap.scaled(QSize(15, 15),
                                         Qt::IgnoreAspectRatio,
                                         Qt::SmoothTransformation);
    ui->treeWidget->setHeaderLabels({"Наименование", "Децемальный №"});
    ui->label_dec->clear();
    ui->label_name->clear();
    ui->label_iconsearch->setPixmap(pixmapScaled);
}

void MainWindow::on_treeWidget_currentItemChanged(QTreeWidgetItem *current,
                                                  QTreeWidgetItem *previous)
{
    QString converter;

    if (ui->treeWidget->isEnabled()) {
        converter = ui->treeWidget->currentItem()->text(2);
        if (!converter.isEmpty()) {
            ui->label_name->setText(ui->treeWidget->currentItem()->text(0));
            ui->label_dec->setText(ui->treeWidget->currentItem()->text(1));
            imgPath = QDir::currentPath() + "/img/" + converter;
            scene->clear();
            imageViewer();

        } else {
            scene->clear();
            ui->label_dec->clear();
            ui->label_name->clear();
        }
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
            theme->setText(1, jsonCatalogAr.at(i).toObject().value("DecTheme").toString());
            QJsonArray articleAr = jsonCatalogAr.at(i).toObject().value("Article").toArray();

            for (int j = 0; j < articleAr.count(); j++) {
                QTreeWidgetItem *article = new QTreeWidgetItem(theme);
                article->setText(0, articleAr.at(j).toObject().value("AtricleName").toString());
                article->setText(1, articleAr.at(j).toObject().value("DecArticle").toString());
                article->setText(2, articleAr.at(j).toObject().value("PathArticle").toString());
                QJsonArray consistAr = articleAr.at(j).toObject().value("Consist").toArray();

                for (int k = 0; k < consistAr.count(); k++) {
                    QTreeWidgetItem *consist = new QTreeWidgetItem(article);
                    consist->setText(0, consistAr.at(k).toObject().value("Name").toString());
                    consist->setText(1, consistAr.at(k).toObject().value("Dec").toString());
                    consist->setText(2, consistAr.at(k).toObject().value("Path").toString());
                }
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
        file.close();
        ui->treeWidget->setEnabled(true);
        treeViewer(jsonCatalog);

    } else {
        qDebug() << "error parce";
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

void MainWindow::searching()
{
    QString searchRe = ui->lineEdit_search->text();
    if (searchRe.isEmpty()) {
        return;
    }
    QList<QTreeWidgetItem *> searchList;
    searchList = ui->treeWidget->findItems(searchRe, Qt::MatchContains | Qt::MatchRecursive, 0);

    if (searchList.isEmpty()) {
        searchList = ui->treeWidget->findItems(searchRe, Qt::MatchContains | Qt::MatchRecursive, 1);
    }

    for (QTreeWidgetItem *item : searchList) {
        ui->treeWidget->setCurrentItem(item, 0);
    }
}

void MainWindow::on_lineEdit_search_textChanged(const QString &arg1)
{
    searching();
}
