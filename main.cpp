#include <iostream>
#include "src/Image.hpp"
#include "src/Camera.hpp"
#include "src/Sphere.hpp"
#include "src/Scene.hpp"
#include "src/Material.hpp"

int main(int argc, char const *argv[])
{
    Scene scene;
    Image im(160, 120);
    Camera cam(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), 0.f);

    // Create materials
    Material snowMaterial( glm::vec3( 0.9f,  0.8f,  0.7f), 0.9f, 0.1f, 0.0f );
    Material woodMaterial( glm::vec3(0.55f, 0.27f,  0.0f), 0.4f, 0.1f, 0.0f );

    // Add objects
    scene.addObject(new Sphere(glm::vec3(0, 0, 0), 1.f, snowMaterial));
    scene.addObject(new Sphere(glm::normalize(glm::vec3(1, 1, -1)), 0.2f, woodMaterial));

    float angle = 0.f;
    while (true)
    {
        im.render(scene, cam);
        im.save("./out.png");

        angle += deg2rad(2.f);
        cam.setPosition(glm::vec3(4 * cos(angle), 2, 4 * sin(angle)));
        cam.setRotation(glm::vec3(0, 0, 0));
    }
    return 0;
}
