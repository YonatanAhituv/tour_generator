#include "tour_generator.h"

#include "geopoint.h"
#include "geotools.h"
using namespace std;
vector<TourCommand> TourGenerator::generate_tour(const Stops& stops) const {
  POI poi;
  POI prev_poi;
  stops.get_poi_data(0, prev_poi.name, prev_poi.commentary);
  prev_poi.getLocation(m_db);
  vector<TourCommand> instructions;
  vector<GeoPoint> route;
  TourCommand currentCommand;
  string newStreet;
  string street;
  string direction;
  double distance = 0;
  double angle;
  for (int i = 0; i < stops.size(); i++) {
    stops.get_poi_data(i, poi.name, poi.commentary);
    poi.getLocation(m_db);
    if (prev_poi != poi) {  // We were somewhere else before
      route = m_router->route(prev_poi.loc, poi.loc);
      for (int j = 1; j < route.size(); j++) {  // Start on 2nd point
        angle = angle_of_line(route[j - 1], route[j]);
        street = m_db->get_street_name(route[j - 1], route[j]);
        if (angle < 22.5) {
          direction = "east";
        } else if (angle < 67.5) {
          direction = "northeast";
        } else if (angle < 112.5) {
          direction = "north";
        } else if (angle < 157.5) {
          direction = "northwest";
        } else if (angle < 202.5) {
          direction = "west";
        } else if (angle < 247.5) {
          direction = "southwest";
        } else if (angle < 292.5) {
          direction = "south";
        } else if (angle < 337.5) {
          direction = "southeast";
        } else {
          direction = "east";
        }
        distance += distance_earth_miles(route[j - 1], route[j]);
        if (j + 1 == route.size()) {  // Last run
          currentCommand.init_proceed(direction, street, distance, route[j - 1],
                                      route[j]);
          instructions.push_back(currentCommand);
          distance = 0;
        }
        if (j + 1 < route.size()) {  // Not reading out of bounds
          newStreet = m_db->get_street_name(route[j], route[j + 1]);
          angle = angle_of_turn(route[j - 1], route[j], route[j + 1]);
          if (newStreet != street || angle >= 1) {
            currentCommand.init_proceed(direction, street, distance,
                                        route[j - 1], route[j]);
            instructions.push_back(currentCommand);
            distance = 0;
          }
          if (angle >= 1 && street != newStreet) {
            if (angle < 180) {
              currentCommand.init_turn("left", newStreet);
            } else if (angle <= 359) {
              currentCommand.init_turn("right", newStreet);
            }
            instructions.push_back(currentCommand);
          }
        }
      }
    }
    currentCommand.init_commentary(poi.name, poi.commentary);
    instructions.push_back(currentCommand);
    prev_poi = poi;
  }
  return instructions;
}