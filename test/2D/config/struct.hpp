#pragma once

struct dvec2{
  double x,y;
  bool operator==(const dvec2& v){
    return (v.x==x && v.y==y);
  }
};

struct dvec3{
  double x,y,z;
  bool operator==(const dvec3& v){
    return (v.x==x && v.y==y && v.z==z);
  }
};