/**
 * @file buffer2.h
 * @brief This file contains the declaration of the Buffer2 class. It is used for project 4.
 * @class Buffer2
 * @brief A class to handle reading and writing of Zipcode2 objects to a file.
 * @details The Buffer2 class is responsible for reading and writing Zipcode2 objects to a file.
 *          It uses a buffer to store data read from the file, and provides methods to pack and unpack
 *          Zipcode2 objects into strings that can be written to or read from the file. The buffer size
 *          is configurable, and the class provides error handling for file operations. 
 */

#ifndef BUFFER2_H
#define BUFFER2_H

#include "zipcode2.h" ///< Include the Zipcode2 class
#include <fstream> ///< Include the fstream library for file input/output operations
#include <string>  ///< Include the string library for string manipulation
#include <vector>  ///< Include the vector library for dynamic array operations

/**
 * @class Buffer2
 * @brief A class to handle buffering operations for Zipcode2 objects.
 */
class Buffer2 {
public:
    /**
     * @brief Construct a new Buffer2 object.
     */
    Buffer2();

    /**
     * @brief Set the Record Count.
     * @param count The new record count.
     */
	void setRecordCount(int count);

    /**
     * @brief Get the Record Count.
     * @return int The current record count.
     */
	int getRecordCount() const;

    /**
     * @brief Get the Last Record Length.
     * @return int The length of the last record.
     */
	int getLastRecordLength() const { return lastRecordLength; };

    /**
     * @brief Write a Zipcode2 object to a file.
     * @param zipcode The Zipcode2 object to write.
     * @param outFile The output file stream.
     * @return int The number of characters written.
     */
    int Write(Zipcode2& zipcode, std::fstream& outFile);

    /**
     * @brief Read a Zipcode2 object from a file.
     * @param inFile The input file stream.
     * @param offset The offset in the file to start reading from. Default is -1, which means read from the current position.
     * @return Zipcode2 The Zipcode2 object read from the file.
     */
    Zipcode2 Read(std::fstream& inFile, int offset = -1);

    /**
     * @brief Read a Zipcode2 object from a file using a key.
     * @param inFile The input file stream.
     * @param key The key to use for reading.
     * @param offset The offset in the file to start reading from.
     * @return Zipcode2 The Zipcode2 object read from the file.
     */
    Zipcode2 DRead(std::fstream& inFile, const int key, int offset);

    /**
     * @brief Pack a Zipcode2 object into a string.
     * @param zipcode The Zipcode2 object to pack.
     * @return std::string The packed Zipcode2 object as a string.
     */
    std::string Pack(Zipcode2& zipcode);

    /**
     * @brief Unpack a Zipcode2 object from a string.
     * @param str The string to unpack.
     * @return Zipcode2 The unpacked Zipcode2 object.
     */
    Zipcode2 Unpack(std::string str);

    /**
     * @brief Print a Zipcode2 object.
     * @param zipcode The Zipcode2 object to print.
     */
    void Print(Zipcode2& zipcode);

    /**
     * @brief Flush the output buffer to a file.
     * @param outFile The output file stream.
     */
    void Flush(std::fstream& outFile);

    /**
     * @brief Calculate the record count in a file.
     * @param inFile The input file stream.
     * @return int The record count.
     */
	int CalculateRecordCount(std::fstream& inFile);

    /**
     * @brief Update the field order for packing/unpacking.
     * @param newOrder The new field order.
     */
	void UpdateFieldOrder(std::vector<std::string> newOrder);	
	
private:
    std::vector<std::string> obuffer;  ///< Output buffer.
    std::vector<std::string> ibuffer;  ///< Input buffer.
    std::streampos ibufferStartPos;  ///< Position in the file where the input buffer starts.
    std::streampos ibufferEndPos;  ///< Position in the file where the input buffer ends.
    size_t obufferIndex;  ///< Index into the output buffer.
    size_t ibufferIndex;  ///< Index into the input buffer.
    const size_t BUFFER_SIZE = 2000;  ///< Size of the buffer. Adjust this value as needed.
    int lastRecordLength;  ///< Length of the last record.
	int recordCount;  ///< Record count.
	std::vector<std::string> fieldOrder;  ///< Field order for packing/unpacking.
};

#endif
