/**
 * @file zipcode2.cpp
 * @brief This file contains the implementation of the Zipcode2 class. It is used for project 4.
 */
 
#include "zipcode2.h" ///< Include the Zipcode2 class
#include <sstream>   ///< Include the stringstream library for string manipulation
#include <vector>    ///< Include the vector library for dynamic array operations

/**
 * @brief Construct a new Zipcode2 object.
 * 
 * Initializes all fields to their default values.
 */
Zipcode2::Zipcode2() {
    zipcode = 0;
    placeName = "";
    state = "";
    county = "";
    lat = 0.0;
    lon = 0.0;
}

/**
 * @brief Set the Zip Code.
 * @param newZipcode The new zipcode.
 */
void Zipcode2::setZipcode(int newZipcode) {
    zipcode = newZipcode;
}

/**
 * @brief Set the Place Name.
 * @param newPlaceName The new place name.
 */
void Zipcode2::setPlaceName(std::string newPlaceName) {
    placeName = newPlaceName;
}

/**
 * @brief Set the State.
 * @param newState The new state.
 */
void Zipcode2::setState(std::string newState) {
    state = newState;
}

/**
 * @brief Set the County.
 * @param newCounty The new county.
 */
void Zipcode2::setCounty(std::string newCounty) {
    county = newCounty;
}

/**
 * @brief Set the Latitude.
 * @param newLat The new latitude.
 */
void Zipcode2::setLat(double newLat) {
    lat = newLat;
}

/**
 * @brief Set the Longitude.
 * @param newLon The new longitude.
 */
void Zipcode2::setLon(double newLon) {
    lon = newLon;
}

/**
 * @brief Pack the Zipcode2 object into a string.
 * @param fieldOrder The order of the fields for packing.
 * @return std::string The packed Zipcode2 object as a string.
 */
std::string Zipcode2::Pack(const std::vector<std::string>& fieldOrder) {
    std::stringstream ss;
    for (const auto& field : fieldOrder) {
        if (field == "Zip Code") {
            ss << zipcode << ",";
        } else if (field == "Place Name") {
            ss << placeName << ",";
        } else if (field == "State") {
            ss << state << ",";
        } else if (field == "County") {
            ss << county << ",";
        } else if (field == "Lat") {
            ss << lat << ",";
        } else if (field == "Long") {
            ss << lon << ",";
        }
    }
    std::string result = ss.str();
    result.pop_back(); // remove the trailing comma
    return result;
}

/**
 * @brief Unpack a string into a Zipcode2 object.
 * @param str The string to unpack.
 */
void Zipcode2::Unpack(std::string str) {
    std::stringstream ss(str);
    ss >> zipcode;
    ss.ignore(1, ',');
    std::getline(ss, placeName, ',');
    std::getline(ss, state, ',');
    std::getline(ss, county, ',');
    ss >> lat;
    ss.ignore(1, ',');
    ss >> lon;
}

/**
 * @brief Print the Zipcode2 object.
 */
void Zipcode2::Print()
{
    std::cout << "Zip Code: " << zipcode << "\n";
    std::cout << "Place Name: " << placeName << "\n";
    std::cout << "State: " << state << "\n";
    std::cout << "County: " << county << "\n";
    std::cout << "Latitude: " << lat << "\n";
    std::cout << "Longitude: " << lon << "\n";
}

/**
 * @brief Check if all fields are in their default state.
 * @return bool True if all fields are in their default state, false otherwise.
 */
bool Zipcode2::isEmpty() const {
    return zipcode == 0 && placeName.empty() && state.empty() && county.empty() && lat == 0.0 && lon == 0.0;
}

