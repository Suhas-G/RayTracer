#include <core/assert.h>
#include <core/scalar.h>
#include <core/matrix.h>
#include <core/image.h>
#include <rt/world.h>
#include <rt/renderer.h>
#include <rt/multithreadedrenderer.h>
#include <rt/loaders/obj.h>
#include <rt/groups/bvh.h>
#include <rt/groups/simplegroup.h>
#include <rt/solids/sphere.h>
#include <rt/solids/infiniteplane.h>
#include <rt/cameras/perspective.h>
#include <rt/integrators/casting.h>
#include <rt/integrators/raytrace.h>
#include <rt/integrators/recraytrace.h>
#include <rt/lights/pointlight.h>
#include <rt/materials/dummy.h>
#include <rt/materials/flatmaterial.h>
#include <rt/textures/constant.h>
#include <rt/textures/checkerboard.h>
#include <rt/textures/imagetex.h>
#include <rt/materials/lambertian.h>
#include <rt/materials/phong.h>
#include <rt/materials/combine.h>
#include <rt/materials/dielectric.h>
#include <rt/materials/fuzzyconductor.h>
#include <rt/normalmappers/randomrough.h>
#include <rt/normalmappers/imagenormal.h>
#include <core/miscellaneous.h>


using namespace rt;


inline ConstantTexture* blacktexture() {
    return new ConstantTexture(RGBColor::rep(0.0f));
}

