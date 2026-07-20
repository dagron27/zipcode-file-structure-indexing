/**
 * @file zipcode2.h
 * @brief This file contains the declaration of the Zipcode2 class. It is used for project 4.
 * @class Zipcode2
 * @brief A class for handling zipcode records with associated geographic information.
 * @details The Zipcode2 class provides methods to pack and unpack the data into a string for storage, 
 *          as well as getter and setter methods for each data field. This class is used in project 4.
 */
 
#ifndef ZIPCODE2_H
#define ZIPCODE2_H

#include <string> ///< Include the string library for string manipulation
#include <iostream> ///< Include the iostream library for input/output operations
#include <vector> ///< Include the vector library for dynamic array operations

class Zipcode2 {
public:
    /**
     * @brief Construct a new Zipcode2 object.
     */
    Zipcode2();

    /**
     * @brief Get the Zipcode.
     * @return int The zipcode.
     */
    int getZipcode() { return zipcode; };

    /**
     * @brief Get the Place Name.
     * @return std::string The place name.
     */
    std::string getPlaceName() { return placeName; };

    /**
     * @brief Get the State.
     * @return std::string The state.
     */
    std::string getState() { return state; };

    /**
     * @brief Get the County.
     * @return std::string The county.
     */
    std::string getCounty() { return county; };

    /**
     * @brief Get the Latitude.
     * @return double The latitude.
     */
    double getLat() { return lat; };

    /**
     * @brief Get the Longitude.
     * @return double The longitude.
     */
    double getLon() { return lon; };


    /**
     * @brief Set the Zipcode.
     * @param zipcode The new zipcode.
     */
    void setZipcode(int zipcode);

    /**
     * @brief Set the Place Name.
     * @param placeName The new place name.
     */
    void setPlaceName(std::string placeName);

    /**
     * @brief Set the State.
     * @param state The new state.
     */
    void setState(std::string state);

    /**
     * @brief Set the County.
     * @param county The new county.
     */
    void setCounty(std::string county);

    /**
     * @brief Set the Latitude.
     * @param lat The new latitude.
     */
    void setLat(double lat);

    /**
     * @brief Set the Longitude.
     * @param lon The new longitude.
     */
    void setLon(double lon);

   /**
     * @brief Pack the Zipcode2 object into a string.
     * @param fieldOrder The order of the fields for packing.
     * @return std::string The packed Zipcode2 object as a string.
     */
    std::string Pack(const std::vector<std::string>& fieldOrder);

    /**
     * @brief Unpack a string into a Zipcode2 object.
     * @param str The string to unpack.
     */
    void Unpack(std::string str);

    /**
     * @brief Print the Zipcode2 object.
     */
    void Print();

    /**
     * @brief Check if the Zipcode2 object is empty.
     * @return bool True if the Zipcode2 object is empty, false otherwise.
     */
    bool isEmpty() const;	
	
private:
    int zipcode;             ///< The zipcode.
    std::string placeName;   ///< The place name.
    std::string state;       ///< The state.
    std::string county;      ///< The county.
    double lat;              ///< The latitude.
    double lon;              ///< The longitude.
};

#endif