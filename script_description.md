# Script description
## Introduction
The script file is a F# file (with .fs extension) containing the parameters of the program. 
It is composed of 3 sections: the first one contains the parameters of the fuzzy system, the second one contains 
the parameters of the co-evolution and the third one contains the parameters of the fitness. 
The parameters are written in the following format: <parameter_name> = <parameter_value>. 
The script file also contains two functions: the first one is used to set the parameters of the fuzzy system, 
the second one is used to run the co-evolution. Those functions are mandatory.

## Fuzzy system parameters

The fuzzy system parameters are the following:
- **fixedVars**: a boolean value indicating whether the variables of the fuzzy system are fixed or not. If it is the case
then the number of variables are equal to nbMaxVarPerRule, and they are chosen in the order of the variables in the database.
- **nbRules**: the number of rules in the fuzzy system. 
- **nbMaxVarPerRule**: the maximum number of variables per rule.
- **nbOutVars**: the number of output variables. (In case of multiple output variables, the fitness parameters are computed by averaging the fitness parameters of each output variable)
- **nbInSets**: the number of input sets per variable.
- **nbOutSets**: the number of output sets per variable.
- **inVarsCodeSize**: the number of bits used to code the input variables.
- **outVarsCodeSize**: the number of bits used to code the output variables.
- **inSetsCodeSize**: the number of bits used to code one input set's parameters
- **outSetsCodeSize**: the number of bits used to code one output set's parameters
- **inSetsPosCodeSize**: the number of bits used to code the position of the input sets.
- **outSetPosCodeSize**: the number of bits used to code the position of the output sets.

## Co-evolution parameters

- **maxGenPop1**: the maximum number of generations for the population of the membership functions.
- **maxFitPop1**: the maximum fitness for the population of the membership functions.
- **elitePop1**: the number of elite individuals in the population of the membership functions.
- **popSizePop1**: the size of the population of the membership functions.
- **cxProbPop1**: the crossover probability for the population of the membership functions.
- **mutFlipIndPop1**: the probability that an individual is a target for a mutation in the population of the membership functions.
- **mutFlipBitPop1**: the probability that a bit of an individual is mutated in the population of the membership functions.
- **elitePop2**: the number of elite individuals in the population of the rules.
- **popSizePop2**: the size of the population of the rules.
- **cxProbPop2**: the crossover probability for the population of the rules.
- **mutFlipIndPop2**: the probability that an individual is a target for a mutation in the population of the rules.
- **mutFlipBitPop2**: the probability that a bit of an individual is mutated in the population of the rules.


## Fitness parameters

- **sensitivityW**: the weight of the sensitivity in the fitness.
- **specificityW**: the weight of the specificity in the fitness.
- **accuracyW**: the weight of the accuracy in the fitness.
- **ppvW**: the weight of the ppv in the fitness.
- **rmseW**: the weight of the rmse in the fitness.
- **rrseW**: the weight of the rrse in the fitness.
- **raeW**: the weight of the rae in the fitness.
- **mxeW**: the weight of the mxe in the fitness.
- **distanceThresholdW**: the weight of the distance threshold in the fitness.
- **distanceMinThresholdW**: the weight of the distance min threshold in the fitness.
- **dontCareW**: the weight of the dont care in the fitness.
- **overLearnW**: the weight of the over learn in the fitness.
- **threshold**: the threshold used to compute the fitness.
- **threshActivated**: a boolean value indicating whether the threshold is activated or not.


## Functions

- **doSetParams**: this function is used to set the parameters of the fuzzy system specified in the first section of the script file. Its content is composed of : 
```fsharp
    function doSetParams(){
        this.setParams( <parameter_list> );
    }
```

