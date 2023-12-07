# Script description
## Introduction
The script file is a F# file (with .fs extension) containing the parameters of the program. 
It is composed of 3 sections: the first one contains the parameters of the fuzzy system, the second one contains 
the parameters of the co-evolution and the third one contains the parameters of the fitness. 
The parameters are written in the following format: <parameter_name> = <parameter_value>. 

## Fuzzy system parameters

// Fuzzy system parameters
fixedVars = false;
nbRules = 5;
nbMaxVarPerRule = 5;
nbOutVars = 1;
nbInSets = 2;
nbOutSets = 2;
inVarsCodeSize = 5;
outVarsCodeSize = 1;
inSetsCodeSize = 2;
outSetsCodeSize = 1;
inSetsPosCodeSize = 6;
outSetPosCodeSize = 1;

The fuzzy system parameters are the following:
- **fixedVars**: a boolean value indicating whether the variables are fixed or not. 
- **nbRules**: the number of rules in the fuzzy system.
- **nbMaxVarPerRule**: the maximum number of variables per rule.
- **nbOutVars**: the number of output variables.
- **nbInSets**: the number of input sets.
- **nbOutSets**: the number of output sets.
- **inVarsCodeSize**: the number of bits used to code the input variables.
- **outVarsCodeSize**: the number of bits used to code the output variables.
- **inSetsCodeSize**: the number of bits used to code the input sets.
- **outSetsCodeSize**: the number of bits used to code the output sets.
- **inSetsPosCodeSize**: the number of bits used to code the position of the input sets.
- **outSetPosCodeSize**: the number of bits used to code the position of the output sets.
