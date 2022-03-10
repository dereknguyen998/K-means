GOAL: gain deeper understanding about clustering by implementing a simple k-means algorithm.
DESCRIPTION: Implemented a class that provides users to: 
		1. Read in text file contains all the data points 
            	2. Initialize random centroids 
            	3. Clusters data points
            	4. Write data points with their cluster number into a text file 
The program was implemented in C++ using REPLIT, an online IDE. 

FILES: 
1) Two dimensional data points text file 
input1.txt 
input2.txt
input3.txt
input4.txt 
2) Source code 
k_means.cpp 

HOW TO COMPILE ON LINUX? 
g++ -o k_means k_means.cpp -std=c++11
./k_means [k] [input file]
k: an integer that holds the number of clusters 
input file: a txt file that contains 2 dimensional data 
The image below describe the input text file 
[Image]

OUTPUT: 
The image below describe the output text file 
[Image] 

WHAT'S NEXT? 
An implementation of a GUI where users can visualize the 2-dimensional data as well as the clustering result. 

