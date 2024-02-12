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
    
    glm::vec3 renderPixel(const Scene& scene, const glm::vec2& p, const Camera& cam);

public:
    Image(int w, int h);
    ~Image();

    void render(const Scene& scene, const Camera& cam);
    void save(std::string path);
};
