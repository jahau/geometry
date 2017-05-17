#ifndef BOOST_GEOMETRY_PROJECTIONS_CASS_HPP
#define BOOST_GEOMETRY_PROJECTIONS_CASS_HPP

// Boost.Geometry - extensions-gis-projections (based on PROJ4)
// This file is automatically generated. DO NOT EDIT.

// Copyright (c) 2008-2015 Barend Gehrels, Amsterdam, the Netherlands.

// This file was modified by Oracle on 2017.
// Modifications copyright (c) 2017, Oracle and/or its affiliates.
// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is converted from PROJ4, http://trac.osgeo.org/proj
// PROJ4 is originally written by Gerald Evenden (then of the USGS)
// PROJ4 is maintained by Frank Warmerdam
// PROJ4 is converted to Boost.Geometry by Barend Gehrels

// Last updated version of proj: 4.9.1

// Original copyright notice:

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include <boost/geometry/srs/projections/impl/base_static.hpp>
#include <boost/geometry/srs/projections/impl/base_dynamic.hpp>
#include <boost/geometry/srs/projections/impl/projects.hpp>
#include <boost/geometry/srs/projections/impl/factory_entry.hpp>
#include <boost/geometry/srs/projections/impl/pj_mlfn.hpp>

#include <boost/geometry/srs/projections/epsg_traits.hpp>

namespace boost { namespace geometry
{

namespace srs { namespace proj
{
    struct cass {};

}} //namespace srs::proj

namespace projections
{
    #ifndef DOXYGEN_NO_DETAIL
    namespace detail { namespace cass
    {

            //static const double EPS10 = 1e-10;
            //static const double C1 = .16666666666666666666;
            //static const double C2 = .00833333333333333333;
            //static const double C3 = .04166666666666666666;
            //static const double C4 = .33333333333333333333;
            //static const double C5 = .06666666666666666666;

            template <typename T>
            inline T C1() { return .16666666666666666666666666666666666666; }
            template <typename T>
            inline T C2() { return .00833333333333333333333333333333333333; }
            template <typename T>
            inline T C3() { return .04166666666666666666666666666666666666; }
            template <typename T>
            inline T C4() { return .33333333333333333333333333333333333333; }
            template <typename T>
            inline T C5() { return .06666666666666666666666666666666666666; }

            template <typename T>
            struct par_cass
            {
                T m0;
                T en[EN_SIZE];
            };

            // template class, using CRTP to implement forward/inverse
            template <typename CalculationType, typename Parameters>
            struct base_cass_ellipsoid : public base_t_fi<base_cass_ellipsoid<CalculationType, Parameters>,
                     CalculationType, Parameters>
            {

                typedef CalculationType geographic_type;
                typedef CalculationType cartesian_type;

                par_cass<CalculationType> m_proj_parm;

                inline base_cass_ellipsoid(const Parameters& par)
                    : base_t_fi<base_cass_ellipsoid<CalculationType, Parameters>,
                     CalculationType, Parameters>(*this, par) {}

                // FORWARD(e_forward)  ellipsoid
                // Project coordinates from geographic (lon, lat) to cartesian (x, y)
                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    static const CalculationType C1 = cass::C1<CalculationType>();
                    static const CalculationType C2 = cass::C2<CalculationType>();
                    static const CalculationType C3 = cass::C3<CalculationType>();

                    CalculationType n = sin(lp_lat);
                    CalculationType c = cos(lp_lat);
                    xy_y = pj_mlfn(lp_lat, n, c, this->m_proj_parm.en);
                    n = 1./sqrt(1. - this->m_par.es * n * n);
                    CalculationType tn = tan(lp_lat); CalculationType t = tn * tn;
                    CalculationType a1 = lp_lon * c;
                    c *= this->m_par.es * c / (1 - this->m_par.es);
                    CalculationType a2 = a1 * a1;
                    xy_x = n * a1 * (1. - a2 * t *
                        (C1 - (8. - t + 8. * c) * a2 * C2));
                    xy_y -= this->m_proj_parm.m0 - n * tn * a2 *
                        (.5 + (5. - t + 6. * c) * a2 * C3);
                }

                // INVERSE(e_inverse)  ellipsoid
                // Project coordinates from cartesian (x, y) to geographic (lon, lat)
                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    static const CalculationType C3 = cass::C3<CalculationType>();
                    static const CalculationType C4 = cass::C4<CalculationType>();
                    static const CalculationType C5 = cass::C5<CalculationType>();

                    CalculationType ph1;

