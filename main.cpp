#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <cstdio>
#include <limits>
#include <iostream>
#include <fstream>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define PATH_MAX 2000
const float sphere_radius = 1.5;   // all the explosion fits in a sphere with this radius. The center lies in the origin.
const float noise_amplitude = 1.0; // amount of noise applied to the sphere (towards the center)

inline void mainImage(glm::vec4 &fragColor, const glm::vec2 &fragCoord);
glm::mat3 construitCamera( const glm::vec3 &co, const glm::vec3 &ci, float ar);

float hash(const float n)
{
  float x = sin(n) * 43758.5453f;
  return x - floor(x);
}

template <typename T>
inline T lerp(const T &v0, const T &v1, float t)
{
  return v0 + (v1 - v0) * std::max(0.f, std::min(1.f, t));
}

float noise(const glm::vec3 &x)
{
  glm::vec3 p(floor(x.x), floor(x.y), floor(x.z));
  glm::vec3 f(x.x - p.x, x.y - p.y, x.z - p.z);
  f = f * (f * (glm::vec3(3.f, 3.f, 3.f) - f * 2.f));
  float n = glm::dot(p, glm::vec3(1.f, 57.f, 113.f));
  return lerp(lerp(
                  lerp(hash(n + 0.f), hash(n + 1.f), f.x),
                  lerp(hash(n + 57.f), hash(n + 58.f), f.x), f.y),
              lerp(
                  lerp(hash(n + 113.f), hash(n + 114.f), f.x),
                  lerp(hash(n + 170.f), hash(n + 171.f), f.x), f.y),
              f.z);
}

glm::vec3 rotate(const glm::vec3 &v)
{
  return glm::vec3(glm::dot(glm::vec3(0.00f, 0.80f, 0.60f), v),
                   glm::dot(glm::vec3(-0.80f, 0.36f, -0.48f), v),
                   glm::dot(glm::vec3(-0.60f, -0.48f, 0.64f), v));
}
float fractal_brownian_motion(const glm::vec3 &x)
{
  glm::vec3 p = rotate(x);
  float f = 0;
  f += 0.5000 * noise(p);
  p = p * 2.32f;
  f += 0.2500 * noise(p);
  p = p * 3.03f;
  f += 0.1250 * noise(p);
  p = p * 2.61f;
  f += 0.0625 * noise(p);
  return f / 0.9375f;
}

glm::vec3 palette_fire(const float d)
{                                        // simple linear gradent yellow-orange-red-darkgray-gray. d is supposed to vary from 0 to 1
  const glm::vec3 yellow(1.7, 1.3, 1.0); // note that the color is "hot", i.e. has components >1
  const glm::vec3 orange(1.0, 0.6, 0.0);
  const glm::vec3 red(1.0, 0.0, 0.0);
  const glm::vec3 darkgray(0.2, 0.2, 0.2);
  const glm::vec3 gray(0.4, 0.4, 0.4);

  float x = std::max(0.f, std::min(1.f, d));
  if (x < .25f)
    return lerp(gray, darkgray, x * 4.f);
  else if (x < .5f)
    return lerp(darkgray, red, x * 4.f - 1.f);
  else if (x < .75f)
    return lerp(red, orange, x * 4.f - 2.f);
  return lerp(orange, yellow, x * 4.f - 3.f);
}

float signed_distance(const glm::vec3 &p)
{ // this function defines the implicit surface we render
  float displacement = -fractal_brownian_motion(p * 3.4f) * noise_amplitude;
  return glm::length(p) - (sphere_radius + displacement);
}

bool sphere_trace(const glm::vec3 &orig, const glm::vec3 &dir, glm::vec3 &pos)
{ // Notice the early discard; in fact I know that the noise() function produces non-negative values,
  pos = orig;
  for (size_t i = 0; i < 128; i++)
  {
    float d = signed_distance(pos);
    if (d < 0)
      return true;
    pos = pos + dir * std::max(d * 0.1f, .01f); // note that the step depends on the current distance, if we are far from the surface, we can do big steps
  }
  return false;
}

glm::vec3 distance_field_normal(const glm::vec3 &pos)
{ // simple finite differences, very sensitive to the choice of the eps constant
  const float eps = 0.1;
  float d = signed_distance(pos);
  float nx = signed_distance(pos + glm::vec3(eps, 0, 0)) - d;
  float ny = signed_distance(pos + glm::vec3(0, eps, 0)) - d;
  float nz = signed_distance(pos + glm::vec3(0, 0, eps)) - d;
  return glm::normalize(glm::vec3(nx, ny, nz));
}

