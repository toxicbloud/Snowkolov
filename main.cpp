#include <iostream>
#include "src/Image.hpp"
#include "src/Camera.hpp"
#include "src/Sphere.hpp"
#include "src/Capsule.hpp"
#include "src/Scene.hpp"
#include "src/Material.hpp"

int main(int argc, char const *argv[])
{
    Scene scene;
    Image im(320, 240);
    Camera cam(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));

    // Create materials
    Material snowMaterial( glm::vec3( 1.0f, 0.95f,  0.9f), 1.0f, 0.1f, 0.0f );
    Material woodMaterial( glm::vec3(0.55f, 0.27f,  0.0f), 0.4f, 0.1f, 0.0f );
    Material rockMaterial( glm::vec3( 0.4f,  0.4f,  0.4f), 0.6f, 0.4f, 0.0f );

    // Add objects
        // Body
        scene.addObject(new Sphere(glm::vec3(0, 0, 0), 0.8f, snowMaterial));
        scene.addObject(new Sphere(glm::vec3(0, 1.0, 0), 0.6f, snowMaterial));
        scene.addObject(new Sphere(glm::vec3(0, 1.8, 0), 0.4f, snowMaterial));

        // Eyes
        scene.addObject(new Sphere(glm::vec3(-0.15, 1.9, -0.3), 0.08f, rockMaterial));
        scene.addObject(new Sphere(glm::vec3( 0.15, 1.9, -0.3), 0.08f, rockMaterial));

    float angle = 4.f;
    while (true)
    {
        im.render(scene, cam);
        im.save("./out.png");

        angle += deg2rad(20.f);
        cam.setPosition(glm::vec3(4 * cos(angle), 2.5, 4 * sin(angle)));
        cam.setRotation(glm::vec3(0, 1, 0)); // target position
    }
    return 0;
}
