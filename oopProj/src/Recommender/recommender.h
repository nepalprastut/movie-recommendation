#ifndef RECOMMENDER_H
#define RECOMMENDER_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QDebug>

class Recommendation {
private:
    QVector<QString> movies;               // List of movie titles
    QVector<QVector<float>> featureVectors; // Combined feature vectors
    QMap<QString, int> wordIndex;          // Index for bag-of-words
    int index = 0;                         // Index counter for new words

    // Calculate cosine similarity
    float cosineSimilarity(const QVector<float>& vec1, const QVector<float>& vec2);

    void tokenizeAndVectorize(const QString& data, QVector<float>& vector, float weight = 1.0);

public:
    // Constructor
    Recommendation();

    // Vectorizes movie data from the CSV file
    void vectorizer(const QString& fPath);

    // Recommends movies based on a given movie title
    QVector<QPair<float, QString>> recommendMovies(const QString& movieTitle);

};

#endif // RECOMMENDER_H
