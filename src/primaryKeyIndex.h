/**
 * @file primaryKeyIndex.h
 * @brief This file contains the declaration of the PrimaryKeyIndex class.
 * @class PrimaryKeyIndex
 * @brief An index based on the primary key of the zipcode records.
 * @details This class is defined in project 3.
 */


#ifndef PRIMARY_KEY_INDEX_H
#define PRIMARY_KEY_INDEX_H

#include "headerRecord.h"  ///< Include the HeaderRecord class for handling the header record
#include <fstream>         ///< Include the fstream library for file handling operations
#include <map>             ///< Include the map library for storing the primary key index

/**
 * @class PrimaryKeyIndex
 * @brief A class to handle the primary key index.
 */
class PrimaryKeyIndex {
public:
    /**
     * @brief Construct a new PrimaryKeyIndex object.
     */
    PrimaryKeyIndex();

    /**
     * @brief Destroy the PrimaryKeyIndex object.
     */
    ~PrimaryKeyIndex();

    /**
     * @brief Create an index from a file.
     * @param inFile The input file stream.
     * @return bool True if the index was created successfully, false otherwise.
     */
    bool createIndex(std::fstream& inFile);

    /**
     * @brief Write the index to a file.
     * @param outFile The output file stream.
     */
    void writeIndexToFile(std::fstream& outFile);

    /**
     * @brief Read the index from a file.
     * @param inFile The input file stream.
     */
    void readIndexFromFile(std::fstream& inFile);

    /**
     * @brief Search the index for a zipcode.
     * @param zipcode The zipcode to search for.
     * @return int The index of the zipcode if found, -1 otherwise.
     */
    int searchIndex(int zipcode);

    /**
     * @brief Set the HeaderRecord objects for the data and index files.
     * @param dataHR The HeaderRecord object for the data file.
     * @param indexHR The HeaderRecord object for the index file.
     */
	void setHeaderRecord(HeaderRecord& dataHR, HeaderRecord& indexHR);
	
private:
    std::fstream indexFile;       ///< The index file stream.
    std::map<int, int> intIndex;  ///< The primary key index.
    int headerOffset = 450;       ///< The offset of the header in the file. This is used to skip the header when reading the records.
};

#endif // PRIMARY_KEY_INDEX_H