#include "router.h"

#include <list>
#include <queue>

#include "geotools.h"
#include "hashmap.h"
#include "support.h"
using namespace std;
Router::Router(const GeoDatabaseBase& geo_db) { m_db = &geo_db; }
Router::~Router() {}

vector<GeoPoint> Router::mapToVector(const HashMap<GeoPoint>& path,
                                     const GeoPoint& start,
                                     const GeoPoint& goal) const {
  list<GeoPoint> points;
  points.push_front(goal);
  GeoPoint currentPoint = goal;
  while (!pointsAreEqual(currentPoint, start)) {
    currentPoint = *(path.find(geoPointToString(currentPoint)));
    points.push_front(currentPoint);
  }
  vector<GeoPoint> output(points.begin(), points.end());  // Convert to vector
  return output;
}
// vector<GeoPoint> Router::route(const GeoPoint& pt1,
//                                const GeoPoint& pt2) const {  // A*
//   priority_queue<pointVisited> fringe;
//   pointVisited currentPoint =
//       pointVisited(pt1, 0,
//                    0);  // Seed algorithm, start node is 0 from start node,
//                         // heuristic doesn't matter here
//   pointVisited prevPoint;
//   vector<GeoPoint> connectedPoints;
//   HashMap<bool> pointHit;
//   HashMap<GeoPoint> pathTaken;
//   HashMap<double> distances;
//   fringe.push(currentPoint);
//   while (!pointsAreEqual(currentPoint, pt2) && !fringe.empty()) {
//     prevPoint = currentPoint;
//     while (pointHit.find(geoPointToString(currentPoint)) != nullptr) {
//       currentPoint = fringe.top();
//       fringe.pop();
//     }
//     pointHit.insert(geoPointToString(currentPoint), true);
//     if (!pointsAreEqual(currentPoint,
//                         prevPoint)) {  // Make bidirectional connection
//       pathTaken[geoPointToString(currentPoint)] = prevPoint;
//     }
//     cerr << "Weight: " << currentPoint.weight
//          << " Distance from start node: " << currentPoint.distance
//          << " Lat & Long: " << currentPoint.sLatitude << " "
//          << currentPoint.sLongitude << endl;
//     connectedPoints = m_db->get_connected_points(currentPoint);
//     for (int i = 0; i < connectedPoints.size();
//          i++) {  // Iterate over neightbors
//       if (pathTaken.find(geoPointToString(connectedPoints[i])) ==
//           nullptr) {  // Did not visit this point from here as it is not on
//                       // the path
//         fringe.push(
//             pointVisited(connectedPoints[i],
//                          distance_earth_km(currentPoint, connectedPoints[i])
//                          +
//                              currentPoint.distance,
//                          distance_earth_km(connectedPoints[i], pt2)));
//       }
//     }
//   }
//   return mapToVector(pathTaken, pt1, pt2);
// }

double Router::h(const GeoPoint& start, const GeoPoint& end) const {
  return distance_earth_miles(start, end);
}
vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const {
  priority_queue<pointVisited> fringe;

  HashMap<GeoPoint> cameFrom;
  HashMap<double> gScore;
  gScore[geoPointToString(pt1)] = 0;

  HashMap<double> fScore;
  fScore[geoPointToString(pt1)] = h(pt1, pt2);
  fringe.push(pointVisited(pt1, fScore[geoPointToString(pt1)]));
  GeoPoint currentPoint;
  vector<GeoPoint> connectedPoints;
  double possibleGScore;
  while (!fringe.empty()) {
    currentPoint = fringe.top();
    fringe.pop();
    if (pointsAreEqual(currentPoint, pt2)) {
      return mapToVector(cameFrom, pt1, pt2);
    }
    connectedPoints = m_db->get_connected_points(currentPoint);
    for (int i = 0; i < connectedPoints.size(); i++) {
      possibleGScore = gScore[geoPointToString(currentPoint)] +
                       distance_earth_miles(currentPoint, connectedPoints[i]);
      if (gScore.find(geoPointToString(connectedPoints[i])) == nullptr ||
          possibleGScore < gScore[geoPointToString(connectedPoints[i])]) {
        cameFrom[geoPointToString(connectedPoints[i])] = currentPoint;
        gScore[geoPointToString(connectedPoints[i])] = possibleGScore;
        fScore[geoPointToString(connectedPoints[i])] =
            possibleGScore + h(connectedPoints[i], pt2);
        fringe.push(pointVisited(connectedPoints[i],
                                 fScore[geoPointToString(connectedPoints[i])]));
      }
    }
  }
  return vector<GeoPoint>();  // empty
}