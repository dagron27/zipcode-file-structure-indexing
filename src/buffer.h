/**
 * @file buffer.h
 * @brief This file contains the declaration of the Buffer class. It is used for project 3.
 * @class Buffer
 * @brief A class to handle reading and writing of Zipcode objects to a file.
 * @details The Buffer class is responsible for reading and writing Zipcode objects to a file.
 *          It uses a buffer to store data read from the file, and provides methods to pack and unpack
 *          Zipcode objects into strings that can be written to or read from the file. The buffer size
 *          is configurable, and the class provides error handling for file operations. 
 */
 
#ifndef BUFFER_H
#define BUFFER_H

#include "zipcode.h" ///< Include the Zipcode class
#include <fstream> ///< Include the fstream library for file input/output operations
#include <string>  ///< Include the string library for string manipulation
#include <vector>  ///< Include the vector library for dynamic array operations

class Buffer {
public:
    /**
     * @brief Construct a new Buffer object.
     * @param filename The name of the file to read/write.
     */
    Buffer(std::string filename);

    /**
     * @brief Destroy the Buffer object.
     */
    ~Buffer();

    /**
     * @brief Write a Zipcode object to the file.
     * @param zipcode The Zipcode object to write.
     */
    void Write(Zipcode& zipcode);

    /**
     * @brief Read a Zipcode object from the file.
     * @return The Zipcode object read from the file.
     */
    Zipcode Read();

    /**
     * @brief Pack a Zipcode object into a string.
     * @param zipcode The Zipcode object to pack.
     * @return The packed string.
     */
    std::string Pack(Zipcode& zipcode);

    /**
     * @brief Unpack a Zipcode object from a string.
     * @param str The string to unpack.
     * @return The unpacked Zipcode object.
     */
    Zipcode Unpack(std::string str);

    /**
     * @brief Print a Zipcode object.
     * @param zipcode The Zipcode object to print.
     */
    void Print(Zipcode& zipcode);

private:
    std::fstream file; ///< The file to read/write.
    std::vector<std::string> buffer; ///< The buffer to hold the data.
    size_t buffer_index; ///< The current index in the buffer.
    size_t buffer_size = 2000; ///< The size of the buffer.
};

#endif