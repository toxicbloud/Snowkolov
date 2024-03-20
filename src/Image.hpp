#pragma once
#include "Camera.hpp"
#include "Scene.hpp"
#include <string>
#include <vector>

class Image
{
private:
    int width;
    int height;
    std::vector<glm::vec3> frame;
    int envmapWidth;
    int envmapHeight;
    std::vector<glm::vec3> envmapData;
    glm::vec3 noise[256*256];
    
    glm::vec3 renderPixel(const Scene& scene, const glm::vec2& p, const Camera& cam);
    glm::vec3 envmapPixel(const glm::vec3& d);
    glm::vec2 hash( glm::vec2 p );
    glm::vec3 voronoi(glm::vec2 &x,glm::vec2 &cpId );


public:
    Image(int w, int h);
    ~Image();

    void render(const Scene& scene, const Camera& cam);
    void save(std::string path);
};
