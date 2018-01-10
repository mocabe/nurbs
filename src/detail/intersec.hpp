#pragma once

#include "split.hpp"
#include <cmath>
#include <limits>
#include <stack>
namespace nurbs {

template <class T, class K>
template <class PtTp, class>
void NURBS<T, K>::intersec(
  const point_type& origin,
  const point_type& direction,
  std::vector<point_type>& result) {

  // NaN
  static constexpr double nan = std::numeric_limits<double>::quiet_NaN();

  //////  Helpers  //////

  // Convex hull edge (with intersection info)
  struct hullEdge {
    // Ctor
    hullEdge(size_t index, bool tt) : idx{index}, t{tt} {}
    // index of point
    size_t idx;
    // flag
    bool t;
  };

  // calculate signed distance between line and point
  static auto lineDistance =
    [](const point_type& org, const point_type& dir, const auto& p) {
      point_type q{get<0>(p) - get<0>(org), get<1>(p) - get<1>(org)};
      // dir ^ q
      auto c = get<0>(dir) * get<1>(q) - get<1>(dir) * get<0>(q);
      auto div = std::sqrt(dot(dir, dir));
      assert(div != 0);
      return c / div;
    };

  // is cur->nxt upward? (in Bezier Clipping coordinate)
  static auto is_upward =
    [](
      const std::vector<double>& distances, // distance list
      size_t o,                             // index of origin
      size_t cur,                           // index of cur
      size_t nxt                            // index of nxt
    ) {
      std::pair<double, double> pcur{(double)cur / (distances.size() - 1), distances[cur]};
      std::pair<double, double> pnxt{(double)nxt / (distances.size() - 1), distances[nxt]};
      std::pair<double, double> po{(double)o / (distances.size() - 1), distances[o]};
      // cross
      auto c = (pcur.first - po.first) * (pnxt.second - po.second) -
               (pcur.second - po.second) * (pnxt.first - po.first);
      if (c >= 0) return true;
      return false;
    };

  // convex hull class
  class convexHull {
  public:
    convexHull(
      const NURBS& nurbs, const std::vector<double>& ds, size_t bgn, size_t end)
      : distances{ds} {

      upperHull.reserve(nurbs.degree());
      lowerHull.reserve(nurbs.degree());
      zero_knot_range =
        nurbs.knot_range().second - nurbs.knot_range().first < 1e-8;

      assert(bgn < end);
      assert(distances.size() > end);
      // init
      upperHull.emplace_back(bgn);
      lowerHull.emplace_back(bgn);

      for (size_t index = bgn + 1; index <= end; ++index) {
        assert(upperHull.back() == lowerHull.back());
        // update upper hull
        for (size_t i = upperHull.size() - 1; 0 < i; --i) {
          if (is_upward(distances, index, upperHull[i], upperHull[i - 1])) {
            upperHull.pop_back();
          } else
            break;
        }
        upperHull.emplace_back(index);
        // update lower hull
        for (size_t i = lowerHull.size() - 1; 0 < i; --i) {
          if (!is_upward(distances, index, lowerHull[i], lowerHull[i - 1])) {
            lowerHull.pop_back();
          } else
            break;
        }
        lowerHull.emplace_back(index);
      }
    }

    convexHull() = delete;
    convexHull(const convexHull&) = delete;
    auto calcIntersecs(double v) const {
      std::vector<std::pair<std::vector<size_t>::const_iterator, double>> ret;
      for (size_t i = 0; i < upperHull.size() - 1; ++i) {
        auto tIntersec = calcIntersec(upperHull[i], upperHull[i + 1], v);
        if (!std::isnan(tIntersec)) {
          ret.emplace_back(upperHull.begin() + i, tIntersec);
        }
      }
      for (size_t i = 0; i < lowerHull.size() - 1; ++i) {
        auto tIntersec = calcIntersec(lowerHull[i], lowerHull[i + 1], v);
        if (!std::isnan(tIntersec)) {
          ret.emplace_back(lowerHull.begin() + i, tIntersec);
        }
      }
      for (auto&& r : ret)
        if (!std::isnan(r.second))
          if (zero_knot_range) r.second = 0;
      return ret;
    }

  private:
    double calcIntersec(size_t bgn, size_t end, double v) const {
      if (end < bgn) std::swap(bgn, end);

      double dbgn = distances[bgn] - v;
      double dend = distances[end] - v;

      if (dbgn == 0) {
        return (double)bgn / (distances.size() - 1);
      } else if (dend == 0) {
        return (double)end / (distances.size() - 1);
      } else {
        if (std::signbit(dbgn) != std::signbit(dend)) {
          double len = std::abs(dbgn) + std::abs(dend);
          double t = std::abs(dbgn) / len;
          assert(0 <= (t * (end - bgn) + bgn) / (distances.size() - 1));
          assert((t * (end - bgn) + bgn) / (distances.size() - 1) <= 1);
          return (t * (end - bgn) + bgn) / (distances.size() - 1);
        } else
          return nan;
      }
      return nan;
    }
    const std::vector<double>& distances;
    std::vector<size_t> upperHull, lowerHull;
    bool zero_knot_range;
  };

  // non recursive impl
  struct strd_nurbs_clipper_nonrecursive {
    static void clip(
      NURBS&& nurbs,
      point_type origin,
      point_type direction,
      std::vector<point_type>& result) {

    // minimum clipping error
#if defined(NURBS_INTERSEC_ERROR)
      static constexpr double error = NURBS_INTERSEC_ERROR;
#else
      static constexpr double error = 1e-7;
#endif

      // curve stack
      std::stack<NURBS> curves;
      curves.push(std::move(nurbs));

      // distance list
      std::vector<double> distances;

      while (!curves.empty()) {
        if constexpr (debug_msg_level >= 2)
          std::cerr << "Stack size: " << curves.size() << std::endl;
        // pop first element
        auto curve = std::move(curves.top());
        curves.pop();

        if constexpr (debug_msg_level >= 2) {
          std::cerr << "------- Starting NURBS Clipping" << std::endl;
          std::cerr << "Degree= " << curve.degree_ << std::endl;
          std::cerr << "Points:" << std::endl;
          for (auto&& p : curve.points_)
            std::cerr << "  {" << p.x << "," << p.y << "," << p.z << "}\n";
          std::cerr << "Knots: " << std::endl;
          for (auto&& d : curve.knots_)
            std::cerr << d << " ";
          std::cerr << std::endl;
        }

        // clear tmp objects
        distances.clear();
        // init tmp objects
        distances.reserve(curve.psize());

        double min_distance = std::numeric_limits<double>::max();
        double max_distance = 0;
        // create distance list
        for (auto it = curve.pbegin(); it != curve.pend(); ++it) {
          double d = lineDistance(origin, direction, *it);
          min_distance = std::min(min_distance, std::abs(d));
          max_distance = std::max(max_distance, std::abs(d));
          distances.push_back(d);
        }

        // initialize convex hull class
        convexHull cHull(curve, distances, 0, curve.psize() - 1);
        auto tintersecs = cHull.calcIntersecs(0.);

        // bezier clipping parameters
        double tmin = nan;
        double tmax = nan;
        std::vector<size_t>::const_iterator tmin_itr;
        for (auto && [ itr, t ] : tintersecs) {
          if (std::isnan(tmin)) {
            tmin = t;
            tmin_itr = itr;
          } else {
            if (t < tmin) {
              tmin = t;
              tmin_itr = itr;
            }
          }
          if (std::isnan(tmax)) {
            tmax = t;
          } else {
            if (t > tmax) { tmax = t; }
          }
        }

        assert(max_distance >= min_distance);
        // convergence test
        bool convergence_flag = true;
        if (max_distance > error) { convergence_flag = false; }
        if (convergence_flag) {
          result.push_back(degenerate<point_type>(curve.points_[*tmin_itr]));
          continue;
        }

        if (!std::isnan(tmin) && !std::isnan(tmax)) {
          // tmin == tmax
          if (tmax - tmin < error) {
            if constexpr (debug_msg_level >= 2)
              std::cerr << "tmin == tmax" << std::endl;
            size_t p1_index = *tmin_itr;
            size_t p2_index = *std::next(tmin_itr);

            auto& p1 = curve.points_[p1_index];
            auto& p2 = curve.points_[p2_index];

            if (tmin == 0) {
              if (std::abs(distances[p1_index]) < 1e-2)
                result.push_back(degenerate<point_type>(p1));
              continue;
            } else if (tmax == 1) {
              if (std::abs(distances[p2_index]) < 1e-2)
                result.push_back(degenerate<point_type>(p2));
              continue;
            }

            if (max_distance < 1e-2) {
              result.push_back(
                degenerate<point_type>(curve.points_[*tmin_itr]));
            } else {
              auto[head, tail] = std::move(curve).split(0.5);
              curves.push(std::move(head));
              curves.push(std::move(tail));
            }
            continue;
          }
          assert(tmax >= tmin);
          // when convergence is too slow
          if (tmax - tmin > 0.2) {
            if constexpr (debug_msg_level >= 2)
              std::cerr << "Split at 0.5" << std::endl;
            auto[left, right] = std::move(curve).split(0.5);
            curves.push(std::move(right));
            curves.push(std::move(left));
            continue;
          } else {
            if constexpr (debug_msg_level >= 2)
              std::cerr << "Split at " << tmin << "," << tmax << std::endl;
            auto[head, body_tail] = curve.split(tmin);
            auto[body, tail] =
              std::move(body_tail).split((tmax - tmin) / (1. - tmin));

            // When split fails
            if (
              std::signbit(
                lineDistance(origin, direction, body.points_.front())) ^
              std::signbit(
                lineDistance(origin, direction, body.points_.back()))) {
              curves.push(std::move(body));
            } else {
              auto new_split = std::move(curve).split(0.5);
              curves.push(std::move(new_split.first));
              curves.push(std::move(new_split.second));
            }
            continue;
          }
        }
      }
    }
  };

  //////   Main    //////

  if constexpr (is_debug) {
    std::cout << ">>>> Starting NURBS.intersec()" << std::endl;
    std::cout << "Degree= " << degree_ << std::endl;
    std::cout << "Points:" << std::endl;
    for (auto&& p : points_)
      std::cout << "  {" << p.x << "," << p.y << "," << p.z << "}\n";
    std::cout << "Knots: " << std::endl;
    for (auto&& d : knots_)
      std::cout << d << " ";
    std::cout << std::endl;
  }
  // distance list
  std::vector<double> distances;
  distances.reserve(points_.size());

  // calc all distances betwenn control points and the line
  for (auto&& p : points_)
    distances.emplace_back(lineDistance(origin, direction, p));

  // zero degree
  if (distances.size() == 1) {
    if (distances.front() == 0)
      result.push_back({points_.front().x, points_.front().y});
    return;
  }

  // convex hull
  std::vector<hullEdge> upperHull;
  upperHull.reserve(degree());
  std::vector<hullEdge> lowerHull;
  lowerHull.reserve(degree());

  // initialize convex hull
  // initConvexHull(*this, distances, 0, degree() -1, upperHull, lowerHull);
  static auto checkIntersec =
    [](
      size_t bgn, size_t end,              // edge
      const std::vector<double>& distances // distance list
    ) {
      if (end < bgn) std::swap(bgn, end);
      if (distances[bgn] == 0) {
        return true;
      } else if (distances[end] == 0) {
        return true;
      } else {
        if (std::signbit(distances[bgn]) != std::signbit(distances[end])) {
          return true;
        } else
          return false;
      }
      return false;
    };
  upperHull.clear();
  upperHull.emplace_back(0, false);

  lowerHull.clear();
  lowerHull.emplace_back(0, false);

  for (size_t index = 1; index < degree(); ++index) {
    assert(upperHull.back().idx == lowerHull.back().idx);
    // update upper hull
    for (size_t i = upperHull.size() - 1; 0 < i; --i) {
      if (is_upward(distances, index, upperHull[i].idx, upperHull[i - 1].idx)) {
        upperHull.pop_back();
      } else
        break;
    }
    upperHull.emplace_back(index, false);
    // update t
    upperHull[upperHull.size() - 2].t = checkIntersec(
      upperHull[upperHull.size() - 2].idx, upperHull[upperHull.size() - 1].idx,
      distances);

    // update lower hull
    for (size_t i = lowerHull.size() - 1; 0 < i; --i) {
      if (!is_upward(
            distances, index, lowerHull[i].idx, lowerHull[i - 1].idx)) {
        lowerHull.pop_back();
      } else
        break;
    }
    lowerHull.emplace_back(index, false);
    // update t
    lowerHull[lowerHull.size() - 2].t = checkIntersec(
      lowerHull[lowerHull.size() - 2].idx, lowerHull[lowerHull.size() - 1].idx,
      distances);
  }

  if constexpr (is_debug) {
    std::cout << "Initialized first convex hull: " << std::endl;
    std::cout << "  upper: ";
    for (auto&& e : upperHull)
      std::cout << e.idx << ",";
    std::cout << std::endl;
    std::cout << "  lower: ";
    for (auto&& e : lowerHull)
      std::cout << e.idx << ",";
    std::cout << std::endl;

    std::cout << " Entering Marching convex hull algorithm" << std::endl;
  }

  // Marching convex hull algorithm
  for (size_t index = degree(); index < points_.size(); ++index) {
    // head addition
    assert(upperHull.back().idx == lowerHull.back().idx);
    // upper
    for (size_t i = upperHull.size() - 1; 0 < i; --i) {
      if (is_upward(distances, index, upperHull[i].idx, upperHull[i - 1].idx)) {
        upperHull.pop_back();
      } else
        break;
    }
    upperHull.emplace_back(index, false);
    // update t
    upperHull[upperHull.size() - 2].t = checkIntersec(
      upperHull[upperHull.size() - 2].idx, upperHull[upperHull.size() - 1].idx,
      distances);
    // lower
    for (size_t i = lowerHull.size() - 1; 0 < i; --i) {
      if (!is_upward(
            distances, index, lowerHull[i].idx, lowerHull[i - 1].idx)) {
        lowerHull.pop_back();
      } else
        break;
    }
    lowerHull.emplace_back(index, false);
    // update t
    lowerHull[lowerHull.size() - 2].t = checkIntersec(
      lowerHull[lowerHull.size() - 2].idx, lowerHull[lowerHull.size() - 1].idx,
      distances);
    if constexpr (is_debug) {
      std::cout << "------------ MCH Algo: " << index << std::endl;
      std::cout << "  upper: ";
      for (auto&& e : upperHull)
        std::cout << e.idx << ",";
      std::cout << std::endl;
      std::cout << "  lower: ";
      for (auto&& e : lowerHull)
        std::cout << e.idx << ",";
      std::cout << std::endl;
    }

    // is there intersections in current convex hull?
    bool has_intersec = false;
    for (auto & [ idx, t ] : upperHull) {
      if (t) {
        has_intersec = true;
        if constexpr (is_debug)
          std::cout << "found intersec in upperHull" << std::endl;
      }
      if (has_intersec) break;
    }
    for (auto & [ idx, t ] : lowerHull) {
      if (t) {
        has_intersec = true;
        if constexpr (is_debug)
          std::cout << "found intersec in lowerHull" << std::endl;
      }
      if (has_intersec) break;
    }

    if (has_intersec) {
      // create saturated nurbs curve
      std::vector<wpoint_type> pts(
        points_.begin() + (index - degree()), points_.begin() + index + 1);
      std::vector<knot_type> kts(
        knots_.begin() + (index - degree()),
        knots_.begin() + 2 * (degree() + 1) + (index - degree()));
      if constexpr (is_debug) {
        std::cout << "Points: [" << index - degree() << "," << index << "] "
                  << pts.size() << std::endl;
        std::cout << "Knots: [" << index - degree() << "," << 2 * (degree() + 1)
                  << "] " << kts.size() << std::endl;
      }
      assert(kts.size() == pts.size() + degree() + 1);
      NURBS clip{std::move(pts), std::move(kts), degree()};
      // execute bezier clipping
      // strtd_nurbs_clipper::clip(0, std::move(clip), origin, direction,
      // result);
      strd_nurbs_clipper_nonrecursive::clip(
        std::move(clip), origin, direction, result);
    }

    // tail removal
    assert(upperHull.front().idx == lowerHull.front().idx);
    assert(upperHull.size() >= 2 && lowerHull.size() >= 2);
    if (upperHull[1].idx < lowerHull[1].idx) {
      lowerHull.front() = upperHull[1];
      // update t
      lowerHull.front().t =
        checkIntersec(lowerHull[0].idx, lowerHull[1].idx, distances);
      upperHull.erase(upperHull.begin());
    } else {
      upperHull.front() = lowerHull[1];
      // update t
      upperHull.front().t =
        checkIntersec(upperHull[0].idx, lowerHull[1].idx, distances);
      lowerHull.erase(lowerHull.begin());
    }
  }
}
} // namespace nurbs