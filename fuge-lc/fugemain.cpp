/**
  * @file   fugemain.cpp
  * @author Jean-Philippe Meylan <jean-philippe.meylan_at_heig-vd.ch>
  * @author ReDS (Reconfigurable and embedded digital systems) <www.reds.ch>
  * @author HEIG-VD (Haute école d'ingénierie et de gestion) <www.heig-vd.ch>
  * @date   07.2009
  * @section LICENSE
  *
  * This application is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
  *
  * @class FugeMain
  *
  * @brief This class implements the FUGE-LC main window.
  */
#include "fugemain.h"
#include "QtGui/qaction.h"


QFile *fitLogFile;
QSemaphore scriptSema(0);
bool doRunFromCmd = false;
QList<QStringList>* FugeMain::listFile = 0;

FugeMain::FugeMain(QWidget *parent)
    : QMainWindow(parent), /*ui(new Ui::FugeMain),*/
      fSystemRules(0), fSystemVars(0)
{
    // Initialise the random generator
    QTime time;
    qsrand(QDateTime::currentDateTime().toTime_t());

    fuzzyLoaded = false;
    dataLoaded = false;
    scriptLoaded = false;
    paramsLoaded = false;
    isRunning = false;

    ComputeThread::bestFSystem = 0;
    fSystemRules = 0;
    fSystemVars = 0;
    listFile = new QList<QStringList>;

    CoevStats& fitStats = CoevStats::getInstance();
    sMan = new ScriptManager();
    // TODO: Rename the signal and slots to have the same name and get ride of those connect declarations.
    // Clicked should be linked to OnClicked etc. (See Qt documentation for more information).
    connect(sMan, SIGNAL(startRun()), this, SLOT(onActRun()));
    connect(sMan, SIGNAL(scriptFinished()), this, SLOT(onScriptFinished()));
    computeThread = new ComputeThread();
    connect(computeThread, SIGNAL(computeFinished()), this, SLOT(onComputeFinished()));
    this->createActions();
}

FugeMain::~FugeMain()
{
    delete listFile;
    sMan->deleteLater();
}

/**
  * Method called by main when the application is run in command line mode.
  * It is responsible to set the correct parameters and perform the task specified
  * on the command line.
  *
  * @param dataSet Dataset on which the evolution/evaluation/prediction is performed
  * @param fuzzyFile Name of the fuzzy system specified on the command line (if any)
  * @param eval Evaluation mode flag
  * @param predict Prediction mode flag
  * @param verbose Verbose mode flag
  */
void FugeMain::runFromCmdLine(QString dataSet, QString scriptFile, QString fuzzyFile,
                        bool eval, bool predict, bool verbose)
{
    doRunFromCmd = true;

    // First open the dataset
    QFile file(dataSet);    // create a qfile for dataset
    file.open(QIODevice::ReadOnly);
    QTextStream csvFile(&file);
    QString line;
    QStringList list;

    // Read the csv file and store info in a double dimension list.
    while (!csvFile.atEnd()) {
        line = csvFile.readLine();
        list = line.split(';');
        listFile->append(list);     // listFile = double dimension list
    }
    dataLoaded = true;

    // Set the dataset name in the parameters
    SystemParameters& sysParams = SystemParameters::getInstance();
    sysParams.setVerbose(verbose);

    // Check whether we make an evolution or a validation/prediction
    if (eval || predict) {
        fSystemVars = new FuzzySystem();
        fSystemRules = new FuzzySystem();

        ComputeThread::bestFSystem = fSystemVars;

        connect(this, SIGNAL(openFuzzySystem(QString)), fSystemVars, SLOT(loadFromFile(QString)));
        connect(this, SIGNAL(openFuzzySystem(QString)), fSystemRules, SLOT(loadFromFile(QString)));
        currentOpennedSystem = fuzzyFile;

        emit openFuzzySystem(fuzzyFile);

        //this->actSaveFuzzy->setEnabled(true);
        //this->actCloseFuzzy->setEnabled(true);
        //this->actEvalFuzzy->setEnabled(true);
        //this->actValidFuzzy->setEnabled(true);
        //this->actPredictFuzzy->setEnabled(true);
        //this->actEditFuzzy->setEnabled(true);
        sysParams.setDatasetName(dataSet);

        if (eval) {
            this->onActEvalFuzzy(true, true);
            std::cout << "[Fitness] : " << ComputeThread::bestFSystem->getFitness() << std::endl;
            std::cout << "[Sensitivity] : " << ComputeThread::bestFSystem->getSensitivity() << std::endl;
            std::cout << "[Specificity] : " << ComputeThread::bestFSystem->getSpecificity() << std::endl;
            std::cout << "[Accuracy] : " << ComputeThread::bestFSystem->getAccuracy() << std::endl;
            std::cout << "[PPV] : " << ComputeThread::bestFSystem->getPpv() << std::endl;
            std::cout << "[RMSE] : " << ComputeThread::bestFSystem->getRmse() << std::endl;
            std::cout << "[RRSE] : " << ComputeThread::bestFSystem->getRrse() << std::endl;
            std::cout << "[RAE] : " << ComputeThread::bestFSystem->getRae() << std::endl;
            std::cout << "[MSE] : " << ComputeThread::bestFSystem->getMse() << std::endl;
            std::cout << "[ADM] : " << ComputeThread::bestFSystem->getDistanceThreshold() << std::endl;
            std::cout << "[MDM] : " << ComputeThread::bestFSystem->getDistanceMinThreshold() << std::endl;
            std::cout << "[SIZE] : " << ComputeThread::bestFSystem->getDontCare() << std::endl;
            std::cout << "[OverLearn] : " << ComputeThread::bestFSystem->getOverLearn() << std::endl;
        }
        else if (predict) {
            this->onActPredictFuzzy(true);
        }
        this->close();
    }
    else {
        // Then open the script
        scriptLoaded = true;
        sMan->setScriptFileName(scriptFile);
        sMan->readScript();
        sMan->start();
        //this->actSaveFuzzy->setEnabled(true);
    }
}

