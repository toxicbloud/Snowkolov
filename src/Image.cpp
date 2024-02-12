#include "Image.hpp"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb_image_write.h"

Image::Image(int w, int h)
    : width(w), height(h)
{
    this->frame = std::vector<glm::vec3>(w * h);
}

Image::~Image()
{
    
}

void Image::render(const Scene& scene, const Camera& cam)
{
    for (int x = 0; x < this->width; x++)
    {
        for (int y = 0; y < this->height; y++)
        {
            glm::vec2 pixel(
                x * 2.f / (float)this->width - 1.f,
                y * 2.f / (float)this->height - 1.f
            );
            pixel.x *= (float)this->width / (float)this->height; // aspect ratio
            this->frame[x + y * this->width] = renderPixel(scene, pixel, cam);
        }
    }
}

#define MAX_NB_ITER 50
glm::vec3 Image::renderPixel(const Scene& scene, const glm::vec2& p, const Camera& cam)
{
    float focale = 2.f;
    float prec = 0.0001f;

    glm::mat3 viewMatrix = cam.getViewMatrix();
    glm::vec3 dir = viewMatrix * glm::normalize(glm::vec3(p.x, p.y, focale));
    glm::vec3 point(cam.position);
    float t = focale;

    std::vector<Rendered*> objects = scene.getObjects();
    size_t nbObjects = objects.size();

    float minDist = MAXDIST;
    Rendered* object = nullptr;
    for (int i = 0; i < MAX_NB_ITER; i++)
    {
        for (size_t i = 0; i < nbObjects; i++)
        {
            Rendered* r = objects[i];
            float dist = r->distance(point);
            if (dist < minDist)
            {
                minDist = dist;
                object = r;
            }
        }

        if (abs(minDist) < prec * t) break;

        point += dir * minDist;
        t += minDist;
    }
    if (abs(minDist) < prec * t)
        return object->render();
    return glm::vec3(0.f, 0.f, 0.f);
}

void Image::save(std::string path)
{
    uint8_t* pixels = new uint8_t[width * height * 3];

    int index = 0;
    for (int j = height - 1; j >= 0; j--)
    {
        for (int i = 0; i < width; i++)
        {
            pixels[index++] = (uint8_t) (this->frame[i + j * width].x * 255.f);
            pixels[index++] = (uint8_t) (this->frame[i + j * width].y * 255.f);
            pixels[index++] = (uint8_t) (this->frame[i + j * width].z * 255.f);
        }
    }

    stbi_flip_vertically_on_write(1);
    stbi_write_png(path.c_str(), this->width, this->height, 3, pixels, this->width * 3);
}
