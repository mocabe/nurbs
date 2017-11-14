#pragma once

#include <vector>
#include <memory>
#include <utility>
#include <algorithm>
#include <cassert>
#include <stdexcept>

#if defined(NURBS_ENABLE_MULTI_THREADING)
  #include <thread>
  #ifndef NURBS_THREAD_NUM
    #define NURBS_THREAD_NUM 4
  #endif
#endif

#include "../util/setup.hpp"
#include "point_traits.hpp"
#include "knot_traits.hpp"

namespace nurbs{

/**
 * @class
 * @brief NURBS curve class
 */
template <class T, class K>
class NURBS {
public:
  //point type
  using point_type = typename point_traits<T>::point_type;
  // weighted point type which has atleast (dim(point_type) +1) dimension
  using wpoint_type = typename point_traits<T>::weighted_point_type;
  // knot_type.
  using knot_type = typename knot_traits<K>::knot_type;

  /**
  * @fun
  * @brief returns degree
  */
  size_t degree() const noexcept{
    return degree_;
  };

  /**
   * @fun
   * @brief returns order ( degree +1)
   */
  size_t order() const noexcept{
    return degree_+1;
  }
  /**
   * @fun
   * @brief returns number of point thi class has
   */
  size_t psize() const noexcept{
    return points_.size();
  }

  /**
   * @fun
   * @brief returns number of knot vectors this class has
   */
  size_t ksize() const noexcept{
    return knots_.size();
  }

  /**
   * @fun
   * @brief returns a copy of knots
   */
  std::vector<knot_type> knots() const{
    return knots_;
  }

  /**
   * @fun
   * @brief set new knots
   */
  bool set_knots(const std::vector<knot_type>& knots){
    if(knots.size() != knots_.size()) return false;
    knots_ = knots;
    std::sort(knots_.begin(), knots_.end());
    return true;
  }

  /**
   * @fun
   * @brief set new knots
   */
  bool set_knots(std::vector<knot_type>&& knots){
    if(knots.size() != knots_.size()) return false;
    knots_ = std::move(knots);
    std::sort(knots_.begin(), knots_.end());
    return true;
  }


  /**
   * @fun
   * @brief returns a copy of points
   */
  std::vector<wpoint_type> points() const{
    return points_;
  }

  /**
    * @fun
    * @brief set new points
    */
  bool set_points(const std::vector<wpoint_type>& points){
    if(points.size() != points_.size()) return false;
    points_ = points;
    return true;
  }

  /**
    * @fun
    * @brief set new points
    */
  bool set_points(std::vector<wpoint_type>&& points){
    if(points.size() != points_.size()) return false;
    points_ = std::move(points);
    return true;
  }

  /**
   * @fun
   * @brief returns an iterator of points.begn()
   * @return const iterator created by points.begin()
   */
  typename std::vector<wpoint_type>::const_iterator pbegin() const{
    return points_.begin();
  }
  /**
   * @fun
   * @brief returns an iterator of points.begn()
   * @return iterator created by points.begin()
   */
  typename std::vector<wpoint_type>::iterator pbegin() {
    return points_.begin();
  }

  /**
   * @fun
   * @brief returns an iterator of knots.begn()
   * @return const iterator created by knots.begin()
   */
  typename std::vector<knot_type>::const_iterator kbegin() const{
    return knots_.begin();
  }

  /**
   * @fun
   * @brief returns an iterator of points.end()
   * @return const iterator created by points.end()
   */
  typename std::vector<wpoint_type>::const_iterator pend() const{
    return points_.end();
  }
  /**
   * @fun
   * @brief returns an iterator of points.end()
   * @return iterator created by points.end()
   */
  typename std::vector<wpoint_type>::iterator pend() {
    return points_.end();
  }
  /**
   * @fun
   * @brief returns an iterator of points.end()
   * @return const iterator created by knost.end()
   */
  typename std::vector<knot_type>::const_iterator kend() const{
    return knots_.end();
  }

  /**
   * @fun
   * @brief Element access for points
   */
  wpoint_type& pget(size_t index){
    return points_[index];
  }
  /**
   * @fun
   * @brief Element access for points
   */
  const wpoint_type& pget(size_t index) const{
    return points_[index];
  }

  /**
   * @fun
   * @brief Element access for knots.
   */
  const knot_type& kget(size_t index) const{
    return knots_[index];
  }

  /**
   * @fun
   * @brief returns avalable knot range for evaluate
   * @return pair of starting/ending knots
   */
  std::pair<knot_type, knot_type> knot_range() const {
    return {knots_[degree_], knots_[points_.size()]};
  }

  /**
   * @fun
   * @brief evaluate NURBS curve
   * @param t relative param for evaluate ( 0(0%) to 1(100%) )
   * @return result as 3D vector
   */
  point_type evaluate(knot_type t) const {
    return evaluate_impl(t);
  }

