/** 
 * @file proj3Updated.cpp
 * @brief A program to analyze geographical data from a ZCR file and display extreme zip codes for each state.
 * 
 * This program reads geographical data from a ZCR file, processes it to find extreme zip codes (easternmost, westernmost, northernmost, and southernmost) for each state,
 * and displays the results in a formatted table. It utilizes the RecordBuffer class to store and retrieve records, and various helper functions for data processing
 * and output formatting.
 * 
 * @author Daniel Leone
 * @date 04/05/2024
 */

#include "zipcode2.h" 	///< Include the Zipcode2 class
#include "buffer2.h"  	///< Include the Buffer2 class
#include <fstream>  	///< Include the fstream library for file input/output operations
#include <iostream>  	///< Include the iostream library for input/output operations
#include <iomanip>   	///< Include the iomanip library for output formatting
#include <map>       	///< Include the map library for storing the extreme coordinates

// Function prototypes
void processData(Buffer2& buffer, std::fstream& file, std::map<std::string, double>& easternmost, std::map<std::string, double>& westernmost, std::map<std::string, double>& northernmost, std::map<std::string, double>& southernmost);
void printData(const std::map<std::string, double>& easternmost, const std::map<std::string, double>& westernmost, const std::map<std::string, double>& northernmost, const std::map<std::string, double>& southernmost);

/**
 * @brief The main function of the program.
 * @return 0 if the program executed successfully.
 */
int main() {
    // Ask the user for the input file name
    std::string inputFileName;
    std::cout << "Enter the name of the ZCR file to be opened: ";
    std::getline(std::cin, inputFileName);

    std::fstream file(inputFileName, std::ios::in | std::ios::out);
    // Create a Buffer2 object for your ZCR file
    Buffer2 buffer;

    // Create maps to hold the extreme coordinates for each state
    std::map<std::string, double> easternmost;
    std::map<std::string, double> westernmost;
    std::map<std::string, double> northernmost;
    std::map<std::string, double> southernmost;

    // Process the data
    processData(buffer, file, easternmost, westernmost, northernmost, southernmost);

    // Print the data
    printData(easternmost, westernmost, northernmost, southernmost);

    return 0;
}

/**
 * @brief Function to read and process the data from the file.
 * @param buffer The Buffer2 object.
 * @param easternmost, westernmost, northernmost, southernmost Maps to hold the extreme coordinates for each state.
 */
void processData(Buffer2& buffer, std::fstream& file, std::map<std::string, double>& easternmost, std::map<std::string, double>& westernmost, std::map<std::string, double>& northernmost, std::map<std::string, double>& southernmost) {
    // Read each line from the file
    Zipcode2 zipcode;
    while ((zipcode = buffer.Read(file)), zipcode.getZipcode() != 0) {
        // Get the current state
        std::string state = zipcode.getState();

        // Update the extreme coordinates for this state
        if (easternmost.find(state) == easternmost.end() || zipcode.getLon() < easternmost[state]) {
            easternmost[state] = zipcode.getLon();
        }
        if (westernmost.find(state) == westernmost.end() || zipcode.getLon() > westernmost[state]) {
            westernmost[state] = zipcode.getLon();
        }
        if (northernmost.find(state) == northernmost.end() || zipcode.getLat() > northernmost[state]) {
            northernmost[state] = zipcode.getLat();
        }
        if (southernmost.find(state) == southernmost.end() || zipcode.getLat() < southernmost[state]) {
            southernmost[state] = zipcode.getLat();
        }
    }
}

/**
 * @brief Function to print the extreme coordinates for each state.
 * @param easternmost, westernmost, northernmost, southernmost Maps holding the extreme coordinates for each state.
 */
void printData(const std::map<std::string, double>& easternmost, const std::map<std::string, double>& westernmost, const std::map<std::string, double>& northernmost, const std::map<std::string, double>& southernmost) {
    // Print the header row
    std::cout << std::left << std::setw(6) << "State"
              << std::right << std::setw(15) << "Easternmost"
              << std::setw(15) << "Westernmost"
              << std::setw(15) << "Northernmost"
              << std::setw(15) << "Southernmost\n";

    // Print the extreme coordinates for each state
    for (const auto& pair : easternmost) {
        std::string state = pair.first;
        std::cout << std::left << std::setw(6) << state
                  << std::right << std::setw(13) << std::fixed << std::setprecision(4) << easternmost.at(state)
                  << std::setw(15) << std::fixed << std::setprecision(4) << westernmost.at(state)
                  << std::setw(15) << std::fixed << std::setprecision(4) << northernmost.at(state)
                  << std::setw(14) << std::fixed << std::setprecision(4) << southernmost.at(state) << "\n";
    }
}
