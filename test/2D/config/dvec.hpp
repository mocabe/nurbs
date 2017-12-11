#pragma once

struct vec2{
  double x,y;
  bool operator==(const vec2& v) const{
    return (v.x==x && v.y==y);
  }
};

struct vec3{
  double x,y,z;
  bool operator==(const vec3& v) const{
    return (v.x==x && v.y==y && v.z==z);
  }
};