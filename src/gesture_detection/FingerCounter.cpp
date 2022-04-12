#include "FingerCounter.h"
#include "iostream"

#define LIMIT_ANGLE_SUP 60
#define LIMIT_ANGLE_INF 5
#define BOUNDING_RECT_FINGER_SIZE_SCALING 0.3
#define BOUNDING_RECT_NEIGHBOR_DISTANCE_SCALING 0.05

FingerCounter::FingerCounter(void) {
    color_blue = Scalar(255, 0, 0);
    color_green = Scalar(0, 255, 0);
    color_red = Scalar(0, 0, 255);
    color_black = Scalar(0, 0, 0);
    color_white = Scalar(255, 255, 255);
    color_yellow = Scalar(0, 255, 255);
    color_purple = Scalar(255, 0, 255);
}

FingerAndCoordinates FingerCounter::findFingersCount(Mat input_image, Mat frame) {
    Mat contours_image = Mat::zeros(input_image.size(), CV_8UC3);

    // check if the source image is good
    if (input_image.empty())
        return {};

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours(input_image, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // we need at least one contour to work
    if (contours.empty())
        return {};

    // find the biggest contour (let's suppose it's our hand)
    int biggest_contour_index = -1;
    double biggest_area = 0.0;

    for (int i = 0; i < contours.size(); i++) {
        double area = contourArea(contours[i], false);
        if (area > biggest_area) {
            biggest_area = area;
            biggest_contour_index = i;
        }
    }

    if (biggest_contour_index < 0 || biggest_area < 100)
        return {};

    // find the convex hull object for each contour and the defects, two different data structure are needed by the OpenCV api
    vector<Point> hull_points;
    vector<int> hull_ints;

    // for drawing the convex hull and for finding the bounding rectangle
    convexHull(contours[biggest_contour_index], hull_points);
    convexHull(contours[biggest_contour_index],hull_ints,false, true);
    drawContours(frame, contours, biggest_contour_index, color_green, 2 );
    polylines(frame, hull_points, true, color_blue);

    // we need at least 3 points to find the defects
    vector<Vec4i> defects;
    if (hull_ints.size() > 3)
        convexityDefects(contours[biggest_contour_index], hull_ints, defects);
    else
        return {};

    int cnt = 0;

    // we separate the defects keeping only the ones of intrest
    Point start_point;
    Point far_point;
    Point end_point;

    for (int i = 0; i < defects.size(); i++) {
        start_point = contours[biggest_contour_index][defects[i].val[0]];
        end_point = contours[biggest_contour_index][defects[i].val[1]];
        far_point = contours[biggest_contour_index][defects[i].val[2]];
        double angle = findAngle(far_point,start_point,end_point);

        if(defects[i].val[3] > 1000 and angle <=CV_PI/2){
            cnt = cnt+1;
            circle(frame, end_point, 8, color_white, -1);
        }
    }
    if(cnt != 0) {
        Moments m = moments(contours[biggest_contour_index], true);
        Point center(m.m10/m.m00,m.m01/m.m00);
        Point farthestPoint = farthest_point(defects, contours[biggest_contour_index], center);
        circle(frame, farthestPoint, 8, color_yellow, -1);
        return {cnt, farthestPoint.x, farthestPoint.y};
    }
    return {};
}

Point FingerCounter::farthest_point(vector<Vec4i> defects, vector<Point>contour, Point centroid){
    int index = 0;
    int max_y = 0;
    for(int i; i < contour.size(); i++){
        if(contour[i].y > max_y){
            index = i;
        }
    }
    return contour[index];
}

double FingerCounter::findPointsDistance(Point a, Point b) {
    Point difference = a - b;
    return sqrt(difference.ddot(difference));
}

double FingerCounter::findAngle(Point a, Point b, Point c) {
    double ab = findPointsDistance(c, b);
    double bc = findPointsDistance(a, b);
    double ca = findPointsDistance(a, c);
    return acos((bc * bc + ca * ca - ab * ab) / (2 * bc * ca));
}