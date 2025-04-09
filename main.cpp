#include "CosineDistance.h"
#include <iostream>

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }
    
    std::string filename = argv[1];
    
    try {

        std::vector<Vector> vectors = readVectorsFromFile(filename);
        
        if (vectors.empty()) {
            std::cerr << "Error: No vectors found in the file\n";
            return 1;
        }
        
        std::cout << "Read " << vectors.size() << " vectors of dimension " 
                  << vectors[0].getDimension() << " from " << filename << std::endl;
        
        // Calculate pairwise distances
        std::vector<VectorPair> pairs = calculateAllPairwiseDistances(vectors);
        
        // Output sorted pairs
        std::cout << "\nSorted by cosine distance:\n";
        for (const auto& pair : pairs) {
            std::cout << pair.index1 << " " << pair.index2 << " cos dist = " 
                      << pair.distance << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}