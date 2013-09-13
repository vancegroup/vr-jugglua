/** @file
        @brief Header

        @date 2013

        @author
        Ryan Pavlik
        <rpavlik@iastate.edu> and <abiryan@ryand.net>
        http://academic.cleardefinition.com/
        Iowa State University Virtual Reality Applications Center
        Human-Computer Interaction Graduate Program

*/

//           Copyright Iowa State University 2013.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef INCLUDED_MissingOperators_h_GUID_34D0864D_4524_40B2_6A46_C286F4FDC7E4
#define INCLUDED_MissingOperators_h_GUID_34D0864D_4524_40B2_6A46_C286F4FDC7E4

// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
// - none

namespace {
    template <typename T, char Dim = T::num_components> struct VectorEnabler {
        typedef typename T::value_type value_type;
    };

    template <typename T>
    inline typename VectorEnabler<T>::value_type operator*(T const &a,
                                                           T const &b) {
        typename T::value_type sum = 0;
        for (int i = 0; i < T::num_components; ++i) {
            sum += a[i] * b[i];
        }
        return sum;
    }

    /*inline GLbyte operator*(osg::Vec2b const& a, osg::Vec2b const& b) {
            return a[0] * b[0] + a[1] * b[1];
    }
    inline GLshort operator*(osg::Vec2s const& a, osg::Vec2s const& b) {
            return a[0] * b[0] + a[1] * b[1];
    }
    inline GLbyte operator*(osg::Vec3b const& a, osg::Vec3b const& b) {
            return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
    }
    inline GLshort operator*(osg::Vec3s const& a, osg::Vec3s const& b) {
            return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
    }
    */
}
#endif // INCLUDED_MissingOperators_h_GUID_34D0864D_4524_40B2_6A46_C286F4FDC7E4
