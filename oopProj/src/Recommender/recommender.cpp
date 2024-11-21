#include "recommender.h"
#include <cmath>
#include <algorithm>
#include <QFile>
#include <QTextStream>
#include <QDebug>

Recommendation::Recommendation() {}

// Calculate cosine similarity
float Recommendation::cosineSimilarity(const QVector<float>& vec1, const QVector<float>& vec2) {
    float dotProduct = 0.0, magnitude1 = 0.0, magnitude2 = 0.0;

    if (vec1.size() != vec2.size()) {
        qDebug() << "Vector size mismatch in cosine similarity calculation!";
        return 0;
    }

    for (int i = 0; i < vec1.size(); ++i) {
        dotProduct += vec1[i] * vec2[i];
        magnitude1 += vec1[i] * vec1[i];
        magnitude2 += vec2[i] * vec2[i];
    }

    float denominator = sqrt(magnitude1) * sqrt(magnitude2);
    return (denominator == 0) ? 0 : (dotProduct / denominator);
}

// Tokenize the input string (genres, keywords, etc.) and vectorize it
void Recommendation::tokenizeAndVectorize(const QString& data, QVector<float>& vector, float weight) {
    QStringList words = data.split(' ', Qt::SkipEmptyParts); // Split by spaces

    for (const QString& word : words) {
        if (!wordIndex.contains(word)) {
            wordIndex[word] = index++;
            for (auto& vec : featureVectors) {
                vec.append(0);
            }
        }
        if (wordIndex[word] >= vector.size()) {
            vector.resize(wordIndex[word] + 1, 0);
        }
        vector[wordIndex[word]] += weight;
    }
}

// Vectorize the movie data from the CSV file
void Recommendation::vectorizer(const QString& fPath) {
    QFile file(fPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening file:" << fPath;
        return;
    }

    QTextStream in(&file);
    QString line;
    in.readLine(); // Skip the header line

    while (in.readLineInto(&line)) {
        QStringList fields = line.split(',', Qt::SkipEmptyParts);

        if (fields.size() < 5) {
            qDebug() << "Malformed line, skipping:" << line;
            continue;
        }

        QString title = fields[0];
        QString genres = fields[1];
        QString keywords = fields[2];
        QString cast = fields[3];
        QString director = fields[4];

        if (title.isEmpty() || genres.isEmpty() || cast.isEmpty() || director.isEmpty()) {
            qDebug() << "Incomplete data, skipping:" << title;
            continue;
        }

        movies.append(title);

        QVector<float> featureVector(wordIndex.size(), 0);
        tokenizeAndVectorize(genres, featureVector, 1.0);  // Weight for genres
        tokenizeAndVectorize(keywords, featureVector, 0.8); // Weight for keywords
        tokenizeAndVectorize(cast, featureVector, 1.5);    // Weight for cast
        tokenizeAndVectorize(director, featureVector, 3.0); // Increased weight for director

        featureVectors.append(featureVector); // Add to the list of vectors
    }

    file.close();
    qDebug() << "Vectorization complete. Movies processed:" << movies.size();
}

// Recommend movies similar to the given title
QVector<QPair<float, QString>> Recommendation::recommendMovies(const QString& movieTitle) {
    int movieIndex = movies.indexOf(movieTitle);
    if (movieIndex == -1) {
        qDebug() << "Movie not found:" << movieTitle;
        return {};
    }

    QVector<float> movieVec = featureVectors[movieIndex];
    QVector<QPair<float, QString>> similarityScores;

    for (int i = 0; i < movies.size(); ++i) {
        if (i != movieIndex) {
            float similarity = cosineSimilarity(movieVec, featureVectors[i]);
            similarityScores.append(qMakePair(similarity, movies[i]));
        }
    }

    std::sort(similarityScores.begin(), similarityScores.end(), std::greater<>());
    if (similarityScores.size() > 10) {
        similarityScores.resize(10);
    }

    return similarityScores;
}
