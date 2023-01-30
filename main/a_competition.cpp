#include <vector>
#include <chrono>

#include <core/color.h>
#include <core/vector.h>
#include <core/homogeneouscoord.h>
#include <core/point.h>
#include <core/miscellaneous.h>

#include <rt/intersection.h>
#include <rt/world.h>
#include <rt/multithreadedrenderer.h>

#include <rt/cameras/perspective.h>

#include <rt/coordmappers/coordmapper.h>
#include <rt/coordmappers/spherical.h>
#include <rt/coordmappers/environment.h>

#include <rt/groups/group.h>
#include <rt/groups/simplegroup.h>
#include <rt/groups/bvh.h>

#include <rt/integrators/recraytrace.h>

#include <rt/lights/arealight.h>
#include <rt/lights/pointlight.h>
#include <rt/lights/spotlight.h>

#include <rt/loaders/obj.h>

#include <rt/materials/lambertian.h>
#include <rt/materials/flatmaterial.h>
#include <rt/materials/combine.h>
#include <rt/materials/phong.h>
#include <rt/materials/transmission.h>
#include <rt/materials/dielectric.h>

#include <rt/normalmappers/imagenormal.h>
#include <rt/normalmappers/randomrough.h>

#include <rt/solids/quad.h>
#include <rt/solids/sphere.h>
#include <rt/solids/environment.h>

#include <rt/textures/constant.h>
#include <rt/textures/imagetex.h>




using namespace rt;

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




inline ConstantTexture* colorTexture(const RGBColor &color) {
    return new ConstantTexture(color);
}

inline ConstantTexture* blacktexture() {
    return colorTexture(RGBColor::rep(0.0f));
}

inline RGBColor bluecolor() {
    return RGBColor(0.744, 0.887, 0.930);
}

inline RGBColor orange() {
    return RGBColor(0.910, 0.408, 0.137);
}

inline RGBColor yellowishOrange() {
    return RGBColor(0.910, 0.708, 0.237);
}

inline RGBColor streetlampblue() {
    return RGBColor(0.1, 0.7, 0.7);
}

