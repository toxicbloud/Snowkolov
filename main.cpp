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
    Image im(160, 120);
    Camera cam(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));

    // Create materials
    Material snowMaterial( glm::vec3( 1.0f, 0.95f,  0.9f), 1.0f, 0.1f, 0.0f );
    Material woodMaterial( glm::vec3(0.55f, 0.27f,  0.0f), 0.8f, 0.1f, 0.0f );
    Material rockMaterial( glm::vec3( 0.4f,  0.4f,  0.4f), 0.4f, 0.4f, 0.0f );

    // Add objects
        // Body
        scene.addObject(new Sphere(glm::vec3(0, 0, 0), 0.8f, snowMaterial));
        scene.addObject(new Sphere(glm::vec3(0, 1.0, 0), 0.6f, snowMaterial));
        scene.addObject(new Sphere(glm::vec3(0, 1.8, 0), 0.4f, snowMaterial));

        // Eyes
        scene.addObject(new Sphere(glm::vec3(-0.15, 1.9, -0.3), 0.08f, rockMaterial));
        scene.addObject(new Sphere(glm::vec3( 0.15, 1.9, -0.3), 0.08f, rockMaterial));

        // Arms
        scene.addObject(new Capsule(glm::vec3( 0.5, 1.2, 0),    glm::vec3( 0.8, 1.3, -0.1), 0.05f, woodMaterial));
        scene.addObject(new Capsule(glm::vec3( 0.8, 1.3, -0.1), glm::vec3( 1.0, 1.5, -0.2), 0.05f, woodMaterial));
        scene.addObject(new Capsule(glm::vec3(-0.5, 1.3, 0),    glm::vec3(-0.8, 1.2, -0.0), 0.05f, woodMaterial));
        scene.addObject(new Capsule(glm::vec3(-0.8, 1.2, -0.0), glm::vec3(-1.2, 1.0, -0.1), 0.05f, woodMaterial));

        // Fingers
        scene.addObject(new Capsule(glm::vec3( 1.0, 1.5, -0.2), glm::vec3( 1.1, 1.7, -0.4),   0.03f, woodMaterial));
        scene.addObject(new Capsule(glm::vec3( 1.0, 1.5, -0.2), glm::vec3( 1.2, 1.6, -0.4),   0.03f, woodMaterial));
        scene.addObject(new Capsule(glm::vec3( 1.0, 1.5, -0.2), glm::vec3( 1.15, 1.5, -0.4), 0.03f, woodMaterial));

        scene.addObject(new Capsule(glm::vec3(-1.2, 1.0, -0.1), glm::vec3(-1.25, 1.2, -0.2), 0.03f, woodMaterial));
        scene.addObject(new Capsule(glm::vec3(-1.2, 1.0, -0.1), glm::vec3(-1.35, 1.1, -0.2), 0.03f, woodMaterial));
        scene.addObject(new Capsule(glm::vec3(-1.2, 1.0, -0.1), glm::vec3(-1.25, 0.9, -0.2), 0.03f, woodMaterial));

    float angle = 4.f;
    while (true)
    {
        float radius = 3;
        glm::vec3 focus(0.0, 1.0, 0.0);
        glm::vec3 shift(radius * cos(angle), radius / 2, radius * sin(angle));
        cam.setPosition(focus + shift);
        cam.setRotation(focus);
        
        im.render(scene, cam);
        im.save("./out.png");

        angle += deg2rad(20.f);
    }
    return 0;
}
