/**
 * @authors: Fabrice Atrevi <atrevifabrice@gmail.com>; Lionel Houssou <jaderne@gmail.com>; Gaël Aglin <aglingael@gmail.com>
 */

#include <stdio.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <unordered_set>
#include <random>
#include <ctime>

using namespace std;

class book{
public:
    int id_book;
    int score;
public:
    
    book();
    void setScore(int);
    
    
    
};

class librairie{
public:
    int id_lib;
    vector<book> b;
    int time_d;
    long max_book;
    int ponderation;
    
public:
    librairie();
    
    void libScore();
    void setBook(vector<book>);
};

class solution{
public:
    
    int D, B, L;
    vector<librairie> libs;
    
    solution();
    
    void load(string inFilePath);
    vector<librairie> greedySolution();
    void writeSolution(vector<librairie> solution, string outFilePath);
    void runGreedySolution(string inFilePath, string outFilePath);
    vector<vector<int>> localSearchSolution();
    void writeSolution(vector<vector<int>> solution, string outFilePath);
    void runLocalSearchSolution(string inFilePath, string outFilePath);
};
