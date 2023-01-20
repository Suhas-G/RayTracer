#include <core/point.h>
#include <core/vector.h>
#include <core/color.h>
#include <core/image.h>

#include <rt/intersection.h>
#include <rt/world.h>
#include <rt/renderer.h>


#include <rt/cameras/perspective.h>
#include <rt/groups/simplegroup.h>

#include <rt/solids/triangle.h>
#include <rt/solids/sphere.h>
#include <rt/solids/environment.h>
#include <rt/solids/quad.h>

#include <rt/integrators/raytrace.h>


#include <rt/coordmappers/coordmapper.h>
#include <rt/coordmappers/tmapper.h>
#include <rt/coordmappers/spherical.h>
#include <rt/coordmappers/environment.h>


#include <rt/textures/imagetex.h>
#include <rt/textures/checkerboard.h>
#include <rt/textures/perlin.h>
#include <rt/textures/constant.h>

#include <rt/materials/material.h>
#include <rt/materials/flatmaterial.h>
#include <rt/materials/lambertian.h>
#include <rt/materials/phong.h>
#include <rt/materials/combine.h>

#include <rt/lights/pointlight.h>
#include <rt/lights/directional.h>

using namespace rt;



void environment_mapper() {
    static const float scale = 0.001f;
    Image img(400, 400);
    World world;
    SimpleGroup scene;
    world.scene = &scene;

    PerspectiveCamera cam(Point(278*scale, 273*scale, -800*scale), Vector(0, 0, 1), Vector(0, 1, 0), 0.686f, 0.686f);

    
    CheckerboardTexture* redtex = new CheckerboardTexture(RGBColor(.7f,0.1f,0.1f), RGBColor(0.3f,0.1f,0.1f));
    ConstantTexture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));

    ImageTexture* whitetex = new ImageTexture("models/stones_diffuse.png");
    ImageTexture* clamptex = new ImageTexture("models/stones_diffuse.png", ImageTexture::CLAMP);
    ImageTexture* mirrtex = new ImageTexture("models/stones_diffuse.png", ImageTexture::MIRROR);

    LambertianMaterial white(blacktex, whitetex);
    LambertianMaterial clamp(blacktex, clamptex);
    LambertianMaterial mirror(blacktex, mirrtex);

    LambertianMaterial red(blacktex, redtex);

    //point light
    world.light.push_back(new PointLight(Point((178)*scale,429.99f*scale,(279.5f)*scale),RGBColor::rep(50000.0f*scale*scale)));
    world.light.push_back(new PointLight(Point(478*scale,229.99f*scale,(-59.5f)*scale),RGBColor::rep(15000.0f*scale*scale)));

    world.light.push_back(new PointLight(Point(490*scale,159.99f*scale,279.5f*scale),RGBColor(4000.0f*scale*scale,0,0)));
    world.light.push_back(new PointLight(Point(40*scale,159.99f*scale,249.5f*scale),RGBColor(500.0f*scale*scale,3000.0f*scale*scale,500.0f*scale*scale)));

    DirectionalLight dirl(Vector(0.2f,-0.5f,0.5f).normalize(), RGBColor(1000000.0f*scale*scale,0,0));
    world.light.push_back(&dirl);

    TriangleMapper* bottomleft = new TriangleMapper(Point(0,0,0), Point(0,3,0), Point(3,0,0));
    TriangleMapper* topright = new TriangleMapper(Point(3,3,0), Point(3,0,0), Point(0,3,0));

    float hsq2 = 0.5f / std::sqrt(2.0f);
    SphericalCoordMapper* sphere1 = new SphericalCoordMapper(Point(.4f,.45f,.3f),Vector(0.0f,hsq2,hsq2),Vector(0.5f,0.0f,0.0f));
    SphericalCoordMapper* sphere2 = new SphericalCoordMapper(Point(.3f,.1f,.3f),Vector(0.0f,hsq2,-hsq2),Vector(0.5f,0.0f,0.0f));
    //floor
    scene.add(new Triangle(Point(000.f,000.f,000.f)*scale, Point(000.f,000.f,560.f)*scale, Point(550.f,000.f,000.f)*scale, bottomleft, &clamp));
    scene.add(new Triangle(Point(550.f,000.f,560.f)*scale, Point(550.f,000.f,000.f)*scale, Point(000.f,000.f,560.f)*scale, topright, &clamp)); 


    //left wall
    scene.add(new Triangle(Point(550.f,000.f,000.f)*scale, Point(550.f,000.f,560.f)*scale, Point(550.f,550.f,000.f)*scale, bottomleft, &red));
    scene.add(new Triangle(Point(550.f,550.f,560.f)*scale, Point(550.f,550.f,000.f)*scale, Point(550.f,000.f,560.f)*scale, topright, &red));

    //sphere
    scene.add(new Sphere(Point(400.f,450.f,300.f)*scale, 150.f*scale, sphere1, &white));
    scene.add(new Sphere(Point(200.f,100.f,300.f)*scale, 150.f*scale, sphere2, &white));

    EnvironmentMapper* environmentMapper = new EnvironmentMapper(2, 1);
    ImageTexture* environmentTexture = new ImageTexture("models/thatch_chapel_4k.png", ImageTexture::REPEAT, ImageTexture::BILINEAR);
    FlatMaterial environmentMaterial(environmentTexture);
    
    // environment
    
    scene.add(new Environment(environmentMapper, &environmentMaterial));

    RayTracingIntegrator integrator(&world);

    Renderer engine(&cam, &integrator);
    engine.setSamples(16);
    engine.render(img);
    img.writeEXR("mapper-environment-view1.exr");
}

