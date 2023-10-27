/**
  * @file   fugemain.h
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

#ifndef FUGEMAIN_H
#define FUGEMAIN_H

#include <QFile>
#include <iostream>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <numeric>

#include <QLabel>
#include <QTime>
#include <QTextStream>
#include <QStringList>
#include <QMessageBox>
#include <QSemaphore>
#include <QProcess>

#include "fuzzysystem.h"

#include "computethread.h"
#include "scriptmanager.h"
#include "systemparameters.h"
#include "fuzzyvariable.h"
#include "fuzzyset.h"
#include "fuzzyrulegenome.h"
#include "fuzzymembershipsgenome.h"
#include "fuzzyrule.h"
#include "fuzzysystem.h"

class ComputeThread;

extern QFile *fitLogFile;

class FugeMain : public QObject
{
    Q_OBJECT

public:
    FugeMain();
    ~FugeMain();

    void runFromCmdLine(QString dataSet, QString scriptFile, QString fuzzyFile,
                        bool eval, bool predict, bool verbose);
    static QList<QStringList>* listFile;
    static FuzzySystem* getNewFuzzySystem(QList<QStringList>* listFile);

private:
    void createActions();
    ComputeThread* computeThread;

    FuzzySystem* fSystemVars;
    FuzzySystem* fSystemRules;

    ScriptManager* sMan;
    QAction* actRun;
    QAction* actEvalFuzzy;
    QAction* actPredictFuzzy;

    QString currentOpennedSystem;
    void setDefaultSysParams();

    bool fuzzyLoaded;
    bool dataLoaded;
    bool scriptLoaded;
    bool paramsLoaded;
    bool isRunning;

public slots:
    void onComputeFinished();
    void onScriptFinished();

private slots:
    void onActRun();
    void onActEvalFuzzy(bool doValid = false, bool fromCmd = false);
    void onActPredictFuzzy(bool fromCmd = false);

signals:
    void clearStats();
    void closeStats();
    void saveFuzzySystem(QString fileName);
    void openFuzzySystem(QString fileName);
};

#endif // FUGEMAIN_H