  /**
   * @fun
   * @brief evaluate NURBS curve from given t range
   * @param range evaluate range(max: 0 to 1)
   * @param interval of each evaluate
   * @return evaluated points
   */
  std::vector<point_type> evaluate(const std::pair<knot_type, knot_type> &range,
                                   knot_type interval) const {
    knot_type tBegin = range.first;
    knot_type tEnd = range.second;

    // clamp range
    //std::clamp(tBegin,0,1);
    tBegin = std::max(0.,std::min(tBegin,1.));
    //std::clamp(tEnd,0,1);
    tEnd = std::max(0.,std::min(tEnd,1.));

    if (tBegin > tEnd) std::swap(tBegin,tEnd);

    // clamp interval
    //std::clamp(interval,-1,1);
    interval = std::max(-1.,std::min(interval,1.));


    // edge cases
    if (interval == 0) { 
      std::vector<point_type> ret(2);
      ret[0] = ret[1] = evaluate(tBegin);
      return ret;
    } else 
    if (interval == 1) {
      std::vector<point_type> ret(2);
      ret[0] = evaluate(tBegin);
      ret[1] = evaluate(tEnd);
      return ret;
    } else 
    if (interval == -1) {
      std::vector<point_type> ret(2);
      ret[0] = evaluate(tEnd);
      ret[1] = evaluate(tBegin);
      return ret;
    }

    knot_type cBegin{tBegin};
    knot_type cEnd{tEnd};

    // negative interval
    if (interval < 0)
      std::swap(cBegin,cEnd);


    // return vector
    std::vector<point_type> ret((size_t)((cEnd - cBegin) / interval) + 2);

    // eval begin point
    ret.front() = evaluate(cBegin);


#if defined(NURBS_ENABLE_MULTI_THREADING)
    // Simple multi threading feature with std::thread.

    std::vector<std::thread> threads{};
    // create threads with rambda function
    for (size_t thread_num = 0; thread_num < NURBS_THREAD_NUM;
         ++thread_num) {
      threads.emplace_back([&, thread_num] {
        // buffer for evaluation
      #if defined(NURBS_NON_RECURSIVE) && !defined(NURBS_ENABLE_STACK_ALLOCATION)
        std::vector<wpoint_type> buff(degree_ + 1);
      #else
        std::vector<wpoint_type> buff(); //not used
      #endif
        // push back each evaluated point
        for (size_t i = thread_num; i < ret.size();
             i += NURBS_THREAD_NUM) {
          ret[i] = evaluate_impl(cBegin + i * interval, buff);
        }
      });
    };

    // wait until all threads finish their work
    for (auto &th : threads)
      th.join();

#else ////NURBS_ENABLE_MULTI_THREADING

  #if defined(NURBS_NON_RECURSIVE) && !defined(NURBS_ENABLE_STACK_ALLOCATION)
    std::vector<wpoint_type> buff(degree_ + 1);
  #else
    std::vector<wpoint_type> buff; // not used
  #endif

    // push back each evaluated point
    for (size_t i = 1; i < ret.size() - 1; ++i)
      ret[i] = evaluate_impl(cBegin + i * interval, buff);

#endif ////NURBS_ENABLE_MULTI_THREADING

    // eval end point
    ret.back() = evaluate(cEnd);

    return ret;
  }

  /**
   * @fun
   * @brief evaluate all area of NURBS curve
   * calling evaluete({0,1}, interval)
   * @param interval interval between each evaluation
   * @return a set of evaluated 3D vectors
   */
  std::vector<point_type> evaluate_all(double interval) const{
    return evaluate({0., 1.}, interval);
  }

  /**
   * @fun
   * @brief init NURBS as 1 point (weight will be set to 1.), 0 degree
   */
  NURBS() : points_{wpoint_type()}, knots_{0, 1}, degree_{0} {
    get<dimension_v<point_type>>(points_.front()) = 1;
  }

  /** 
   * @fun
   * @brief constructor
   * @param points control points of 4D vector
   * @param knots knot vectors
   * @param degree degree of curve
   */
  NURBS(const std::vector<wpoint_type>& points, const std::vector<knot_type> &knots, size_t degree){
    /* Argument tests */
    if (knots.size() != points.size() + degree + 1)
      throw std::invalid_argument("invalid knot size");

    /* Member initialization */
    points_ = points;
    knots_ = knots;
    degree_ = degree;
    
    /* Normalization */
    std::sort(knots_.begin(), knots_.end());
  }

  /** @fun
   * @brief copy constructor */
  NURBS(const NURBS &nurbs)
    : points_{nurbs.points_}, knots_{nurbs.knots_}, degree_{nurbs.degree_}{}

  /** @fun
   * @brief move constructor */
  NURBS(NURBS &&nurbs)
    : points_{std::move(nurbs.points_)}, knots_{std::move(nurbs.knots_)}, degree_{nurbs.degree_}{}

  /** @fun
   * @brief destructor */
  ~NURBS(){}

  /** @fun
   * @brief copy operator */
  NURBS &operator=(const NURBS &nurbs){
    points_ = nurbs.points_;
    knots_ = nurbs.knots_;
    degree_ = nurbs.degree_;
    return *this;
  }

  /** @fun
   * @brief move operator */
  NURBS &operator=(NURBS &&nurbs){
    points_ = std::move(nurbs.points_);
    knots_ = std::move(nurbs.knots_);
    degree_ = nurbs.degree_;
    return *this;
  }

private:

