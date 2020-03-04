/**
 * @authors: Fabrice Atrevi <atrevifabrice@gmail.com>; Lionel Houssou <jaderne@gmail.com>; Gaël Aglin <aglingael@gmail.com>
 */

#include "class.h"

using namespace std;

book::book() {

}

void book::setScore(int s) {
    score = s;
}

librairie::librairie() {

}

void librairie::setBook(vector<book> bo) {
    b = bo;
}

void librairie::libScore() {
    for (int i = 0; i < b.size(); i++) {
        ponderation += b.at(i).score;
    }
    ponderation = (ponderation * max_book) / time_d;
}

solution::solution() {

}

void solution::load(string fileName) {

    ofstream writeToFile;

    string txtToWrite = "";
    string txtFromFile = "";
    string delimiter = " ";

    vector<book> bks;

    ifstream readFromFile(fileName, ios_base::in);
    if (readFromFile.is_open()) {
        readFromFile >> B >> L >> D;

        // Books score extraction
        for (int i = 0; i < B; i++) {
            book bk;
            bk.id_book = i;
            readFromFile >> bk.score;

            bks.push_back(bk);
        }

        // Library
        for (int i = 0; i < L; i++) {
            librairie lib;
            int nj, tj, mj;
            readFromFile >> nj >> tj >> mj;

            lib.time_d = tj;
            lib.max_book = mj;

            for (int j = 0; j < nj; j++) {

                lib.id_lib = i;
                int b_id;
                readFromFile >> b_id;

                lib.b.push_back(bks.at(b_id));
            }
            libs.push_back(lib);
        }
    } else cout << "Unable to open file";
}


struct descendingBook {
    inline bool operator()(const book &lib1, const book &lib2) {
        if (lib1.score == lib2.score)
            return lib1.score > lib2.score;
        return lib1.score > lib2.score;
    }
};

struct descendingLib {
    inline bool operator()(const librairie &lib1, const librairie &lib2) {
        if (lib1.ponderation == lib2.ponderation)
            return lib1.ponderation > lib2.ponderation;
        return lib1.ponderation > lib2.ponderation;
    }
};


vector<librairie> solution::greedySolution() {
    vector<librairie> res;
    for (int i = 0; i < libs.size(); i++) {
        libs[i].libScore();
    }
    // sort
    sort(libs.begin(), libs.end(), descendingLib());

    // Choix
    int d_restant = D;

    for (int i = 0; i < libs.size(); i++) {
        if (d_restant - libs[i].time_d > 0) {
            res.push_back(libs[i]);

            d_restant -= libs[i].time_d;
        } else {
            break;
        }
    }
    return res;
}

void solution::writeSolution(vector<librairie> solution, string outFilePath) {

    ofstream writeToFile(outFilePath, ios_base::out);
    writeToFile << solution.size() << endl;

    int remainingDays = D;
    int scoreTotal = 0; //the score total value is not realistic since some we produce solution with number of books greater than possible
    for (librairie lib : solution) {
        remainingDays -= lib.time_d;
        long nb_book = min((long) lib.b.size(), lib.max_book * remainingDays);
        writeToFile << lib.id_lib << " " << nb_book << endl;

        sort(lib.b.begin(), lib.b.end(), descendingBook());
        for (int i = 0; i < nb_book; ++i) {
            writeToFile << lib.b[i].id_book << " ";
            scoreTotal += lib.b[i].score;
        }
        writeToFile << endl;
    }
    cout << "score total = undefined" << endl;
}

//functors necessary for the books hashset (unordered_set)
//we use this structure to speed up the search since the complexity of the hash is constant
struct HashBook {
    size_t operator()(const book &bk) const {
        return bk.id_book;
    }
};
struct CompareBook {
    bool operator()(book const &lhs, book const &rhs) const {
        return lhs.id_book == rhs.id_book;
    }
};

//same functors for libraries as for books
struct HashLib {
    int operator()(const librairie &lb) const {
        return lb.id_lib;
    }
};
struct CompareLib {
    bool operator()(librairie const &lhs, librairie const &rhs) const {
        return lhs.id_lib == rhs.id_lib;
    }
};

