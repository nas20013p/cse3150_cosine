#ifndef COSINE_DISTANCE_H
#define COSINE_DISTANCE_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <utility>


class Vector {
private:
    std::vector<double> elements;

public:

    Vector() = default;
    explicit Vector(std::vector<double> elems) : elements(std::move(elems)) {}
    
    // Get the dimension of the vector
    size_t getDimension() const { return elements.size(); }
    
    double at(size_t index) const { 
        if (index >= elements.size()) throw std::out_of_range("Index out of range");
        return elements[index]; 
    }
    
    // Vector operations
    double dotProduct(const Vector& other) const {
        if (getDimension() != other.getDimension())
            throw std::invalid_argument("Vectors must have the same dimension");
        
        double result = 0.0;
        for (size_t i = 0; i < getDimension(); ++i) {
            result += elements[i] * other.at(i);
        }
        return result;
    }
    
    double magnitude() const {
        double sum = 0.0;
        for (const auto& element : elements) {
            sum += element * element;
        }
        return std::sqrt(sum);
    }
    
    double cosineDistance(const Vector& other) const {
        if (getDimension() != other.getDimension())
            throw std::invalid_argument("Vectors must have the same dimension");
        
        double dot = dotProduct(other);
        double mag1 = magnitude();
        double mag2 = other.magnitude();
        
        if (mag1 == 0.0 || mag2 == 0.0)
            throw std::invalid_argument("Cannot compute cosine distance for zero vectors");
        
        double cos_similarity = dot / (mag1 * mag2);
        
        if (cos_similarity > 1.0) cos_similarity = 1.0;
        if (cos_similarity < -1.0) cos_similarity = -1.0;
        
        return std::acos(cos_similarity);
    }
    
    // Display the vector
    friend std::ostream& operator<<(std::ostream& os, const Vector& vec) {
        for (size_t i = 0; i < vec.getDimension(); ++i) {
            os << vec.elements[i];
            if (i < vec.getDimension() - 1) os << " ";
        }
        return os;
    }
};

// Struct to represent a pair of vectors and their distance
struct VectorPair {
    size_t index1;
    size_t index2;
    double distance;
    
    // Sort by distance 
    bool operator<(const VectorPair& other) const {
        return distance < other.distance;
    }
};

// Read vectors from a file
std::vector<Vector> readVectorsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    
    std::vector<Vector> vectors;
    std::string line;
    
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<double> elements;
        double element;
        
        while (iss >> element) {
            elements.push_back(element);
        }
        
        if (!elements.empty()) {
            if (elements.size() < 2) {
                throw std::runtime_error("Vectors must have dimension k ≥ 2");
            }
            vectors.push_back(Vector(elements));
        }
    }
    
    // Check that all vectors have the same dimension
    if (!vectors.empty()) {
        size_t dimension = vectors[0].getDimension();
        for (const auto& vec : vectors) {
            if (vec.getDimension() != dimension) {
                throw std::runtime_error("All vectors must have the same dimension");
            }
        }
    }
    
    return vectors;
}

// Calculate all pairwise cosine distances
std::vector<VectorPair> calculateAllPairwiseDistances(const std::vector<Vector>& vectors) {
    std::vector<VectorPair> pairs;
    
    for (size_t i = 0; i < vectors.size(); ++i) {
        for (size_t j = 0; j < vectors.size(); ++j) {
            if (i != j) {  
                try {
                    double distance = vectors[i].cosineDistance(vectors[j]);
                    pairs.push_back({i, j, distance});
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Cannot calculate cosine distance between vectors " 
                              << i << " and " << j << ": " << e.what() << std::endl;
                }
            }
        }
    }
    
    // Sort by distance 
    std::sort(pairs.begin(), pairs.end());
    
    return pairs;
}

#endif 