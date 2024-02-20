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
#include "src/Noise.hpp"

#define SNOW_NOISE(x) new Noise(x, 23.f, 0.01f, 768.0f)

int main(int argc, char const *argv[])
{
    Scene scene;
    Image im(200, 200);
    Camera cam(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));

    // Create materials
    Material snowMaterial   ( glm::vec3( 1.0f, 0.95f,  0.9f),  0.9f, 0.1f, 0.0f );
    Material furMaterial    ( glm::vec3( 1.0f,  1.0f,  1.0f),  1.0f, 0.1f, 0.0f );
    Material redMaterial    ( glm::vec3( 1.0f,  0.1f,  0.2f),  1.0f, 0.1f, 0.0f );
    Material woodMaterial   ( glm::vec3(0.55f, 0.27f,  0.0f),  0.8f, 0.1f, 0.0f );
    Material rockMaterial   ( glm::vec3( 0.4f,  0.4f,  0.4f),  0.4f, 0.4f, 0.0f );
    Material carrotMaterial ( glm::vec3(0.97f,  0.5f,  0.12f), 0.6f, 0.1f, 0.0f );

    // Add objects
        // Body
        scene.addObject(new SmoothUnion(
            SNOW_NOISE(new Sphere(glm::vec3(0, 0, 0), 0.8f, snowMaterial)),
            SNOW_NOISE(new Sphere(glm::vec3(0, 1.0, 0), 0.6f, snowMaterial)),
            0.2f,
            snowMaterial
        ));
        scene.addObject(SNOW_NOISE(new Sphere(glm::vec3(0, 1.8, 0), 0.4f, snowMaterial)));

        // Eyes
        scene.addObject(new Sphere(glm::vec3(-0.15, 1.9, -0.3), 0.08f, rockMaterial));
        scene.addObject(new Sphere(glm::vec3( 0.15, 1.9, -0.3), 0.08f, rockMaterial));

        // Nose
        scene.addObject(new Cone(glm::vec3(0, 1.8, -0.35f), glm::vec3(deg2rad(90), 0, 0), 0.05f, 0.2f, carrotMaterial));

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
        scene.addObject(new SmoothUnion(
            new Cone(glm::vec3(0, 2.21, 0), glm::vec3(0, 0, 0), 0.25f, 0.35f, redMaterial),
            new Capsule(glm::vec3(0, 2.46, 0), glm::vec3(-0.05, 2.56, 0.1), 0.07f, redMaterial),
            0.1f,
            redMaterial
        ));
        scene.addObject(new Sphere(glm::vec3(-0.12, 2.52, 0.14), 0.07f, snowMaterial));

    float angle = deg2rad(-90.f);
    const bool loop = false;
    do
    {
        float radius = 3.f;
        glm::vec3 focus(0, 1.6, 0);
        glm::vec3 shift(radius * cos(angle), radius / 2, radius * sin(angle));
        cam.setPosition(focus + shift);
        cam.setRotation(focus);

        im.render(scene, cam);
        im.save("./out.png");

        angle += deg2rad(10.f);
    } while (loop);

    return 0;
}
