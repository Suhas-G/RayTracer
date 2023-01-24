#include <core/assert.h>
#include <core/scalar.h>
#include <core/matrix.h>
#include <core/image.h>
#include <rt/world.h>
#include <rt/renderer.h>
#include <rt/loaders/obj.h>
#include <rt/groups/bvh.h>
#include <rt/groups/simplegroup.h>
#include <rt/solids/sphere.h>
#include <rt/cameras/perspective.h>
#include <rt/integrators/casting.h>
#include <rt/integrators/raytrace.h>
#include <rt/lights/pointlight.h>
#include <rt/materials/dummy.h>
#include <rt/materials/flatmaterial.h>
#include <rt/textures/constant.h>
#include <rt/materials/lambertian.h>
#include <rt/materials/phong.h>
#include <rt/materials/combine.h>

using namespace rt;


rt::MatLib* buildMaterialLibrary() {
    ConstantTexture* blackTex = new ConstantTexture(RGBColor::rep(0.0f));

    LambertianMaterial *upperSphere = new LambertianMaterial(blackTex, new ConstantTexture(RGBColor(0.8, 0.105699, 0.108560)));
    LambertianMaterial *cube = new LambertianMaterial(blackTex, new ConstantTexture(RGBColor(0.467103, 0.8, 0.012934)));
    CombineMaterial *bottomSphere = new CombineMaterial();
    bottomSphere->add(new PhongMaterial(new ConstantTexture(RGBColor(0.742991, 0.742991, 0.742991)), 30), 0.5);
    bottomSphere->add(new LambertianMaterial(blackTex, new ConstantTexture(RGBColor(0.175987, 0.162798, 0.8))), 0.5);

    rt::MatLib* matlib = new rt::MatLib();
    matlib->insert(std::make_pair("Cube", cube));
    matlib->insert(std::make_pair("BottomSphere", bottomSphere));
    matlib->insert(std::make_pair("UpperSphere", upperSphere));

    return matlib;
}   

void a_trial() {
    Image img(600, 600);

    BVH* scene = new BVH();

    rt::MatLib *lib = buildMaterialLibrary();
    // loadOBJMat(lib, "models/", "cornell-box.mtl");
    loadOBJ(scene, "models/", "cornell-box.obj", lib);
    // DummyMaterial* mat = new DummyMaterial();
    // FlatMaterial *mat = new FlatMaterial(new ConstantTexture(RGBColor::rep(1.0f)));
    // scene->setMaterial(mat);
	scene->rebuildIndex();
    World world;
    world.scene = scene;

    std::cout << "Bounds : "<< scene->getBounds() << std::endl;

    PointLight *light = new PointLight(Point(-42.86680221557617, 12.695966720581055, 0.0), RGBColor::rep(1.0f));
    world.light.push_back(light);

    // Matrix rotation = Matrix(
    //         HomogeneousCoord(0, -1, 0, 0),
    //         HomogeneousCoord(0, 0, -1, 0),
    //         HomogeneousCoord(1, 0, 0, 0),
    //         HomogeneousCoord(0, 0, 0, 1)
    //         );
    Vector up =  Vector(0, 1, 0);
    Vector forward = Vector(1, 0, 0);

    PerspectiveCamera cam(Point(-33.35, 9, -0.2), forward, up, pi/3, pi/3);

    

    RayTracingIntegrator integrator(&world);
    Renderer engine(&cam, &integrator);
    // engine.setSamples(100);
    engine.render(img);
    img.writeEXR("trial-cornell-box.exr");

}