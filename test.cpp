#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "CosineDistance.h"
#include <cmath>

TEST_CASE("Vector construction and basic operations") {
    Vector v({1.0, 2.0, 3.0});
    
    CHECK(v.getDimension() == 3);
    CHECK(v.at(0) == 1.0);
    CHECK(v.at(1) == 2.0);
    CHECK(v.at(2) == 3.0);
    
    CHECK_THROWS_AS(v.at(3), std::out_of_range);
}

TEST_CASE("Vector dot product") {
    Vector v1({1.0, 2.0, 3.0});
    Vector v2({4.0, 5.0, 6.0});
    
    CHECK(v1.dotProduct(v2) == doctest::Approx(32.0));
    
    Vector v3({1.0, 2.0});
    CHECK_THROWS_AS(v1.dotProduct(v3), std::invalid_argument);
}

TEST_CASE("Vector magnitude") {
    Vector v1({3.0, 4.0});
    CHECK(v1.magnitude() == doctest::Approx(5.0));
    
    Vector v2({1.0, 1.0, 1.0});
    CHECK(v2.magnitude() == doctest::Approx(std::sqrt(3.0)));
}

TEST_CASE("Cosine distance - example from lab document (3D)") {
    Vector v1({1.0, 1.0, 1.0});
    Vector v2({1.0, 1.0, 0.0});
    
    double cos_dist = v1.cosineDistance(v2);
    double expected = 0.61548;
    
    CHECK(cos_dist == doctest::Approx(expected).epsilon(0.00001));
}

TEST_CASE("Cosine distance - example from lab document (5D)") {
    Vector v1({1.0, 1.0, 1.0, 1.0, 1.0});
    Vector v2({1.0, 1.0, 1.0, 1.0, 0.0});
    
    double cos_dist = v1.cosineDistance(v2);
    double expected = 0.463648;
    
    CHECK(cos_dist == doctest::Approx(expected).epsilon(0.00001));
}

TEST_CASE("Cosine distance - error cases") {
    Vector v1({1.0, 1.0});
    Vector v2({0.0, 0.0});
    Vector v3({1.0, 1.0, 1.0});
    
    CHECK_THROWS_AS(v1.cosineDistance(v2), std::invalid_argument);  // Zero vector
    CHECK_THROWS_AS(v1.cosineDistance(v3), std::invalid_argument);  // Different dimensions
}

TEST_CASE("Distance calculation and sorting") {
    std::vector<Vector> vectors = {
        Vector({1.0, 1.0, 1.0}),
        Vector({1.0, 1.0, 0.0}),
        Vector({0.0, 1.0, 1.0})
    };
    
    std::vector<VectorPair> pairs = calculateAllPairwiseDistances(vectors);
    
    // Check that we get the expected number of pairs
    CHECK(pairs.size() == 6);  
    
    // Check that pairs are sorted by distance
    for (size_t i = 1; i < pairs.size(); ++i) {
        CHECK(pairs[i-1].distance <= pairs[i].distance);
    }
}

TEST_CASE("File reading") {
    std::string filename = "test_vectors.txt";
    std::ofstream outfile(filename);
    outfile << "1 1 1\n";
    outfile << "1 1 0\n";
    outfile << "0 1 1\n";
    outfile.close();
    
    std::vector<Vector> vectors = readVectorsFromFile(filename);
    
    CHECK(vectors.size() == 3);
    CHECK(vectors[0].getDimension() == 3);
    CHECK(vectors[1].getDimension() == 3);
    CHECK(vectors[2].getDimension() == 3);
    
    // Clean up
    std::remove(filename.c_str());
}