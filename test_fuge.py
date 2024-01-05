
import os

# Initialize all the path variables
path_to_fuge_4 = "/Users/mer/Documents/fuge/build-FUGE-LC-Qt_macoOS_v4-Debug/FUGE-LC.app/Contents/MacOS/FUGE-LC"
path_to_fuge_5 = "/Users/mer/Documents/fuge/build-FUGE-LC-Qt_macOS_v5-Debug/FUGE-LC.app/Contents/MacOS/FUGE-LC"
path_to_fuge_6 ="/Users/mer/Documents/fuge/build-FUGE-LC-Qt_6_5_3_for_macOS-Debug/FUGE-LC.app/Contents/MacOS/FUGE-LC"
path_to_fuge_6_cmake = "/Users/mer/Documents/fuge/fuge-lc/cmake-build-debug/FUGE-LC.app/Contents/MacOS/FUGE-LC"
path_to_dataset_arr = "/Users/mer/Documents/projet/dataset/arrhythmia_binary_sorted.csv"
path_to_dataset_arr_pred = "/Users/mer/Documents/projet/dataset/arrhythmia_binary_sorted_pred.csv"
path_to_script_arr = "/Users/mer/Documents/projet/dataset/test.fs"
path_to_dataset_can = "/Users/mer/Documents/projet/dataset/CancerDiag2_headers.csv"
path_to_script_can = "/Users/mer/Documents/projet/dataset/CancerRef.fs"
path_to_script_result = "/Users/mer/Desktop/script_result/"

print("----- testing fuge")

# Run the fuge app to create the fuzzy systems
os.system(path_to_fuge_6_cmake + " -d " + path_to_dataset_arr + " -s " + path_to_script_arr + " -g no" + '\n')

# wait for the fuzzy systems to be created
print("waiting for the fuzzy systems to be created...")

# Go read the names of the fuzzy systems created
path_to_fuzzy_systems = path_to_script_result + "fuzzySystems/"

# read the names of the files in this directory and store them in a list
fuzzy_sys_names = []
for file in os.listdir(path_to_fuzzy_systems):
    if file.endswith(".ffs"):
        fuzzy_sys_names.append(file)

# if the evaluation folder doesn't exist, create it
if not os.path.exists(path_to_script_result + "evaluation"):
    os.makedirs(path_to_script_result + "evaluation")
           
# Run the fuge app to evaluate the fuzzy systems
for fuzzy_sys_name in fuzzy_sys_names:
    path_to_result = path_to_script_result + "evaluation/" + fuzzy_sys_name
    os.system(path_to_fuge_6_cmake + " --evaluate " + " -d " + path_to_dataset_arr + " -s " + path_to_script_arr + " -f " + path_to_fuzzy_systems + fuzzy_sys_name + " -g no > " + path_to_result + '\n')

 
# read the result from result file, find the fitness and print it
for fuzzy_sys_name in fuzzy_sys_names:
    path_to_result = path_to_script_result + "evaluation/" + fuzzy_sys_name
    with open(path_to_result, 'r') as f:
        for line in f:
            if "Fitness" in line:
                fitness = line.split(" ")[2]
                print("----- fitness is " + fitness)
                break
            
# Delete the result file
#os.system("rm " + path_to_script_result + "evaluation/*")

# Delete the fuzzy system file
#os.system("rm " + path_to_script_result + "fuzzySystems/*")

# Delete the temporary files
#os.system("rm " + path_to_script_result + "temp/*")



    
    
    
