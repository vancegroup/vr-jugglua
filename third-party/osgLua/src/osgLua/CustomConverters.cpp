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
#include <osgLua/introspection/variant_cast>

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

#include <osg/Matrixd>
#include <osg/Matrixf>

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
				virtual introspection::Value convert(const introspection::Value& src) const {
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

				virtual introspection::CastType getCastType() const {
					return introspection::STATIC_CAST;
				}
		};


		template<typename T>
		struct MatrixInfo;

		template<typename T>
		struct ValMatrixInfo {
			static typename T::value_type const * getPtr(const introspection::Value& src) {
				return introspection::variant_cast<T>(src).ptr();
			}
			template<typename InputType>
			static introspection::Value create(InputType input) {
				return T(input);
			}
		};

		template<typename T>
		struct RefMatrixInfo {
			static typename T::value_type const * getPtr(const introspection::Value& src) {
				return introspection::variant_cast<T const *>(src)->ptr();
			}
			template<typename InputType>
			static introspection::Value create(InputType input) {
				return new T(input);
			}
		};

		template<>
		struct MatrixInfo<osg::RefMatrixf> : RefMatrixInfo<osg::RefMatrixf> {};
		template<>
		struct MatrixInfo<osg::RefMatrixd> : RefMatrixInfo<osg::RefMatrixd> {};
		template<>
		struct MatrixInfo<osg::Matrixf> : ValMatrixInfo<osg::Matrixf> {};
		template<>
		struct MatrixInfo<osg::Matrixd> : ValMatrixInfo<osg::Matrixd> {};

		template<typename SrcType, typename DestType>
		class MatrixPrecisionConverter : public introspection::Converter {

				virtual ~MatrixPrecisionConverter() {}
				virtual introspection::Value convert(const introspection::Value& src) const {
					return MatrixInfo<DestType>::create(MatrixInfo<SrcType>::getPtr(src));
					//return MatrixInfo<DestType>::create(introspection::variant_cast<SrcType const&>(src).ptr());
				}

				virtual introspection::CastType getCastType() const {
					return introspection::STATIC_CAST;
				}
		};

		template<typename T1, typename T2>
		inline void registerVectorPrecisionConverter() {
			introspection::ConverterProxy cvt1to2(typeof(T1), typeof(T2), new VectorPrecisionConverter<T1, T2>());
			introspection::ConverterProxy cvt2to1(typeof(T2), typeof(T1), new VectorPrecisionConverter<T2, T1>());
		}

		template<typename T1, typename T2>
		inline void registerMatrixPrecisionConverter() {
			introspection::ConverterProxy cvt1to2(typeof(T1), typeof(T2), new MatrixPrecisionConverter<T1, T2>());
			introspection::ConverterProxy cvt2to1(typeof(T2), typeof(T1), new MatrixPrecisionConverter<T2, T1>());
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

		registerMatrixPrecisionConverter<osg::Matrixd, osg::Matrixf>();
		registerMatrixPrecisionConverter<osg::Matrixd, osg::RefMatrixd>();
		registerMatrixPrecisionConverter<osg::Matrixf, osg::RefMatrixf>();
	}
} // end of namespace osgLua
