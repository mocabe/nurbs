#pragma once

struct vec3{
  double x,y,z;
  bool operator==(const vec3& v) const{
    return (v.x==x && v.y==y && v.z==z);
  }
};

struct vec4{
  double x,y,z,w;
  bool operator==(const vec4& v) const{
    return (v.x==x && v.y==y && v.z==z && v.w == w);
  }
};