/**
 * @file buffer.cpp
 * @brief This file contains the implementation of the Buffer class. It is used for project 3.
 */

#include "buffer.h" ///< Include the Buffer class
#include <iostream> ///< Include the iostream library for input/output operations
#include <vector>  ///< Include the vector library for dynamic array operations

/**
 * @brief Construct a new Buffer object and open the file.
 * @param filename The name of the file to open.
 */
Buffer::Buffer(std::string filename) : buffer_index(0) {
    file.open(filename, std::ios::in | std::ios::out);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        exit(1);
    }
    
    // Skip the header line
    std::string header;
    std::getline(file, header);
    // Fill the buffer initially
    for (size_t i = 0; i < buffer_size && !file.eof(); ++i) {
        std::string line;
        std::getline(file, line);
        buffer.push_back(line);
    }
}

/**
 * @brief Destroy the Buffer object and close the file if it is open.
 */
Buffer::~Buffer() {
    if (file.is_open()) {
        file.close();
    }
}

/**
 * @brief Write a Zipcode object to the file.
 * @param zipcode The Zipcode object to write.
 */
void Buffer::Write(Zipcode& zipcode) {
    // If the file is not open, print an error message and return
    if (!file.is_open()) {
        std::cerr << "File is not open. Cannot write to file." << std::endl;
        return;
    }
    // Pack the Zipcode object into a string
    std::string packed = Pack(zipcode);
    // Write the packed string to the file
    file << packed << std::endl;
}

/**
 * @brief Read a Zipcode object from the file.
 * @return The Zipcode object read from the file.
 */
Zipcode Buffer::Read() {
    // If the buffer is empty, refill it
    if (buffer_index >= buffer.size()) {
        buffer.clear();
        buffer_index = 0;
        for (size_t i = 0; i < buffer_size; ++i) {
            std::string line;
            std::getline(file, line);
            if (line.empty()) {
                break;
            }
            buffer.push_back(line);
        }
    }
    // If the buffer is still empty after trying to refill it, we've reached the end of the file
    if (buffer.empty()) {
        return Zipcode();
    }
    // Unpack the current line in the buffer into a Zipcode object
    Zipcode result = Unpack(buffer[buffer_index]);
    // Safely increment buffer_index
    buffer_index++;
    // Return the unpacked Zipcode object
    return result;
}

/**
 * @brief Pack a Zipcode object into a string.
 * @param zipcode The Zipcode object to pack.
 * @return The packed string.
 */
std::string Buffer::Pack(Zipcode& zipcode) {
    return zipcode.Pack();
}

/**
 * @brief Unpack a string into a Zipcode object.
 * @param str The string to unpack.
 * @return The unpacked Zipcode object.
 */
Zipcode Buffer::Unpack(std::string str) {
    Zipcode zipcode;
    zipcode.Unpack(str);
    return zipcode;
}

/**
 * @brief Print the contents of a Zipcode object.
 * @param zipcode The Zipcode object to print.
 */
void Buffer::Print(Zipcode& zipcode) {
    std::cout << "Zip Code: " << zipcode.getZipcode() << std::endl;
    std::cout << "Place Name: " << zipcode.getPlaceName() << std::endl;
    std::cout << "State: " << zipcode.getState() << std::endl;
    std::cout << "County: " << zipcode.getCounty() << std::endl;
    std::cout << "Latitude: " << zipcode.getLat() << std::endl;
    std::cout << "Longitude: " << zipcode.getLon() << std::endl;
}