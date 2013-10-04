#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

#include <QListWidgetItem>
#include <QObject>
#include "src/MeshField/MainMesh/mainmesh.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    for (int i = 0; i < nEvents; ++i) {
        ui->eventComboBox->addItem(getEventName(i));
    }

    ui->tableWidget->setColumnCount(2);

    QStringList list;
    list << "Argument" << "Value";
    ui->tableWidget->setHorizontalHeaderLabels(list);

    QStringList list2;
    list2 << "X" << "Y";

#if ENS_DIM == 3
    list2 << "Z";
#else
    ui->tableWidget_mesh->setRowCount(2);
#endif
    ui->tableWidget_mesh->setVerticalHeaderLabels(list2);


    QStringList list3;
    list3 << "Start" << "End";
    ui->tableWidget_mesh->setHorizontalHeaderLabels(list3);

    vec masses = zeros(1);

    ensemble = new Ensemble(masses);

    mat topology(ENS_DIM, 2);
    topology << 0 << 1 << endr << 0 << 1;

#if ENS_DIM == 3
    topology << 0 << 1 << endr;
#endif

    mainMesh = new MainMesh(topology, *ensemble);

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < ENS_DIM; ++j) {
            ui->tableWidget_mesh->setItem(j, i, new QTableWidgetItem(
                                              QString::fromStdString(boost::lexical_cast<std::string>(topology(i, j)))));
        }
    }

    addMesh(mainMesh);

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_eventComboBox_currentIndexChanged(int index)
{
    std::vector<int> inputs = getInputs(index);
    updateTable(inputs);

}

void MainWindow::updateTable(std::vector<int> inputs)
{
    int n = inputs.size();

    if (n == 0){
        ui->tableWidget->setDisabled(true);
    } else {
        if (!ui->tableWidget->isEnabled()){
            ui->tableWidget->setEnabled(true);
        }
    }

    ui->tableWidget->setRowCount(n);

    int i = 0;
    for (int inputIndex : inputs) {
        ui->tableWidget->setCellWidget(i, 0, new QLabel(getInputName(inputIndex), ui->tableWidget));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(getStructValFromIndex(inputIndex)));
        i++;
    }
}


void MainWindow::updateStructs(int i, int j)
{
    std::cout << i << j << std::endl;
}

void MainWindow::addCurrentEvent()
{
    MeshField* currentMesh = meshFields.at(ui->meshComboBox->currentIndex());
    Event * currentEvent;

    try{
       currentEvent = getEvent(ui->eventComboBox->currentIndex());
    }
    catch(...) {
        warning("Invalid input arguments.");
        return;
    }

    currentMesh->addEvent(*currentEvent);
    ui->EventList->addItem(QString::fromStdString(currentEvent->getType()));

}

void MainWindow::addCurrentMesh()
{

    mat topology(2, ENS_DIM);

    try{
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < ENS_DIM; ++j) {
                topology(i, j) = boost::lexical_cast<double>(ui->tableWidget_mesh->item(j, i)->text().toStdString());
            }
        }
    }
    catch (std::exception & e) {
        warning(e.what() + (std::string)"  |  invalid topology numbers." );
        return;
    }

    std::string name = ui->lineEdit->text().toStdString();

    for (MeshField* mf : meshFields) {
        if (mf->description.compare(name) == 0){
            warning("A mesh with an equal name already exists.");
            return;
        }
    }

    MeshField* meshField = new MeshField(topology, *ensemble, name);
    MeshField* currentMesh = meshFields.at(ui->meshComboBox->currentIndex());

    try {
        addMesh(meshField, currentMesh);
    }
    catch (std::exception & e) {
        warning(e.what() + (std::string)"  |  invalid mesh topology.");
        return;
    }



}

void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
    int newIndex = ui->eventComboBox->currentIndex();

    std::vector<int> inputs = getInputs(newIndex);

    int n = inputs.size();

    if (n == 0) return;

    int inputIndex = inputs.at(row);

    QTableWidgetItem* newValue = ui->tableWidget->item(row, column);

    setStructValues(inputIndex, newValue->text());


}

void MainWindow::updateMeshComboBox()
{
    ui->meshComboBox->clear();

    for (MeshField* mf : meshFields) {
        ui->meshComboBox->addItem(mf->description.c_str());
    }
}

void MainWindow::fillMeshTopology(int i)
{
    const mat & topology = meshFields.at(i)->topology;

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < ENS_DIM; ++j) {
            ui->tableWidget_mesh->item(j, i)->setText(QString::fromStdString(boost::lexical_cast<std::string>(topology(i, j))));
        }
    }
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    int n = ui->meshComboBox->count();

    for (int i = 0; i < n; ++i) {
        if (arg1 == ui->meshComboBox->itemText(i)) {
            fillMeshTopology(i);
            return;
        }
    }

}

void MainWindow::on_pushButton_3_clicked()
{

    int tab = ui->tabWidget->currentIndex();

    switch (tab) {
    case TAB_EVENTS:
        addCurrentEvent();
        break;
    case TAB_MESHES:
        addCurrentMesh();
        break;
    default:
        warning("INVALID TAB");
        break;
    }


}

void MainWindow::updateMeshLists()
{

    int n = ui->meshComboBox->currentIndex();

    if (n == -1) return;

    ui->listWidget->clear();
    ui->EventList->clear();

    MeshField* currentMesh = meshFields.at(n);

    for(MeshField* mesh : currentMesh->getSubfields()){
        ui->listWidget->addItem(QString::fromStdString(mesh->description));
    }

    for (Event* event : currentMesh->getEvents()) {
        ui->EventList->addItem(QString::fromStdString(event->getType()));
    }
}

void MainWindow::on_meshComboBox_currentIndexChanged(int index)
{
    updateMeshLists();
    ui->lineEdit->setText(ui->meshComboBox->currentText());
}

void MainWindow::on_pushButton_clicked()
{

    int i, n;
    switch (activeList) {
    case TAB_EVENTS:

        i = ui->EventList->currentIndex().row();
        n = ui->meshComboBox->currentIndex();

        meshFields.at(n)->removeEvent(i);
        updateMeshLists();

        break;
    case TAB_MESHES:

        i = ui->listWidget->currentIndex().row();
        n = ui->meshComboBox->currentIndex();

        meshFields.at(n)->removeSubMesh(i);

        updateMeshLists();

        break;
    default:
        warning("invalid active list..");
        break;
    }
}

void MainWindow::on_EventList_itemClicked(QListWidgetItem *item)
{
    activeList = TAB_EVENTS;
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    activeList = TAB_MESHES;
}

void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    std::string name = ui->listWidget->item(index.row())->text().toStdString();

    for (int i = 0; i < ui->meshComboBox->count(); ++i) {
        if (name.compare(ui->meshComboBox->itemText(i).toStdString()) == 0) {
            ui->meshComboBox->setCurrentIndex(i);
            return;
        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    int T = 10000;
    try {
        mainMesh->eventLoop(T);
    }

    catch(std::exception & e){
        warning(e.what() + (std::string) "  |  event Loop failed.");
    }
}
