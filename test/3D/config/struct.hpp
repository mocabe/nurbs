#pragma once

struct dvec3{
  double x,y,z;
  bool operator==(const dvec3& v) const{
    return (v.x==x && v.y==y && v.z==z);
  }
};

struct dvec4{
  double x,y,z,w;
  bool operator==(const dvec4& v) const{
    return (v.x==x && v.y==y && v.z==z && v.w == w);
  }
};