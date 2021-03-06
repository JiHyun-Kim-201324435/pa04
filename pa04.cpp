// pa04.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

// C++ program to implement Quick Hull algorithm
// to find convex hull.
#include <iostream>
//#include <bits/stdc++.h>
#include <set>
using namespace std;

// iPair is integer pairs
#define iPair pair<int, int>

// Stores the result (points of convex hull)
set<iPair> hull;

iPair min1, min2;

// Returns the side of point p with respect to line
// joining points p1 and p2.
int findSide(iPair p1, iPair p2, iPair p)
{
	int val = (p.second - p1.second) * (p2.first - p1.first) -
		(p2.second - p1.second) * (p.first - p1.first);

	if (val > 0)
		return 1;
	if (val < 0)
		return -1;
	return 0;
}

// Returns the square of distance between
// p1 and p2.
int dist(iPair p, iPair q)
{
	return (p.second - q.second) * (p.second - q.second) +
		(p.first - q.first) * (p.first - q.first);
}

// returns a value proportional to the distance
// between the point p and the line joining the
// points p1 and p2
int lineDist(iPair p1, iPair p2, iPair p)
{
	return abs((p.second - p1.second) * (p2.first - p1.first) -
		(p2.second - p1.second) * (p.first - p1.first));
}

// End points of line L are p1 and p2.  side can have value
// 1 or -1 specifying each of the parts made by the line L
void quickHull(iPair a[], int n, iPair p1, iPair p2, int side)
{
	int ind = -1;
	int max_dist = 0;

	// finding the point with maximum distance
	// from L and also on the specified side of L.
	for (int i = 0; i<n; i++)
	{
		int temp = lineDist(p1, p2, a[i]);
		if (findSide(p1, p2, a[i]) == side && temp > max_dist)
		{
			ind = i;
			max_dist = temp;
		}
	}

	// If no point is found, add the end points
	// of L to the convex hull.
	if (ind == -1)
	{
		hull.insert(p1);
		hull.insert(p2);
		return;
	}

	// Recur for the two parts divided by a[ind]
	quickHull(a, n, a[ind], p1, -findSide(a[ind], p1, p2));
	quickHull(a, n, a[ind], p2, -findSide(a[ind], p2, p1));
}

void printHull(iPair a[], int n)
{
	// a[i].second -> y-coordinate of the ith point
	if (n < 3)
	{
		cout << "Convex hull not possible\n";
		return;
	}

	// Finding the point with minimum and
	// maximum x-coordinate
	int min_x = 0, max_x = 0;
	for (int i = 1; i<n; i++)
	{
		if (a[i].first < a[min_x].first)
			min_x = i;
		if (a[i].first > a[max_x].first)
			max_x = i;
	}

	// Recursively find convex hull points on
	// one side of line joining a[min_x] and
	// a[max_x]
	quickHull(a, n, a[min_x], a[max_x], 1);

	// Recursively find convex hull points on
	// other side of line joining a[min_x] and
	// a[max_x]
	quickHull(a, n, a[min_x], a[max_x], -1);

	cout << "The points in Convex Hull are:\n";
	while (!hull.empty())
	{
		cout << "(" << (*hull.begin()).first << ", "
			<< (*hull.begin()).second << ") ";
		hull.erase(hull.begin());
	}
}

struct Point
{
	int x, y;
};


/* Following two functions are needed for library function qsort().
Refer: http://www.cplusplus.com/reference/clibrary/cstdlib/qsort/ */

// Needed to sort array of points according to X coordinate
int compareX(const void* a, const void* b)
{
	Point *p1 = (Point *)a, *p2 = (Point *)b;
	return (p1->x - p2->x);
}
// Needed to sort array of points according to Y coordinate
int compareY(const void* a, const void* b)
{
	Point *p1 = (Point *)a, *p2 = (Point *)b;
	return (p1->y - p2->y);
}

// A utility function to find the distance between two points
float dist(Point p1, Point p2)
{
	return sqrt((p1.x - p2.x)*(p1.x - p2.x) +
		(p1.y - p2.y)*(p1.y - p2.y)
	);
}

