#ifndef SUPPORT_H
#define SUPPORT_H
#include <string>

#include "geopoint.h"
bool splitStrByDelimiter(const std::string s, std::string& s1, std::string& s2,
                         const char delimiter);
std::string geoPointToString(const GeoPoint& point);
GeoPoint stringToGeoPoint(const std::string& point);
bool pointsAreEqual(const GeoPoint& g1, const GeoPoint& g2);
#endif