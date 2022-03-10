#include <algorithm>
#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <stdio.h>
#include <unordered_map>
#include <vector>
using namespace std;

/*
Purpose: This struct provides a custom hash function for the unordered_map
unordered_map<pair<int, int>, vector<pair<int, int>>, hash_pair> clusters. 
Since the key is a pair<int,int> we need a custom hash function. 
*/ 
struct hash_pair {
  template <class T1, class T2> size_t operator()(const pair<T1, T2> &p) const {
    auto hash1 = hash<T1>{}(p.first);
    auto hash2 = hash<T2>{}(p.second);
    return hash1 ^ hash2;
  }
}; 

/*
Purpose: The K_means functions contains some functionality like: 
            1. Read in text file contains all the data points 
            2. Initialize random centroids 
            3. Clusters data points
            4. Write data points with their cluster number into a text file 
*/ 
class K_means{
  private:
    // Function to hash a pair
    int numb_clusters;
    string file_name; 
    int min_x, min_y, max_x, max_y; 
    vector<pair<int, int>> points; 
    unordered_map<pair<int, int>, vector<pair<int, int>>, hash_pair> clusters; 
  public: 
    K_means(int n, string f){
      numb_clusters = n;  
      file_name = f;  
      min_x = INT_MAX;
      min_y = INT_MAX;
      max_x = INT_MIN;
      max_y = INT_MIN;
    };  
    void readData(); 
    void init(); 
    void findKmeans(); 
    void output(); 
}; 
    
/*
Purpose: Read in text file contains all the data points 
*/ 
void K_means::readData() {
  // Open file to read in data points
  fstream file;
  file.open(file_name, ios::in);

  // If file does not exist 
  if (!file.is_open()) {
    cout << "Can't open the file" << endl;
  } else {
    // Populate vector points with data from the text file
    int x;
    int y;
    // Update min_x, max_x, min_y, max_y
    while (file >> x and file >> y) {
      min_x = min(min_x, x);
      max_x = max(max_x, x);
      min_y = min(min_y, y);
      max_y = max(max_y, y);
      points.push_back(make_pair(x, y));
    }
  }
  file.close();
}

/*
Purpose: Initialize random centroids 
*/ 
void K_means::init() {
  // Populate the clusters hash map
  srand((unsigned)time(0));
  for (int i = 0; i < numb_clusters; i++) {
    int x = (rand() % (max_x + 1 - min_x)) + min_x;
    int y = (rand() % (max_y + 1 - min_y)) + min_y;
    while (clusters.count(make_pair(x, y))) {
      int x = (rand() % (max_x + 1 - min_x)) + min_x;
      int y = (rand() % (max_y + 1 - min_y)) + min_y;
    }
    clusters.insert({make_pair(x, y), {}});
  }
}

/*
Purpose: Clusters data points
*/
void K_means::findKmeans() {
  bool newCentroid = true;
  while (newCentroid) {
    newCentroid = false;
    // Erase all the points assign to each clusters 
    for (auto cluster : clusters) {
      cluster.second.clear();
    }

    // Assign points to its clearest clusters
    for (auto point : points) {
      long min_dist = INT_MAX;
      pair<int, int> closest_cluster;
      for (auto cluster : clusters) {
        // Find the Euclidean distance between the point and the cluster
        long curr_dist = sqrt(pow((point.first - cluster.first.first), 2) +
                              pow((point.second - cluster.first.second), 2));

        // Update the min_dist and closest cluster points if found a closer cluster to the point
        if (min_dist > curr_dist) {
          min_dist = curr_dist;
          closest_cluster =
              make_pair(cluster.first.first, cluster.first.second);
        }
      }
      // Assign the point to the cluster
      clusters[closest_cluster].push_back(point);
    }
    
    // Recalculate the centroid for each clusters
    for (auto cluster : clusters) {
      if (cluster.second.size() == 0){
        continue; 
      } 
      int sum_x = 0;
      int sum_y = 0;
      for (auto p : cluster.second) {
        sum_x += p.first;
        sum_y += p.second;
      }
      int centroid_x = sum_x / cluster.second.size();
      int centroid_y = sum_y / cluster.second.size();

      // If the new centroid is found, erase current centroid 
      // and create a new centroid 
      if (!clusters.count(make_pair(centroid_x, centroid_y))) {
        newCentroid = true;
        clusters.erase(cluster.first);
        clusters.insert({make_pair(centroid_x, centroid_y), {}});
      }
    }
  }
}

/* 
Write data points with their cluster number into a text file 
*/ 
void K_means::output(){
  ofstream MyFile("output.txt"); 
  int index = 1; 
  for(auto c: clusters){
    for(auto point: c.second){
      MyFile << point.first << "\t" << point.second << "\t" << index << "\n"; 
    }
    index++; 
  }

  MyFile.close(); 
}

int main(int argc, char *argv[]) {
  // Validate the number of arguments
  if (argc != 3) {
    cout << "Invalid number of arguments. Need 2 arguments" << endl;
    return -1;
  }

  // Read in the number of clusters needed
  int numb_clusters = stoi(argv[1]);
  string file_name = argv[2];

  // Create an instance of K_means 
  K_means k_means(numb_clusters, file_name); 
  
  // Gather data points from the text file
  k_means.readData();   
  
  // Populate random number for the clusters
  k_means.init(); 

  // findKmeans(clusters, points);
  k_means.findKmeans(); 

  // Output data points with cluster points to a text file 
  k_means.output(); 

  return 0;
}