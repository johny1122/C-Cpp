/**
 * @file RecommenderSystem.cpp
 * @author  Jonathan Birnbaum
 * @date 18/06/2020
 *
 * @brief RecommenderSystem class implementation
 */

#include "RecommenderSystem.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>

#define DELIMITER_INPUT ' '
#define NO_RANK "NA"
#define EXIT_FAIL (-1)
#define ERROR_MSG_USER_NOT_FOUND "USER NOT FOUND"

using std::istringstream;

static double calculateNorm (const valarray<double>& vec, int vectorSize);
static double calculateDotProduct (const valarray<double>& leftVec, const valarray<double>& rightVec, int vectorSize);
static double calculateExpectedRate(const unordered_map<string, double>& mostSimilarMap, const unordered_map<string,
                                    double>& seenMovies);


/**
 * @brief loading the data from the 2 files
 * @param moviesFeatureFilePath: path to features file
 * @param userRankFilePath: path to ranks file
 * @return 0 for success and -1 for failure
 */
int RecommenderSystem::loadData(const string& moviesFeatureFilePath, const string& userRankFilePath)
{
    std::ifstream fileStreamFeature(moviesFeatureFilePath);
    if (!fileStreamFeature.is_open())
    {
        std::cerr << "Unable to open file " << moviesFeatureFilePath << std::endl;
        return EXIT_FAIL;
    }
    if (!fileStreamFeature.good())
    {
        return EXIT_FAIL;
    }

    std::ifstream fileStreamRank(userRankFilePath);
    if (!fileStreamRank.is_open())
    {
        std::cerr << "Unable to open file " << userRankFilePath << std::endl;
        return EXIT_FAIL;
    }
    if (!fileStreamRank.good())
    {
        return EXIT_FAIL;
    }

//-------------------- LOAD FEATURES FILE -----------------------

    string line;
    while (std::getline(fileStreamFeature, line))
    {
        std::istringstream iss(line);
        string movieName;
        std::getline(iss, movieName, DELIMITER_INPUT);
        _moviesFeatures[movieName];

        vector<double> tempVec;
        int numFeatures = 0;
        double featureRate;
        while (iss >> featureRate)
        {
            tempVec.push_back(featureRate);
            numFeatures++;
        }
        valarray<double>tempVal (tempVec.size());
        std::copy(begin(tempVec), end(tempVec), begin(tempVal));
        _moviesNorm[movieName] = calculateNorm(tempVal, tempVec.size());
        _moviesFeatures[movieName] = tempVal;
        _numberFeatures = numFeatures;
    }

//-------------------- LOAD RANK FILE -----------------------

    string movieName;
    std::getline(fileStreamRank, line);
    std::istringstream iss(line);
    vector<string> allMovies;
    while (iss >> movieName)
    {
        allMovies.push_back(movieName);
    }
    for (int index = 0; index < (int)allMovies.size(); index++)
    {
        _allMoviesWithIndex[allMovies[index]] = index;
    }

    int counter = 0;
    while (std::getline(fileStreamRank, line))
    {
        std::istringstream iss(line);
        string userName;
        std::getline(iss, userName, DELIMITER_INPUT);
        _users[userName];

        string rank;
        double numberSeen = 0;
        while (iss >> rank)
        {
            if (rank == NO_RANK)
            {
                _users[userName]._unSeenMovies.push_back(allMovies[counter]);
            }
            else
            {
                _users[userName]._seenMovies[allMovies[counter]] = std::stod(rank);
                numberSeen++;
            }
            counter++;
        }
        _users[userName]._numberSeenMovies = numberSeen;
        counter = 0;
    }

    return EXIT_SUCCESS;
}


/**
 * @brief calculate the norm of a vector
 * @param vec: given vector to calculate norm of
 * @param vectorSize: size of vector
 * @return norm result
 */
static double calculateNorm (const valarray<double>& vec, int vectorSize)
{
    double result = 0;
    for (int index = 0; index < vectorSize; index++)
    {
        result += pow(vec[index], 2.0);
    }
    return sqrt(result);
}


/**
 * @brief calculate dot product of 2 vectors
 * @param leftVec: left vector in the product
 * @param rightVec: right vector in the product
 * @param vectorSize: size of the vectors
 * @return dot product result
 */
static double calculateDotProduct (const valarray<double>& leftVec, const valarray<double>& rightVec, int vectorSize)
{
    double result = 0;
    for (int index = 0; index < vectorSize; index++)
    {
        result += leftVec[index] * rightVec[index];
    }
    return result;
}


/**
 * @brief find the most similar movie according to a user preferences
 * @param userName: string of user's name to find a movie for
 * @return the name of the most recommended movie to the given user
 */
