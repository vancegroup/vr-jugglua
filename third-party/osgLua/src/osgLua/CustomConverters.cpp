/**
	@file
	@brief Implementation

	@date 2011

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// Internal Includes
#include "CustomConverters.h"

// Library/third-party includes
#include <osgLua/introspection/Converter>
#include <osgLua/introspection/ConverterProxy>

#include <osg/Vec2d>
#include <osg/Vec2f>
#include <osg/Vec2s>
#include <osg/Vec2b>

#include <osg/Vec3d>
#include <osg/Vec3f>

#include <osg/Vec4d>
#include <osg/Vec4f>
#include <osg/Vec4s>
#include <osg/Vec4b>
#include <osg/Vec4ub>

// Standard includes
#include <iostream>

namespace osgLua {
	namespace {
		template<int Val>
		struct IntToType {};

		template<typename SrcType, typename DestType>
		class VectorPrecisionConverter : public introspection::Converter {
			typedef typename DestType::value_type DestValueType;
			enum { num_components = DestType::num_components };
			typedef IntToType<DestType::num_components> NumComponentsType;

			virtual ~VectorPrecisionConverter() {}
			virtual introspection::Value convert(const introspection::Value& src) const
			{
				return convertVec(introspection::variant_cast<SrcType>(src), NumComponentsType());
			}

			DestType convertVec(SrcType const& input, IntToType<2>) const {
				return DestType(static_cast<DestValueType>(input[0]),
					static_cast<DestValueType>(input[1]));
			}

			DestType convertVec(SrcType const& input, IntToType<3>) const {
				return DestType(static_cast<DestValueType>(input[0]),
					static_cast<DestValueType>(input[1]),
					static_cast<DestValueType>(input[2]));
			}

			DestType convertVec(SrcType const& input, IntToType<4>) const {
				return DestType(static_cast<DestValueType>(input[0]),
					static_cast<DestValueType>(input[1]),
					static_cast<DestValueType>(input[2]),
					static_cast<DestValueType>(input[3]));
			}

			virtual introspection::CastType getCastType() const { return introspection::STATIC_CAST; }
		};

		template<typename T>
		inline introspection::Type const& getIntrospectionType() {
			static introspection::Type const& t = typeof(T);
			return t;
		}
		template<typename T1, typename T2>
		inline void registerVectorPrecisionConverter() {
			introspection::ConverterProxy cvt1to2(getIntrospectionType<T1>(), getIntrospectionType<T2>(), new VectorPrecisionConverter<T1, T2>());
			introspection::ConverterProxy cvt2to1(getIntrospectionType<T2>(), getIntrospectionType<T1>(), new VectorPrecisionConverter<T2, T1>());
		}

	} // end of anonymous namespace

	void registerCustomConverters() {
		std::cerr << "Registering custom converters." << std::endl;
		registerVectorPrecisionConverter<osg::Vec2d, osg::Vec2f>();
		registerVectorPrecisionConverter<osg::Vec2s, osg::Vec2b>();

		registerVectorPrecisionConverter<osg::Vec3d, osg::Vec3f>();

		registerVectorPrecisionConverter<osg::Vec4d, osg::Vec4f>();
		//registerVectorPrecisionConverter<osg::Vec4f, osg::Vec4s>();
		registerVectorPrecisionConverter<osg::Vec4s, osg::Vec4b>();
		registerVectorPrecisionConverter<osg::Vec4b, osg::Vec4ub>();
	}
} // end of namespace osgLua
