# Build-A-City

HOW TO RUN 
 1. Run   
 ` $./risingCity file_name`
    Where file_name is the name of the file that has the input test data.
    
 2. Input format
 
    Input test data should be given in the following format.
 
    **Insert(buildingNum, total_time)**
    
    **Print(buildingNum)**
    
    **Print (buildingNum1, buildingNum2)**
    
 
    You cannot insert a building for construction to the data structures unless global time equals to the arrival time of the construction. All the time data are       given in days.
 
    Following is an example of input.
 
    **0: Insert(5,25)**

    **2: Insert(9,30)**

    **7: Insert(30,3)**

    **9: Print (30)**

    **10: Insert(1345,12)**

    **13: Print (10,100)**

    **14: Insert(345,14)**

    **39: Insert(4455,14)**
 
    The number at the beginning of each command is the global time that the command has appeared in the system. You must have a global time counter, which starts at     0. You can read the input command only when the global time matches the time in the input command. You can assume this time is an integer in increasing order.       When no input remains, construction continues on the remaining buildings until all are complete.
    
  3. Output format
    
    Insert(buildingNum ,total_time) should produce no output unless buildingNum is a duplicate in which case you should output an error and stop.
 
    PrintBuilding (buildingNum) will output the (buildingNum,executed_time,total_time) triplet if the buildingNum exists. If not print (0,0,0).
 
    PrintBuilding (buildingNum1, buildingNum2) will output all (buildingNum,executed_time,total_time) triplets separated by commas in a single line including       buildingNum1 and buildingNum2; if they exist. If there is no building in the specified range, output (0,0,0). You should not print an additional comma at the end of the line.

   Other oupt includes completion date of each building and completion date of city.
 
   All output should go to a file named “output_file.txt”.