string RecommenderSystem::recommendByContent(const string& userName) const
{
    if (_users.find(userName) == _users.end())
    {
        return ERROR_MSG_USER_NOT_FOUND;
    }

    // calculate average
    double average = 0;
    for (const auto& _movie : _users.at(userName)._seenMovies)
    {
        average += _movie.second;
    }
    average /= _users.at(userName)._numberSeenMovies;

    // normalizing rate vector
    unordered_map<string, double> tempMap_seen = _users.at(userName)._seenMovies;
    for (auto& _movie : tempMap_seen)
    {
        _movie.second -= average;
    }

    // calculating preferring vector
    valarray<double> preferVector (_numberFeatures);
    for (const auto& _movie : tempMap_seen)
    {
        preferVector += (_movie.second * _moviesFeatures.at(_movie.first));
    }

    // calculate similarity
    unordered_map<string, double> similarityMap; // (movie_name, similarity_value)
    double preferVectorNorm = calculateNorm(preferVector, _numberFeatures);

    for (const auto& _movie : _users.at(userName)._unSeenMovies)
    {
        double dotProductResult = calculateDotProduct(preferVector, _moviesFeatures.at(_movie), _numberFeatures);
        double movieNorm = _moviesNorm.at(_movie);
        similarityMap[_movie] = dotProductResult / (preferVectorNorm * movieNorm);
    }

    string mostSimilarMovie;
    double maxSimilarity = 0;
    bool firstElement = true;
    for (const auto& _movie : similarityMap)
    {
        if (firstElement)
        {
            maxSimilarity = _movie.second;
            mostSimilarMovie = _movie.first;
            firstElement = false;
        }
        else if (maxSimilarity < _movie.second)
        {
            maxSimilarity = _movie.second;
            mostSimilarMovie = _movie.first;
        }
        else if (maxSimilarity == _movie.second)
        {
            if (_allMoviesWithIndex.at(_movie.first) < _allMoviesWithIndex.at(mostSimilarMovie))
            {
                mostSimilarMovie = _movie.first;
            }
        }
    }
    return mostSimilarMovie;
}


/**
 * @brief calculate the final expected rate of an unseen movie
 * @param mostSimilarMap: map of the most similar seen movies and their similarity value
 * @param seenMovies: map of seen movies and their's user's rate
 * @return final rate
 */
static double calculateExpectedRate(const unordered_map<string, double>& mostSimilarMap, const unordered_map<string,
                                    double>& seenMovies)
{
    double upperFraction = 0;
    double bottomFraction = 0;
    for (const auto& _movie : mostSimilarMap)
    {
        upperFraction += (_movie.second * seenMovies.at(_movie.first));
        bottomFraction += _movie.second;

    }
    return upperFraction / bottomFraction;
}


/**
 * @brief calculate the expected rate of an unseen movie according to given user's preferences
 * @param movieName: unseen movie given
 * @param userName: given user name
 * @param k: a parameter to the filtering algorithm
 * @return similarity value result to the given unseen movie
 */
double RecommenderSystem::predictMovieScoreForUser(const string &movieName, const string &userName, int k) const
{
    if ((_users.find(userName) == _users.end()) || (_moviesFeatures.find(movieName) == _moviesFeatures.end()))
    {
        return EXIT_FAIL;
    }

    unordered_map<string, double> similarityMap; // (movie_name, similarity_value)
    double unSeenVectorNorm = _moviesNorm.at(movieName);
    for (const auto& _seenMovie : _users.at(userName)._seenMovies)
    {
        double dotProductResult = calculateDotProduct(_moviesFeatures.at(movieName),
                _moviesFeatures.at(_seenMovie.first), _numberFeatures);
        double seenMovieNorm = _moviesNorm.at(_seenMovie.first);
        similarityMap[_seenMovie.first] = dotProductResult / (unSeenVectorNorm * seenMovieNorm);
    }

    unordered_map<string, double> mostSimilarMap; // (movie_name, similarity_value)
    for (int i = 0; i < k; i++)
    {
        double maxSimilarValue = 0;
        string mostSimilarMovie;
        bool firstElement = true;
        for (const auto& _movie : similarityMap)
        {
            if (firstElement)
            {
                maxSimilarValue = _movie.second;
                mostSimilarMovie = _movie.first;
                firstElement = false;
            }
            else if (maxSimilarValue < _movie.second)
            {
                maxSimilarValue = _movie.second;
                mostSimilarMovie = _movie.first;
            }
        }
        mostSimilarMap[mostSimilarMovie] = maxSimilarValue;
        similarityMap.erase(mostSimilarMovie);
    }

    return calculateExpectedRate(mostSimilarMap, _users.at(userName)._seenMovies);
}


/**
 * @brief find the movie to be the most recommended by given user according to movies he had been seen
 * @param userName: given user name
 * @param k: a parameter to the filtering algorithm
 * @return the most recommended movie
 */
string RecommenderSystem::recommendByCF(const string& userName, int k) const
{
    if (_users.find(userName) == _users.end())
    {
        return ERROR_MSG_USER_NOT_FOUND;
    }

    string mostSimilarMovie;
    double maxSimilarValue = 0;
    bool firstElement = true;
    for (const auto& _unSeenMovie : _users.at(userName)._unSeenMovies)
    {
        double result = predictMovieScoreForUser(_unSeenMovie, userName, k);
        if (firstElement)
        {
            maxSimilarValue = result;
            mostSimilarMovie = _unSeenMovie;
            firstElement = false;
        }
        else if (maxSimilarValue < result)
        {
            maxSimilarValue = result;
            mostSimilarMovie = _unSeenMovie;
        }
        else if (maxSimilarValue == result)
        {
            if (_allMoviesWithIndex.at(_unSeenMovie) < _allMoviesWithIndex.at(mostSimilarMovie))
            {
                mostSimilarMovie = _unSeenMovie;
            }
        }
    }

    return mostSimilarMovie;
}