- **doRun**: this function is used to run the co-evolution. It is composed of a list of runs. It is possible to use this 
methods to run multiple runs of co-evolution with different parameters. Careful, the parameters values initialized in the
above will be overwritten by the values specified in the doRun function. The content of the function is the following:
```fsharp
function doRun()
{
    // for each parameter that we want to explore, we create a list of values
    <parameter1_list> = <parameter1_values>;
    <parameter2_list> = <parameter2_values>;
    ...
    <parameterN_list> = <parameterN_values>;
    var qtyExp = 1 // Number of experiments to run for each combination of parameters

    // Multiple coevolution runs with different parameters
    for (var i = 0; i < parameter1_list.length; i++) {
        for(var j = 0; j < parameter2_list.length; j++) {
            ...
            for (var k = 0; k < parameterN_list.length; k++) {
                    for (var n = 0; n < qtyExp; n++) {
                        parameter1 = parameter1_list[i]
                        parameter2 = parameter2_list[j]
                        ...
                        parameterN = parameterN_list[l]
                        this.runEvo();
                    }
                }
            }
        }
    }
}
```

# Chromosome structure
## Rule chromosome
#### Antecedents 
The antecedents of the rule is represented as sequences of tuples, where each tuple consists of a variable number 
and its corresponding set number. The length of the variable number is specified by the parameter inVarsCodeSize, 
and the length of the set number is specified by the parameter inSetsCodeSize. With this information, it is possible 
to extract and interpret the first part of the contents of the chromosome, as each tuple in the sequence contributes 
to the rule's definition.

#### Consequents
The consequent of the rule is also represented as sequences of tuples, where each tuple consists of a output number 
and its corresponding set number. The length of the output number is specified by the parameter outVarsCodeSize, and
the length of the set number is specified by the parameter outSetsCodeSize. The consequent is located at the end of
the chromosome. It starts at the end of the maxNbAntecedents * (inVarsCodeSize + inSetsCodeSize) bits.

#### Full chromosome
The full rule chromosome is composed of multiple rules, each rule being composed of the antecedents and the consequent.
Here is an example of a chromosome with 2 rules, 2 antecedents per rule and 1 consequent per rule, with an 2 bits code for
the input and output variables and 3 bits code for the input and output sets. 
The chromosome is as follows: 011111101000010110011010100011.  The first 15 bits decribe the first rule : 011111101000010. 
The first 5 bits (01111) are the first antecedent (var = 01, set = 111), the next 5 bits (11010) are the second 
antecedent (var = 11, set = 010). The last 5 bits (00010) are the consequent (out = 00, set = 010). The same logic can be
applied to the second rule (110011010100011).
```
   011111101000010110011010100011
   | |  | |  | |  | |  | |  | | 
   | |  | |  | |  | |  | |  | set3
   | |  | |  | |  | |  | |  out0
   | |  | |  | |  | |  | set5
   | |  | |  | |  | |  var2
   | |  | |  | |  | set1
   | |  | |  | |  var3
   | |  | |  | set2
   | |  | |  out0
   | |  | set2
   | |  var3
   | set7
   var1
   <------------->                  rule 1
                  <------------->   rule 2
```

## Membership chromosome

#### Fuzzy input and output set
The membership chromosome is composed of multiple sets. Each fuzzy set contains a name, a number (for the identification),
a position (which is the position of the peak of the set) and an evaluation (which is the value obtained for a given input).
The shape of the set have always the same format :
```
______               ________ 
      \   /\   /\   /          The first and the last sets are trapezoidal
       \ /  \ /  \ /           and all internal sets have a triangular shape.
        X    X    X
       / \  / \  / \
______/___\/___\/___\________
```
The number of bit for the position of the peak is coded with the parameter inSetsPosCodeSize. The number of the set per 
variable is coded with the parameter nbInSets. The number of bits to encode the set number is coded with the parameter
inSetsCodeSize. The evaluation of a set allows to compute the value of the membership function for a given input. 
The aggregation method is the maximum of the evaluations of the sets. The defuzzification method is the center of gravity
(COA) of the evaluations of the sets.
In the chromosome, each set is defined by 


#### Full chromosome
The membership chromosome is composed of multiple fuzzy sets. Each input variable has a number of sets defined by the parameter
nbInSets and each output variable has a number of sets defined by the parameter nbOutSets. The chromosome is composed of
the input sets organized by variable followed by the output sets organized by variable as well. 

