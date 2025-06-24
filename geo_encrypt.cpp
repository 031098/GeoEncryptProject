#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

// Simple XOR encryption/decryption
std::string xorCipher(const std::string &data, char key) {
    std::string result = data;
    for (char &c : result) {
        c ^= key;
    }
    return result;
}

// Store encrypted file
void storeFile() {
    std::string country, state, district, description;
    std::cout << "Enter Country: ";
    std::getline(std::cin, country);
    std::cout << "Enter State: ";
    std::getline(std::cin, state);
    std::cout << "Enter District: ";
    std::getline(std::cin, district);
    std::cout << "Enter Description: ";
    std::getline(std::cin, description);

    // Construct folder path
    fs::path dirPath = fs::path("data") / country / state / district;

    // Create directories if they don't exist
    try {
        fs::create_directories(dirPath);
    } catch (const fs::filesystem_error &e) {
        std::cerr << "Error creating directories: " << e.what() << "\n";
        return;
    }

    // Build file content
    std::string fileContent = 
        "Country: " + country + "\n" +
        "State: " + state + "\n" +
        "District: " + district + "\n" +
        "Description: " + description + "\n";

    // Encrypt content
    std::string encrypted = xorCipher(fileContent, 'K'); // 'K' is the key

    // Save to file
    fs::path filePath = dirPath / "info.dat";
    std::ofstream outFile(filePath, std::ios::binary);
    if (!outFile) {
        std::cerr << "Failed to open file for writing.\n";
        return;
    }
    outFile << encrypted;
    outFile.close();

    std::cout << "File stored successfully at: " << filePath << "\n";
}

// Retrieve and decrypt file
void retrieveFile() {
    std::string country, state, district;
    std::cout << "Enter Country: ";
    std::getline(std::cin, country);
    std::cout << "Enter State: ";
    std::getline(std::cin, state);
    std::cout << "Enter District: ";
    std::getline(std::cin, district);

    // Construct file path
    fs::path filePath = fs::path("data") / country / state / district / "info.dat";

    // Check if file exists
    if (!fs::exists(filePath)) {
        std::cerr << "Error: File does not exist at: " << filePath << "\n";
        return;
    }

    // Read encrypted content
    std::ifstream inFile(filePath, std::ios::binary);
    if (!inFile) {
        std::cerr << "Failed to open file for reading.\n";
        return;
    }
    std::string encrypted((std::istreambuf_iterator<char>(inFile)),
                           std::istreambuf_iterator<char>());
    inFile.close();

    // Decrypt
    std::string decrypted = xorCipher(encrypted, 'K');

    std::cout << "Decrypted content:\n";
    std::cout << "-----------------------------\n";
    std::cout << decrypted << "\n";
}

int main() {
    while (true) {
        std::cout << "\n--- Encrypted Geo File Manager ---\n";
        std::cout << "1. Store a new file\n";
        std::cout << "2. Retrieve and view a file\n";
        std::cout << "3. Exit\n";
        std::cout << "Select an option (1-3): ";
        
        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            storeFile();
        } else if (choice == "2") {
            retrieveFile();
        } else if (choice == "3") {
            std::cout << "Exiting...\n";
            break;
        } else {
            std::cout << "Invalid option. Try again.\n";
        }
    }

    return 0;
}