#ifndef TOUR_GENERATOR_H
#define TOUR_GENERATOR_H
#include <vector>

#include "base_classes.h"
#include "geopoint.h"
#include "tourcmd.h"
class TourGenerator : public TourGeneratorBase {
 public:
  TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router) {
    m_db = &geodb;
    m_router = &router;
  };
  virtual ~TourGenerator(){};
  virtual std::vector<TourCommand> generate_tour(const Stops& stops) const;

 private:
  const GeoDatabaseBase* m_db;
  const RouterBase* m_router;
  struct POI {
    std::string name;
    std::string commentary;
    GeoPoint loc;
    void getLocation(const GeoDatabaseBase* m_db) {
      m_db->get_poi_location(name, loc);
    }
    bool operator==(const POI& other) const { return name == other.name; }
    bool operator!=(const POI& other) const { return name != other.name; }
  };
};
#endif