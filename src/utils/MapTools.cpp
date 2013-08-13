#include <cstdlib>
#include <cmath>

#include "MapTools.h"

int MapTools::poi_in_circle(double x, double y, const poi_circle* circle) {
	double distance = get_distance(x, y, circle->cent_x, circle->cent_y);

	if (distance < circle->distance)
		return 1;

	return 0;
}

int MapTools::poi_in_rect(double x, double y, const poi_rect* rect) {
	if (x >= rect->left_top_x && x <= rect->right_bottom_x
			&& y >= rect->right_bottom_y && y <= rect->left_top_y)
		return 1;

	return 0;
}

int MapTools::get_poi_circle(poi_circle* circle, double cx, double cy,
		double distance) {
	circle->cent_x = cx;
	circle->cent_y = cy;
	circle->distance = distance;

	return 0;
}

int MapTools::get_poi_rect(poi_rect* rect, double x, double y,
		double distance) {
	return MapTools::get_poi_rect(rect, x, y, distance, distance);
}

int MapTools::get_poi_rect(poi_rect* rect, double x, double y,
		double x_distance, double y_distance) {
	double dx = x_distance / (6378137 * cos(x * 0.0174532925199433))
			* 57.2957805;
	double dy = y_distance / 6378137 * 57.2957805;

	rect->left_top_x = x - dx;
	rect->left_top_y = y + dy;

	rect->right_bottom_x = x + dx;
	rect->right_bottom_y = y - dy;

	return 0;
}

double MapTools::get_distance(double x1, double y1, double x2, double y2) {
	double lat_rad_1 = y1 * 0.0174532925199433;
	double lat_rad_2 = y2 * 0.0174532925199433;
	double lat_rad = lat_rad_1 - lat_rad_2;

	double lng_rad_1 = x1 * 0.0174532925199433;
	double lng_rad_2 = x2 * 0.0174532925199433;
	double lng_rad = lng_rad_1 - lng_rad_2;

	double f = 2.0
			* asin(
					sqrt(
							pow(sin(lat_rad / 2.0), 2.0)
									+ cos(lat_rad_1) * cos(lat_rad_2)
											* pow(sin(lng_rad / 2.0), 2.0)));

	return f * 6378137.0;
}

