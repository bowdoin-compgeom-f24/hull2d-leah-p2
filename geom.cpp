#include "geom.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <iostream>
#include<algorithm>

#include<array>
#include<functional>
#include <stack>
#include <vector>

using namespace std; 

/* **************************************** */
/* returns the signed area of triangle abc. The area is positive if c
   is to the left of ab, and negative if c is to the right of ab
 */
int signed_area2D(point2d a, point2d b, point2d c) {
  float Ax = b.x - a.x; 
  float Ay = b.y - a.y; 
  float Bx = c.x - a.x; 
  float By = c.y - a.y; 
  return Ax*By - Ay*Bx;  
}

float epsilon = 0.1;

/* **************************************** */
/* return 1 if p,q,r collinear, and 0 otherwise */
int collinear(point2d p, point2d q, point2d r) {
  if (abs(signed_area2D(p, q, r)) < epsilon) { return 1; }
  else { return 0; }

}


/* **************************************** */
/* return 1 if c is  strictly left of ab; 0 otherwise */
int left_strictly(point2d a, point2d b, point2d c) {
  if (signed_area2D(a, b, c) > 0) { return 1; } 
  else { return 0; }
}


/* return 1 if c is left of ab or on ab; 0 otherwise */
int left_on(point2d a, point2d b, point2d c) {
  if (left_strictly(a,b,c) || collinear(a,b,c)) { return 1; }
  else { return 0; } 
}

// Global var p0 to use in compare_angle()
point2d p0;

// For sorting points CCW of p0 by polar angle
// Compares polar angle wrt p0 for two points, returns true if 1st point has larger angle wrt p0
bool compare_angle(point2d a, point2d b) {
  return atan2(a.y - p0.y, a.x - p0.x) < atan2(b.y - p0.y, b.x - p0.x);
}

// Compute the convex hull of pts, and store the points on the hull in hull
void graham_scan(vector<point2d>& pts, vector<point2d>& hull ) {

  printf("hull2d (graham scan): start\n"); 
  hull.clear(); //should be empty, but clear it to be safe

  //Find lowest point
  p0 = pts[0];
  for (int i = 1; i < pts.size(); i+=1) {
      if (pts[i].y < p0.y) {
        p0 = pts[i];
      }
  }
  
  // Sorts pts CCW by polar angle to p0
  sort(pts.begin(), pts.end(), compare_angle);

  // Initialize stack 
  stack<point2d> my_stack;
  my_stack.push(pts[0]);
  my_stack.push(pts[1]);
  my_stack.push(pts[2]);

  // Keeping track of top two points in stack
  point2d prev = pts[1];
  point2d curr = pts[2];

  // Traverse ordered points using Graham Scan and add to stack
  for (int i = 3; i < pts.size(); i++) {

    // Remove points until not making a right turn
    while(left_on(prev, curr, pts[i]) == 0) {
      curr = prev; 
      my_stack.pop(); // Remove curr
      my_stack.pop();
      prev = my_stack.top(); // Get new prev
      my_stack.push(curr); // Re-add old prev
    }
  
    if(collinear(prev, curr, pts[i])) { 
      // Bools to determine which collinear point should be removed... 
      bool c1 = prev.x < curr.x && curr.x < pts[i].x;
      bool c2 = prev.x > curr.x && curr.x > pts[i].x;
      bool c3 = prev.y < curr.y && curr.y < pts[i].y;
      bool c4 = prev.y > curr.y && curr.y > pts[i].y;

      bool b1 = curr.x < prev.x && prev.x < pts[i].x;
      bool b2 = curr.x > prev.x && prev.x > pts[i].x;
      bool b3 = curr.y < prev.y && prev.y < pts[i].y;
      bool b4 = curr.y > prev.y && prev.y > pts[i].y;

      // If any of ci conditions are met, remove curr point from hull & add pts[i]
      if((c4 || c2) || (c3 || c1)) {
        my_stack.pop(); // Remove curr
        my_stack.push(pts[i]); // Add pts[i]
        curr = pts[i];
      }

      // If any of bi conditions are met, remove prev point from hull & add pts[i]
      else if ((b1 || b2) || (b3 || b4)) {
        prev = curr;
        my_stack.pop();
        my_stack.pop();
        my_stack.push(prev);
        my_stack.push(pts[i]); // Add pts[i]
        curr = pts[i];
      } 
      else {} //
    }

    else { // If strictly left, always add pts[i]
        prev = curr;
        my_stack.push(pts[i]);
        curr = pts[i];
    }
  }

  // Add points to hull
  while (my_stack.size() > 0) {
    hull.push_back(my_stack.top());
    my_stack.pop();
  }

  printf("hull2d (graham scan): end\n"); 
  return; 
}

