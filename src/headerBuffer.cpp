/** 
 * @file headerBuffer.cpp
 * @brief This file contains the implementation of the HeaderBuffer class.
 */

#include "headerBuffer.h" ///< Include the HeaderBuffer class for handling the header buffer
#include <iostream>       ///< Include the iostream library for input/output operations
#include <sstream>        ///< Include the stringstream library for string manipulation

/**
 * @brief Write the header to a file.
 * 
 * Converts the header record to a string, loads the header into the buffer, writes the buffer to the file, and returns the number of bytes written.
 * @param hr The HeaderRecord object to write.
 * @param file The output file stream.
 * @return int The number of bytes written, or -1 if an error occurred.
 */
int HeaderBuffer::WriteHeader(HeaderRecord& hr, std::fstream& file) {
    try {
        // Convert the header record to a string
        std::string header = hr.Pack();

        // Calculate the number of padding spaces needed
        int paddingSize = MAX_HEADER_SIZE - header.size() - 1; // Subtract 1 for the newline character

        // If the header is larger than the reserved space, print an error message and return
        if (paddingSize < 0) {
            std::cerr << "Error: Header is too large." << std::endl;
            return -1;
        }

        // Add padding spaces to the end of the header
        header.append(paddingSize, ' ');

        // Add the newline character to the end of the header
        header.append("\n");

        // Write the padded header to the file
        file << header;

        // Return the number of bytes written
        return header.size();
    } catch (const std::exception& e) {
        std::cerr << "Exception occurred in WriteHeader: " << e.what() << std::endl;
        return -1;  // Return an error code
    }
}

/**
 * @brief Read the header from a file.
 * 
 * Clears the buffer, reads the header from the file, unpacks the header into the HeaderRecord object, and checks for consistency.
 * @param hr The HeaderRecord object to read into.
 * @param file The input file stream.
 * @return bool True if the header was read successfully and is consistent, false otherwise.
 */
bool HeaderBuffer::ReadHeader(HeaderRecord& hr, std::fstream& file) {
    try {
        // Clear the buffer and read the header from the file
        buffer.clear();
        for (int i = 0; i < 16; ++i) {
            std::string line;
            std::getline(file, line);
            buffer.push_back(line);
        }

		// Read the header from the file
        std::string header;
        for (int i = 0; i < 16; ++i) {
            header += buffer[i] + "\n";
        }

        // Unpack the header into the HeaderRecord object
        hr.Unpack(header);

        // Check for consistency
        if (hr.getFileStructureType() == "ZCR" && hr.getVersion() == "1.0" && hr.getHeaderRecordSize() == 0 && hr.getFieldsPerRecord() == 6) {
            // The header is default (blank)
            return true;
        } else if (hr.getHeaderRecordSize() > 0 && hr.getFieldsPerRecord() == 6) {
            // The header is consistent
            return true;
        } else {
            std::cout << "FAILED TO READ HEADER" << std::endl;
            // The header is not consistent
            return false;
        }    
    } catch (const std::exception& e) {
        std::cerr << "Exception occurred in ReadHeader: " << e.what() << std::endl;
        return false;
    }
}
