#include "geodb.h"

#include <fstream>

#include "geotools.h"
#include "support.h"
using namespace std;

GeoDatabase::Path GeoDatabase::stringToPath(const string& coordinate,
                                            int numOfSets,
                                            const char delimiter) const {
  Path convertedPath;
  string latitude;
  string longitude;
  string nextSection = coordinate;
  string* targetCoord = &latitude;
  GeoPoint* targetPoint = &convertedPath.start;
  for (int i = 1; i <= numOfSets * 2;
       i++) {  // There are 2 numbers per set, subtract one boundary
    splitStrByDelimiter(nextSection, *targetCoord, nextSection, delimiter);
    if (i % 2 == 0) {  // We've gathered a pair
      *targetPoint = GeoPoint(latitude, longitude);
      targetPoint = &convertedPath.end;  // Switch to end of path
      targetCoord = &latitude;
    } else {
      targetCoord = &longitude;
    }
  }
  return convertedPath;
}
string GeoDatabase::pathToString(const Path& path) {
  return path.start.sLatitude + " " + path.start.sLongitude + " " +
         path.end.sLatitude + " " + path.end.sLongitude;
}

void GeoDatabase::insertSegment(const string& name, const Path& path) {
  vector<GeoPoint>* intersection1 =
      m_segments.find(geoPointToString(path.start));
  vector<GeoPoint>* intersection2 = m_segments.find(geoPointToString(path.end));
  if (intersection1 == nullptr) {
    vector<GeoPoint> newVec;
    m_segments.insert(geoPointToString(path.start),
                      newVec);  // Insert intersection one
  }
  if (intersection2 == nullptr) {
    vector<GeoPoint> newVec;
    m_segments.insert(geoPointToString(path.end),
                      newVec);  // Insert intersection two
  }
  // Bidirectional connections
  m_streets.insert(pathToString(path), name);
  m_streets.insert(
      geoPointToString(path.end) + " " + geoPointToString(path.start), name);
  m_segments.find(geoPointToString(path.start))->push_back(path.end);
  m_segments.find(geoPointToString(path.end))->push_back(path.start);
}

bool GeoDatabase::load(const string& map_data_file) {
  ifstream mapData(map_data_file);
  if (!mapData) {
    return false;
  }
  string name;
  string poiName;
  string coordinates;
  string poiNumber;
  Path streetPath;
  GeoPoint mp;
  Path mpPath;
  for (int i = 0; mapData; i++) {
    switch (i) {
      case 0:  // Line 1: Street Name
        getline(mapData, name);
        break;
      case 1:  // Line 2: Start & End Geopoints of Segment
        getline(mapData, coordinates);
        streetPath = stringToPath(coordinates);
        insertSegment(name, streetPath);
        break;
      case 2:  // Line 3: POI
        getline(mapData, poiNumber);
        if (poiNumber != "0") {  // If we have any POI
          mp = midpoint(streetPath.start, streetPath.end);
          mpPath = Path(streetPath.start, mp);
          insertSegment(name, mpPath);
          mpPath = Path(mp, streetPath.end);
          insertSegment(name, mpPath);
          for (int i = 0; i < stoi(poiNumber); i++) {
            getline(mapData, poiName);
            splitStrByDelimiter(poiName, poiName, coordinates, '|');
            mpPath = Path(mp, stringToGeoPoint(coordinates));
            insertSegment("a path", mpPath);  // Add path to streets
            m_poi.insert(poiName,
                         mpPath);  // Ensure midpoint is connected to POI
          }
        }
        i = -1;  // Reset i after POI processed (will go back up to 0 on next
                 // run)
        break;
    }
  }
  return true;
}
bool GeoDatabase::get_poi_location(const string& poi, GeoPoint& point) const {
  const Path* poiPath = m_poi.find(poi);
  if (poiPath == nullptr) return false;  // No POI of name found
  point =
      poiPath->end;  // Get end of path from midpoint to poi (poi is endpoint)
  return true;
}
vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const {
  const vector<GeoPoint>* connectedSegments =
      m_segments.find(geoPointToString(pt));
  vector<GeoPoint> connectedPoints;
  if (connectedSegments == nullptr) {
    return connectedPoints;
  }
  for (int i = 0; i < connectedSegments->size(); i++) {
    connectedPoints.push_back(connectedSegments->at(i));
  }
  return connectedPoints;
}
string GeoDatabase::get_street_name(const GeoPoint& pt1,
                                    const GeoPoint& pt2) const {
  const string* streetName =
      m_streets.find(geoPointToString(pt1) + " " + geoPointToString(pt2));
  return streetName == nullptr ? "" : *streetName;
}