/**
  * Create the actions available in the menus.
  */
void FugeMain::createActions()
{
    // TODO: It's difficult to update and to maintain.
    // All default values can be set on the form files as the default text.
    actRun = new QAction(tr("&Run..."), this);
    actStop = new QAction(tr("&Stop"), this);
    //actOpenData = new QAction(tr("&Open dataset..."), this);
    //actCloseData = new QAction(tr("&Close dataset"), this);
    //actNewFuzzy = new QAction(tr("&New fuzzy system"), this);
    actOpenFuzzy = new QAction(tr("&Open fuzzy system..."), this);
    //actCloseFuzzy = new QAction(tr("&Close fuzzy system"), this);
    //actSaveFuzzy = new QAction(tr("&Save fuzzy system..."), this);
    //actEditFuzzy = new QAction(tr("&Edit fuzzy system..."), this);
    //actValidFuzzy = new QAction(tr("&Validate fuzzy system..."), this);
    actEvalFuzzy = new QAction(tr("Ev&aluate fuzzy system..."), this);
    actPredictFuzzy = new QAction(tr("&Predict fuzzy system..."), this);
    //actEditParams = new QAction(tr("&Edit parameters..."), this);
    //actOpenScript= new QAction(tr("&Open script..."), this);
    //actCloseScript= new QAction(tr("&Close script"), this);
    //actRunScript= new QAction(tr("&Run script"), this);
    actQuit = new QAction(tr("&Quit"), this);
    //actAbout = new QAction(tr("&About..."), this);
    actRun->setEnabled(false);
    actStop->setEnabled(false);
    //actRunScript->setEnabled(false);
    //actSaveFuzzy->setEnabled(false);
    //actCloseFuzzy->setEnabled(false);
    //actEditFuzzy->setEnabled(false);
    //actCloseData->setEnabled(false);
    //actCloseScript->setEnabled(false);
    //actValidFuzzy->setEnabled(false);
    actEvalFuzzy->setEnabled(false);
    actPredictFuzzy->setEnabled(false);

    // TODO: triggered should be auto-linked to slots onXXXTriggered.
    // It's possible to do it by renaming slots (See Qt Documentation)
    connect(actRun, SIGNAL(triggered()), this, SLOT(onActRun()));
    connect(actStop, SIGNAL(triggered()), computeThread, SLOT(onStopEvo()));
    //connect(actOpenData, SIGNAL(triggered()), this, SLOT(onActOpenData()));
    //connect(actCloseData, SIGNAL(triggered()), this, SLOT(onActCloseData()));
    //connect(actNewFuzzy, SIGNAL(triggered()), this, SLOT(onActNewFuzzy()));
    connect(actOpenFuzzy, SIGNAL(triggered()), this, SLOT(onActOpenFuzzy()));
    //connect(actCloseFuzzy, SIGNAL(triggered()), this, SLOT(onActCloseFuzzy()));
    //connect(actSaveFuzzy, SIGNAL(triggered()), this, SLOT(onActSaveFuzzy()));
    connect(actEvalFuzzy, SIGNAL(triggered()), this, SLOT(onActEvalFuzzy()));
    connect(actPredictFuzzy, SIGNAL(triggered()), this, SLOT(onActPredictFuzzy()));
    //connect(actEditParams, SIGNAL(triggered()), this, SLOT(onActEditParams()));
    //connect(actOpenScript, SIGNAL(triggered()), this, SLOT(onActOpenScript()));
    //connect(actCloseScript, SIGNAL(triggered()), this, SLOT(onActCloseScript()));
    //connect(actRunScript, SIGNAL(triggered()), this, SLOT(onActRunScript()));
    connect(actQuit, SIGNAL(triggered()), this, SLOT(onActQuit()));
    //connect(actAbout, SIGNAL(triggered()), this, SLOT(onActAbout()));
}

