/**
 * @file headerBuffer.h
 * @brief This file contains the declaration of the HeaderBuffer class.
 * @class HeaderBuffer
 * @brief A class for managing the header of a ZRC file and associated IDX.
 * @details The HeaderBuffer class is used to read and write the header of a data file. It holds the contents of the header in a buffer.
 */

#ifndef HEADER_BUFFER_H
#define HEADER_BUFFER_H

#include "headerRecord.h" ///< Include the HeaderRecord class for handling the header record
#include <fstream>        ///< Include the fstream library for file handling operations
#include <vector>         ///< Include the vector library for dynamic array operations

/**
 * @class HeaderBuffer
 * @brief A class to handle the header buffer.
 */
class HeaderBuffer {
public:
    /**
     * @brief Write the header to a file.
     * @param hr The HeaderRecord object to write.
     * @param outFile The output file stream.
     * @return int The number of characters written.
     */
    int WriteHeader(HeaderRecord& hr, std::fstream& outFile);

    /**
     * @brief Read the header from a file.
     * @param hr The HeaderRecord object to read into.
     * @param inFile The input file stream.
     * @return bool True if the header was read successfully, false otherwise.
     */
    bool ReadHeader(HeaderRecord& hr, std::fstream& inFile);

    /**
     * @brief Get the maximum header size.
     * @return int The maximum header size.
     */
	 int getMaxHeaderSize() const { return MAX_HEADER_SIZE; };

private:
    std::vector<std::string> buffer;  ///< The buffer to hold the contents of the header.
    const int MAX_HEADER_SIZE = 450;  ///< The maximum size of the header.
};

#endif // HEADER_BUFFER_H