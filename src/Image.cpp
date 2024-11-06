#include "Image.hpp"
#include "Rendered.hpp"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb_image_write.h"

#include <glm/gtc/noise.hpp>
#include <glm/gtc/constants.hpp>

#define PI 3.1415926f

Image::Image(int w, int h)
    : width(w), height(h)
{
    this->frame = std::vector<glm::vec3>(w * h);
    std::string envmapPath = "./envmap.jpg";
    int n = -1;
    unsigned char* data = stbi_load(envmapPath.c_str(), &envmapWidth, &envmapHeight, &n, 0);
    if (!data || n != 3)
    {
        std::cerr << "Error: failed to load envmap" << std::endl;
        exit(1);
    }
    envmapData = std::vector<glm::vec3>(envmapWidth * envmapHeight);
    for (int i = 0; i < envmapWidth * envmapHeight; i++)
    {
        envmapData[i] = glm::vec3(
            (float) data[i * 3] / 255.f,
            (float) data[i * 3 + 1] / 255.f,
            (float) data[i * 3 + 2] / 255.f
        );
    }

    for( int j=0; j<256; j++ )
    for( int i=0; i<256; i++ )
    {
        glm::vec2 uv = (glm::vec2(i,j)+0.5f)/256.0f;
        this->noise[i+j*256] = glm::vec3(rand()/(float)RAND_MAX,rand()/(float)RAND_MAX,rand()/(float)RAND_MAX);
    }
}

Image::~Image()
{
}

void Image::render(const Scene& scene, const Camera& cam)
{
    #pragma omp parallel for
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

glm::vec2 Image::hash( glm::vec2 p )
{    
    int indexX = int(p.x*255.0) % 256;
    int indexY = int(p.y*255.0) % 256;
    if (indexX < 0) indexX += 256;
    if (indexY < 0) indexY += 256;
    return glm::vec2(this->noise[indexX + indexY * 256].x, this->noise[indexX + indexY * 256].y);
}
/**
 * Thanks to iq course
 */
glm::vec3 Image::voronoi(glm::vec2 &x,glm::vec2 &cpId )
{
    glm::vec2 n = glm::floor(x);
    glm::vec2 f = glm::fract(x);

    //----------------------------------
    // first pass: regular voronoi
    //----------------------------------
	glm::vec2 mg, mr;

    float md = 8.0;
    for( int j=-1; j<=1; j++ )
    for( int i=-1; i<=1; i++ )
    {
        glm::vec2 g = glm::vec2(float(i),float(j));
		glm::vec2 o = this->hash( n + g );
        glm::vec2 r = g + o - f;
        float d = glm::dot(r,r);
        if( d<md )
        {
            md = d;
            mr = r;
            mg = g;
        }
    }

    //----------------------------------
    // second pass: distance to borders
    //----------------------------------
    md = 8.0;
    for( int j=-2; j<=2; j++ )
    for( int i=-2; i<=2; i++ )
    {
        glm::vec2 g = mg + glm::vec2(float(i),float(j));
		glm::vec2 o = hash( n + g );	
        glm::vec2 r = g + o - f;

        if( glm::dot(mr-r,mr-r)>0.000001 )
		{
        // distance to line		
        float d = glm::dot( 0.5f*(mr+r), glm::normalize(r-mr) );
        md = glm::min( md, d );
		}
    }
	
	cpId = n+mg;

    return glm::vec3( md, mr );
}
/**
 * convert -1 1 sinus to 0 1 sinus
 */
inline float sin01(float theta)
{
    return sin(theta) * 0.5 + 0.5;
}

inline float boxfilter( float x, float min, float max )
{
	if( x < min || x > max )
		return 0.0;
	else
		return x;
}

#define MAX_DIST 10.f
#define MAX_NB_ITER 300
glm::vec3 Image::renderPixel(const Scene& scene, const glm::vec2& p, const Camera& cam)
{
    float focale = 2.f;
    float prec = 0.002f;

    glm::mat3 viewMatrix = cam.getViewMatrix();
    glm::vec3 dir = viewMatrix * glm::normalize(glm::vec3(p.x, p.y, focale));
    glm::vec3 point(cam.getPosition());
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
            float dist = r->getDistance(point);
            if (dist < minDist)
            {
                minDist = dist;
                object = r;
            }
        }
        
        if (abs(minDist) < prec * t || t > MAX_DIST) break;
        
        point += dir * minDist;
        t += minDist;
    }

    // snowflakes and envmap handling
    glm::vec3 color;
    glm::vec2 cpid;
    glm::vec2 zoomed = p / 0.2f;
    glm::vec3 c = voronoi(zoomed, cpid);

    float centerDist = glm::length(glm::vec2(c.y, c.z));
    float borderDist = c.x;

    float size = glm::mix(0.1, 0.20, sin01(cpid.x - cpid.y));

    // get angle to closest point
    float angle = glm::atan(c.z, c.y);
    float angvel = glm::sin(cpid.x * 123.0 + cpid.y * 451.0) * 0.5 * glm::pi<float>();

    float numpeds = glm::floor(glm::mix(5.0, 9.0, sin01(cpid.x + cpid.y)));
    float pedval = sin01(angle * numpeds);
    color = glm::mix((abs(minDist) < prec * t) ? object->render(point, dir) : envmapPixel(dir), glm::vec3(1.f), pow(pedval, 4.0));

    // some radial details
    float numrings = glm::floor(glm::mix(1.0f, 4.0f, sin01(cpid.x * 100.0 + cpid.y * 42.0f)));
    float ringsharp = 2.0f;
    float warpdist = glm::mix(centerDist * 0.8f, centerDist, pedval);
    float ringval = sin01(warpdist / (size * 0.8f) * 2.0f * glm::pi<float>() * numrings - glm::pi<float>() * 0.5f);
    color = glm::mix(color, glm::vec3(1.0f), pow(ringval, ringsharp));
    color = glm::mix(color, (abs(minDist) < prec * t) ? object->render(point, dir) : envmapPixel(dir), glm::smoothstep(size * 0.8f, size * 1.0f, centerDist));

    return color;
}

glm::vec3 Image::envmapPixel(const glm::vec3& dir)
{
    float u = 0.5f + (float) atan2(dir.z, dir.x) / (2.f * PI);
    float v = 0.5f - (float) asin(dir.y) / PI;

    int x = (int) (u * envmapWidth);
    int y = (int) (v * envmapHeight);
    int index = (x + y * envmapWidth);

    return envmapData[index];
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
