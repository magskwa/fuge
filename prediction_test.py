
# if there is a file in the fuzzySystems folder
# do a prediction on it

import os


# initialize the path variables f, d, s
path_to_fuge_6 ="/Users/mer/Documents/fuge/build-FUGE-LC-Qt_6_5_3_for_macOS-Debug/FUGE-LC.app/Contents/MacOS/FUGE-LC"
path_to_dataset_arr_pred = "/Users/mer/Documents/projet/dataset/arrhythmia_binary_sorted_pred.csv"
path_to_script_arr = "/Users/mer/Documents/projet/dataset/test.fs"
path_to_fuzzySystems = "/Users/mer/Desktop/script_result/fuzzySystems"

# go fetch the name of the fist fuzzy system
fsys_name = "One.ffs"
path_to_fsys = path_to_fuzzySystems + "/" + fsys_name

# create the result folder if it doesn't exist
#if not os.path.exists("/Users/mer/Desktop/script_result/prediction"):
#   os.makedirs("/Users/mer/Desktop/script_result/prediction")
# execute the prediction on the dataset
os.system(path_to_fuge_6 + " -f " + path_to_fsys + " -d " + path_to_dataset_arr_pred + " -s " + path_to_script_arr + " -g no --predict " + '\n' )