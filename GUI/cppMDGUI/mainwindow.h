#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../../src/Event/predefinedEvents/predefinedevents.h"
#include "../../src/MD/mdsolver.h"
#include "../../src/MD/forces/forces.h"

#include "viz/qtplatform.h"
#include "../../src/MeshField/MainMesh/mainmesh.h"

#include <QMainWindow>
#include <boost/lexical_cast.hpp>
#include <exception>
#include <QThread>

struct Params {
    double dt = 0.001;
    double T0 = 1;
    double delta = 0.5;
    double tau = 15;
    int xyz = 0;
    int freq = 1;
};

class QListWidgetItem;
class QModelIndex;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    MainMesh* getMainMesh() {
        return mainMesh;
    }

    Ensemble* getEnsemble() {
        return ensemble;
    }

    std::vector<MeshField*> getMeshFeilds(){
        return meshFields;
    }

    void setPlatform(QtPlatform * p) {
        viz = p;
        viz->setAdvanceTimerInterval(0.01);
    }

    QtPlatform * getViz() {
        return viz;
    }

private slots:

    void on_eventComboBox_currentIndexChanged(int index);

    void on_tableWidget_cellChanged(int row, int column);

    void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_3_clicked();

    void on_meshComboBox_currentIndexChanged(int index);

    void on_pushButton_clicked();

    void on_EventList_itemClicked(QListWidgetItem *item);

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_listWidget_doubleClicked(const QModelIndex &index);

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;

    MainMesh* mainMesh;
    Ensemble* ensemble;

    QtPlatform * viz;

    std::vector<MeshField*> meshFields;

    struct Params params;

    QThread mainThread;

    int activeList = -1;

    void addMesh(MeshField *child, MeshField* parent=NULL) {

        if (parent != NULL) {
            parent->addSubField(*child);
        }

        meshFields.push_back(child);
        updateMeshLists();
        updateMeshComboBox();
    }

    void warning(std::string text){
        std::cout << text << std::endl;
    }

    void updateMeshLists();

    void updateMeshComboBox();
    void fillMeshTopology(int i);

    void updateTable(std::vector<int> inputs);
    void updateStructs(int i, int j);

    void addCurrentEvent();
    void addCurrentMesh();

    enum TABS {
        TAB_EVENTS,
        TAB_MESHES
    };


    static const int nEvents = 13;

    static const int nInputs = 6;

    enum INPUTS {
        INPUT_DT,
        INPUT_T0,
        INPUT_delta,
        INPUT_tau,
        INPUT_xyz,
        INPUT_freq
    };

    enum EVENTS {
        PERIODIC,
        VELOCITYVERLET1,
        VELOCITYVERLET2,
        RANDOMSHUFFLE,
        BERENDSENTHERMOSTAT,
        COUNTPARTICLES,
        PROGRESSREPORT,
        COMPRESS,
        EXPAND,
        SAVETOFILE,
        LENNARDJONESFORCE,
        MDSOLVER,
        STALL
    };

    std::vector<int> getInputs(int EVENT){

        std::vector<int> inputs;

        switch (EVENT) {
        case PERIODIC:
            break;
        case VELOCITYVERLET1:
            inputs.push_back(INPUT_DT);
            break;
        case VELOCITYVERLET2:
            inputs.push_back(INPUT_DT);
            break;
        case BERENDSENTHERMOSTAT:
            inputs.push_back(INPUT_T0);
            inputs.push_back(INPUT_tau);
            inputs.push_back(INPUT_DT);
            break;
        case COMPRESS:
            inputs.push_back(INPUT_delta);
            inputs.push_back(INPUT_xyz);
            break;
        case EXPAND:
            inputs.push_back(INPUT_delta);
            inputs.push_back(INPUT_xyz);
            break;
        case SAVETOFILE:
            inputs.push_back(INPUT_freq);
            break;
        case MDSOLVER:
            inputs.push_back(INPUT_T0);
            inputs.push_back(INPUT_DT);
            break;
        case STALL:
            inputs.push_back(INPUT_DT);
            break;
        default:
            break;
        }

        return inputs;

    }

    QString getStructValFromIndex(int INDEX)
    {

        switch (INDEX) {
        case INPUT_DT:
            return QString::number(params.dt);
            break;
        case INPUT_T0:
            return QString::number(params.T0);
            break;
        case INPUT_delta:
            return QString::number(params.delta);
            break;
        case INPUT_tau:
            return QString::number(params.tau);
            break;
        case INPUT_xyz:
            return QString::number(params.xyz);
            break;
        case INPUT_freq:
            return QString::number(params.freq);
            break;
        default:
            return QString(((std::string)"fail at" + boost::lexical_cast<std::string>(INDEX)).c_str());
            break;
        }



    }

    QString getInputName(int INPUT){
        switch (INPUT) {
        case INPUT_DT:
            return "dt";
            break;
        case INPUT_T0:
            return "T0";
            break;
        case INPUT_delta:
            return "delta";
            break;
        case INPUT_tau:
            return "tau";
            break;
        case INPUT_xyz:
            return "xyz";
            break;
        case INPUT_freq:
            return "freq";
            break;
        default:
            return QString(((std::string)"fail at" + boost::lexical_cast<std::string>(INPUT)).c_str());
            break;
        }
    }


    void setStructValues(int INPUT, QString value){

        try{
            switch (INPUT) {
            case INPUT_DT:
                params.dt = boost::lexical_cast<double>(value.toStdString());
                break;
            case INPUT_T0:
                params.T0 = boost::lexical_cast<double>(value.toStdString());
                break;
            case INPUT_delta:
                params.delta = boost::lexical_cast<double>(value.toStdString());
                break;
            case INPUT_tau:
                params.tau = boost::lexical_cast<double>(value.toStdString());
                break;
            case INPUT_xyz:
                params.xyz = boost::lexical_cast<int>(value.toStdString());
                break;
            case INPUT_freq:
                params.freq = boost::lexical_cast<int>(value.toStdString());;
                break;
            default:
                break;
            }
        }
        catch (std::exception & e) {
            warning(e.what());
        }
    }


    QString getEventName(int EVENT){
        switch (EVENT) {
        case PERIODIC:
            return "periodicScaling";
            break;
        case VELOCITYVERLET1:
            return "VelocityVerletFirstHalf";
            break;
        case VELOCITYVERLET2:
            return "VelocityVerletSecondHalf";
            break;
        case RANDOMSHUFFLE:
            return "randomShuffle";
            break;
        case BERENDSENTHERMOSTAT:
            return "BerendsenThermostat";
            break;
        case COUNTPARTICLES:
            return "countAtoms";
            break;
        case PROGRESSREPORT:
            return "ReportProgress";
            break;
        case COMPRESS:
            return "CompressMesh";
            break;
        case EXPAND:
            return "ExpandMesh";
            break;
        case SAVETOFILE:
            return "SaveToFile";
            break;
        case LENNARDJONESFORCE:
            return "LennardJonesForce";
            break;
        case MDSOLVER:
            return "MD initializer";
            break;
        case STALL:
            return "Stall";
            break;
        default:
            return QString(((std::string)"fail at" + boost::lexical_cast<std::string>(EVENT)).c_str());
            break;
        }
    }


    mat sigmaTable;
    mat epsTable;

    Event* getEvent(int EVENT){
        switch (EVENT) {
        case PERIODIC:
            return new periodicScaling();
            break;
        case VELOCITYVERLET1:
            return new VelocityVerletFirstHalf(params.dt);
            break;
        case VELOCITYVERLET2:
            return new VelocityVerletSecondHalf(params.dt);
            break;
        case RANDOMSHUFFLE:
            return new randomShuffle();
            break;
        case BERENDSENTHERMOSTAT:
            return new BerendsenThermostat(params.T0, params.tau, params.dt);
            break;
        case COUNTPARTICLES:
            return new countAtoms();
            break;
        case PROGRESSREPORT:
            return new ReportProgress();
            break;
        case COMPRESS:
            return new ContractMesh(params.delta, params.xyz);
            break;
        case EXPAND:
            return new ExpandMesh(params.delta, params.xyz);
            break;
        case SAVETOFILE:
            return new SaveToFile(params.freq);
            break;
        case LENNARDJONESFORCE:
            return new LennardJonesForce(sigmaTable, epsTable);
            break;
        case MDSOLVER:
            return new mdSolver(params.T0, params.dt);
            break;
        case STALL:
            return new stall(params.dt);
            break;
        default:
            return NULL;
            break;
        }
    }


};


class WorkerThread : public QThread
{
    Q_OBJECT
public:
    explicit WorkerThread(MainWindow * mw = 0);

    void run() Q_DECL_OVERRIDE {
        mw->getMainMesh()->eventLoop(100);
        mw->getViz()->stopAdvanceTimer();
        emit finished();
    }
signals:
    void finished();
private:
    MainWindow* mw;
};

#endif // MAINWINDOW_H