                    ph1 = pj_inv_mlfn(this->m_proj_parm.m0 + xy_y, this->m_par.es, this->m_proj_parm.en);
                    CalculationType tn = tan(ph1); CalculationType t = tn * tn;
                    CalculationType n = sin(ph1);
                    CalculationType r = 1. / (1. - this->m_par.es * n * n);
                    n = sqrt(r);
                    r *= (1. - this->m_par.es) * n;
                    CalculationType dd = xy_x / n;
                    CalculationType d2 = dd * dd;
                    lp_lat = ph1 - (n * tn / r) * d2 *
                        (.5 - (1. + 3. * t) * d2 * C3);
                    lp_lon = dd * (1. + t * d2 *
                        (-C4 + (1. + 3. * t) * d2 * C5)) / cos(ph1);
                }

                static inline std::string get_name()
                {
                    return "cass_ellipsoid";
                }

            };

            // template class, using CRTP to implement forward/inverse
            template <typename CalculationType, typename Parameters>
            struct base_cass_spheroid : public base_t_fi<base_cass_spheroid<CalculationType, Parameters>,
                     CalculationType, Parameters>
            {

                typedef CalculationType geographic_type;
                typedef CalculationType cartesian_type;

                par_cass<CalculationType> m_proj_parm;

                inline base_cass_spheroid(const Parameters& par)
                    : base_t_fi<base_cass_spheroid<CalculationType, Parameters>,
                     CalculationType, Parameters>(*this, par) {}

                // FORWARD(s_forward)  spheroid
                // Project coordinates from geographic (lon, lat) to cartesian (x, y)
                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    xy_x = asin(cos(lp_lat) * sin(lp_lon));
                    xy_y = atan2(tan(lp_lat) , cos(lp_lon)) - this->m_par.phi0;
                }

                // INVERSE(s_inverse)  spheroid
                // Project coordinates from cartesian (x, y) to geographic (lon, lat)
                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    CalculationType dd = xy_y + this->m_par.phi0;
                    lp_lat = asin(sin(dd) * cos(xy_x));
                    lp_lon = atan2(tan(xy_x), cos(dd));
                }

                static inline std::string get_name()
                {
                    return "cass_spheroid";
                }

            };

            // Cassini
            template <typename Parameters, typename T>
            void setup_cass(Parameters& par, par_cass<T>& proj_parm)
            {
                if (par.es) {
                    if (!pj_enfn(par.es, proj_parm.en))
                        BOOST_THROW_EXCEPTION( projection_exception(0) );
                    proj_parm.m0 = pj_mlfn(par.phi0, sin(par.phi0), cos(par.phi0), proj_parm.en);
                } else {
                }
            }

    }} // namespace detail::cass
    #endif // doxygen

    /*!
        \brief Cassini projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Cylindrical
         - Spheroid
         - Ellipsoid
        \par Example
        \image html ex_cass.gif
    */
    template <typename CalculationType, typename Parameters>
    struct cass_ellipsoid : public detail::cass::base_cass_ellipsoid<CalculationType, Parameters>
    {
        inline cass_ellipsoid(const Parameters& par) : detail::cass::base_cass_ellipsoid<CalculationType, Parameters>(par)
        {
            detail::cass::setup_cass(this->m_par, this->m_proj_parm);
        }
    };

    /*!
        \brief Cassini projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Cylindrical
         - Spheroid
         - Ellipsoid
        \par Example
        \image html ex_cass.gif
    */
    template <typename CalculationType, typename Parameters>
    struct cass_spheroid : public detail::cass::base_cass_spheroid<CalculationType, Parameters>
    {
        inline cass_spheroid(const Parameters& par) : detail::cass::base_cass_spheroid<CalculationType, Parameters>(par)
        {
            detail::cass::setup_cass(this->m_par, this->m_proj_parm);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Static projection
        BOOST_GEOMETRY_PROJECTIONS_DETAIL_STATIC_PROJECTION(srs::proj::cass, cass_spheroid, cass_ellipsoid)

        // Factory entry(s)
        template <typename CalculationType, typename Parameters>
        class cass_entry : public detail::factory_entry<CalculationType, Parameters>
        {
            public :
                virtual base_v<CalculationType, Parameters>* create_new(const Parameters& par) const
                {
                    if (par.es)
                        return new base_v_fi<cass_ellipsoid<CalculationType, Parameters>, CalculationType, Parameters>(par);
                    else
                        return new base_v_fi<cass_spheroid<CalculationType, Parameters>, CalculationType, Parameters>(par);
                }
        };

        template <typename CalculationType, typename Parameters>
        inline void cass_init(detail::base_factory<CalculationType, Parameters>& factory)
        {
            factory.add_to_factory("cass", new cass_entry<CalculationType, Parameters>);
        }

    } // namespace detail
    #endif // doxygen

} // namespace projections

}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_PROJECTIONS_CASS_HPP