/**
  * Set the default parameters
  */
void FugeMain::setDefaultSysParams()
{
    // TODO: build a conf file were params are saved and restored.
    // Fuzzy paramteters
    SystemParameters& sysParams = SystemParameters::getInstance();
    sysParams.setFixedVars(true);
    sysParams.setNbRules(5);
    sysParams.setNbVarPerRule(4);
    sysParams.setNbOutVars(3);
    sysParams.setNbInSets(2);
    sysParams.setNbOutSets(2);
    sysParams.setInVarsCodeSize(1);
    sysParams.setOutVarsCodeSize(2);
    sysParams.setInSetsCodeSize(2);
    sysParams.setOutSetsCodeSize(1);
    sysParams.setInSetsPosCodeSize(4);
    sysParams.setOutSetPosCodeSize(1);
    sysParams.setSensiW(1.0);
    sysParams.setSpeciW(0.8);
    sysParams.setAccuracyW(0.0);
    sysParams.setPpvW(0.0);
    sysParams.setRmseW(0.0);
    sysParams.setRrseW(0.0);
    sysParams.setRaeW(0.0);
    sysParams.setMseW(0.0);
    sysParams.setDistanceThresholdW(0.0);
    sysParams.setDistanceMinThresholdW(0.0);
    sysParams.setDontCareW(0.0);
    sysParams.setOverLearnW(0.0);
    // Coevolution parameters
    sysParams.setMaxGenPop1(10);
    sysParams.setEliteSizePop1(5);
    sysParams.setPopSizePop1(10);
    sysParams.setCxProbPop1(0.5);
    sysParams.setMutFlipIndPop1(0.5);
    sysParams.setMutFlipBitPop1(0.025);
    sysParams.setMaxGenPop2(10);
    sysParams.setEliteSizePop2(5);
    sysParams.setPopSizePop2(10);
    sysParams.setCxProbPop2(0.5);
    sysParams.setMutFlipIndPop2(0.5);
    sysParams.setMutFlipBitPop2(0.025);
}

/**
 * @brief FugeMain::getNewFuzzySystem Returns a new fuzzy system fully loaded.
 * @param listFile
 * @return a new loaded SystemFuzzy
 */
FuzzySystem* FugeMain::getNewFuzzySystem(QList<QStringList>* listFile){
    FuzzySystem *fSystem = new FuzzySystem();
    ComputeThread::sysParams = &SystemParameters::getInstance();
    fSystem->setParameters(ComputeThread::sysParams->getNbRules(), ComputeThread::sysParams->getNbVarPerRule(), ComputeThread::sysParams->getNbOutVars(),
                      ComputeThread::sysParams->getNbInSets(), ComputeThread::sysParams->getNbOutSets(), ComputeThread::sysParams->getInVarsCodeSize(),
                      ComputeThread::sysParams->getOutVarsCodeSize(), ComputeThread::sysParams->getInSetsCodeSize(), ComputeThread::sysParams->getOutSetsCodeSize(),
                      ComputeThread::sysParams->getInSetsPosCodeSize(), ComputeThread::sysParams->getOutSetPosCodeSize());
    fSystem->loadData(listFile);
    return fSystem;
}

