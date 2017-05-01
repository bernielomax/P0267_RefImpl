//#define _HAS_CXX17 1
#include "io2d.h"
#include "xio2dhelpers.h"
#include "xcairoenumhelpers.h"

using namespace std;
using namespace std::experimental::io2d;

namespace std {
	namespace experimental {
		namespace io2d {
			inline namespace v1 {
				const ::std::error_category& io2d_category() noexcept {
					static io2d_error_category ec;
					return ec;
				}

				display_surface make_display_surface(int preferredWidth, int preferredHeight, format preferredFormat, scaling scl, refresh_rate rr, double desiredFramerate) {
					return { preferredWidth, preferredHeight, preferredFormat, scl, rr, desiredFramerate };
				}

				display_surface make_display_surface(int preferredWidth, int preferredHeight, format preferredFormat, int preferredDisplayWidth, int preferredDisplayHeight, scaling scl, refresh_rate rr, double desiredFramerate) {
					return { preferredWidth, preferredHeight, preferredFormat, preferredDisplayWidth, preferredDisplayHeight,scl, rr, desiredFramerate };
				}

				image_surface make_image_surface(format fmt, int width, int height) {
					return image_surface(fmt, width, height);
				}

				image_surface make_image_surface(image_surface& sfc) {
					image_surface retval(sfc.format(), sfc.width(), sfc.height());
					retval.map([&sfc](mapped_surface& rvms) {
						sfc.map([&rvms](mapped_surface& sfcms) {
							memcpy(rvms.data(), sfcms.data(), static_cast<size_t>(rvms.height() * rvms.stride()));
						});
					});
					retval.mark_dirty();
					return retval;
				}

				vector_2d point_for_angle(const vector_2d & ctr, double ang, double mgn) {
					vector_2d v{ mgn, 0.0 };
					auto m = matrix_2d::init_rotate(ang);
					return ctr + m.transform_point(v);
				}

				double angle_for_point(const vector_2d& ctr, const vector_2d& pt) {
					auto xDiff = pt.x() - ctr.x();
					auto yDiff = pt.y() - ctr.y();
					//auto xDiff = ctr.x() - pt.x();
					//auto yDiff = ctr.y() - pt.y();
					auto angle = atan2(yDiff, xDiff);
					if ((abs(angle) < numeric_limits<double>::epsilon() * 100.0) || (angle > 0.0 && (abs(angle - two_pi<double>) < numeric_limits<double>::epsilon() * 100.0))) {
						return 0.0;
					}
					if (angle < 0.0) {
						//return angle + two_pi<double>;
						//return (angle - two_pi<double>) + two_pi<double>;
						return abs(angle);
					}
					return two_pi<double> - angle;
					//auto result = atan2(abs(yDiff), abs(xDiff));
					//if (xDiff < 0.0) {
					//	if (yDiff >= 0.0) {
					//		return result + pi<double>;
					//	}
					//	else {
					//		return result + half_pi<double>;
					//	}
					//}
					//else {
					//	if (yDiff >= 0.0) {
					//		return result + three_pi_over_two<double>;
					//	}
					//	// else leave it alone, it's already in the 0 to two pi range.
					//}
					//return result;
				}

				int format_stride_for_width(format format, int width) noexcept {
					return cairo_format_stride_for_width(_Format_to_cairo_format_t(format), width);
				}
			}
		}
	}
}

namespace std {
	::std::error_condition make_error_condition(io2d_error err) noexcept {
		return ::std::error_condition(static_cast<int>(err), io2d_category());
	}

	::std::error_code make_error_code(io2d_error e) noexcept {
		return ::std::error_code(static_cast<int>(e), io2d_category());
	}

}