  /**
   * @fun
   * @brief evaluate impl
   */
  point_type evaluate_impl(knot_type t) const{
#if defined(NURBS_NON_RECURSIVE) && !defined(NURBS_ENABLE_STACK_ALLOCATION)
    std::vector<wpoint_type> buff(degree_ +1);
#else
    std::vector<wpoint_type> buff;
#endif
    return evaluate_impl(t, buff);
  }

  /**
   * @fun
   * @brief evaluate impl
   */
  point_type evaluate_impl(knot_type t, std::vector<wpoint_type>& heap_buffer) const {

    // clamp
    t = std::max(0., std::min(t, 1.));

    auto range = knot_range();

    // convert ratio to actual knot range for evaluate
    t = range.first + t * (range.second - range.first);

    return knot_evaluate_DeBoor(t, heap_buffer);
  }

  /** 
   * @fun
   * @brief Calculate De Boor's Algorithm
   */
  point_type knot_evaluate_DeBoor(double t, std::vector<wpoint_type>& heap_buffer) const {

    size_t index;
    // find knot span
    for (index = degree_; index < points_.size() - 1; ++index) {
      if (knots_[index] <= t && t < knots_[index + 1])
        break;
    }
    assert(points_.size() > index);

#if defined(NURBS_NON_RECURSIVE) // De Boor's Algorithm - Non-recursive impl

  #if  defined(NURBS_ENABLE_STACK_ALLOCATION)
    wpoint_type *buff =
        (wpoint_type *)alloca(sizeof(wpoint_type) * (degree_ + 1));
    std::copy(points_.begin() + index - degree_, points_.begin() + index + 1,
              buff);
  #else  // NURBS_ENABLE_STACK_ALLOCATION
    //std::vector<wpoint_type> buff(degree_ +1);
    heap_buffer.resize(degree_ + 1);
    auto& buff = heap_buffer;
    std::copy(points_.begin() + index - degree_, points_.begin() + index + 1,
              buff.begin());
  #endif // NURBS_ENABLE_STACK_ALLOCATION

    for (size_t i = 0; i < degree_ + 1; ++i) {
      rmult<dimension_v<point_type> - 1>(buff[i],
                                         get<dimension_v<point_type>>(buff[i]));
    }

    for (size_t i = 0; i < degree_; ++i) {
      for (size_t j = 0; j < degree_ - i; ++j) {
        size_t idx = index - degree_ + i + j + 1;
        knot_type d = knots_[idx + degree_ + 1 - (i + 1)] - knots_[idx];
        knot_type a = (d == 0) ? 0 : (t - knots_[idx]) / d;
        buff[j] = buff[j] * (1 - a) + buff[j + 1] * a;
      }
    }
    wpoint_type r = buff[0];
#else // NURBS_NON_RECURSIVE  // De Boor's Algorithm - Recursive impl

    struct {
      wpoint_type operator()(const NURBS<T, K> &nurbs, knot_type t, size_t i,
                             size_t k) const {
        if (k == 0) {
          // convert n degree rational bspine to n+1 degree non-rational bspline

          wpoint_type ret = nurbs.points_[i];
          auto w = get<dimension_v<point_type>>(ret);
          w = w < 0 ? 0 : w;
          rmult<dimension_v<point_type> - 1>(ret, w);
          return ret;
        }
        knot_type d = nurbs.knots_[i + nurbs.degree_ + 1 - k] - nurbs.knots_[i];
        knot_type a = (d == 0) ? 0 : (t - nurbs.knots_[i]) / d;

        return ((*this)(nurbs, t, i - 1, k - 1) * (1 - a)) +
               ((*this)(nurbs, t, i, k - 1) * a);
      }
    } calc;

    // evaluate
    wpoint_type r = calc(*this, t, index, degree_);

#endif // NURBS_NON_RECURSIVE

    // Zero weight workaround: estimating possible return point
    if (get<dimension_v<point_type>>(r) == 0) {
      bool flag = false;

      // P[index-degree] to P[size-1]
      for (size_t i = index - degree_; i < points_.size(); ++i) {
        if (get<dimension_v<point_type>>(points_[i]) == 0)
          flag = true;

        if (flag && get<dimension_v<point_type>>(points_[i]) != 0)
          return degenerate<point_type>(points_[i]);
      }
      flag = false;
      // P[index] back to P[1]
      for (size_t i = index; 0 < i; --i) {
        if (get<dimension_v<point_type>>(points_[i]) == 0)
          flag = true;

        if (flag && get<dimension_v<point_type>>(points_[i]) != 0)
          return degenerate<point_type>(points_[i]);
      }

      // points_[0]
      if (flag && get<dimension_v<point_type>>(points_[0]) != 0)
        return degenerate<point_type>(points_[0]);
    }

    if (get<dimension_v<point_type>>(r) != 0)
      rdiv<dimension_v<point_type> - 1>(r, get<dimension_v<point_type>>(r));

    // return as point_type
    return degenerate<point_type>(r);
  }

private:
  std::vector<wpoint_type> points_;
  std::vector<knot_type> knots_;
  size_t degree_;
};

}
