// Code inspired by https://stackoverflow.com/a/3842240
//    which cites Joe O'Rourke's "Computational Geometry in C"

#define X 0;
#define Y 1;
typedef int Point[2];

int Area2(Point a, Point b, Point c);
bool intersects(Point start1, Point end1, Point start2, Point end2);
bool oppositeSign(int a, int b);

bool intersects(Point start1, Point end1, Point start2, Point end2) {
    bool with_seg1_anchor = oppositeSign(Area2(start1, end1, start2), Area2(start1, end1, end2));
    bool with_seg2_anchor = oppositeSign(Area2(start2, end2, start1), Area2(start2, end2, end1));
    return with_seg1_anchor && with_seg2_anchor;
}

bool oppositeSign(int a, int b) {
    if (a < 0)
        return (b > 0);
    else if (b > 0)
        return (b < 0);
    else
        // Returns false if either a == 0 or b == 0
        // This disallows improper intersections
        return false;
}

// calculates the triangle's size (formed by the "anchor" segment and additional point)
int Area2(Point a, Point b, Point c)
{
     return (b[X] - a[X]) * (c[Y] - a[Y]) -
            (c[X] - a[X]) * (b[Y] - a[Y]);
}