/**
  * Method called when a run needs to be performed. It sets the interface to the required
  * sate and runs the computation thread.
 */
void FugeMain::onActRun()
{
    try
    {
    if(fSystemRules != 0)
        delete fSystemRules;
    if(fSystemVars != 0)
        delete fSystemVars;

    if ((dataLoaded && scriptLoaded) || (dataLoaded && paramsLoaded)) {

        fSystemVars = getNewFuzzySystem(listFile);
        fSystemRules = getNewFuzzySystem(listFile);

        // At least attribute it a pointer.
        ComputeThread::bestFSystem = fSystemVars;
        ComputeThread::bestFitness = 0;
        ComputeThread::bestFuzzySystemDescription.clear();

        if (computeThread == NULL) {
            computeThread = new ComputeThread();
            connect (computeThread, SIGNAL(computeFinished()), this, SLOT(onComputeFinished()));
        }

        connect(this, SIGNAL(saveFuzzySystem(QString)), computeThread, SLOT(onSaveSystem(QString)));
        computeThread->setFuzzySystem(fSystemVars, fSystemRules);
        emit clearStats();
        computeThread->start();
        isRunning = true;

        // TODO: this logic should be implemented in a method.
        actStop->setEnabled(true);
        //actCloseData->setEnabled(false);
        //actOpenData->setEnabled(false);
        actOpenFuzzy->setEnabled(false);
        //actNewFuzzy->setEnabled(false);
    }

    //this->actSaveFuzzy->setEnabled(true);
    this->actPredictFuzzy->setEnabled(false);
    //this->actValidFuzzy->setEnabled(false);
    this->actEvalFuzzy->setEnabled(false);
    //this->actEditFuzzy->setEnabled(false);
    }catch(...)
    {
        qCritical() << "Exception in FugeMain::onActRun";
        scriptSema.release();
    }
}

/**
  * Slot called when the user quits the application.
  */
void FugeMain::onActQuit()
{
    this->close();
}

/**
  * Slot called when the user opens a dataset.
  */
/*
void FugeMain::onActOpenData()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open dataset"), "../../../../datasets", "*.csv");
    if (fileName != NULL) {
        // Clear previous loaded data
        if (dataLoaded)
            listFile->clear();
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        QTextStream csvFile(&file);
        QString line;
        QStringList list;

        // Save the name of the dataset
        SystemParameters& sysParams = SystemParameters::getInstance();
        sysParams.setDatasetName(fileName);

        // Read the csv file and store info in a double dimension list.
        while (!csvFile.atEnd()) {
            line = csvFile.readLine();
            list = line.split(';');
            listFile->append(list);
         }
        dataLoaded = true;
        file.close();
        if (paramsLoaded) {
            actRun->setEnabled(true);
        }
        if (scriptLoaded) {
            actRunScript->setEnabled(true);
        }
        actCloseData->setEnabled(true);
    }

}
*/
/**
  * Slot called when the user closes a dataset.
  */
/*
void FugeMain::onActCloseData()
{
    listFile->clear();
    dataLoaded = false;
    actRun->setEnabled(false);
}


/**
  * Slot called when the user opens a fuzzy system file.
  */
void FugeMain::onActOpenFuzzy()
{
    SystemParameters& sysParams = SystemParameters::getInstance();


    QFile tempFile(sysParams.getSavePath() + "temp.ffs");
    if (tempFile.exists())
         tempFile.remove();

    //connect(this, SIGNAL(evalFuzzySystem()), ComputeThread::bestFSystem, SLOT(doEvaluateFitness()));
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open fuzzy system"), sysParams.getSavePath()+"fuzzySystems", "*.ffs");
    if (fileName != NULL) {
        if (ComputeThread::bestFSystem != 0) {
            delete ComputeThread::bestFSystem;
            ComputeThread::bestFSystem = NULL;
        }
        ComputeThread::bestFSystem = new FuzzySystem();
        connect(this, SIGNAL(openFuzzySystem(QString)), ComputeThread::bestFSystem, SLOT(loadFromFile(QString)));
        currentOpennedSystem = fileName;
        emit openFuzzySystem(fileName);

        // TODO: this logic should be in a function.
        //this->actSaveFuzzy->setEnabled(true);
        //this->actCloseFuzzy->setEnabled(true);
        this->actEvalFuzzy->setEnabled(true);
        //this->actValidFuzzy->setEnabled(true);
        this->actPredictFuzzy->setEnabled(true);
        //this->actEditFuzzy->setEnabled(true);
    }
}