int main()
{
  const int width = 640 ;   // image width
  const int height = 480 ;  // image height
  const float fov = M_PI / 3.; // field of view angle
  std::vector<glm::vec3> framebuffer(width * height);

#pragma omp parallel for
  for (size_t j = 0; j < height; j++)
  { // actual rendering loop
    for (size_t i = 0; i < width; i++)
    {
      float dir_x = (i + 0.5) - width / 2.;
      float dir_y = -(j + 0.5) + height / 2.; // this flips the image at the same time
      float dir_z = -height / (2. * tan(fov / 2.));
      glm::vec4 col;
      mainImage(col, glm::vec2(i, j));
      framebuffer[i + j * width] = glm::vec3(col);
      // glm::vec3 hit;
      // std::vector<glm::vec3> spheres = {glm::vec3(0, 0, 3), glm::vec3(2, 0, 3)};

      // if (sphere_trace(glm::vec3(0, 0, 3), glm::normalize(glm::vec3(dir_x, dir_y, dir_z)), hit))
      // { // the camera is placed to (0,0,3) and it looks along the -z axis
      //   float noise_level = (sphere_radius - glm::length(hit)) / noise_amplitude;
      //   glm::vec3 light_dir = glm::normalize(glm::vec3(10, 10, 10) - hit); // one light is placed to (10,10,10)
      //   float light_intensity = std::max(0.4f, glm::length(light_dir * distance_field_normal(hit)));
      //   framebuffer[i + j * width] = palette_fire((-.2 + noise_level) * 2) * light_intensity;
      // }
      // else
      // {
      //   framebuffer[i + j * width] = glm::vec3(0.2, 0.7, 0.8); // background color
      // }
    }
  }

  uint8_t* pixels = new uint8_t[width * height * 3];

    int index = 0;
    for (int j = height - 1; j >= 0; --j)
    {
        for (int i = 0; i < width; ++i)
        {
            float r = (float) framebuffer[i+j*width].x;
            float g = (float) framebuffer[i+j*width].y;
            float b = (float) framebuffer[i+j*width].z;
            int ir = int(255.99 * r);
            int ig = int(255.99 * g);
            int ib = int(255.99 * b);

            pixels[index++] = ir;
            pixels[index++] = ig;
            pixels[index++] = ib;
        }
    }

    stbi_write_png("out.png", width, height, 3, pixels, width * 3);

    std::ofstream ofs("./out.ppm", std::ios::binary); // save the framebuffer to file
  ofs << "P6\n"
      << width << " " << height << "\n255\n";
  for (size_t i = 0; i < height * width; ++i)
  {
    for (size_t j = 0; j < 3; j++)
    {
      ofs << (char)(std::max(0, std::min(255, static_cast<int>(255 * framebuffer[i][j]))));
    }
  }
  ofs.close();

  return 0;
}

inline void mainImage(glm::vec4 &fragColor, const glm::vec2 &fragCoord)
{
  fragColor = glm::vec4(cos(fragCoord.x), cos(fragCoord.y), fragCoord.x-fragCoord.y, 1.0);

    // Définition de la caméra à partir de son point cible
    glm::vec3 cibleCamera = glm::vec3(0.0); // Point visé par la caméra (centre monde par défaut)
    float focale = 2.0;            // Distance focale
    float angle = 0.25f / 2.0;   // Angle de positionnement de la caméra autour du point cible (rotation sur verticale)
    float distanceCamera = 10.0;   // Distance de la caméra au point cible
    float angleRoulis = 0.0;       // Angle de roulis autour de l'axe optique
    // Position du centre optique de la caméra avec prise en compte de la souris
    glm::vec3 centreCamera = cibleCamera + glm::vec3(distanceCamera * cos(angle + M_PI),  // ... Compléter la rotation avec la position x de la souris
                                           10.0 , distanceCamera * sin(angle + M_PI)); // ... Compléter la rotation avec la position x de la souris
    float distanceCameraCible = distance(centreCamera, cibleCamera);

    // Construction de la matrice Caméra vers Monde
    glm::mat3 camera = construitCamera(centreCamera, cibleCamera, angleRoulis);


}

glm::mat3 construitCamera( const glm::vec3 &co, const glm::vec3 &ci, float ar) {
    glm::vec3 cw,cp,cu,cv;
    // calcul de la direction du regard
    cw = normalize(ci - co);
    // calcul de la rotation de la verticale du monde par le roulis caméra ar
    // cp = vec3(cos(ar), sin(ar), 0.0);
    cp = glm::vec3(0.0, 1.0, 0.0);
    // calcul de l'axe X caméra
    cu = normalize(cross(cw, cp));
    // calcul de l'axe Y caméra
    cv = cross(cu, cw);
    return glm::mat3(cu, cv, cw);
}