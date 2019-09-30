//
// Copyright (c) 2019 mocabe (https://github.com/mocabe)
// SPDX-License-Identifier: MIT
//

#pragma once

#include "dimension.hpp"

#include <vector>
#include <stdexcept>
#include <algorithm>

namespace mocabe::nurbs {

  /// Standard NURBS data
  template <class VectorT, class KnotT, size_t Degree>
  class nurbs_data
  {
    static_assert(
      std::is_floating_point_v<KnotT>,
      "Knot type should be floating point");

    static_assert(
      dimension_v<PointT> != 0,
      "Point type should have propery specialized acccessor traits");

  public:
    /// Point type
    using point_type = VectorT;
    /// Knot type
    using knot_type = KnotT;

    /// Construct empty NURBS data
    nurbs_data();

    /// Construct NURBS data from exising data
    nurbs_data(
      const std::vector<point_type>& ps,
      const std::vector<knot_type>& ks);

    /// Construct NURBS data from exising data
    nurbs_data(std::vector<point_type>&& ps, const std::vector<knot_type>& ks);

    /// Construct NURBS data from exising data
    nurbs_data(const std::vector<point_type>& ps, std::vector<knot_type>&& ks);

    /// Construct NURBS data from exising data
    nurbs_data(std::vector<point_type>&& ps, std::vector<knot_type>&& ks);

  public: /* NURBS data concept functions */
    /// Size of points
    [[nodiscard]] auto psize() const -> size_t;

    /// Size of knots
    [[nodiscard]] auto ksize() const -> size_t;

    /// Access point
    [[nodiscard]] auto point(size_t n) const -> const point_type&;

    /// Access point
    [[nodiscard]] auto point(size_t n) -> point_type&;

    /// Access knot
    [[nodiscard]] auto knot(size_t n) const -> const knot_type&;

    /// Access knot
    [[nodiscard]] auto knot(size_t n) -> knot_type&;

  public: /* Other functions */
    /// Return degree of this curve
    [[nodiscard]] auto degree() const -> size_t;

    /// Return degree() + 1
    [[nodiscard]] auto order() const -> size_t;

    /// Set new points
    void set_points(const std::vector<point_type>& ps);

    /// Set new knots
    void set_knots(const std::vector<knot_type>& ks);

    /// begin/end for points
    [[nodiscard]] auto pbegin() const;
    [[nodiscard]] auto pbegin();
    [[nodiscard]] auto pend() const;
    [[nodiscard]] auto pend();

    /// begin/end for knots
    [[nodiscard]] auto kbegin() const;
    [[nodiscard]] auto kend() const;

  private:
    std::vector<point_type> m_points;
    std::vector<knot_type> m_knots;
  };

  template <class P, class K, size_t D>
  nurbs_data<P, K, D>::nurbs_data()
  {
  }

  template <class P, class K, size_t D>
  nurbs_data<P, K, D>::nurbs_data(
    const std::vector<point_type>& ps,
    const std::vector<knot_type>& ks)
  {
    if (!std::is_sorted(ks.begin(), ks.end()))
      throw std::invalid_argument("Unsorted knot vector");

    if (ks.size() != ps.size() + Degree + 1)
      throw std::invalid_argument("Invalid size of knot vector");

    m_points = ps;
    m_knots  = ks;
  }

  template <class P, class K, size_t D>
  nurbs_data<P, K, D>::nurbs_data(
    std::vector<point_type>&& ps,
    const std::vector<knot_type>& ks)
  {
    if (!std::is_sorted(ks.begin(), ks.end()))
      throw std::invalid_argument("Unsorted knot vectors");

    if (ks.size() != ps.size() + Degree + 1)
      throw std::invalid_argument("Invalid size of knot vector");

    m_points = std::move(ps);
    m_knots  = ks;
  }

  template <class P, class K, size_t D>
  nurbs_data<P, K, D>::nurbs_data(
    const std::vector<point_type>& ps,
    std::vector<knot_type>&& ks)
  {
    if (!std::is_sorted(ks.begin(), ks.end()))
      throw std::invalid_argument("Unsorted knot vectors");

    if (ks.size() != ps.size() + Degree + 1)
      throw std::invalid_argument("Invalid size of knot vector");

    m_points = ps;
    m_knots  = std::move(ks);
  }

  template <class P, class K, size_t D>
  nurbs_data<P, K, D>::nurbs_data(
    std::vector<point_type>&& ps,
    std::vector<knot_type>&& ks)
  {
    if (!std::is_sorted(ks.begin(), ks.end()))
      throw std::invalid_argument("Unsorted knot vectors");

    if (ks.size() != ps.size() + Degree + 1)
      throw std::invalid_argument("Invalid size of knot vector");

    m_points = std::move(ps);
    m_knots  = std::move(ks);
  }

  template <class P, class K, size_t D>
  auto nurbs_data<P, K, D>::psize() const -> size_t
  {
    return m_points.size();
  }

  template <class P, class K, size_t D>
  auto nurbs_data<P, K, D>::ksize() const -> size_t
  {
    return m_knots.size();
  }

  template <class P, class K, size_t D>
  auto nurbs_data<P, K, D>::point(size_t n) const -> const point_type&
  {
    return m_points[n];
  }

  template <class P, class K, size_t D>
  auto nurbs_data<P, K, D>::point(size_t n) -> point_type&
  {
    return m_points[n];
  }

  template <class P, class K, size_t D>
  auto nurbs_data<P, K, D>::knot(size_t n) const -> const knot_type&
  {
    return m_knots[n];
  }

  template <class P, class K, size_t D>
  auto nurbs_data<P, K, D>::knot(size_t n) -> knot_type&
  {
    return m_knots[n];
  }

  template <class P, class K, size_t D>
  auto nurbs_data<P, K, D>::degree() const -> size_t
  {
    return Degree;
  }

  template <class P, class K, size_t D>
  auto nurbs_data<P, K, D>::order() const -> size_t
  {
    return degree() + 1;
  }

  template <class P, class K, size_t D>
  void nurbs_data<P, K, D>::set_points(const std::vector<point_type>& ps)
  {
    if (ps.size() != m_points.size())
      throw std::invalid_argument("Invalid size of points");

    m_points = ps;
  }

  template <class P, class K, size_t D>
  void nurbs_data<P, K, D>::set_knots(const std::vector<knot_type>& ks)
  {
    if (ks.size() != m_knots.size())
      throw std::invalid_argument("Invalid size of knots");

    if (!std::is_sorted(ks.begin(), ks.end()))
      throw std::invalid_argument("Unsorted knots");

    m_knots = knots;
  }

  template <class P, class K, size_t D>
  auto nurbs_data<P, K, D>::pbegin() const
  {
    return m_points.begin();
  }

  template <class P, class K, size_t D>
  auto nurbs_data<P, K, D>::pbegin()
  {
    return m_points.begin();
  }

  template <class P, class K, size_t D>
  auto nurbs_data<P, K, D>::pend() const
  {
    return m_points.end();
  }

  template <class P, class K, size_t D>
  auto nurbs_data<P, K, D>::pend()
  {
    return m_points.end();
  }

  template <class P, class K, size_t D>
  auto nurbs_data<P, K, D>::kbegin() const
  {
    return m_knots.begin();
  }

  template <class P, class K, size_t D>
  auto nurbs_data<P, K, D>::kend() const
  {
    return m_knots.end();
  }

} // namespace mocabe::nurbs