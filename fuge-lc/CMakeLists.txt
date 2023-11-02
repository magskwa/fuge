cmake_minimum_required(VERSION 3.16)
project(FUGE-LC VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "/Users/mer/Qt/6.5.3/macos/lib/cmake/")

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Concurrent Gui Widgets Xml)

qt_standard_project_setup()

qt_add_executable(FUGE-LC WIN32 MACOSX_BUNDLE
    Dependencies/duktape/duk_config.h
    Dependencies/duktape/duktape.c Dependencies/duktape/duktape.h
    coev/coevcooperator.cpp coev/coevcooperator.h
    coev/coevolution.cpp coev/coevolution.h
    coev/coevstats.cpp coev/coevstats.h
    computethread.cpp computethread.h
    fugemain.cpp fugemain.h
    fuzzy/defuzzmethod.cpp fuzzy/defuzzmethod.h
    fuzzy/defuzzmethodcoa.cpp fuzzy/defuzzmethodcoa.h
    fuzzy/defuzzmethodsingleton.cpp fuzzy/defuzzmethodsingleton.h
    fuzzy/fuzzymemberships.cpp fuzzy/fuzzymemberships.h
    fuzzy/fuzzymembershipscoco.cpp fuzzy/fuzzymembershipscoco.h
    fuzzy/fuzzymembershipsgenome.cpp fuzzy/fuzzymembershipsgenome.h
    fuzzy/fuzzyoperator.cpp fuzzy/fuzzyoperator.h
    fuzzy/fuzzyoperatorand.cpp fuzzy/fuzzyoperatorand.h
    fuzzy/fuzzyrule.cpp fuzzy/fuzzyrule.h
    fuzzy/fuzzyrulegenome.cpp fuzzy/fuzzyrulegenome.h
    fuzzy/fuzzyset.cpp fuzzy/fuzzyset.h
    fuzzy/fuzzysystem.cpp fuzzy/fuzzysystem.h
    fuzzy/fuzzyvariable.cpp fuzzy/fuzzyvariable.h
    fuzzymembershipssingle.cpp fuzzymembershipssingle.h
    libGGA/EvolutionEngine/ReproductionMethod/Crossover/arithmetic.cpp libGGA/EvolutionEngine/ReproductionMethod/Crossover/arithmetic.h
    libGGA/EvolutionEngine/ReproductionMethod/Crossover/crossover.cpp libGGA/EvolutionEngine/ReproductionMethod/Crossover/crossover.h
    libGGA/EvolutionEngine/ReproductionMethod/Crossover/onepoint.cpp libGGA/EvolutionEngine/ReproductionMethod/Crossover/onepoint.h
    libGGA/EvolutionEngine/ReproductionMethod/Crossover/uniform.cpp libGGA/EvolutionEngine/ReproductionMethod/Crossover/uniform.h
    libGGA/EvolutionEngine/ReproductionMethod/Mutation/mutate.cpp libGGA/EvolutionEngine/ReproductionMethod/Mutation/mutate.h
    libGGA/EvolutionEngine/ReproductionMethod/Mutation/toggling.cpp libGGA/EvolutionEngine/ReproductionMethod/Mutation/toggling.h
    libGGA/EvolutionEngine/SelectionMethod/elitism.cpp libGGA/EvolutionEngine/SelectionMethod/elitism.h
    libGGA/EvolutionEngine/SelectionMethod/elitismwithrandom.cpp libGGA/EvolutionEngine/SelectionMethod/elitismwithrandom.h
    libGGA/EvolutionEngine/SelectionMethod/entityselection.cpp libGGA/EvolutionEngine/SelectionMethod/entityselection.h
    libGGA/EvolutionEngine/SelectionMethod/generationalreplacement.cpp libGGA/EvolutionEngine/SelectionMethod/generationalreplacement.h
    libGGA/EvolutionEngine/SelectionMethod/proportionalselection.cpp libGGA/EvolutionEngine/SelectionMethod/proportionalselection.h
    libGGA/EvolutionEngine/SelectionMethod/rankbasedselection.cpp libGGA/EvolutionEngine/SelectionMethod/rankbasedselection.h
    libGGA/EvolutionEngine/SelectionMethod/roulettewheel.cpp libGGA/EvolutionEngine/SelectionMethod/roulettewheel.h
    libGGA/EvolutionEngine/SelectionMethod/selectionpressure.cpp libGGA/EvolutionEngine/SelectionMethod/selectionpressure.h
    libGGA/EvolutionEngine/SelectionMethod/tournamentselection.cpp libGGA/EvolutionEngine/SelectionMethod/tournamentselection.h
    libGGA/EvolutionEngine/SelectionMethod/truncatedrankbasedselection.cpp libGGA/EvolutionEngine/SelectionMethod/truncatedrankbasedselection.h
    libGGA/EvolutionEngine/evolutionengine.cpp libGGA/EvolutionEngine/evolutionengine.h
    libGGA/Population/Individual/Genotype/genotype.cpp libGGA/Population/Individual/Genotype/genotype.h
    libGGA/Population/Individual/individual.cpp libGGA/Population/Individual/individual.h
    libGGA/Population/Individual/popentity.cpp libGGA/Population/Individual/popentity.h
    libGGA/Population/Individual/representative.cpp libGGA/Population/Individual/representative.h
    libGGA/Population/population.cpp libGGA/Population/population.h
    libGGA/StatisticEngine/EvolutionaryMesure/PopulationDiversity/allpairpossibility.cpp libGGA/StatisticEngine/EvolutionaryMesure/PopulationDiversity/allpairpossibility.h
    libGGA/StatisticEngine/EvolutionaryMesure/PopulationDiversity/entropic.cpp libGGA/StatisticEngine/EvolutionaryMesure/PopulationDiversity/entropic.h
    libGGA/StatisticEngine/EvolutionaryMesure/PopulationDiversity/populationdiversity.cpp libGGA/StatisticEngine/EvolutionaryMesure/PopulationDiversity/populationdiversity.h
    libGGA/StatisticEngine/EvolutionaryMesure/evolutionarymesure.cpp libGGA/StatisticEngine/EvolutionaryMesure/evolutionarymesure.h
    libGGA/StatisticEngine/statisticengine.cpp libGGA/StatisticEngine/statisticengine.h
    libGGA/Utility/qbitarrayutility.cpp libGGA/Utility/qbitarrayutility.h
    libGGA/Utility/randomgenerator.cpp libGGA/Utility/randomgenerator.h
    main.cpp
    scriptmanager.cpp scriptmanager.h
    systemparameters.cpp systemparameters.h
)
target_include_directories(FUGE-LC PRIVATE
    Dependencies/duktape
    coev
    fuzzy
    libGGA
    libGGA/EvolutionEngine
    libGGA/EvolutionEngine/EvolutionEngine
    libGGA/EvolutionEngine/ReproductionMethod
    libGGA/EvolutionEngine/ReproductionMethod/Crossover
    libGGA/EvolutionEngine/ReproductionMethod/Mutation
    libGGA/EvolutionEngine/SelectionMethod
    libGGA/Population
    libGGA/Population/Individual
    libGGA/Population/Individual/Genotype
    libGGA/StatisticEngine
    libGGA/StatisticEngine/EvolutionaryMesure
    libGGA/StatisticEngine/EvolutionaryMesure/EvolutionaryMesure
    libGGA/StatisticEngine/EvolutionaryMesure/PopulationDiversity
    libGGA/Utility
)

target_link_libraries(FUGE-LC PRIVATE
    Qt::Concurrent
    Qt::Core
    Qt::Gui
    Qt::Widgets
    Qt::Xml
)


# Resources:
set(fuzzyResources_resource_files
    "coev/coev-memberships.conf"
    "coev/coev-rules.conf"
)

qt_add_resources(FUGE-LC "fuzzyResources"
    PREFIX
        "/"
    FILES
        ${fuzzyResources_resource_files}
)

install(TARGETS FUGE-LC
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET FUGE-LC
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
