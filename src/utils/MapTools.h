#ifndef DH_MAP_TOOLS
#define DH_MAP_TOOLS

struct poi_rect
{
    double left_top_x;
    double left_top_y;
    double right_bottom_x;
    double right_bottom_y;

    poi_rect()
        :left_top_x(0.0), left_top_y(0.0), right_bottom_x(0.0), right_bottom_y(0.0)
    {
    }

    poi_rect(double _lx, double _ly, double _rx, double _ry)
        :left_top_x(_lx), left_top_y(_ly), right_bottom_x(_rx), right_bottom_y(_ry)
    {
    }
};

struct poi_circle
{
    double cent_x;
    double cent_y;
    double distance;

    poi_circle()
        :cent_x(0.0), cent_y(0.0), distance(0.0)
    {
    }

    poi_circle(double _cx, double _cy, double _ds)
        :cent_x(_cx), cent_y(_cy), distance(_ds)
    {
    }

};

class MapTools
{
public:
    static int poi_in_rect(double x, double y, const poi_rect* rect);
    static int poi_in_circle(double x, double y, const poi_circle* circle);

    static int get_poi_circle(poi_circle* circle, double cx, double cy, double distance);
    static int get_poi_rect(poi_rect* rect, double cx, double cy, double distance);
    static int get_poi_rect(poi_rect* rect, double cx, double cy, double x_distance, double y_distance);

    static double get_distance(double x1, double y1, double x2, double y2);
};

#endif
