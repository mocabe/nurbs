#pragma once

#include <vector>
#include <utility>
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <type_traits>

#include "../util/setup.hpp"
#include "point_traits.hpp"
#include "knot_traits.hpp"

#include "../util/tags.hpp"


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
   * @brief init NURBS as 1 point (weight will be set to 1.), 0 degree
   */
  NURBS() : points_{wpoint_type() * 0}, knots_{0, 1}, degree_{0} {
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
  template <class EvalTag = tags::default_eval_tag>
  point_type evaluate(knot_type t) const {
    return evaluate_impl<EvalTag>(t);
  }

  /**
   * @fun
   * @brief evaluate NURBS curve from given t range
   * @param range evaluate range(max: 0 to 1)
   * @param interval of each evaluate
   * @return evaluated points
   */
  template <class EvalTag = tags::default_eval_tag>
  std::vector<point_type> evaluate(const std::pair<knot_type, knot_type> &range,
                                   knot_type interval) const {
    std::vector<point_type> result;
    evaluate<EvalTag>(range, interval, result);
    return result;
  }

  /**
   * @fun
   * @brief evaluate NURBS curve from given t range
   * @param range evaluate range(max: 0 to 1)
   * @param interval of each evaluate
   * @param reference to result vector
   */
  template <class EvalTag = tags::default_eval_tag>
  void evaluate(const std::pair<knot_type, knot_type> &range,
                knot_type interval, std::vector<point_type> &result) const {
    knot_type tBegin = range.first;
    knot_type tEnd = range.second;

    // clamp range
    tBegin = std::clamp(tBegin, knot_type(0), knot_type(1));
    tEnd = std::clamp(tEnd, knot_type(0), knot_type(1));

    if (tBegin > tEnd)
      std::swap(tBegin, tEnd);

    // clamp interval
    interval = std::clamp(interval, knot_type(-1), knot_type(1));

    // clear result
    result.clear();

    // edge cases
    if (interval == 0) {
      result.reserve(2);
      result.emplace_back(evaluate<EvalTag>(tBegin));
      // result[1] = result[0]
      result.push_back(result.back());
      return;
    } else if (interval == 1) {
      result.reserve(2);
      result.emplace_back(evaluate<EvalTag>(tBegin));
      result.emplace_back(evaluate<EvalTag>(tEnd));
      return;
    } else if (interval == -1) {
      result.reserve(2);
      result.emplace_back(evaluate<EvalTag>(tEnd));
      result.emplace_back(evaluate<EvalTag>(tBegin));
      return;
    }

    knot_type cBegin{tBegin};
    knot_type cEnd{tEnd};

    // negative interval
    if (interval < 0)
      std::swap(cBegin, cEnd);

    // reserve
    auto sz = ((size_t)((cEnd - cBegin) / interval) + 2);
    result.reserve(sz);

    // eval begin point
    result.emplace_back(evaluate<EvalTag>(cBegin));

    std::vector<wpoint_type> buff;
    if constexpr (std::is_same_v<EvalTag, tags::NonRecursive<tags::HeapArray>>)
      buff.resize(degree_ + 1);

    // push back each evaluated point
    for (size_t i = 1; i < sz - 1; ++i)
      result.emplace_back(evaluate_impl<EvalTag>(cBegin + i * interval, buff));

    // eval end point
    result.emplace_back(evaluate<EvalTag>(cEnd));

    return;
    }

    /**
     * @fun
     * @brief evaluate all area of NURBS curve
     * calling evaluete({0,1}, interval)
     * @param interval interval between each evaluation
     * @return a set of evaluated 3D vectors
     */
    template <class EvalTag = tags::default_eval_tag>
    std::vector<point_type> evaluate_all(double interval) const {
      return evaluate<EvalTag>({knot_type(0), knot_type(1)}, interval);
    }

    /**
     * @fun
     * @brief evaluate all area of NURBS curve
     * calling evaluete({0,1}, interval)
     * @param interval interval between each evaluation
     * @param result reference to result
     */
    template <class EvalTag = tags::default_eval_tag>
    void evaluate_all(double interval, std::vector<point_type>& result) const {
      evaluate<EvalTag>({knot_type(0), knot_type(1)}, interval, result);
    }

    /**
     * @fun
     * @brief reverse curve
     */
    NURBS& reverse(); // reverse.hpp 

    /**
     * @fun
     * @brief insert knot
     */
    NURBS &knot_insert(knot_type); // knot_insert.hpp

    /**
     * @fun
     * @brief calc tangent at t
     * @return non-normalized tangent vector
     */
    point_type tangent(knot_type t) const; // tangent.hpp

  private:
    /**
     * @fun
     * @brief evaluate impl
     */
    template <class EvalTag> 
    point_type evaluate_impl(knot_type t) const {
      std::vector<wpoint_type> buff;
      return evaluate_impl<EvalTag>(t, buff);
    }

    /**
     * @fun
     * @brief evaluate impl
     */
    template <class EvalTag>
    point_type evaluate_impl(knot_type t, std::vector<wpoint_type> & heap_buffer) const {

      // clamp
      t = std::clamp(t, knot_type(0), knot_type(1));

      auto range = knot_range();

      // convert ratio to actual knot range for evaluate
      t = range.first + t * (range.second - range.first);

      return knot_evaluate_DeBoor<EvalTag>(t, heap_buffer);
    }

    /**
     * @fun
     * @brief Calculate De Boor's Algorithm
     */
    template <class EvalTag>
    point_type knot_evaluate_DeBoor(double t, std::vector<wpoint_type> &heap_buffer) const {
      using namespace tags;
      using namespace std;
      size_t index;
      // find knot span
      for (index = 0; index < knots_.size() - 2; ++index) {
        if (knots_[index] <= t && t < knots_[index + 1])
          break;
      }

      while (knots_[index] == knots_[index + 1]) {
        if (index == 0)
          break;
        --index;
      }
      assert(knots_.size() > index);

      // return vector
      wpoint_type r;
      if constexpr (is_same_v<EvalTag, NonRecursive<StackArray>> ||
                    is_same_v<EvalTag, NonRecursive<HeapArray>>) {
        // buffer
        wpoint_type *buff;
        if constexpr (is_same_v<EvalTag, NonRecursive<StackArray>>) {
          buff = (wpoint_type *)alloca(sizeof(wpoint_type) * (degree_ + 1));
        } else if constexpr (is_same_v<EvalTag, NonRecursive<HeapArray>>) {
          heap_buffer.resize(degree_ + 1);
          buff = heap_buffer.data();
        }
        for (size_t i = 0; i < degree_ + 1; ++i) {
          // invalid index
          if (index + i < degree_ || index - degree_ + i >= points_.size()) {
            buff[i] = wpoint_type{} * 0;
          } else {
            buff[i] = points_[index - degree_ + i];
            rmult<dimension_v<point_type> - 1>(
                buff[i], get<dimension_v<point_type>>(buff[i]));
          }
        }

        for (size_t i = 0; i < degree_; ++i) {
          for (size_t j = 0; j < degree_ - i; ++j) {
            size_t idx = index - degree_ + i + j + 1;
            // invalid index
            if (idx >= knots_.size() ||
                idx + degree_ + 1 - (i + 1) >= knots_.size()) {
              buff[j] = wpoint_type{} * 0;
            } else {
              knot_type d = knots_[idx + degree_ + 1 - (i + 1)] - knots_[idx];
              knot_type a = (d == 0) ? 0 : (t - knots_[idx]) / d;
              buff[j] = buff[j] - (buff[j] - buff[j + 1]) * a;
            }
          }
        }
        r = buff[0];
      } else if constexpr (is_same_v<EvalTag, Recursive>) {
        struct {
          wpoint_type operator()(const NURBS<T, K> &nurbs, knot_type t,
                                 size_t i, size_t k) const {
            if (k == 0) {
              // invalid index
              if (i >= nurbs.points_.size())
                return set<dimension_v<point_type>>(wpoint_type{} * 0, 1);

              // convert n degree rational bspine to n+1 degree non-rational
              // bspline
              wpoint_type ret = nurbs.points_[i];
              auto w = get<dimension_v<point_type>>(ret);
              w = w < 0 ? 0 : w;
              rmult<dimension_v<point_type> - 1>(ret, w);
              return ret;
            }

            // invalid index
            if (i >= nurbs.knots_.size() ||
                i + nurbs.degree_ + 1 - k >= nurbs.knots_.size())
              return wpoint_type{} * 0;

            knot_type d =
                nurbs.knots_[i + nurbs.degree_ + 1 - k] - nurbs.knots_[i];
            knot_type a = (d == 0) ? 0 : (t - nurbs.knots_[i]) / d;

            return ((*this)(nurbs, t, i - 1, k - 1) * (1 - a)) +
                   ((*this)(nurbs, t, i, k - 1) * a);
          }
        } calc;

        // evaluate
        r = calc(*this, t, index, degree_);
      } else {
        static_assert(!sizeof(EvalTag), "Invalid Eval Tag");
      }
      // Zero weight workaround: estimating possible return point
      if (get<dimension_v<point_type>>(r) == 0) {
        size_t bot = (index + 1 < degree_) ? 0 : index + 1 - degree_;
        // P[index-degree+1] to P[size-1]
        for (size_t i = bot; i < points_.size(); ++i)
          if (get<dimension_v<point_type>>(points_[i]) != 0)
            return degenerate<point_type>(points_[i]);
        // P[index] back to P[1]
        for (size_t i = bot; 0 < i; --i)
          if (get<dimension_v<point_type>>(points_[i]) != 0)
            return degenerate<point_type>(points_[i]);
        // P[0]
        return degenerate<point_type>(points_[0]);
      } else {
        rdiv<dimension_v<point_type> - 1>(r, get<dimension_v<point_type>>(r));
      }
      // return as point_type
      return degenerate<point_type>(r);
    }

  private:
    std::vector<wpoint_type> points_;
    std::vector<knot_type> knots_;
    size_t degree_;
};

}
