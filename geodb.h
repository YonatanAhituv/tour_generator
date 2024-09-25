#ifndef GEODB_H
#define GEODB_H
#include <vector>

#include "base_classes.h"
#include "geopoint.h"
#include "hashmap.h"
class GeoDatabase : public GeoDatabaseBase {
 public:
  GeoDatabase(){};
  virtual ~GeoDatabase(){};
  virtual bool load(const std::string& map_data_file);
  virtual bool get_poi_location(const std::string& poi, GeoPoint& point) const;
  virtual std::vector<GeoPoint> get_connected_points(const GeoPoint& pt) const;
  virtual std::string get_street_name(const GeoPoint& pt1,
                                      const GeoPoint& pt2) const;

 private:
  struct Path {
    Path(){};
    Path(GeoPoint startPoint, GeoPoint endPoint)
        : start(startPoint.sLatitude, startPoint.sLongitude),
          end(endPoint.sLatitude, endPoint.sLongitude){};
    GeoPoint start;
    GeoPoint end;
  };

  void insertSegment(const std::string& name, const Path& path);
  std::string pathToString(const Path& path);
  Path stringToPath(const std::string& coordinate, int numOfSets = 2,
                    const char delimiter = ' ') const;
  HashMap<std::vector<GeoPoint>> m_segments;
  HashMap<std::string> m_streets;
  HashMap<Path> m_poi;
};
#endif