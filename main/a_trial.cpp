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
#include <rt/solids/environment.h>
#include <rt/solids/infiniteplane.h>
#include <rt/solids/disc.h>
#include <rt/solids/quad.h>
#include <rt/cameras/perspective.h>
#include <rt/integrators/casting.h>
#include <rt/integrators/raytrace.h>
#include <rt/integrators/recraytrace.h>
#include <rt/lights/pointlight.h>
#include <rt/lights/arealight.h>
#include <rt/lights/spotlight.h>
#include <rt/materials/dummy.h>
#include <rt/materials/flatmaterial.h>
#include <rt/materials/transmission.h>
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
#include <rt/coordmappers/environment.h>
#include <rt/coordmappers/spherical.h>
#include <unordered_map>


using namespace rt;


inline ConstantTexture* blacktexture() {
    return new ConstantTexture(RGBColor::rep(0.0f));
}

class QuadMapper : public CoordMapper {
    public:
        QuadMapper(const Vector& offset, const Vector& scale = Vector::rep(1.f)) : offset(offset), scale(scale) { }
        virtual Point getCoords(const Intersection& hit) const {
            HomogeneousCoord p = HomogeneousCoord(scale) * HomogeneousCoord(hit.local()) + HomogeneousCoord(offset);
            return Point(p[0], p[1], p[2]);
        }

    private:
        Vector offset;
        Vector scale;
};

std::vector<rt::Quad*> makeBox(Group* scene, const Point& aaa, const Vector& forward, const Vector& left, const Vector& up, CoordMapper* texMapper, Material* material) {
    std::vector<rt::Quad*> quads;
    Quad* q1 = new Quad(aaa, left, forward, texMapper, material);
    quads.push_back(q1);
    scene->add(q1);
    Quad* q2 = new Quad(aaa, forward, up, texMapper, material);
    quads.push_back(q2);
    scene->add(q2);
    Quad* q3 = new Quad(aaa, up, left, texMapper, material);
    quads.push_back(q3);
    scene->add(q3);
    Point bbb = aaa + forward + left + up;
    Quad* q4 = new Quad(bbb, -forward, -left, texMapper, material);
    quads.push_back(q4);
    scene->add(q4);
    Quad* q5 = new Quad(bbb, -up, -forward, texMapper, material);
    quads.push_back(q5);
    scene->add(q5);
    Quad* q6 = new Quad(bbb, -left, -up, texMapper, material);
    quads.push_back(q6);
    scene->add(q6);
    return quads;
}

void addMoon(World* world, Group* scene, Point center) {
    SphericalCoordMapper* mapper = new SphericalCoordMapper(Point(0, 0, 0), Vector(0, 1, 0), Vector(1, 0, 0));
    LambertianMaterial* moonMaterial = new LambertianMaterial(blacktexture(),
                                            new ImageTexture("models/textures/moon.png"));
    
    Sphere* moon = new Sphere(Point(-80.431, 20.0, 310.316), 30.0f, mapper, moonMaterial);
    // Disc* moonDisc = new Disc(Point(7.431, 12.0, 60.316), Vector(0, -1, 0), 5.0f, nullptr, moonMaterial);
    // Disc* moonDisc2 = new Disc(Point(7.431, 20.0, 40.316), Vector(0, 0, 1), 5.0f, nullptr, moonMaterial);
    scene->add(moon);
    // scene->add(moonDisc);
    // scene->add(moonDisc2);
    // world->light.push_back(new AreaLight(moonDisc));
    // world->light.push_back(new AreaLight(moonDisc2));
    world->light.push_back(new PointLight(Point(-80.431, 20.0, 225.316), RGBColor::rep(8000.0f)));
    world->light.push_back(new SpotLight(Point(-80.431, 20.0, 225.316), Vector(0, 0, -1), 0.8 * rt::pi, 0.0001f, RGBColor::rep(4000.0f)));
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
    
    ImageTexture* riverBedTexture = new ImageTexture("models/textures/river_small_rocks_diff_2k.png");
    LambertianMaterial* riverBed = new LambertianMaterial(blacktexture(), riverBedTexture); //new ConstantTexture(RGBColor(0.52, 0.12, 0.05))
    
    ImageTexture* groundTexture = new ImageTexture("models/textures/grassy_cobblestone_diff_2k.png");
    LambertianMaterial* ground = new LambertianMaterial(blacktexture(), groundTexture); //  new ConstantTexture(RGBColor(0.0f, 0.183f, 0.0f))
    // FlatMaterial* ground = new FlatMaterial(groundTexture);

    ImageTexture* blendTexture = new ImageTexture("models/textures/brick_wall_001_diffuse_1k.png");
    LambertianMaterial* riverBedGroundBlend = new LambertianMaterial(blacktexture(), blendTexture);

    rt::MatLib* lib = new rt::MatLib();
    lib->insert(std::make_pair("River", river));
    lib->insert(std::make_pair("RiverBed", riverBed));
    lib->insert(std::make_pair("Ground", ground));
    lib->insert(std::make_pair("RiverWall", riverBedGroundBlend));

    return lib;
}


