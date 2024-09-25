#ifndef ROUTER_H
#define ROUTER_H
#include "base_classes.h"
#include "hashmap.h"
class Router : public RouterBase {
 public:
  Router(const GeoDatabaseBase& geo_db);
  virtual ~Router();
  virtual std::vector<GeoPoint> route(const GeoPoint& pt1,
                                      const GeoPoint& pt2) const;

 private:
  const GeoDatabaseBase* m_db;
  struct pointVisited : GeoPoint {
    pointVisited(){};
    pointVisited(const GeoPoint& g1, double weightGiven)
        : GeoPoint(g1.sLatitude, g1.sLongitude) {
      weight = weightGiven;
    };
    double weight;
    bool operator<(const pointVisited& other) const {
      if (weight > other.weight) {  // Invert to create min heap
        return true;
      }
      return false;
    }
  };
  std::vector<GeoPoint> mapToVector(const HashMap<GeoPoint>& path,
                                    const GeoPoint& start,
                                    const GeoPoint& goal) const;
  double h(const GeoPoint& start, const GeoPoint& end) const;
};
#endif