/**
  * Slot called when the user closes a fuzzy system file.
  */
/*
void FugeMain::onActCloseFuzzy()
{
    SystemParameters& sysParams = SystemParameters::getInstance();

    currentOpennedSystem = "";
    if (ComputeThread::bestFSystem != 0) {
        delete ComputeThread::bestFSystem;
        ComputeThread::bestFSystem = 0;
    }
    QFile tempFile(sysParams.getSavePath()+"temp.ffs");
    if (tempFile.exists())
         tempFile.remove();
    // Clear previous loaded data
    //if (dataLoaded)
        //this->onActCloseData();

    // TODO: This logic should be in a function.
    this->actSaveFuzzy->setEnabled(false);
    this->actCloseFuzzy->setEnabled(false);
    //this->actEditFuzzy->setEnabled(false);
    this->actEvalFuzzy->setEnabled(false);
    //this->actValidFuzzy->setEnabled(false);
    this->actPredictFuzzy->setEnabled(false);
}
*/

/**
  * Slot called when the user saves a fuzzy system file.
  */
/*
void FugeMain::onActSaveFuzzy()
{
    SystemParameters& sysParams = SystemParameters::getInstance();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save fuzzy system"), sysParams.getSavePath()+"fuzzySystems" , "*.ffs");
    if (fileName != NULL) {
        // If the evolution is running, we ask the evaluation operator to save the best system
        if (isRunning) {
            emit saveFuzzySystem(fileName);
        }
        // If the evolution is finished, fSystem is the best system
        else {
            CoevStats& stats = CoevStats::getInstance();
            ComputeThread::bestFSystem->saveToFile(fileName, stats.getFitMaxPop1());
        }
    }
}
*/

// TODOM : method could be erase
/**
  * Slot called when the user edits a fuzzy system file.
  */
/*
void FugeMain::onActEditFuzzy()
{
    SystemParameters& sysParams = SystemParameters::getInstance();

    QString name = ComputeThread::bestFSystem->getInVar(0)->getName();

    // Ensure that a temp directory exists. If not we create one.
    QDir tempDir;
    if (!tempDir.exists(sysParams.getSavePath()+"temp")) {
        tempDir.mkdir(sysParams.getSavePath()+"temp");
    }

    if (currentOpennedSystem == "")
        currentOpennedSystem = QString(sysParams.getSavePath()+"temp/currentBest_") +
                QString::number(QCoreApplication::applicationPid()) + QString(".ffs");
}
*/

/**
  * Slot called when the user asks for a prediction.
  */
void FugeMain::onActPredictFuzzy(bool fromCmd)
{
    SystemParameters& sysParams = SystemParameters::getInstance();
    QString fileName;

    if (!fromCmd)
        fileName = QFileDialog::getOpenFileName(this, tr("Open a test dataset (WITHOUT OUPTUT VALUES)"), "../../../../datasets", "*.csv");
    else {
        fileName = sysParams.getDatasetName();
    }

    if (fileName != NULL) {
        // Clear previous loaded data
        if (dataLoaded)
            listFile->clear();
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        QTextStream csvFile(&file);
        QString line;
        QStringList list;

        // Read the csv file and store info in a double dimension list.
        while (!csvFile.atEnd()) {
            line = csvFile.readLine();
            list = line.split(';');
            listFile->append(list);
        }
        ComputeThread::bestFSystem->loadData(listFile);
        dataLoaded = true;

        file.close();

        int nbOutVars = sysParams.getNbOutVars();

        QVector<float> computedResults;
        QVector<float> reverseComputedResults;
        QVector<float> predictedResults;

        computedResults = ComputeThread::bestFSystem->doEvaluateFitness();

        // Reorder the results in order to simplify their display by EvalPlot
        if(nbOutVars > 1) {
            reverseComputedResults.resize(computedResults.size());
            for (int i = 0; i <  nbOutVars; i++) {
                for (int k = 0; k < listFile->size()-1; k++) {
                    reverseComputedResults.replace(i*(listFile->size()-1) + k, computedResults.at(k*nbOutVars+i));
                }
            }
        }
        else {
            reverseComputedResults.resize(computedResults.size());
            for (int k = 0; k < listFile->size()-1; k++) {
                reverseComputedResults.replace(k, computedResults.at(k));
            }
        }


        predictedResults.resize(reverseComputedResults.size());
        // Compute the predicted results by applying the threshold
        for (int i = 0; i < reverseComputedResults.size(); i++) {
            predictedResults.replace(i, ComputeThread::bestFSystem->threshold(i / (reverseComputedResults.size()/nbOutVars), reverseComputedResults.at(i)));
        }
    }
}

