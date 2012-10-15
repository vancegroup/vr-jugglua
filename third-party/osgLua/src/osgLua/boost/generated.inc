#include <boost/mpl/set.hpp>
#include <osg/Matrixd>
#include <osg/Matrixf>
#include <osg/Vec2b>
#include <osg/Vec2d>
#include <osg/Vec2f>
#include <osg/Vec2s>
#include <osg/Vec3b>
#include <osg/Vec3d>
#include <osg/Vec3f>
#include <osg/Vec3s>
#include <osg/Vec4b>
#include <osg/Vec4d>
#include <osg/Vec4f>
#include <osg/Vec4s>
#include <osg/Vec4ub>
#include <osg/Quat>
namespace osgTraits {
	typedef boost::mpl::set<::osg::Matrixd, ::osg::Matrixf> matrix_types;
	template<typename T> struct is_matrix : boost::mpl::has_key<matrix_types, T>::type {};
	typedef boost::mpl::set<::osg::Vec2b, ::osg::Vec2d, ::osg::Vec2f, ::osg::Vec2s, ::osg::Vec3b, ::osg::Vec3d, ::osg::Vec3f, ::osg::Vec3s, ::osg::Vec4b, ::osg::Vec4d, ::osg::Vec4f, ::osg::Vec4s, ::osg::Vec4ub> vector_types;
	template<typename T> struct is_vector : boost::mpl::has_key<vector_types, T>::type {};
	typedef boost::mpl::set<::osg::Quat> quat_types;
	template<typename T> struct is_quat : boost::mpl::has_key<quat_types, T>::type {};
} // end of namespace osgTraits