vector<vector<int>> solution::localSearchSolution() {
    //this code is based on local search. Ideally, we should keep the n best solutions at each iteration
    //such that we select one radomly to create a such of diversification. Unfortunately, we only do
    //intensification in this code so it has the same cons as hill clibimg algorithm. However, you can
    //improve it if you want ;-)

    //overall solution parameters
    int remainingDays = D;
    long scoreTotal = 0;
    unordered_set<book, HashBook, CompareBook> alreadyScanned;
    unordered_set<librairie, HashLib, CompareLib> selectLib;
    vector<vector<int>> solution;

    while (remainingDays > 0 && selectLib.size() < L) { //each iteration of the local search

        //define the variables to keep the best result and save it at the end of the iteration
        //instead of scalars, a structure should be used to keep the solution to have a clean representation but we don't do it :-(
        //Moreover, stl collections must be used to integrate diversification in the code such that the solution
        //of the iteration will be picked among the best n kept.
        librairie *bestlib = nullptr;
        long bestScore = 0;
        vector<book> bestBooks;

        for (librairie &lib: libs) {
            //if the current library is always selected in the solution, go to the next
            if (selectLib.find(lib) != selectLib.end())
                continue;

            //if the remaining days after signing up some libraries and this one is not enough to perform a scan
            int localRemainingDays = remainingDays - lib.time_d;
            if (localRemainingDays <= 0)
                continue;

            //compute the maximum possible number of books to scan
            long maxbooks = lib.max_book * localRemainingDays;
            //sort books of the library from the most valuable to the lowest
            sort(lib.b.begin(), lib.b.end(), descendingBook());

            //j is the number counter while i is the index of the existing books
            long i = 0, j = 0;
            //compute the max score provided by the library
            long scoreMax = 0;
            vector<book> possibleBooks;
            while (j < maxbooks && i < lib.b.size()) {
                //if not already scanned
                if (alreadyScanned.find(lib.b[i]) == alreadyScanned.end()) {
                    scoreMax += lib.b[i].score;
                    possibleBooks.push_back(lib.b[i]);
                    j++;
                }
                i++;

            }

            //update best score parameters
            if (bestlib == nullptr) {
                bestlib = &lib;
                bestScore = scoreMax;
                //bestBooks = possibleBooks;
                bestBooks.clear();
                for (int i = 0; i < possibleBooks.size(); ++i) {
                    bestBooks.push_back(possibleBooks[i]);
                }
            } else {
                //this commented code is a way to simulate a kind of diversification. It consider to assign a penalty
                // to the best value of the library before comparing to the current best.
                // It does not work pretty well. It needs many tweaks so we commented it even if it allows us to increase
                //a little bit our score on instances e and f :-)

                /*std::mt19937 rng(time(NULL));
                std::uniform_int_distribution<int> gen(95, 100); // uniform, unbiased
                int r = gen(rng);
                if (scoreMax/(lib.time_d * ((float)r)/100) > bestScore/(bestlib->time_d )){*/


                if (scoreMax / lib.time_d > bestScore / bestlib->time_d) {
                    bestlib = &lib;
                    bestScore = scoreMax;
                    //bestBooks = possibleBooks;
                    bestBooks.clear();
                    for (int i = 0; i < possibleBooks.size(); ++i) {
                        bestBooks.push_back(possibleBooks[i]);
                    }
                }
            }
        }
        //if there is no possible library for an iteration, we cannot do better than the current solution
        //so we break and we print the current solution
        if (bestlib == nullptr)
            break;

        //if there is a solution for the iteration
        //add the solution the selected lib list
        selectLib.insert(*bestlib);
        //add the books of the solution to the scanned books list
        for (int i = 0; i < bestBooks.size(); ++i) {
            alreadyScanned.insert(bestBooks[i]);
        }

        //build the solution
        vector<int> firstline{bestlib->id_lib, (int)bestBooks.size()};
        vector<int> secondline;
        secondline.reserve(bestBooks.size());
        for (auto &bestBook : bestBooks) {
            secondline.push_back(bestBook.id_book);
        }
        solution.push_back(firstline);
        solution.push_back(secondline);

        //compute some values for the next iteration
        remainingDays -= bestlib->time_d;
        scoreTotal += bestScore;
    }
    cout << "score total = " << scoreTotal << endl;
    return solution;
}

void solution::writeSolution(vector<vector<int>> solution, string outFilePath) {
    ofstream writeToFile(outFilePath, ios_base::out);
    writeToFile << solution.size() / 2 << endl;

    for (int j = 0; j < solution.size(); ++j) {
        vector<int> line = solution[j];
        for (int i = 0; i < line.size()-1; ++i) {
            writeToFile << line[i] << " ";
        }
        writeToFile << line[line.size()-1] << endl;
    }
}

void solution::runLocalSearchSolution(string inFilePath, string outFilePath) {
    cout << "lSearch solution : instance " << inFilePath << " started" << endl;
    load(inFilePath);
    vector<vector<int>> solution = localSearchSolution();
    writeSolution(solution, outFilePath);
    cout << "lSearch solution : instance " << inFilePath << " finished" << endl << endl;
}

void solution::runGreedySolution(string inFilePath, string outFilePath) {
    cout << "greedy solution : instance " << inFilePath << " started" << endl;
    load(inFilePath);
    vector<librairie> solution = greedySolution();
    writeSolution(solution, outFilePath);
    cout << "greedy solution : instance " << inFilePath << " finished" << endl << endl;
}