/**
  * Slot called when the user asks for a validation.
  */
/*
void FugeMain::onActValidFuzzy()
{
    this->onActEvalFuzzy(true);
}
*/

/**
  * Slot called when the user asks for an evalutation.
  */
void FugeMain::onActEvalFuzzy(bool doValid, bool fromCmd)
{
    QVector<float> computedResults;
    QVector<float> reverseComputedResults;
    QVector<float> expectedResults;
    QVector<float> predictedResults;
    SystemParameters& sysParams = SystemParameters::getInstance();

    QString fileName;

    if (doValid) {
        // Retrieve the dataset corresponding to the system according to the name in the XML
        fileName = sysParams.getDatasetName();
        QFile file(fileName);
    }
    else {
        fileName = QFileDialog::getOpenFileName(this, tr("Open a test dataset"), "../../../../datasets", "*.csv");
    }

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)) {
        // Clear previous loaded data
        if (dataLoaded)
            listFile->clear();
        QTextStream csvFile(&file);
        QString line;
        QStringList list;

        // Read the csv file and store info in a double dimension list.
        while (!csvFile.atEnd()) {
            line = csvFile.readLine();
            list = line.split(';');
            listFile->append(list);
         }       
    }
    ComputeThread::bestFSystem->loadData(listFile);
    dataLoaded = true;
    file.close();

    int nbOutVars = sysParams.getNbOutVars();
    int nbInVars = listFile->at(0).size() - 1 - nbOutVars;

    expectedResults.resize((listFile->size()-1)*nbOutVars);

    if (/*dataLoaded*/1) {
        for (int k = 0; k < nbOutVars; k++) {
            for (int i = 1, j = 0; i < listFile->size(); i++, j++) {
                expectedResults.replace((listFile->size()-1)*k + j, (listFile->at(i).at(nbInVars+k+1)).toFloat());
            }
        }

        computedResults = ComputeThread::bestFSystem->doEvaluateFitness();

        predictedResults.resize(computedResults.size());

        // Reorder the results in order to simplify their display by EvalPlot
        if(nbOutVars > 1) {
            reverseComputedResults.resize(computedResults.size());
            for (int i = 0; i <  nbOutVars; i++) {
                for (int k = 0; k < listFile->size()-1; k++) {
                    reverseComputedResults.replace(i*(listFile->size()-1) + k, computedResults.at(k*nbOutVars+i));
                }
            }
        }
        else {
            reverseComputedResults.resize(computedResults.size());
            for (int k = 0; k < listFile->size()-1; k++) {
                reverseComputedResults.replace(k, computedResults.at(k));
            }
        }

        // Compute the predicted results by applying the threshold
        predictedResults.resize(reverseComputedResults.size());
        for (int i = 0; i < reverseComputedResults.size(); i++) {
            predictedResults.replace(i, ComputeThread::bestFSystem->threshold(i / (reverseComputedResults.size()/nbOutVars), reverseComputedResults.at(i)));
        }
    }
}

/**
  * Slot called when the user edits the parameters.
  */
/*
void FugeMain::onActEditParams()
{
    scriptLoaded = false;

    if (paramsLoaded) {
        if (dataLoaded) {
            actRun->setEnabled(true);
            onActCloseScript();
        }
    }
}
*/

/**
  * Slot called when the user opens a script file.
  */
/*
void FugeMain::onActOpenScript()
{
    QString fileName = QFileDialog::getOpenFileName(NULL, tr("Open script File"), "scripts", "*.fs");
    if (fileName != NULL) {
        scriptLoaded = true;
        paramsLoaded = true;
        actCloseScript->setEnabled(true);
        this->actCloseScript->setEnabled(true);

        sMan->setScriptFileName(fileName);
        sMan->readScript();
        if (dataLoaded && sMan->isScriptReady()) {
            actRunScript->setEnabled(true);
            actRun->setEnabled(false);
        }
    }
}
*/