void addLightSphere(World* world, Group* scene, Point center, float radius) {
    TransmissionMaterial* spMat = new TransmissionMaterial(0.8, RGBColor(0.980, 0.890, 0.490));
    Sphere* sp = new Sphere(center, radius, nullptr, spMat);
    // Sphere* sp2 = new Sphere(center, 3, nullptr, glass);
    SpotLight* light = new SpotLight(center + Point(0, -2 * radius, 0), Vector(0, -1, 0), rt::pi/6, 0.5f, 100 * RGBColor(0.980, 0.890, 0.490));
    scene->add(sp);
    // scene->add(sp2);
    world->light.push_back(light);
}


void addLightCube(World* world, Group* scene, Point center) {
    LambertianMaterial* cubeMat = new LambertianMaterial(new ConstantTexture(RGBColor(0.910, 0.708, 0.237)),
        new ConstantTexture(RGBColor(0.910, 0.408, 0.137)));
    std::vector<Quad*> quads = makeBox(scene, center, Vector(0, 0, 2), Vector(2, 0, 0), Vector(0, 2, 0), nullptr, cubeMat);
    
    // for (auto q: quads) {
    //     world->light.push_back(new AreaLight(q));
    // }
    PointLight* light = new PointLight(center + Point(0, 5, 0), 50*RGBColor(0.910, 0.708, 0.237));
    world->light.push_back(light);
}

void renderBase() {
    Image img(600, 600);
    World world;
    BVH* scene = new BVH();
    rt::MatLib* lib = buildBaseMaterialLibrary();
    loadOBJ(scene, "models/", "base.obj", lib);
    std::cout << "Loaded base" << std::endl;

    addLightSphere(&world, scene, Point(0, 0, 40), 2.0f);


    PointLight *light = new PointLight(Point(6.076245307922363, 30.944828987121582, 3.42127513885498), RGBColor::rep(10000.0f));
    world.light.push_back(light);

    // addMoon(&world, scene, Point(6.076245307922363, 35.944828987121582, 3.42127513885498));

    scene->rebuildIndex();
    world.scene = scene;
    BBox bounds = scene->getBounds();
    Point to = bounds.center();
    std::cout << "Bounds : "<< bounds << std::endl;

    // PointLight *ambientLight = new PointLight(Point(to.x, 150.0f, to.z), RGBColor::rep(5000.0f));
    // world.light.push_back(ambientLight);
    ConstantTexture* lightsrctex = new ConstantTexture(RGBColor::rep(10.0f));
    Material* lightsource = new LambertianMaterial(lightsrctex, blacktexture());
    Quad* ambientLight = new Quad(Point(bounds.min.x,150.0f,bounds.min.y), Vector(bounds.max.x - bounds.min.x,0,0), Vector(0,0,bounds.max.z - bounds.min.z), nullptr, lightsource);
    world.light.push_back(new AreaLight (ambientLight));

    Point from = Point(-13.600564002990723, 24.634763717651367, -80.85577392578125);

    Vector up, forward;
    std::tie(forward, up) = cameraForwardAndUp(from, to);

    PerspectiveCamera cam(from, forward, Vector(0, 1, 0), pi/3, pi/3);


    RecursiveRayTracingIntegrator integrator(&world);
    integrator.setRecursionDepth(6);
    MultiThreadedRenderer engine(&cam, &integrator);
    engine.setSamples(10);
    engine.render(img);
    img.writeEXR("base.exr");
    std::cout << "Rendered" << std::endl;
}

