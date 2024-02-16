#include <iostream>
#include "src/Image.hpp"
#include "src/Camera.hpp"
#include "src/Sphere.hpp"
#include "src/Capsule.hpp"
#include "src/Torus.hpp"
#include "src/Scene.hpp"
#include "src/Material.hpp"
#include "src/SmoothUnion.hpp"
#include "src/Cone.hpp"

int main(int argc, char const *argv[])
{
    Scene scene;
    Image im(400, 400);
    Camera cam(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));

    // Create materials
    Material snowMaterial ( glm::vec3( 1.0f, 0.95f,  0.9f), 0.9f, 0.1f, 0.0f );
    Material furMaterial  ( glm::vec3( 1.0f,  1.0f,  1.0f), 1.0f, 0.1f, 0.0f );
    Material woodMaterial ( glm::vec3(0.55f, 0.27f,  0.0f), 0.8f, 0.1f, 0.0f );
    Material rockMaterial ( glm::vec3( 0.4f,  0.4f,  0.4f), 0.4f, 0.4f, 0.0f );
    Material carrotMaterial( glm::vec3( 1.0f,  0.5f,  0.0f), 0.9f, 0.1f, 0.0f );

    // Add objects
    // Body
    scene.addObject(new SmoothUnion(
        new Sphere(glm::vec3(0, 0, 0), 0.8f, snowMaterial),
        new Sphere(glm::vec3(0, 1.0, 0), 0.6f, snowMaterial),
        0.2f,
        snowMaterial));
    scene.addObject(new Sphere(glm::vec3(0, 1.8, 0), 0.4f, snowMaterial));

        // Eyes
        scene.addObject(new Sphere(glm::vec3(-0.15, 1.9, -0.3), 0.08f, rockMaterial));
        scene.addObject(new Sphere(glm::vec3( 0.15, 1.9, -0.3), 0.08f, rockMaterial));

        // Nose
        const float noseAngle = glm::radians(45.f);
        Cone nose(glm::vec3(0, 1.8, 0), glm::vec2(glm::sin(noseAngle), glm::cos(noseAngle)), 0.2f, carrotMaterial);
        nose.setRotation(glm::vec3(glm::radians(90.f), glm::radians(180.f), 0));
        scene.addObject(&nose);

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

        // Hat
        scene.addObject(new Torus(glm::vec3(0, 2.1, 0), 0.25f, 0.1f, furMaterial));

    float angle = 4.f;
    const bool loop = false;
    do
    {
        float radius = 3.f;
        glm::vec3 focus(0, 1.0, 0);
        glm::vec3 shift(radius * cos(angle), radius / 2, radius * sin(angle));
        cam.setPosition(focus + shift);
        cam.setRotation(focus);

        im.render(scene, cam);
        im.save("./out.png");

        angle += deg2rad(20.f);
    } while (loop);

    return 0;
}