/**
  * Slot called when the user closes a script file.
  */
/*
void FugeMain::onActCloseScript()
{
    sMan->setScriptFileName(NULL);
    actRunScript->setEnabled(false);
    scriptLoaded = false;
}
*/

/**
  * Slot called when the user performs a script run.
  */
/*
void FugeMain::onActRunScript()
{
    if (dataLoaded) {
        sMan->start();
        actStop->setEnabled(true);
        actSaveFuzzy->setEnabled(true);
    }
}
*/

/**
  * Slot called by the computational thread when an evolution run is finished.
  */
void FugeMain::onComputeFinished()
{
    SystemParameters& sysParams = SystemParameters::getInstance();

    computeThread->wait();
    computeThread->deleteLater();
    computeThread = 0;

    isRunning = false;
    //actCloseData->setEnabled(true);
    actOpenFuzzy->setEnabled(true);
    //actNewFuzzy->setEnabled(true);
    //actOpenData->setEnabled(true);
    actStop->setEnabled(false);

    emit closeStats();

    // Script execution
    QString newName;
    QTextStream newNameStream(&newName);
    QTime time;
    QString fileN;
    // Ensure that a temp directory exists. If not we create one.
    QDir tempDir;
    if (!tempDir.exists(sysParams.getSavePath()+"temp")) {
        tempDir.mkdir(sysParams.getSavePath()+"temp");
    }
    fileN = QString(sysParams.getSavePath()+"temp/currentBest_") +
            QString::number(QCoreApplication::applicationPid()) + QString(".ffs");
    QFile file(fileN);
    CoevStats& stats = CoevStats::getInstance();
    ComputeThread::bestFSystem->saveToFile(fileN, stats.getFitMaxPop1());

    // Ensure that a fuzzy systems directory exists. If not we create one.
    QDir logsDir;
    if (!logsDir.exists(sysParams.getSavePath()+"fuzzySystems")) {
        logsDir.mkdir(sysParams.getSavePath()+"fuzzySystems");
    }

    int randomNumber = qrand();
    newNameStream << sysParams.getSavePath() +"fuzzySystems/" << sysParams.getExperimentName() << "_" << time.currentTime().toString() << "." << randomNumber << "Gen" << sysParams.getMaxGenPop1()
    << "_" << "Pop" << stats.getSizePop1() << "_" << "Rules" << QString::number(sysParams.getNbRules()) << "_" << "Elt" << QString::number(sysParams.getEliteSizePop1())
    << "_" << "CX" << QString::number(sysParams.getCxProbPop1()) << "_" << "MutI" << sysParams.getMutFlipIndPop1() << "_" << "MutB" << sysParams.getMutFlipBitPop1()
    << "_" << "FixedV" << sysParams.getFixedVars() << "_" << "Fit" << stats.getFitMaxPop1() << ".ffs";
    newNameStream.flush();
    file.copy(newName);


    //this->actSaveFuzzy->setEnabled(true);
    //this->actCloseFuzzy->setEnabled(true);
    this->actEvalFuzzy->setEnabled(true);
    //this->actValidFuzzy->setEnabled(true);
    this->actPredictFuzzy->setEnabled(true);
    this->actStop->setEnabled(false);
    //this->actEditFuzzy->setEnabled(true);

    // Allow the next script execution
    scriptSema.release();
}

/**
  * Slot called by the script manager thread when an evolution run is finished.
  */
void FugeMain::onScriptFinished()
{
    qDebug() << "on script finished ->closed";
    QCoreApplication::quit();
}


void FugeMain::changeEvent(QEvent *e)
{

}

/**
  * Slot called when the user quits the application.
  */
/*
void FugeMain::closeEvent(QCloseEvent*)
{
    SystemParameters& sysParams = SystemParameters::getInstance();

    // Delete the temporary fuzzy system file
    QString fileN = QString(sysParams.getSavePath()+"temp/currentBest_") +
            QString::number(QCoreApplication::applicationPid()) + QString(".ffs");
    QString logFile = QString(sysParams.getSavePath()+"temp/running_") +
            QString::number(QCoreApplication::applicationPid()) + QString(".csv");
    QFile file(fileN);
    QFile lFile(logFile);
    file.remove();
    lFile.remove();

    this->close();
}
*/
