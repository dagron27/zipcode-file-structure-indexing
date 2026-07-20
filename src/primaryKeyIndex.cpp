/** 
 * @file primaryKeyIndex.cpp
 * @brief This file contains the implementation of the PrimaryKeyIndex class.
 */

#include "primaryKeyIndex.h" ///< Include the PrimaryKeyIndex class for handling the primary key index
#include "zipcode2.h"        ///< Include the Zipcode2 class for handling zipcode data
#include "headerBuffer.h"    ///< Include the HeaderBuffer class for handling the header buffer
#include "headerRecord.h"    ///< Include the HeaderRecord class for handling the header record
#include "buffer2.h"         ///< Include the Buffer2 class for handling the data buffer
#include <sstream>           ///< Include the stringstream library for string manipulation
/**
 * @brief Construct a new PrimaryKeyIndex object.
 */
PrimaryKeyIndex::PrimaryKeyIndex() {

}

/**
 * @brief Destroy the PrimaryKeyIndex object.
 * 
 * Closes the index file if it is open.
 */
PrimaryKeyIndex::~PrimaryKeyIndex() {
    if (indexFile.is_open()) {
        indexFile.close();
    }
}

/**
 * @brief Create an index from a file.
 * 
 * Reads the header from the input file, updates the field order, opens the index file, generates keys, writes the header to the index file, and writes the index to the index file.
 * @param inFile The input file stream.
 * @return bool True if the index was created successfully, false otherwise.
 */
bool PrimaryKeyIndex::createIndex(std::fstream& inFile) {
    // Check if inFile is open
    if (!inFile.is_open()) {
        std::cerr << "Input file is not open." << std::endl;
        return false;
    }
	HeaderBuffer hb;
	HeaderRecord dataHeader;
	Buffer2 buf;
	if (!hb.ReadHeader(dataHeader, inFile)) {
		std::cerr << "Failed to read header from file2." << std::endl;
		return false;
	}
    buf.UpdateFieldOrder(dataHeader.getFieldOrder());

	// Open File. dataHeader.getIndexFileName() is a bare filename (e.g.
	// "us_postal_codes.idx") persisted inside the .zcr's own header
	// record, not a path -- prefix with "data/" to match where this
	// codebase's data files live.
	std::string indexFileName = "data/" + dataHeader.getIndexFileName();
	std::fstream indexFile(indexFileName, std::ios::out | std::ios::trunc);
	if (!indexFile) {
		std::cerr << "Failed to open index file: " << indexFileName << std::endl;
		return false;
	}

	//Check key type
	std::string type = dataHeader.getFieldType(dataHeader.getPrimaryKey());
	// Initialize offset with the size of the header
	int offset = dataHeader.getHeaderRecordSize();

	Zipcode2 zc;
	// Generate keys
	if (type == "Int") {
		// Use intIndex
		while ((zc = buf.Read(inFile)), !zc.isEmpty()) {
			int key = zc.getZipcode();  // Generate the key
			intIndex[key] = offset;  // Store the current offset
			offset += (buf.getLastRecordLength()+4);  // Update the offset, +4 accounts for indicator and delimiter (i.e. "42 .......\n")
		}
	} else {
	// Handle error: unknown type
	}

	HeaderRecord indexHeader(2);
	indexHeader.setHeaderRecordSize(hb.getMaxHeaderSize());
	setHeaderRecord(dataHeader, indexHeader);

	if (!hb.WriteHeader(indexHeader, indexFile)) {
		std::cerr << "Failed to write header to index file: " << indexFileName << std::endl;
		return false;
	}

	writeIndexToFile(indexFile);
		
	intIndex.clear();
    if (indexFile.is_open()) {
        indexFile.close();
    }

	return true;
}

/**
 * @brief Set the HeaderRecord objects for the data and index files.
 * 
 * Updates the fields of the index HeaderRecord based on the data HeaderRecord.
 * @param dataHR The HeaderRecord object for the data file.
 * @param indexHR The HeaderRecord object for the index file.
 */
void PrimaryKeyIndex::setHeaderRecord(HeaderRecord& dataHR, HeaderRecord& indexHR)
{
	indexHR.setFileStructureType("IDX");
	headerOffset = dataHR.getHeaderRecordSize();
	indexHR.setIndexFileName(dataHR.getIndexFileName());
	indexHR.setIndexFileSchemaInfo(dataHR.getIndexFileSchemaInfo());
	indexHR.setRecordCount(dataHR.getRecordCount());
	indexHR.setField(0, "Key", "Int");
	indexHR.setField(1, "Offset", "Int");
	// Other fields are default values
}

/**
 * @brief Write the index to a file.
 * 
 * Writes each entry in the index to the output file.
 * @param outFile The output file stream.
 */
void PrimaryKeyIndex::writeIndexToFile(std::fstream& outFile) {
    for (const auto& entry : intIndex) {
        outFile << entry.first << "," << entry.second << "\n";
    }
}

/**
 * @brief Read the index from a file.
 * 
 * Reads each line from the input file and adds it to the index.
 * @param inFile The input file stream.
 */
void PrimaryKeyIndex::readIndexFromFile(std::fstream& inFile) {
    // Check if inFile is open
    if (!inFile.is_open()) {
        std::cerr << "Input file is not open." << std::endl;
        return;
    }

    // Skip the header
    //int headerOffset = 450;  // Replace with the actual header size
    inFile.seekg(headerOffset, std::ios::beg);
	
    std::string line;
    while (std::getline(inFile, line)) {
        // Convert the line to a stringstream for easier parsing
        std::stringstream ss(line);
        int key, value;
        char comma;

        if (ss >> key >> comma >> value) {
            intIndex[key] = value;

        } else {
            std::cerr << "Error reading from file." << std::endl;
            break;
        }
    }
}

/**
 * @brief Search the index for a zipcode.
 * 
 * Searches the index for the given zipcode and returns the corresponding offset if found.
 * @param zipcode The zipcode to search for.
 * @return int The offset of the zipcode if found, -1 otherwise.
 */
int PrimaryKeyIndex::searchIndex(int zipcode) {
    auto it = intIndex.find(zipcode);
    if (it != intIndex.end()) {
		return it->second;
    } else {
        return -1;  // Return -1 if the zipcode is not found
    }
}