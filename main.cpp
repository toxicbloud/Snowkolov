#include <iostream>
#include "src/Image.hpp"
#include "src/Camera.hpp"
#include "src/Sphere.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
    Scene scene;
    Image im(160, 90);
    Camera cam(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), 0.f);

    // Add objects
    scene.addObject(new Sphere(glm::vec3(0, 0, 0), 1.f));

    im.render(scene, cam);
    im.save("./out.png");
    return 0;
}