static void makeBox(Group* scene, const Point& aaa, const Vector& forward, const Vector& left, const Vector& up, CoordMapper* texMapper, Material* material) {
    scene->add(new Quad(aaa, left, forward, texMapper, material));
    scene->add(new Quad(aaa, forward, up, texMapper, material));
    scene->add(new Quad(aaa, up, left, texMapper, material));
    Point bbb = aaa + forward + left + up;
    scene->add(new Quad(bbb, -forward, -left, texMapper, material));
    scene->add(new Quad(bbb, -up, -forward, texMapper, material));
    scene->add(new Quad(bbb, -left, -up, texMapper, material));
}


void combined_materials(float scale) {
    Image img(400, 400);
    World world;
    SimpleGroup* scene = new SimpleGroup();
    world.scene = scene;

    PerspectiveCamera cam(Point(278*scale, 273*scale, -800*scale), Vector(0, 0, 1), Vector(0, 1, 0), 0.686f, 0.686f);


    Texture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));
    Texture* whitetex = new ConstantTexture(RGBColor::rep(1.0f));


    PerlinTexture* perlinTex = new PerlinTexture(RGBColor(1.0f,1.0f,0.9f), RGBColor(0.5f,0.5f,1.0f));
    perlinTex->addOctave(20.0f, 50.0f);
    perlinTex->addOctave(10.0f, 100.0f);
    perlinTex->addOctave(5.0f, 200.0f);
    perlinTex->addOctave(2.5f, 400.0f);

    Material* grey = new LambertianMaterial(blacktex, whitetex);
    Material* leftWallMaterial = new LambertianMaterial(blacktex, whitetex);
    Material* rightWallMaterial = new LambertianMaterial(blacktex, whitetex);
    
    CombineMaterial* sphereMaterial = new CombineMaterial();
    sphereMaterial->add(new PhongMaterial(new ConstantTexture(RGBColor(1.0f,1.0f,0.9f)), 64.0f), 0.6);
    sphereMaterial->add(new LambertianMaterial(new ConstantTexture(RGBColor(0, 1, 0)), perlinTex), 0.3);
    float hsq2 = 0.5f / std::sqrt(2.0f);
    CoordMapper* sphereMapper = new SphericalCoordMapper(Point(.3f,.1f,.3f),Vector(0.0f,hsq2,-hsq2),Vector(0.5f,0.0f,0.0f));
    Material* floorMaterial = new PhongMaterial(whitetex, 1.0f);

    //walls
    scene->add(new Quad(Point(000.f, 000.f, 000.f)*scale, Vector(000.f, 000.f, 560.f)*scale, Vector(550.f, 000.f, 000.f)*scale, nullptr, floorMaterial)); //floor
    scene->add(new Quad(Point(550.f, 550.f, 000.f)*scale, Vector(000.f, 000.f, 560.f)*scale, Vector(-550.f, 000.f, 000.f)*scale, nullptr, grey)); //ceiling
    scene->add(new Quad(Point(000.f, 000.f, 560.f)*scale, Vector(000.f, 550.f, 000.f)*scale, Vector(550.f, 000.f, 000.f)*scale, nullptr, grey)); //back wall
    scene->add(new Quad(Point(000.f, 000.f, 000.f)*scale, Vector(000.f, 550.f, 000.f)*scale, Vector(000.f, 000.f, 560.f)*scale, nullptr, rightWallMaterial)); //right wall
    scene->add(new Quad(Point(550.f, 550.f, 000.f)*scale, Vector(000.f, -550.f, 000.f)*scale, Vector(000.f, 000.f, 560.f)*scale, nullptr, leftWallMaterial)); //left wall

    scene->add(new Sphere(Point(150.0f, 100.0f, 150.0f)*scale, 99.0f*scale, sphereMapper, sphereMaterial));

    //tall box
    makeBox(scene, Point(265.f, 000.1f, 296.f)*scale, Vector(049.f, 000.f, 160.f)*scale, Vector(158.f, 000.f, -049.f)*scale, Vector(000.f, 330.f, 000.f)*scale, nullptr, grey);

    //point light
    world.light.push_back(new PointLight(Point((278)*scale,529.99f*scale,(279.5f)*scale),RGBColor::rep(150000.0f*scale*scale)));
    world.light.push_back(new PointLight(Point((278)*scale,229.99f*scale,(-359.5f)*scale),RGBColor::rep(50000.0f*scale*scale)));

    world.light.push_back(new PointLight(Point(490*scale,159.99f*scale,279.5f*scale),RGBColor(40000.0f*scale*scale,0,0)));
    world.light.push_back(new PointLight(Point(40*scale,159.99f*scale,249.5f*scale),RGBColor(5000.0f*scale*scale,30000.0f*scale*scale,5000.0f*scale*scale)));

    //directional light
    DirectionalLight dirl(Vector(0.2f,-0.5f,0.5f).normalize(), RGBColor(0.25f,0.25f,0.5f));
    world.light.push_back(&dirl);

    RayTracingIntegrator integrator(&world);

    Renderer engine(&cam, &integrator);
    engine.setSamples(16);
    engine.render(img);
    img.writeEXR("combined-material.exr");
}


void a_extra_credits() {
    environment_mapper();
    combined_materials(0.001f);
}