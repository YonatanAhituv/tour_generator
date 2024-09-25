#include "support.h"

#include <string>

#include "geopoint.h"
bool splitStrByDelimiter(const std::string s, std::string& s1, std::string& s2,
                         const char delimiter) {
  for (int i = 0; i < s.length(); i++) {
    if (s[i] == delimiter) {
      s1 = s.substr(0, i);
      s2 = s.substr(i + 1);
      return true;
    }
  }
  s1 = s;
  return false;
}
std::string geoPointToString(const GeoPoint& point) {
  return point.sLatitude + " " + point.sLongitude;
}
GeoPoint stringToGeoPoint(const std::string& point) {
  std::string s1;
  std::string s2;
  splitStrByDelimiter(point, s1, s2, ' ');
  GeoPoint newPoint(s1, s2);
  return newPoint;
}
bool pointsAreEqual(const GeoPoint& g1, const GeoPoint& g2) {
  return g1.latitude == g2.latitude && g1.longitude == g2.longitude;
}