rt::MatLib* buildBridgeMaterialLibrary() {

    ImageTexture* plankDiffuse = new ImageTexture("models/textures/dark_wood_diff_1k.png");
    // ImageTexture* plankNormal = new ImageTexture("models/textures/plank_flooring_02_nor_dx_1k.png");

    // ImageNormalMapper* bridgeBaseNm = new ImageNormalMapper(plankNormal);
    ImageTexture* bridgeSupportDiffuse = new ImageTexture("models/textures/fine_grained_wood_col_1k.png");
    LambertianMaterial* bridgeBase = new LambertianMaterial(blacktexture(), plankDiffuse);
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

void addEnvironment(Group* scene, std::string filepath) {
    EnvironmentMapper* environmentMapper = new EnvironmentMapper(2, 1);
    ImageTexture* environmentTexture = new ImageTexture(filepath, ImageTexture::REPEAT, ImageTexture::BILINEAR);
    FlatMaterial* environmentMaterial = new FlatMaterial(environmentTexture);
    scene->add(new Environment(environmentMapper, environmentMaterial));
} 

void renderBridge() {
    Image img(1024, 1024);

    BVH* scene = new BVH();
    rt::MatLib* lib = buildBridgeMaterialLibrary();
    rt::ObjLib* objlib = loadOBJ(scene, "models/", "bridge.obj", lib);
    CG_UNUSED(objlib);
    


    scene->rebuildIndex();
    SimpleGroup* worldScene = new SimpleGroup();
    worldScene->add(scene);
    addEnvironment(worldScene, "models/environment/test.png");
    World world;
    world.scene = worldScene;


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
    integrator.setRecursionDepth(6);
    MultiThreadedRenderer engine(&cam, &integrator);
    engine.setSamples(100);
    engine.render(img);
    img.writeEXR("bridge.exr");
    std::cout << "Rendered" << std::endl;
}




rt::MatLib* buildCompositeSceneMaterialLibrary() {

    rt::MatLib* lib = new rt::MatLib();

    // Lamp post
    FlatMaterial* lampPostLamp = new FlatMaterial(new ConstantTexture(3 * RGBColor(0.1, 0.7, 0.7)));
    CombineMaterial* lampPost = new CombineMaterial();
    // lampPost->add(new FuzzyConductorMaterial(2.485, 3.433, rt::pi / 36.0f), 0.7);
    lampPost->add(new LambertianMaterial(new ConstantTexture(RGBColor::rep(0.0f)), new ConstantTexture(RGBColor(0.2, 0.02, 0.01))), 0.8);
    lampPost->add(new PhongMaterial(new ConstantTexture(RGBColor(0.829, 0.920, 0.239)), 3.0f), 0.2);

    lib->insert(std::make_pair("LampPost", lampPost));
    lib->insert(std::make_pair("LampPostLamp", lampPostLamp));



    // Base
    CombineMaterial* river = new CombineMaterial();
    river->add(new TransmissionMaterial(0.1f, RGBColor(0.0f, 0.01f, 0.2f)), 0.2);
    river->add(new DielectricMaterial(1.333f), 0.8);
    // river->add(new PhongMaterial(new ImageTexture("models/textures/Water_001_COLOR.png"), 902.0f), 0.8);
    river->add(new LambertianMaterial(new ConstantTexture(RGBColor(0, 0, 0.01)), new ConstantTexture(RGBColor(0.0f, 0.2f, 0.5f))), 0.05);
    // Material* river = new DielectricMaterial(1.33);

    // ImageTexture* riverBedTexture = new ImageTexture("models/textures/Sand_002_COLOR.png");
    LambertianMaterial* riverBed = new LambertianMaterial(blacktexture(), 
                                            new ConstantTexture(RGBColor(0.590, 0.317, 0.00590))); //new ConstantTexture(RGBColor(0.52, 0.12, 0.05))
    
    ImageTexture* groundTexture = new ImageTexture("models/textures/snow_03_diff_1k.png");
    LambertianMaterial* ground = new LambertianMaterial(blacktexture(), groundTexture); //  new ConstantTexture(RGBColor(0.0f, 0.183f, 0.0f))
    // FlatMaterial* ground = new FlatMaterial(groundTexture);

    ImageTexture* blendTexture = new ImageTexture("models/textures/brick_wall_001_diffuse_1k.png");
    LambertianMaterial* riverBedGroundBlend = new LambertianMaterial(blacktexture(), blendTexture);

    lib->insert(std::make_pair("River", river));
    lib->insert(std::make_pair("RiverBed", riverBed));
    lib->insert(std::make_pair("Ground", ground));
    lib->insert(std::make_pair("RiverWall", riverBedGroundBlend));

    // Bridge

    ImageTexture* plankDiffuse = new ImageTexture("models/textures/dark_wood_diff_1k.png");
    // ImageTexture* plankNormal = new ImageTexture("models/textures/plank_flooring_02_nor_dx_1k.png");

    // ImageNormalMapper* bridgeBaseNm = new ImageNormalMapper(plankNormal);
    ImageTexture* bridgeSupportDiffuse = new ImageTexture("models/textures/fine_grained_wood_col_1k.png");
    LambertianMaterial* bridgeBase = new LambertianMaterial(blacktexture(), plankDiffuse);
    LambertianMaterial* bridgeSupport = new LambertianMaterial(blacktexture(), bridgeSupportDiffuse);
    LambertianMaterial* bridgeHandSupport = new LambertianMaterial(blacktexture(), bridgeSupportDiffuse);
    LambertianMaterial* bridgeFootSupport = new LambertianMaterial(blacktexture(), bridgeSupportDiffuse);

    lib->insert({"BridgeBase", bridgeBase});
    lib->insert({"BridgeFootSupport", bridgeFootSupport});
    lib->insert({"BridgeHandSupport", bridgeHandSupport});
    lib->insert({"BridgeSupport", bridgeSupport});



    // Temple
    LambertianMaterial* roof = new LambertianMaterial(blacktexture(), new ConstantTexture(RGBColor(0.8, 0, 0.001041)));
    CombineMaterial* templeKalasha = new CombineMaterial();
    templeKalasha->add(new FuzzyConductorMaterial(2.5, 0.2, 0), 0.2);
    templeKalasha->add(new LambertianMaterial(blacktexture(), new ConstantTexture(RGBColor(0.800000, 0.789581, 0.121041))), 0.8);
    LambertianMaterial* templeRoofFrill = new LambertianMaterial(blacktexture(), new ConstantTexture(RGBColor(0.800000, 0.319285, 0.001894)));
    LambertianMaterial* templeSupport = new LambertianMaterial(blacktexture(), new ConstantTexture(RGBColor(0.349313, 0.080915, 0.024137)));


    lib->insert({"Roof", roof});
    lib->insert({"TempleKalasha", templeKalasha});
    lib->insert({"TempleRoofFrill", templeRoofFrill});
    lib->insert({"TempleSupport", templeSupport});

    return lib;
}

void addMaps(rt::ObjLib* objlib) {
    RandomRoughNormalMapper* woodNormal = new RandomRoughNormalMapper(0.1f);
    (*(objlib->find("BridgeBase")->second)).setNormalMapper(woodNormal);
    (*(objlib->find("BridgeFootSupport")->second)).setNormalMapper(woodNormal);
    (*(objlib->find("BridgeHandSupport")->second)).setNormalMapper(woodNormal);
    (*(objlib->find("BridgeSupport")->second)).setNormalMapper(woodNormal);

    ImageNormalMapper* snowNormal = new ImageNormalMapper(new ImageTexture("models/normals/snow_03_nor_dx_1k.png"));
    (*(objlib->find("BaseGround")->second)).setNormalMapper(snowNormal);

}

void renderCompositeScene() {
    Image img(1280, 1280);
    World world;
    SimpleGroup* worldScene = new SimpleGroup();
    BVH* scene = new BVH();

    rt::MatLib* lib = buildCompositeSceneMaterialLibrary();

    CG_UNUSED(lib);
    rt::ObjLib* objlib = loadOBJ(scene, "models/", "composite_scene.obj", lib);
    loadOBJ(scene, "models/", "temple.obj", lib);

    addLightCube(&world, scene, Point(42.645, -0.6621, 6.0934));
    // addLightCube(&world, scene, Point(9.15, -0.6621, 10.0934));

    ImageNormalMapper* snowNormal = new ImageNormalMapper(new ImageTexture("models/normals/snow_03_nor_dx_1k.png"));
    Quad* ground = new Quad(Point(50, -3.2, -0.2), Vector(-100, 0, 0), Vector(0, 0, 50), 
                    new QuadMapper(Vector::rep(0.0f), Vector::rep(1.0f)), 
                    new LambertianMaterial(blacktexture(), new ImageTexture("models/textures/snow_03_diff_1k.png")));
    ground->setNormalMapper(snowNormal);
    // ImageNormalMapper* riverNormal = new ImageNormalMapper(new ImageTexture("models/normals/Water_001_NORM.png"));
    Quad* river = new Quad(Point(50, -3.3, -40), Vector(-100, 0, 0), Vector(0, 0, 40), 
                    new QuadMapper(Vector::rep(0.0f), Vector(1.0f/1024.0f, 1.f/1024.0f, 1.f/1024.0f)), 
                    lib->find("River")->second);
    // river->setNormalMapper(riverNormal);
    scene->add(ground);
    scene->add(river);
    addLightCube(&world, scene, Point(6.48569, -3.5, 27.9291));
    addLightCube(&world, scene, Point(29.391, -3.5, 42.461));
    addLightCube(&world, scene, Point(-10.393, -3.5, 42.461));
    addLightCube(&world, scene, Point(-10.393, -3.5, 14.081));
    addLightCube(&world, scene, Point(29.391, -3.5, 14.081));

    addLightCube(&world, scene, Point(5, 0, -24));
    // world.light.push_back(new PointLight(river->getBounds().center() + Point(0, 5, 0), 30 * RGBColor(0.778, 0.810, 0.809)));

    scene->rebuildIndex();
    CG_UNUSED(objlib);
    // addMaps(objlib);
    std::cout << "Done loading" << std::endl;

    BBox bounds = scene->getBounds();
    float height = bounds.max.y - bounds.min.y;
    Point to = bounds.center();
    std::cout << "Bounds : "<< bounds << std::endl;
    // Point from = Point(-13.600564002990723, 24.634763717651367, -90.85577392578125);
    Point from = Point(-13.600564002990723, 28.634763717651367, -65.85577392578125);
    Vector up, forward;
    std::tie(forward, up) = cameraForwardAndUp(from, to);

    PointLight *light = new PointLight(to + Point(0.0f, 5 * height, 20.0f), RGBColor::rep(5000.0f));
    world.light.push_back(light);
    PointLight *light2 = new PointLight(to + Point(0.0f, 5 * height, -20.0f), RGBColor::rep(5000.0f));
    world.light.push_back(light2);

    // ConstantTexture* lightsrctex = new ConstantTexture(5 * RGBColor::rep(0.5f));
    // Material* lightsource = new LambertianMaterial(lightsrctex, blacktexture());
    // Quad* ambientLight = new Quad(Point(bounds.min.x,150.0f,bounds.min.y), Vector(bounds.max.x - bounds.min.x,0,0), Vector(0,0,bounds.max.z - bounds.min.z), nullptr, lightsource);
    // world.light.push_back(new AreaLight (ambientLight));

    worldScene->add(scene);
    addEnvironment(worldScene, "models/environment/stars.png");
    addMoon(&world, worldScene, to + Point(0.0f, 3 * height, 0.0f));
    // addLightSphere(&world, worldScene, Point(14.431, 2, 23.0f), 2.0f);
    
    world.light.push_back(new SpotLight(Point(-6.5849 , 2.9899, -6.4602f), Vector(0, -1, 0), rt::pi / 3, 0.3f, 100 * RGBColor::rep(0.8f)));
    world.scene = worldScene;

    PerspectiveCamera cam(from, forward, up, pi/3, pi/3);


    RecursiveRayTracingIntegrator integrator(&world);
    integrator.setRecursionDepth(25);
    MultiThreadedRenderer engine(&cam, &integrator);
    engine.setSamples(100);
    std::cout << "Starting to render" << std::endl;
    engine.render(img);
    img.writeEXR("composite_final.exr");
    std::cout << "Rendered" << std::endl;
}

void renderLight() {
    Image img(400, 400);

    DielectricMaterial* glass = new DielectricMaterial(2.44);
    Sphere* sp1 = new Sphere(Point::rep(0), 2, nullptr, glass);

    PointLight* light1 = new PointLight(Point::rep(0), RGBColor(0.980, 0.890, 0.490));

    World world;
    SimpleGroup* scene = new SimpleGroup();
    scene->add(sp1);

    makeBox(scene, Point(2.5, 0, 0), Vector(0, 0, 4), Vector(-4, 0, 0), Vector(0, 4, 0), nullptr, 
        new LambertianMaterial(blacktexture(), new ConstantTexture(RGBColor::rep(0.3f))));
    world.light.push_back(light1);
    world.scene = scene;

    PerspectiveCamera cam(Point(0, 0, -10), Vector(0, 0, 1), Vector(0, 1, 0), 0.686f, 0.686f);
    RecursiveRayTracingIntegrator integrator(&world);
    integrator.setRecursionDepth(30);
    MultiThreadedRenderer engine(&cam, &integrator);
    engine.setSamples(10);
    std::cout << "Starting to render" << std::endl;
    engine.render(img);
    img.writeEXR("light.exr");
    std::cout << "Rendered" << std::endl;
}

void a_trial() {
    // renderLampPost();
    // renderBase();
    // renderBridge();
    renderCompositeScene();
    // renderLight();
}