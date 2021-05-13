/**
* @file RecommenderSystem.h
* @author  Jonathan Birnbaum
* @date 18/06/2020
*
* @brief RecommenderSystem class declaration and documentation
*/

#ifndef EX5_RECOMMENDERSYSTEM_H
#define EX5_RECOMMENDERSYSTEM_H

#include <string>
#include <unordered_map>
#include <vector>
#include <valarray>
#include <iostream>

using std::string;
using std::unordered_map;
using std::vector;
using std::valarray;

/**
 * @brief class representing a recommending movie system
 */
class RecommenderSystem
{
    private:
        /**
         * @brief class representing a user in the cinema
         */
        class User
        {
            public:
                unordered_map<string, double>   _seenMovies; // map(movie_name, user_rate)
                vector<string>                  _unSeenMovies;
                double                          _numberSeenMovies;
        };
        unordered_map<string, valarray<double>> _moviesFeatures; // map(movie_name, valarray(features_rate))
        unordered_map<string, double>           _moviesNorm; // map(movie_name, movie_vector_norm)
        unordered_map<string, User>             _users; // map(user_name, User)
        unordered_map<string, int>              _allMoviesWithIndex; //(movie_name, index)
        int                                     _numberFeatures;

    public:

        /**
         * @brief load the data from the input files
         * @param moviesFeatureFilePath: path to features file
         * @param userRankFilePath: path to user ranks file
         * @return 0 for success and 1 for failure
         */
        int loadData(const string& moviesFeatureFilePath, const string& userRankFilePath);

        /**
         * @brief find the most similar movie according to a user preferences
         * @param userName: string of user's name to find a movie for
         * @return the name of the most recommended movie to the given user
         */
        string recommendByContent(const string& userName) const;

        /**
         * @brief calculate the expected rate of an unseen movie according to given user's perefences
         * @param movieName: unseen movie given
         * @param userName: given user name
         * @param k: a parameter to the filtering algorithm
         * @return similarity value result to the given unseen movie
         */
        double predictMovieScoreForUser(const string& movieName, const string& userName, int k) const;

        /**
        * @brief find the movie to be the most recommended by given user according to movies he had been seen
        * @param userName: given user name
        * @param k: a parameter to the filtering algorithm
        * @return the most recommended movie
        */
        string recommendByCF(const string& userName, int k) const;
};


#endif //EX5_RECOMMENDERSYSTEM_H