// A Brute Force method to return the smallest distance between two points
// in P[] of size n
float bruteForce(Point P[], int n)
{
	float min = FLT_MAX;
	for (int i = 0; i < n; ++i)
		for (int j = i + 1; j < n; ++j)
			if (dist(P[i], P[j]) < min)
				min = dist(P[i], P[j]);
	return min;
}

// A utility function to find minimum of two float values
float min(float x, float y)
{
	return (x < y) ? x : y;
}


// A utility function to find the distance beween the closest points of
// strip of given size. All points in strip[] are sorted accordint to
// y coordinate. They all have an upper bound on minimum distance as d.
// Note that this method seems to be a O(n^2) method, but it's a O(n)
// method as the inner loop runs at most 6 times
float stripClosest(Point strip[], int size, float d)
{
	float min = d;  // Initialize the minimum distance as d

					// Pick all points one by one and try the next points till the difference
					// between y coordinates is smaller than d.
					// This is a proven fact that this loop runs at most 6 times
	for (int i = 0; i < size; ++i)
		for (int j = i + 1; j < size && (strip[j].y - strip[i].y) < min; ++j)
			if (dist(strip[i], strip[j]) < min)
				min = dist(strip[i], strip[j]);

	return min;
}

// A recursive function to find the smallest distance. The array Px contains
// all points sorted according to x coordinates and Py contains all points
// sorted according to y coordinates
float closestUtil(Point Px[], Point Py[], int n)
{
	// If there are 2 or 3 points, then use brute force
	if (n <= 3)
		return bruteForce(Px, n);

	// Find the middle point
	int mid = n / 2;
	Point midPoint = Px[mid];


	// Divide points in y sorted array around the vertical line.
	// Assumption: All x coordinates are distinct.
	Point *Pyl, *Pyr;   
	Pyl = new Point[mid+1];        // y sorted points on left of vertical line
	Pyr = new Point[n - mid - 1];  // y sorted points on right of vertical line
	int li = 0, ri = 0;  // indexes of left and right subarrays
	for (int i = 0; i < n; i++)
	{
		if (Py[i].x <= midPoint.x)
			Pyl[li++] = Py[i];
		else
			Pyr[ri++] = Py[i];
	}

	// Consider the vertical line passing through the middle point
	// calculate the smallest distance dl on left of middle point and
	// dr on right side
	float dl = closestUtil(Px, Pyl, mid);
	float dr = closestUtil(Px + mid, Pyr, n - mid);

	// Find the smaller of two distances
	float d = min(dl, dr);

	// Build an array strip[] that contains points close (closer than d)
	// to the line passing through the middle point
	Point *strip;
	strip = new Point[n];
	int j = 0;
	for (int i = 0; i < n; i++)
		if (abs(Py[i].x - midPoint.x) < d)
			strip[j] = Py[i], j++;

	// Find the closest points in strip.  Return the minimum of d and closest
	// distance is strip[]
	return min(d, stripClosest(strip, j, d));
}

// The main functin that finds the smallest distance
// This method mainly uses closestUtil()
float closest(Point P[], int n)
{
	Point *Px, *Py;
	Px = new Point[n];
	Py = new Point[n];
	for (int i = 0; i < n; i++)
	{
		Px[i] = P[i];
		Py[i] = P[i];
	}

	qsort(Px, n, sizeof(Point), compareX);
	qsort(Py, n, sizeof(Point), compareY);

	// Use recursive function closestUtil() to find the smallest distance
	return closestUtil(Px, Py, n);
}


// Driver code
int main()
{
	iPair a[] = { { 0, 3 },{ 1, 1 },{ 2, 2 },{ 4, 4 },
	{ 0, 0 },{ 1, 2 },{ 3, 1 },{ 3, 3 } };
	int n = sizeof(a) / sizeof(a[0]);
	printHull(a, n);

	Point P[] = { { 2, 3 },{ 12, 30 },{ 40, 50 },{ 5, 1 },{ 12, 10 },{ 3, 4 } };
	n = sizeof(P) / sizeof(P[0]);
	cout << "The smallest distance is " << closest(P, n);
	return 0;
}