static std::vector<rt::Quad*> makeBox(Group* scene, const Point& aaa, const Vector& forward, const Vector& left, const Vector& up, CoordMapper* texMapper, Material* material) {
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


static void addMoon(World* world, Group* scene, Point center, Point cameraPosition) {
    center = Point(-80.431, 20.0, 310.316);
    float radius = 30.0f;
    // Vector dir = cameraPosition - center;

    SphericalCoordMapper* mapper = new SphericalCoordMapper(Point(0, 0, 0), Vector(0, 1, 0), Vector(1, 0, 0));
    LambertianMaterial* moonMaterial = new LambertianMaterial(blacktexture(),
                                            new ImageTexture("models/textures/moon.png"));
    
    Sphere* moon = new Sphere(center, radius, mapper, moonMaterial);
    scene->add(moon);

    // Point lightPos = center - 10 * dir.normalize(); // Point(-80.431, 20.0, 225.316)
    // world->light.push_back(new PointLight(lightPos, 8000.f * bluecolor()));
    // world->light.push_back(new SpotLight(lightPos, Vector(0, 0, -1), 0.8 * rt::pi, 0.0001f, 8000.f * bluecolor()));
    world->light.push_back(new PointLight(Point(-80.431, 20.0, 225.316), RGBColor::rep(8000.0f)));
    world->light.push_back(new SpotLight(Point(-80.431, 20.0, 225.316), Vector(0, 0, -1), 0.8 * rt::pi, 0.0001f, RGBColor::rep(4000.0f)));
}

static void addLightCube(World* world, Group* scene, Point center, float size, float intensity = 20.0f, bool translucent = false) {
    Material* cubeMat = new LambertianMaterial(colorTexture(yellowishOrange()),
                                                         colorTexture(orange()));
    if (translucent) {
        CombineMaterial* cb = new CombineMaterial();
        cb->add(cubeMat, 0.5);
        cb->add(new TransmissionMaterial(0.5, orange()), 0.5f);
        cubeMat = cb;
    } 

    std::vector<Quad*> quads = makeBox(scene, center, Vector(0, 0, size), Vector(size, 0, 0), Vector(0, size, 0), nullptr, cubeMat);
    
    for (auto q: quads) {
        world->light.push_back(new AreaLight(q, intensity));
    }
    // PointLight* light = new PointLight(center + Point(0, 5, 0), 50*RGBColor(0.910, 0.708, 0.237));
    // world->light.push_back(light);
}

static void addEnvironment(Group* scene, std::string filepath) {
    EnvironmentMapper* environmentMapper = new EnvironmentMapper(2, 1);
    ImageTexture* environmentTexture = new ImageTexture(filepath, ImageTexture::REPEAT, ImageTexture::BILINEAR);
    FlatMaterial* environmentMaterial = new FlatMaterial(environmentTexture);
    scene->add(new Environment(environmentMapper, environmentMaterial));
} 

static rt::MatLib* buildCompositeSceneMaterialLibrary() {

    rt::MatLib* lib = new rt::MatLib();

    // Lamp post
    FlatMaterial* lampPostLamp = new FlatMaterial(colorTexture(3 * streetlampblue()));
    CombineMaterial* lampPost = new CombineMaterial();

    lampPost->add(new LambertianMaterial(blacktexture(), colorTexture(RGBColor(0.2, 0.02, 0.01))), 0.8);
    lampPost->add(new PhongMaterial(colorTexture(RGBColor(0.829, 0.920, 0.239)), 3.0f), 0.2);

    lib->insert(std::make_pair("LampPost", lampPost));
    lib->insert(std::make_pair("LampPostLamp", lampPostLamp));

    // Base
    CombineMaterial* river = new CombineMaterial();
    river->add(new TransmissionMaterial(0.1f, RGBColor(0.0f, 0.01f, 0.2f)), 0.2);
    river->add(new DielectricMaterial(1.333f), 0.8);
    // river->add(new PhongMaterial(new ImageTexture("models/textures/Water_001_COLOR.png"), 902.0f), 0.8);
    river->add(new LambertianMaterial(new ConstantTexture(RGBColor(0, 0, 0.01)), new ConstantTexture(RGBColor(0.0f, 0.2f, 0.5f))), 0.05);
    // Material* river = new DielectricMaterial(1.33);

    LambertianMaterial* riverBed = new LambertianMaterial(blacktexture(), 
                                            colorTexture(RGBColor(0.52, 0.12, 0.05))); 
    
    ImageTexture* groundTexture = new ImageTexture("models/textures/snow_03_diff_1k.png");
    LambertianMaterial* ground = new LambertianMaterial(blacktexture(), groundTexture); 

    ImageTexture* blendTexture = new ImageTexture("models/textures/brick_wall_001_diffuse_1k.png");
    LambertianMaterial* riverBedGroundBlend = new LambertianMaterial(blacktexture(), blendTexture);

    lib->insert(std::make_pair("River", river));
    lib->insert(std::make_pair("RiverBed", riverBed));
    lib->insert(std::make_pair("Ground", ground));
    lib->insert(std::make_pair("RiverWall", riverBedGroundBlend));

    // Bridge
    ImageTexture* plankDiffuse = new ImageTexture("models/textures/dark_wood_diff_1k.png");
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

    templeKalasha->add(new PhongMaterial(colorTexture(RGBColor(0.628281, 0.555802, 0.366065)), 51.1), 0.5);
    templeKalasha->add(new LambertianMaterial(blacktexture(), colorTexture(RGBColor(0.75164, 0.60648, 0.22648))), 0.8);
    LambertianMaterial* templeRoofFrill = new LambertianMaterial(blacktexture(), colorTexture(RGBColor(0.800000, 0.319285, 0.001894)));
    LambertianMaterial* templeSupport = new LambertianMaterial(blacktexture(), colorTexture(RGBColor(0.349313, 0.080915, 0.024137)));


    lib->insert({"Roof", roof});
    lib->insert({"TempleKalasha", templeKalasha});
    lib->insert({"TempleRoofFrill", templeRoofFrill});
    lib->insert({"TempleSupport", templeSupport});

    return lib;
}

static void addGround(Group* scene, MatLib* lib) {
    ImageNormalMapper* snowNormal = new ImageNormalMapper(new ImageTexture("models/normals/snow_03_nor_dx_1k.png"), 2.0f);
    Quad* ground = new Quad(Point(50, -3.2, -0.2), Vector(-100, 0, 0), Vector(0, 0, 50), 
                    new QuadMapper(Vector::rep(0.0f), Vector::rep(1.0f)), 
                    lib->find("Ground")->second);
    ground->setNormalMapper(snowNormal);
    scene->add(ground);
}

static void addRiver(Group* scene, MatLib* lib) {
    Quad* river = new Quad(Point(50, -3.4, -40), Vector(-100, 0, 0), Vector(0, 0, 40), 
                    new QuadMapper(Vector::rep(0.0f), Vector(1.0f/1024.0f, 1.f/1024.0f, 1.f/1024.0f)), 
                    lib->find("River")->second);
    scene->add(river);
}


static void addLightCubes(World* world, Group* scene) {

    // Around and in the middle of temple
    addLightCube(world, scene, Point(6.48569, 16.063, 27.9291), 2);
    addLightCube(world, scene, Point(29.391, -2.5, 42.461), 2);
    addLightCube(world, scene, Point(-10.393, -2.5, 42.461), 2);
    addLightCube(world, scene, Point(-10.393, -2.5, 14.081), 2);
    addLightCube(world, scene, Point(29.391, -2.5, 14.081), 2);

    // Upper stairs of temple
    addLightCube(world, scene, Point(6.48569, 27.893, 27.9291), 1, 50.0f);
    addLightCube(world, scene, Point(6.48569, 36.893, 27.9291), 1, 50.0f);

    // Above river
    addLightCube(world, scene, Point(5, 0, -24), 2, 20.0f, true);
}


static void addExtraLights(World* world, Point sceneCenter, float height) {
    // For ambience
    PointLight *light = new PointLight(sceneCenter + Point(0.0f, 5 * height, 20.0f), RGBColor::rep(5000.0f));
    world->light.push_back(light);
    PointLight *light2 = new PointLight(sceneCenter + Point(0.0f, 5 * height, -20.0f), RGBColor::rep(5000.0f));
    world->light.push_back(light2);

    // Spotlight from street lamp
    world->light.push_back(new SpotLight(Point(-6.5849 , 2.9899, -6.4602f), Vector(0, -1, 0), rt::pi / 3, 0.3f, 150 * streetlampblue()));
}

static void addNormalMaps(rt::ObjLib* objlib) {
    RandomRoughNormalMapper* woodNormal = new RandomRoughNormalMapper(0.1f);
    (*(objlib->find("Bridge_Base")->second)).setNormalMapper(woodNormal);
    (*(objlib->find("Foot_Support")->second)).setNormalMapper(woodNormal);
    (*(objlib->find("Bridge_Hand_Support")->second)).setNormalMapper(woodNormal);
    (*(objlib->find("Bridge_Support")->second)).setNormalMapper(woodNormal);
}

static void renderCompositeScene(int size, int samples, int recursionDepth, std::string filename) {
    // Initialisation
    Image img(size, size);
    Point cameraPos = Point(-13.600564002990723, 28.634763717651367, -65.85577392578125);
    World world;
    SimpleGroup* worldScene = new SimpleGroup();
    BVH* scene = new BVH();

    rt::MatLib* lib = buildCompositeSceneMaterialLibrary();


    // Loading objects
    rt::ObjLib* objlib1 = loadOBJ(scene, "models/", "composite_scene.obj", lib);
    rt::ObjLib* objlib2 =  loadOBJ(scene, "models/", "temple.obj", lib);

    addNormalMaps(objlib1);
    CG_UNUSED(objlib2);

    addGround(scene, lib);
    addRiver(scene, lib);

    addLightCubes(&world, scene);

    scene->rebuildIndex();
    std::cout << "Done loading" << std::endl;

    // This has to be done before adding moon and environment
    BBox bounds = scene->getBounds();
    float height = bounds.max.y - bounds.min.y;
    Point to = bounds.center();
    std::cout << "Bounds : "<< bounds << std::endl;


    addExtraLights(&world, to, height);

    worldScene->add(scene);
    addEnvironment(worldScene, "models/environment/stars.png");
    addMoon(&world, worldScene, to + Point(0.0f, 3 * height, 0.0f), cameraPos);
    world.scene = worldScene;

    // Setup camera
    Vector up, forward;
    std::tie(forward, up) = cameraForwardAndUp(cameraPos, to);
    PerspectiveCamera cam(cameraPos, forward, up, pi/3, pi/3);
    
    
    std::cout << "Finished setting up scene" << std::endl;

    
    RecursiveRayTracingIntegrator integrator(&world);
    integrator.setRecursionDepth(recursionDepth);
    MultiThreadedRenderer engine(&cam, &integrator);
    engine.setSamples(samples);


    std::cout << "Starting to render" << std::endl;
    auto start = std::chrono::system_clock::now();
    engine.render(img);
    img.writeEXR(filename);
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> diff = end - start;
    std::cout << "Time taken:" << diff.count() << " s" << std::endl;

    std::cout << "Rendering complete!" << std::endl;
}

void a_competition() {
    renderCompositeScene(600, 16, 6, "competition.exr");
}