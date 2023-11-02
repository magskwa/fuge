# FUGE-LC

## Introduction
This application implements the FuzzyCoCo algorithm. It constructs systems that predict the outcome of a human decision-making 
process while providing an understandable explanation of a possible reasoning leading to it. The constructed fuzzy systems are 
composed of rules and linguistic variables. For more information about the method, please refer to Prof. Carlos Andrés Peña 
Reyes' thesis entitled "Coevolutionary Fuzzy Modeling".

Authors

- FuzzyCoCo algorithm based on *Fuzzy CoCo: a cooperative-coevolutionary approach to fuzzy modeling* from [Carlos Andrés Peña-Reyes](https://orcid.org/0000-0002-2113-6498)
- FUGE-LC by Jean-Philippe Meylan (2009-2010)
- Genetic algorithm update by Yvan Da Silva (2012)
- Code upgrade by [Rochus Keller](http://rochus-keller.ch/) <me@rochus.keller.ch> (2022)

### Notes
FUGE-LC is a software designed for modeling fuzzy systems using the cooperative coevolution approach.
In addition to model creation, the software supports the evaluation of fuzzy systems and making predictions. Originally, this project
was developed with a user interface based on the Qt framework.
Currently, the software is command-line based, so it can be integrated in a pipeline in the future.

### Last major updates

- Project restructuring
- QtScript module replaced by the Duktape JS engine
- Remove of the UI
- Update to Qt6.5


## Data, scripts and parameters
### Dataset
The database file must meet the following requirements:

- CSV file type, separated by semicolons
- Each row (samples) and each column (variables) must start with a label
- The output variables must be placed in the last columns


### Parameters
#### coevolution parameters

The coevolution algorithm can be configured via the following parameters:

- Number of generations to reach before stopping the evolution
- Maximum fitness threshold to reach before stopping the evolution
- Size of the populations (number of individuals)
- Size of the elite (best individuals kept unchanged between to generations)
- Crossover probability
- Probability that an individual is a target for a mutation
- Probability that a bit of an individual is mutated

#### Fitness evaluation parameters

These parameters determine how the overall fitness of a system will be evaluated. A value is computed for all the following measurements and a weight for each one can be selected. The fitness value is the sum of these measurements multiplied by their weight. The measurements available are:

- Sensitivity : TruePos / (TruePos + FalseNeg)
- Specificity : TrueNeg / (TrueNeg + FalsePos)
- Accuracy : (TruePos +TrueNeg) / (TruePos + TrueNeg + FalsePos + FalseNeg)
- PPV : TruePos / (TruePos + FalsePos)
- RMSE : Root Mean Square Error

It is also possible to choose the threshold applied to the output values. Generally, it is selected in the middle of the minimum and maximum value.


#### Fuzzy system parameters
These parameters determine the shape of the emerged fuzzy systems (number of rules, maximum number of variables per rule, number of input/output sets) and the number of bits used to code the different elements (variables, rules, membership functions) in the genome.

There is also parameter that indicates to the software how much output variables are present in the database. Without this indication, the application has no means to know the number output variables.

Be very careful when editing these parameters. Incoherent values can lead to erroneous results and even to the crash of the application.

### Script
The script files have the .fs extension and are coded in the Javascript language. They are composed of the following elements:
- The definition of all the parameters
- The definition of the doSetParams() function
- The definition of the doRun() function which controls the execution of multiple (or single) runs of coevolution

All these elements must be present and complete in the script in order to make it valid. A reference valid script file called ref.fs is present in the bin/script/ folder. Its syntax is quite simple and should be easily understood. Basically, after having defined all the parameters and implemented the doSetParams() function, the doRun() function defines how much runs of evolution will be performed and which parameters will be modified between the runs.



## INSTALLATION

### Exectuables
Executables are available in the [v1.1.3 Release](https://github.com/ThibaultSchowing/FUGE-LC/releases). Those releases are not uptodate with the current version of the source code.

### Requirements

- A C++98 compatible compiler
- Qt 6.5

### FUGE-LC compilation

This project can be compiled by QMake using FUGE-LC.pro file or by CMake using CMakeLists.txt file. 
Compiling with QMake requires to run the following commands:

    $ \<path to Qt bin\>/qmake FUGE-LC.pro; make
To compile the project with CMake, load the CMakeLists.txt file and build the project.


### Functionalities

The three main functionalities of FUGE-LC are the following:

- 1. Creation of a fuzzy system 
- 2. Evaluation of a fuzzy system
- 3. Prediction with a fuzzy system

These options can also be consulted by running the following command:

    $ FUGE-LC –help
The steps required to perform these three actions are described below.


### 1. FUZZY SYSTEM CREATION

In order to create a fuzzy system with FUGE-LC, the following steps must be performed:
- compile the application (with QMake or CMake)
- run the following command line :
 

    $ <path_to_FUGE-LC> -d <path_to_datasetFile> -s <path_to_scriptFile> -g no
- the result will be a .ffs file containing the best fuzzy system found at the path specified in the script

### 2. FUZZY SYSTEM EVALUATION
In order to evaluate a fuzzy system with FUGE-LC, the following steps must be performed:
- run the following command line :
 

    $ <path_to_FUGE-LC> --evaluate -f <path_to_fuzzySystemFile> -d <path_to_datasetFile> -s <path_to_scriptFile> -g no
- the result will be a file containing the values of the fitness measurements for the fuzzy system evaluated



### 3. FUZZY SYSTEM PREDICTION
In order to predict with a fuzzy system with FUGE-LC, the following steps must be performed:
- run the following command line :
 

    $ <path_to_FUGE-LC> --predict -f <path_to_fuzzySystemFile> -d <path_to_datasetFile> -s <path_to_scriptFile> -g no -p yes
- the result will be a file containing the values of the fitness measurements for the fuzzy system evaluated
This feature allows predicting the results for a database without outputs. The database the
user specifies must have no output rows as the program will propose to save the
prediction. This will result in adding a row for the prediction of each output variable.



### Log files

Each coevolution run creates a log file located in bin/logs/. These files are named in a unique way which contains the date, some evolution parameters and the fitness of the best system found. These files are never deleted by the application. They contain the following information:

- A summary of the best system found, including its description (rules & membership functions)
- The parameters of the coevolution
- The statistics (fitness) of each generation