rt::MatLib* buildMaterialLibraryForCornellBox() {
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

void loadCornellBox() {
    Image img(600, 600);

    BVH* scene = new BVH();

    rt::MatLib *lib = buildMaterialLibraryForCornellBox();
    // loadOBJMat(lib, "models/", "cornell-box.mtl");
    loadOBJ(scene, "models/", "cornell-box.obj", lib);
    // DummyMaterial* mat = new DummyMaterial();
    // FlatMaterial *mat = new FlatMaterial(new ConstantTexture(RGBColor::rep(1.0f)));
    // scene->setMaterial(mat);
	scene->rebuildIndex();
    World world;
    world.scene = scene;

    std::cout << "Bounds : "<< scene->getBounds() << std::endl;

    PointLight *light = new PointLight(Point(-42.86680221557617, 12.695966720581055, 0.0), RGBColor::rep(10000.0f));
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
    MultiThreadedRenderer engine(&cam, &integrator);
    engine.setSamples(16);
    engine.render(img);
    img.writeEXR("trial-cornell-box.exr");
}

void testRoughNormalMap() {

    SimpleGroup* scene = new SimpleGroup();
    Image img(800, 800);
    World world;
    PerspectiveCamera cam(Point(0.1f, -1.5f, 0.225f), Vector(0.f, 1.f, -0.5f), Vector(0.f, 0.f, 1.f), 1.0f, 1.0f);
    // RayTracingIntegrator integrator(&world);
    RecursiveRayTracingIntegrator integrator(&world);
    

    // CheckerboardTexture* checkerboardTex = new CheckerboardTexture(RGBColor(1.0f,0.9f,0.7f), RGBColor(0.2f,0.2f,0.0f));
    // FlatMaterial* checkerboard = new FlatMaterial(checkerboardTex);
    // ConstantTexture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));
    // LambertianMaterial* checkerboard = new LambertianMaterial(blacktex, checkerboardTex);

    // RandomRoughNormalMapper* randomNm = new RandomRoughNormalMapper(0.2f);
    // ImageTexture* sandColorTexture = new ImageTexture("models/Sand_002_COLOR.png");
    // ImageTexture* waterColorTexture = new ImageTexture("models/Water_001_COLOR.png");
    // ImageTexture* waterShineTexture = new ImageTexture("models/Water_001_SPEC.png");
    // PhongMaterial* waterShineMaterial = new PhongMaterial(waterShineTexture, 3.0f);
    // LambertianMaterial* sandColorMaterial = new LambertianMaterial(blacktex, sandColorTexture);
    DielectricMaterial* waterMaterial = new DielectricMaterial(5.0f);
    // CombineMaterial* waterMaterial = new CombineMaterial();
    // waterMaterial->add(waterColorMaterial, 1);
    // waterMaterial->add(waterShineMaterial, 0.01);
    // ImageTexture* sandNormalTexture  = new ImageTexture("models/Sand_002_NRM.png");
    // ImageNormalMapper* sandNormalMap = new ImageNormalMapper(sandNormalTexture);
    ImageTexture* waterNormalTexture = new ImageTexture("models/Water_001_NORM.png");
    ImageNormalMapper* waterNormalMap = new ImageNormalMapper(waterNormalTexture);
    InfinitePlane* plane = new InfinitePlane(Point(0.0f,0.0f,-0.018f), Vector(0.01f, 0.0f, 1.0f), nullptr, waterMaterial);
    plane->setNormalMapper(waterNormalMap);
    scene->add(plane);

    PointLight *light = new PointLight(Point(10.86680221557617, 12.695966720581055, 50.0), RGBColor::rep(10000.0f));
    world.light.push_back(light);

    world.scene = scene;

    MultiThreadedRenderer engine(&cam, &integrator);
    engine.setSamples(20);
    engine.render(img);
    std::cout << "Rendered sand" << std::endl;
    img.writeEXR("rough-normalmap.exr");
}


rt::MatLib* buildLampPostMaterialLibrary() {
    // FlatMaterial* lampPostLamp = new FlatMaterial(new ConstantTexture(RGBColor::rep(0.8f)));
    DielectricMaterial* lampPostLamp = new DielectricMaterial(1.52f);
    CombineMaterial* lampPost = new CombineMaterial();
    lampPost->add(new FuzzyConductorMaterial(2.485, 3.433, rt::pi / 36.0f), 0.7);
    lampPost->add(new LambertianMaterial(new ConstantTexture(RGBColor::rep(0.0f)), new ConstantTexture(RGBColor(0.2, 0.02, 0.01))), 0.1);
    lampPost->add(new PhongMaterial(new ConstantTexture(RGBColor(0.829, 0.920, 0.239)), 3.0f), 0.2);

    rt::MatLib* matlib = new rt::MatLib();
    matlib->insert(std::make_pair("LampPost", lampPost));
    matlib->insert(std::make_pair("LampPostLamp", lampPostLamp));
    return matlib;
}

void renderLampPost() {
    Image img(1024, 1024);

    BVH* scene = new BVH();
    rt::MatLib* lib = buildLampPostMaterialLibrary();
    loadOBJ(scene, "models/", "lamppost.obj", lib);

    scene->rebuildIndex();
    World world;
    world.scene = scene;

    
    PointLight *light = new PointLight(Point(6.076245307922363, 25.944828987121582, 3.42127513885498), RGBColor::rep(10000.0f));
    world.light.push_back(light);

    BBox bounds = scene->getBounds();
    Point to = bounds.center();
    std::cout << "Bounds : "<< bounds << std::endl;
    Point from = Point(5.851303100585938, 8.6923067569732666, -20);

    Vector up, forward;
    std::tie(forward, up) = cameraForwardAndUp(from, to);

    PerspectiveCamera cam(from, forward, up, pi/3, pi/3);


    RecursiveRayTracingIntegrator integrator(&world);
    MultiThreadedRenderer engine(&cam, &integrator);
    engine.setSamples(32);
    engine.render(img);
    img.writeEXR("lamp-post.exr");
    std::cout << "Rendered" << std::endl;
}

rt::MatLib* buildBaseMaterialLibrary() {
    CombineMaterial* river = new CombineMaterial();
    river->add(new DielectricMaterial(1.333f), 0.8);
    river->add(new PhongMaterial(new ConstantTexture(RGBColor(0.25f, 0.25f, 0.25f)), 902.0f), 0.15);
    river->add(new LambertianMaterial(new ConstantTexture(RGBColor::rep(0.0f)), new ConstantTexture(RGBColor(0.0f, 0.415f, 0.8f))), 0.05);
    LambertianMaterial* riverBed = new LambertianMaterial(new ConstantTexture(RGBColor::rep(0.0f)), new ConstantTexture(RGBColor(0.52, 0.12, 0.05)));
    LambertianMaterial* ground = new LambertianMaterial(new ConstantTexture(RGBColor::rep(0.0f)), new ConstantTexture(RGBColor(0.0f, 0.183f, 0.0f)));

    rt::MatLib* lib = new rt::MatLib();
    lib->insert(std::make_pair("River", river));
    lib->insert(std::make_pair("RiverBed", riverBed));
    lib->insert(std::make_pair("Ground", ground));

    return lib;
}


void renderBase() {
    Image img(1024, 1024);

    BVH* scene = new BVH();
    rt::MatLib* lib = buildBaseMaterialLibrary();
    loadOBJ(scene, "models/", "base.obj", lib);
    scene->rebuildIndex();
    World world;
    world.scene = scene;

    
    PointLight *light = new PointLight(Point(6.076245307922363, 30.944828987121582, 3.42127513885498), RGBColor::rep(10000.0f));
    world.light.push_back(light);

    BBox bounds = scene->getBounds();
    Point to = bounds.center();
    std::cout << "Bounds : "<< bounds << std::endl;
    Point from = Point(75.92703247070312, 9.428650856018066, 96.34475708007812);

    Vector up, forward;
    std::tie(forward, up) = cameraForwardAndUp(from, to);

    PerspectiveCamera cam(from, forward, up, pi/3, pi/3);


    RecursiveRayTracingIntegrator integrator(&world);
    integrator.setRecursionDepth(30);
    MultiThreadedRenderer engine(&cam, &integrator);
    engine.setSamples(32);
    engine.render(img);
    img.writeEXR("base.exr");
    std::cout << "Rendered" << std::endl;
}

rt::MatLib* buildBridgeMaterialLibrary() {

    // ImageTexture* plankDiffuse = new ImageTexture("models/textures/plank_flooring_02_diff_1k.png");
    // ImageTexture* plankNormal = new ImageTexture("models/textures/plank_flooring_02_nor_dx_1k.png");

    // ImageNormalMapper* bridgeBaseNm = new ImageNormalMapper(plankNormal);
    ImageTexture* bridgeSupportDiffuse = new ImageTexture("models/textures/fine_grained_wood_col_1k.png");
    LambertianMaterial* bridgeBase = new LambertianMaterial(blacktexture(), bridgeSupportDiffuse);
    LambertianMaterial* bridgeSupport = new LambertianMaterial(blacktexture(), bridgeSupportDiffuse);
    LambertianMaterial* bridgeHandSupport = new LambertianMaterial(blacktexture(), bridgeSupportDiffuse);
    LambertianMaterial* bridgeFootSupport = new LambertianMaterial(blacktexture(), bridgeSupportDiffuse);

    rt::MatLib* lib = new rt::MatLib();
    lib->insert({"BridgeBase", bridgeBase});
    lib->insert({"BridgeFootSupport", bridgeFootSupport});
    lib->insert({"BridgeHandSupport", bridgeHandSupport});
    lib->insert({"BridgeSupport", bridgeSupport});

    return lib;
}

void renderBridge() {
    Image img(1024, 1024);

    BVH* scene = new BVH();
    rt::MatLib* lib = buildBridgeMaterialLibrary();
    loadOBJ(scene, "models/", "bridge.obj", lib);
    scene->rebuildIndex();
    World world;
    world.scene = scene;


    BBox bounds = scene->getBounds();
    float height = bounds.max.y - bounds.min.y;
    Point to = bounds.center();
    std::cout << "Bounds : "<< bounds << std::endl;
    Point from = Point(29.371318817138672, 13.336181640625, -53.291690826416016);

    Vector up, forward;
    std::tie(forward, up) = cameraForwardAndUp(from, to);

    PointLight *light = new PointLight(to + Point(0.0f, 2 * height, 0.0f), RGBColor::rep(5000.0f));
    world.light.push_back(light);

    PerspectiveCamera cam(from, forward, up, pi/3, pi/3);


    RecursiveRayTracingIntegrator integrator(&world);
    integrator.setRecursionDepth(30);
    MultiThreadedRenderer engine(&cam, &integrator);
    engine.setSamples(32);
    engine.render(img);
    img.writeEXR("bridge.exr");
    std::cout << "Rendered" << std::endl;
}

void a_trial() {
    // renderLampPost();
    // renderBase();
    renderBridge();
}